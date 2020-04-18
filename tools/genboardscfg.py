#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0+
#
# Author: Masahiro Yamada <yamada.m@jp.panasonic.com>
#

"""
Converter from Kconfig and MAINTAINERS to a board database.

Run 'tools/genboardscfg.py' to create a board database.

Run 'tools/genboardscfg.py -h' for available options.
"""

import errno
import fnmatch
import glob
import multiprocessing
import optparse
import os
import sys
import tempfile
import time

from buildman import kconfiglib

### constant variables ###
OUTPUT_FILE = 'boards.cfg'
CONFIG_DIR = 'configs'
SLEEP_TIME = 0.03
COMMENT_BLOCK = '''#
# List of boards
#   Automatically generated by %s: don't edit
#
# Status, Arch, CPU, SoC, Vendor, Board, Target, Options, Maintainers

''' % __file__

### helper functions ###
def try_remove(f):
    """Remove a file ignoring 'No such file or directory' error."""
    try:
        os.remove(f)
    except OSError as exception:
        # Ignore 'No such file or directory' error
        if exception.errno != errno.ENOENT:
            raise

def check_top_directory():
    """Exit if we are not at the top of source directory."""
    for f in ('README', 'Licenses'):
        if not os.path.exists(f):
            sys.exit('Please run at the top of source directory.')

def output_is_new(output):
    """Check if the output file is up to date.

    Returns:
      True if the given output file exists and is newer than any of
      *_defconfig, MAINTAINERS and Kconfig*.  False otherwise.
    """
    try:
        ctime = os.path.getctime(output)
    except OSError as exception:
        if exception.errno == errno.ENOENT:
            # return False on 'No such file or directory' error
            return False
        else:
            raise

    for (dirpath, dirnames, filenames) in os.walk(CONFIG_DIR):
        for filename in fnmatch.filter(filenames, '*_defconfig'):
            if fnmatch.fnmatch(filename, '.*'):
                continue
            filepath = os.path.join(dirpath, filename)
            if ctime < os.path.getctime(filepath):
                return False

    for (dirpath, dirnames, filenames) in os.walk('.'):
        for filename in filenames:
            if (fnmatch.fnmatch(filename, '*~') or
                not fnmatch.fnmatch(filename, 'Kconfig*') and
                not filename == 'MAINTAINERS'):
                continue
            filepath = os.path.join(dirpath, filename)
            if ctime < os.path.getctime(filepath):
                return False

    # Detect a board that has been removed since the current board database
    # was generated
    with open(output, encoding="utf-8") as f:
        for line in f:
            if line[0] == '#' or line == '\n':
                continue
            defconfig = line.split()[6] + '_defconfig'
            if not os.path.exists(os.path.join(CONFIG_DIR, defconfig)):
                return False

    return True

### classes ###
class KconfigScanner:

    """Kconfig scanner."""

    ### constant variable only used in this class ###
    _SYMBOL_TABLE = {
        'arch' : 'SYS_ARCH',
        'cpu' : 'SYS_CPU',
        'soc' : 'SYS_SOC',
        'vendor' : 'SYS_VENDOR',
        'board' : 'SYS_BOARD',
        'config' : 'SYS_CONFIG_NAME',
        'options' : 'SYS_EXTRA_OPTIONS'
    }

    def __init__(self):
        """Scan all the Kconfig files and create a Kconfig object."""
        # Define environment variables referenced from Kconfig
        os.environ['srctree'] = os.getcwd()
        os.environ['UBOOTVERSION'] = 'dummy'
        os.environ['KCONFIG_OBJDIR'] = ''
        self._conf = kconfiglib.Kconfig(warn=False)

    def __del__(self):
        """Delete a leftover temporary file before exit.

        The scan() method of this class creates a temporay file and deletes
        it on success.  If scan() method throws an exception on the way,
        the temporary file might be left over.  In that case, it should be
        deleted in this destructor.
        """
        if hasattr(self, '_tmpfile') and self._tmpfile:
            try_remove(self._tmpfile)

    def scan(self, defconfig):
        """Load a defconfig file to obtain board parameters.

        Arguments:
          defconfig: path to the defconfig file to be processed

        Returns:
          A dictionary of board parameters.  It has a form of:
          {
              'arch': <arch_name>,
              'cpu': <cpu_name>,
              'soc': <soc_name>,
              'vendor': <vendor_name>,
              'board': <board_name>,
              'target': <target_name>,
              'config': <config_header_name>,
              'options': <extra_options>
          }
        """
        # strip special prefixes and save it in a temporary file
        fd, self._tmpfile = tempfile.mkstemp()
        with os.fdopen(fd, 'w') as f:
            for line in open(defconfig):
                colon = line.find(':CONFIG_')
                if colon == -1:
                    f.write(line)
                else:
                    f.write(line[colon + 1:])

        self._conf.load_config(self._tmpfile)
        try_remove(self._tmpfile)
        self._tmpfile = None

        params = {}

        # Get the value of CONFIG_SYS_ARCH, CONFIG_SYS_CPU, ... etc.
        # Set '-' if the value is empty.
        for key, symbol in list(self._SYMBOL_TABLE.items()):
            value = self._conf.syms.get(symbol).str_value
            if value:
                params[key] = value
            else:
                params[key] = '-'

        defconfig = os.path.basename(defconfig)
        params['target'], match, rear = defconfig.partition('_defconfig')
        assert match and not rear, '%s : invalid defconfig' % defconfig

        # fix-up for aarch64
        if params['arch'] == 'arm' and params['cpu'] == 'armv8':
            params['arch'] = 'aarch64'

        # fix-up options field. It should have the form:
        # <config name>[:comma separated config options]
        if params['options'] != '-':
            params['options'] = params['config'] + ':' + \
                                params['options'].replace(r'\"', '"')
        elif params['config'] != params['target']:
            params['options'] = params['config']

        return params

def scan_defconfigs_for_multiprocess(queue, defconfigs):
    """Scan defconfig files and queue their board parameters

    This function is intended to be passed to
    multiprocessing.Process() constructor.

    Arguments:
      queue: An instance of multiprocessing.Queue().
             The resulting board parameters are written into it.
      defconfigs: A sequence of defconfig files to be scanned.
    """
    kconf_scanner = KconfigScanner()
    for defconfig in defconfigs:
        queue.put(kconf_scanner.scan(defconfig))

def read_queues(queues, params_list):
    """Read the queues and append the data to the paramers list"""
    for q in queues:
        while not q.empty():
            params_list.append(q.get())

def scan_defconfigs(jobs=1):
    """Collect board parameters for all defconfig files.

    This function invokes multiple processes for faster processing.

    Arguments:
      jobs: The number of jobs to run simultaneously
    """
    all_defconfigs = []
    for (dirpath, dirnames, filenames) in os.walk(CONFIG_DIR):
        for filename in fnmatch.filter(filenames, '*_defconfig'):
            if fnmatch.fnmatch(filename, '.*'):
                continue
            all_defconfigs.append(os.path.join(dirpath, filename))

    total_boards = len(all_defconfigs)
    processes = []
    queues = []
    for i in range(jobs):
        defconfigs = all_defconfigs[total_boards * i // jobs :
                                    total_boards * (i + 1) // jobs]
        q = multiprocessing.Queue(maxsize=-1)
        p = multiprocessing.Process(target=scan_defconfigs_for_multiprocess,
                                    args=(q, defconfigs))
        p.start()
        processes.append(p)
        queues.append(q)

    # The resulting data should be accumulated to this list
    params_list = []

    # Data in the queues should be retrieved preriodically.
    # Otherwise, the queues would become full and subprocesses would get stuck.
    while any([p.is_alive() for p in processes]):
        read_queues(queues, params_list)
        # sleep for a while until the queues are filled
        time.sleep(SLEEP_TIME)

    # Joining subprocesses just in case
    # (All subprocesses should already have been finished)
    for p in processes:
        p.join()

    # retrieve leftover data
    read_queues(queues, params_list)

    return params_list

class MaintainersDatabase:

    """The database of board status and maintainers."""

    def __init__(self):
        """Create an empty database."""
        self.database = {}

    def get_status(self, target):
        """Return the status of the given board.

        The board status is generally either 'Active' or 'Orphan'.
        Display a warning message and return '-' if status information
        is not found.

        Returns:
          'Active', 'Orphan' or '-'.
        """
        if not target in self.database:
            print("WARNING: no status info for '%s'" % target, file=sys.stderr)
            return '-'

        tmp = self.database[target][0]
        if tmp.startswith('Maintained'):
            return 'Active'
        elif tmp.startswith('Supported'):
            return 'Active'
        elif tmp.startswith('Orphan'):
            return 'Orphan'
        else:
            print(("WARNING: %s: unknown status for '%s'" %
                                  (tmp, target)), file=sys.stderr)
            return '-'

    def get_maintainers(self, target):
        """Return the maintainers of the given board.

        Returns:
          Maintainers of the board.  If the board has two or more maintainers,
          they are separated with colons.
        """
        if not target in self.database:
            print("WARNING: no maintainers for '%s'" % target, file=sys.stderr)
            return ''

        return ':'.join(self.database[target][1])

    def parse_file(self, file):
        """Parse a MAINTAINERS file.

        Parse a MAINTAINERS file and accumulates board status and
        maintainers information.

        Arguments:
          file: MAINTAINERS file to be parsed
        """
        targets = []
        maintainers = []
        status = '-'
        for line in open(file, encoding="utf-8"):
            # Check also commented maintainers
            if line[:3] == '#M:':
                line = line[1:]
            tag, rest = line[:2], line[2:].strip()
            if tag == 'M:':
                maintainers.append(rest)
            elif tag == 'F:':
                # expand wildcard and filter by 'configs/*_defconfig'
                for f in glob.glob(rest):
                    front, match, rear = f.partition('configs/')
                    if not front and match:
                        front, match, rear = rear.rpartition('_defconfig')
                        if match and not rear:
                            targets.append(front)
            elif tag == 'S:':
                status = rest
            elif line == '\n':
                for target in targets:
                    self.database[target] = (status, maintainers)
                targets = []
                maintainers = []
                status = '-'
        if targets:
            for target in targets:
                self.database[target] = (status, maintainers)

def insert_maintainers_info(params_list):
    """Add Status and Maintainers information to the board parameters list.

    Arguments:
      params_list: A list of the board parameters
    """
    database = MaintainersDatabase()
    for (dirpath, dirnames, filenames) in os.walk('.'):
        if 'MAINTAINERS' in filenames:
            database.parse_file(os.path.join(dirpath, 'MAINTAINERS'))

    for i, params in enumerate(params_list):
        target = params['target']
        params['status'] = database.get_status(target)
        params['maintainers'] = database.get_maintainers(target)
        params_list[i] = params

def format_and_output(params_list, output):
    """Write board parameters into a file.

    Columnate the board parameters, sort lines alphabetically,
    and then write them to a file.

    Arguments:
      params_list: The list of board parameters
      output: The path to the output file
    """
    FIELDS = ('status', 'arch', 'cpu', 'soc', 'vendor', 'board', 'target',
              'options', 'maintainers')

    # First, decide the width of each column
    max_length = dict([ (f, 0) for f in FIELDS])
    for params in params_list:
        for f in FIELDS:
            max_length[f] = max(max_length[f], len(params[f]))

    output_lines = []
    for params in params_list:
        line = ''
        for f in FIELDS:
            # insert two spaces between fields like column -t would
            line += '  ' + params[f].ljust(max_length[f])
        output_lines.append(line.strip())

    # ignore case when sorting
    output_lines.sort(key=str.lower)

    with open(output, 'w', encoding="utf-8") as f:
        f.write(COMMENT_BLOCK + '\n'.join(output_lines) + '\n')

def gen_boards_cfg(output, jobs=1, force=False, quiet=False):
    """Generate a board database file.

    Arguments:
      output: The name of the output file
      jobs: The number of jobs to run simultaneously
      force: Force to generate the output even if it is new
      quiet: True to avoid printing a message if nothing needs doing
    """
    check_top_directory()

    if not force and output_is_new(output):
        if not quiet:
            print("%s is up to date. Nothing to do." % output)
        sys.exit(0)

    params_list = scan_defconfigs(jobs)
    insert_maintainers_info(params_list)
    format_and_output(params_list, output)

def main():
    try:
        cpu_count = multiprocessing.cpu_count()
    except NotImplementedError:
        cpu_count = 1

    parser = optparse.OptionParser()
    # Add options here
    parser.add_option('-f', '--force', action="store_true", default=False,
                      help='regenerate the output even if it is new')
    parser.add_option('-j', '--jobs', type='int', default=cpu_count,
                      help='the number of jobs to run simultaneously')
    parser.add_option('-o', '--output', default=OUTPUT_FILE,
                      help='output file [default=%s]' % OUTPUT_FILE)
    parser.add_option('-q', '--quiet', action="store_true", help='run silently')
    (options, args) = parser.parse_args()

    gen_boards_cfg(options.output, jobs=options.jobs, force=options.force,
                   quiet=options.quiet)

if __name__ == '__main__':
    main()

#!/usr/bin/env python2
# SPDX-License-Identifier: GPL-2.0+

# Copyright (c) 2016 Google, Inc
# Written by Simon Glass <sjg@chromium.org>
#
# Creates binary images from input files controlled by a description
#

"""See README for more information"""

from __future__ import print_function

from distutils.sysconfig import get_python_lib
import glob
import multiprocessing
import os
import site
import sys
import traceback
import unittest

# Bring in the patman and dtoc libraries
our_path = os.path.dirname(os.path.realpath(__file__))
for dirname in ['../patman', '../dtoc', '..', '../concurrencytest']:
    sys.path.insert(0, os.path.join(our_path, dirname))

# Bring in the libfdt module
sys.path.insert(0, 'scripts/dtc/pylibfdt')
sys.path.insert(0, os.path.join(our_path,
                '../../build-sandbox_spl/scripts/dtc/pylibfdt'))

# When running under python-coverage on Ubuntu 16.04, the dist-packages
# directories are dropped from the python path. Add them in so that we can find
# the elffile module. We could use site.getsitepackages() here but unfortunately
# that is not available in a virtualenv.
sys.path.append(get_python_lib())

import cmdline
import command
use_concurrent = True
try:
    from concurrencytest import ConcurrentTestSuite, fork_for_tests
except:
    use_concurrent = False
import control
import test_util

def RunTests(debug, verbosity, processes, args):
    """Run the functional tests and any embedded doctests

    Args:
        debug: True to enable debugging, which shows a full stack trace on error
        verbosity: Verbosity level to use
        args: List of positional args provided to binman. This can hold a test
            name to execute (as in 'binman -t testSections', for example)
        processes: Number of processes to use to run tests (None=same as #CPUs)
    """
    import elf_test
    import entry_test
    import fdt_test
    import ftest
    import image_test
    import test
    import doctest

    result = unittest.TestResult()
    for module in []:
        suite = doctest.DocTestSuite(module)
        suite.run(result)

    sys.argv = [sys.argv[0]]
    if debug:
        sys.argv.append('-D')
    if verbosity:
        sys.argv.append('-v%d' % verbosity)

    # Run the entry tests first ,since these need to be the first to import the
    # 'entry' module.
    test_name = args and args[0] or None
    suite = unittest.TestSuite()
    loader = unittest.TestLoader()
    for module in (entry_test.TestEntry, ftest.TestFunctional, fdt_test.TestFdt,
                   elf_test.TestElf, image_test.TestImage):
        if test_name:
            try:
                suite.addTests(loader.loadTestsFromName(test_name, module))
            except AttributeError:
                continue
        else:
            suite.addTests(loader.loadTestsFromTestCase(module))
    if use_concurrent and processes != 1:
        concurrent_suite = ConcurrentTestSuite(suite,
                fork_for_tests(processes or multiprocessing.cpu_count()))
        concurrent_suite.run(result)
    else:
        suite.run(result)

    # Remove errors which just indicate a missing test. Since Python v3.5 If an
    # ImportError or AttributeError occurs while traversing name then a
    # synthetic test that raises that error when run will be returned. These
    # errors are included in the errors accumulated by result.errors.
    if test_name:
        errors = []
        for test, err in result.errors:
            if ("has no attribute '%s'" % test_name) not in err:
                errors.append((test, err))
            result.testsRun -= 1
        result.errors = errors

    print(result)
    for test, err in result.errors:
        print(test.id(), err)
    for test, err in result.failures:
        print(err, result.failures)
    if result.skipped:
        print('%d binman test%s SKIPPED:' %
              (len(result.skipped), 's' if len(result.skipped) > 1 else ''))
        for skip_info in result.skipped:
            print('%s: %s' % (skip_info[0], skip_info[1]))
    if result.errors or result.failures:
        print('binman tests FAILED')
        return 1
    return 0

def GetEntryModules(include_testing=True):
    """Get a set of entry class implementations

    Returns:
        Set of paths to entry class filenames
    """
    glob_list = glob.glob(os.path.join(our_path, 'etype/*.py'))
    return set([os.path.splitext(os.path.basename(item))[0]
                for item in glob_list
                if include_testing or '_testing' not in item])

def RunTestCoverage():
    """Run the tests and check that we get 100% coverage"""
    glob_list = GetEntryModules(False)
    all_set = set([os.path.splitext(os.path.basename(item))[0]
                   for item in glob_list if '_testing' not in item])
    test_util.RunTestCoverage('tools/binman/binman.py', None,
            ['*test*', '*binman.py', 'tools/patman/*', 'tools/dtoc/*'],
            options.build_dir, all_set)

def RunBinman(options, args):
    """Main entry point to binman once arguments are parsed

    Args:
        options: Command-line options
        args: Non-option arguments
    """
    ret_code = 0

    if not options.debug:
        sys.tracebacklimit = 0

    if options.test:
        ret_code = RunTests(options.debug, options.verbosity, options.processes,
                            args[1:])

    elif options.test_coverage:
        RunTestCoverage()

    elif options.entry_docs:
        control.WriteEntryDocs(GetEntryModules())

    else:
        try:
            ret_code = control.Binman(options, args)
        except Exception as e:
            print('binman: %s' % e)
            if options.debug:
                print()
                traceback.print_exc()
            ret_code = 1
    return ret_code


if __name__ == "__main__":
    (options, args) = cmdline.ParseArgs(sys.argv)
    ret_code = RunBinman(options, args)
    sys.exit(ret_code)

/*
 * (C) Copyright 2004
 * Texas Instruments
 * Richard Woodruff <r-woodruff2@ti.com>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <gj@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/arch/bits.h>
#include <asm/arch/omap2420.h>
#include <asm/proc-armv/ptrace.h>

extern void reset_cpu(ulong addr);
#define TIMER_LOAD_VAL 0

/* macro to read the 32 bit timer */
#define READ_TIMER (*(volatile ulong *)(CFG_TIMERBASE+TCRR))

#ifdef CONFIG_USE_IRQ
/* enable IRQ interrupts */
void enable_interrupts (void)
{
	unsigned long temp;
	__asm__ __volatile__("mrs %0, cpsr\n"
						 "bic %0, %0, #0x80\n"
						 "msr cpsr_c, %0"
						 : "=r" (temp)
						 :
						 : "memory");
}

/*
 * disable IRQ/FIQ interrupts
 * returns true if interrupts had been enabled before we disabled them
 */
int disable_interrupts (void)
{
	unsigned long old,temp;
	__asm__ __volatile__("mrs %0, cpsr\n"
						 "orr %1, %0, #0xc0\n"
						 "msr cpsr_c, %1"
						 : "=r" (old), "=r" (temp)
						 :
						 : "memory");
	return(old & 0x80) == 0;
}
#else
void enable_interrupts (void)
{
	return;
}
int disable_interrupts (void)
{
	return 0;
}
#endif


void bad_mode (void)
{
	panic ("Resetting CPU ...\n");
	reset_cpu (0);
}

void show_regs (struct pt_regs *regs)
{
	unsigned long flags;
	const char *processor_modes[] = {
		"USER_26",  "FIQ_26",   "IRQ_26",   "SVC_26",
		"UK4_26",   "UK5_26",   "UK6_26",   "UK7_26",
		"UK8_26",   "UK9_26",   "UK10_26",  "UK11_26",
		"UK12_26",  "UK13_26",  "UK14_26",  "UK15_26",
		"USER_32",  "FIQ_32",   "IRQ_32",   "SVC_32",
		"UK4_32",   "UK5_32",   "UK6_32",   "ABT_32",
		"UK8_32",   "UK9_32",   "UK10_32",  "UND_32",
		"UK12_32",  "UK13_32",  "UK14_32",  "SYS_32",
	};

	flags = condition_codes (regs);

	printf ("pc : [<%08lx>]    lr : [<%08lx>]\n"
			"sp : %08lx  ip : %08lx  fp : %08lx\n",
			instruction_pointer (regs),
			regs->ARM_lr, regs->ARM_sp, regs->ARM_ip, regs->ARM_fp);
	printf ("r10: %08lx  r9 : %08lx  r8 : %08lx\n",
			regs->ARM_r10, regs->ARM_r9, regs->ARM_r8);
	printf ("r7 : %08lx  r6 : %08lx  r5 : %08lx  r4 : %08lx\n",
			regs->ARM_r7, regs->ARM_r6, regs->ARM_r5, regs->ARM_r4);
	printf ("r3 : %08lx  r2 : %08lx  r1 : %08lx  r0 : %08lx\n",
			regs->ARM_r3, regs->ARM_r2, regs->ARM_r1, regs->ARM_r0);
	printf ("Flags: %c%c%c%c",
			flags & CC_N_BIT ? 'N' : 'n',
			flags & CC_Z_BIT ? 'Z' : 'z',
			flags & CC_C_BIT ? 'C' : 'c', flags & CC_V_BIT ? 'V' : 'v');
	printf ("  IRQs %s  FIQs %s  Mode %s%s\n",
			interrupts_enabled (regs) ? "on" : "off",
			fast_interrupts_enabled (regs) ? "on" : "off",
			processor_modes[processor_mode (regs)],
			thumb_mode (regs) ? " (T)" : "");
}

void do_undefined_instruction (struct pt_regs *pt_regs)
{
	printf ("undefined instruction\n");
	show_regs (pt_regs);
	bad_mode ();
}

void do_software_interrupt (struct pt_regs *pt_regs)
{
	printf ("software interrupt\n");
	show_regs (pt_regs);
	bad_mode ();
}

void do_prefetch_abort (struct pt_regs *pt_regs)
{
	printf ("prefetch abort\n");
	show_regs (pt_regs);
	bad_mode ();
}

void do_data_abort (struct pt_regs *pt_regs)
{
	printf ("data abort\n");
	show_regs (pt_regs);
	bad_mode ();
}

void do_not_used (struct pt_regs *pt_regs)
{
	printf ("not used\n");
	show_regs (pt_regs);
	bad_mode ();
}

void do_fiq (struct pt_regs *pt_regs)
{
	printf ("fast interrupt request\n");
	show_regs (pt_regs);
	bad_mode ();
}

void do_irq (struct pt_regs *pt_regs)
{
	printf ("interrupt request\n");
	show_regs (pt_regs);
	bad_mode ();
}

static ulong timestamp;
static ulong lastinc;

/* nothing really to do with interrupts, just starts up a counter. */
int interrupt_init (void)
{
	int32_t val;

	/* Start the counter ticking up */
	*((int32_t *) (CFG_TIMERBASE + TLDR)) = TIMER_LOAD_VAL;	/* reload value on overflow*/
	val = (CFG_PVT << 2) | BIT5 | BIT1 | BIT0;		/* mask to enable timer*/
	*((int32_t *) (CFG_TIMERBASE + TCLR)) = val;	/* start timer */

	reset_timer_masked(); /* init the timestamp and lastinc value */

	return(0);
}

/*
 * timer without interrupts
 */
void reset_timer (void)
{
	reset_timer_masked ();
}

ulong get_timer (ulong base)
{
	return get_timer_masked () - base;
}

void set_timer (ulong t)
{
	timestamp = t;
}

/* delay x useconds AND perserve advance timstamp value */
void udelay (unsigned long usec)
{
	ulong tmo, tmp;

	if (usec >= 1000) {			/* if "big" number, spread normalization to seconds */
		tmo = usec / 1000;		/* start to normalize for usec to ticks per sec */
		tmo *= CFG_HZ;			/* find number of "ticks" to wait to achieve target */
		tmo /= 1000;			/* finish normalize. */
	} else {					/* else small number, don't kill it prior to HZ multiply */
		tmo = usec * CFG_HZ;
		tmo /= (1000*1000);
	}

	tmp = get_timer (0);		/* get current timestamp */
	if ( (tmo + tmp + 1) < tmp )/* if setting this forward will roll time stamp */
		reset_timer_masked ();	/* reset "advancing" timestamp to 0, set lastinc value */
	else
		tmo	+= tmp;				/* else, set advancing stamp wake up time */
	while (get_timer_masked () < tmo)/* loop till event */
		/*NOP*/;
}

void reset_timer_masked (void)
{
	/* reset time */
	lastinc = READ_TIMER;		/* capture current incrementer value time */
	timestamp = 0;				/* start "advancing" time stamp from 0 */
}

ulong get_timer_masked (void)
{
	ulong now = READ_TIMER;		/* current tick value */

	if (now >= lastinc)			/* normal mode (non roll) */
		timestamp += (now - lastinc); /* move stamp fordward with absoulte diff ticks */
	else						/* we have rollover of incrementer */
		timestamp += (0xFFFFFFFF - lastinc) + now;
	lastinc = now;
	return timestamp;
}

/* waits specified delay value and resets timestamp */
void udelay_masked (unsigned long usec)
{
	ulong tmo;

	if (usec >= 1000) {			/* if "big" number, spread normalization to seconds */
		tmo = usec / 1000;		/* start to normalize for usec to ticks per sec */
		tmo *= CFG_HZ;			/* find number of "ticks" to wait to achieve target */
		tmo /= 1000;			/* finish normalize. */
	} else {					/* else small number, don't kill it prior to HZ multiply */
		tmo = usec * CFG_HZ;
		tmo /= (1000*1000);
	}
	reset_timer_masked ();		/* set "advancing" timestamp to 0, set lastinc vaule */
	while (get_timer_masked () < tmo) /* wait for time stamp to overtake tick number.*/
		/* NOP */;
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk (void)
{
	ulong tbclk;
	tbclk = CFG_HZ;
	return tbclk;
}

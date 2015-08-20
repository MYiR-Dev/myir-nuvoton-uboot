/*
 * Copyright (C) 2014, Bin Meng <bmeng.cn@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/pci.h>
#include <asm/post.h>
#include <asm/arch/device.h>
#include <asm/arch/tnc.h>
#include <asm/fsp/fsp_support.h>
#include <asm/processor.h>

static void unprotect_spi_flash(void)
{
	u32 bc;

	bc = x86_pci_read_config32(TNC_LPC, 0xd8);
	bc |= 0x1;	/* unprotect the flash */
	x86_pci_write_config32(TNC_LPC, 0xd8, bc);
}

int arch_cpu_init(void)
{
	int ret;

	post_code(POST_CPU_INIT);
#ifdef CONFIG_SYS_X86_TSC_TIMER
	timer_set_base(rdtsc());
#endif

	ret = x86_cpu_init_f();
	if (ret)
		return ret;

	return 0;
}

void cpu_irq_init(void)
{
	struct tnc_rcba *rcba;
	u32 base;

	base = x86_pci_read_config32(TNC_LPC, LPC_RCBA);
	base &= ~MEM_BAR_EN;
	rcba = (struct tnc_rcba *)base;

	/* Make sure all internal PCI devices are using INTA */
	writel(INTA, &rcba->d02ip);
	writel(INTA, &rcba->d03ip);
	writel(INTA, &rcba->d27ip);
	writel(INTA, &rcba->d31ip);
	writel(INTA, &rcba->d23ip);
	writel(INTA, &rcba->d24ip);
	writel(INTA, &rcba->d25ip);
	writel(INTA, &rcba->d26ip);

	/*
	 * Route TunnelCreek PCI device interrupt pin to PIRQ
	 *
	 * Since PCIe downstream ports received INTx are routed to PIRQ
	 * A/B/C/D directly and not configurable, we have to route PCIe
	 * root ports' INTx to PIRQ A/B/C/D as well. For other devices
	 * on TunneCreek, route them to PIRQ E/F/G/H.
	 */
	writew(PIRQE, &rcba->d02ir);
	writew(PIRQF, &rcba->d03ir);
	writew(PIRQG, &rcba->d27ir);
	writew(PIRQH, &rcba->d31ir);
	writew(PIRQA, &rcba->d23ir);
	writew(PIRQB, &rcba->d24ir);
	writew(PIRQC, &rcba->d25ir);
	writew(PIRQD, &rcba->d26ir);
}

int arch_misc_init(void)
{
	unprotect_spi_flash();

	return pirq_init();
}

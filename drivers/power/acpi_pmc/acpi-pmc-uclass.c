// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright 2019 Google LLC
 */

#define LOG_CATEGORY UCLASS_ACPI_PMC

#include <common.h>
#include <acpi_s3.h>
#include <dm.h>
#include <log.h>
#include <asm/io.h>
#include <power/acpi_pmc.h>

enum {
	PM1_STS		= 0x00,
	PM1_EN		= 0x02,
	PM1_CNT		= 0x04,

	GPE0_STS	= 0x20,
	GPE0_EN		= 0x30,
};

struct tco_regs {
	u32 tco_rld;
	u32 tco_sts;
	u32 tco1_cnt;
	u32 tco_tmr;
};

enum {
	TCO_STS_TIMEOUT			= 1 << 3,
	TCO_STS_SECOND_TO_STS		= 1 << 17,
	TCO1_CNT_HLT			= 1 << 11,
};

static void pmc_fill_pm_reg_info(struct udevice *dev)
{
	struct acpi_pmc_upriv *upriv = dev_get_uclass_priv(dev);
	int i;

	upriv->pm1_sts = inw(upriv->acpi_base + PM1_STS);
	upriv->pm1_en = inw(upriv->acpi_base + PM1_EN);
	upriv->pm1_cnt = inw(upriv->acpi_base + PM1_CNT);

	log_debug("pm1_sts: %04x pm1_en: %04x pm1_cnt: %08x\n",
		  upriv->pm1_sts, upriv->pm1_en, upriv->pm1_cnt);

	for (i = 0; i < GPE0_REG_MAX; i++) {
		upriv->gpe0_sts[i] = inl(upriv->acpi_base + GPE0_STS + i * 4);
		upriv->gpe0_en[i] = inl(upriv->acpi_base + GPE0_EN + i * 4);
		log_debug("gpe0_sts[%d]: %08x gpe0_en[%d]: %08x\n", i,
			  upriv->gpe0_sts[i], i, upriv->gpe0_en[i]);
	}
}

int pmc_disable_tco_base(ulong tco_base)
{
	struct tco_regs *regs = (struct tco_regs *)tco_base;

	debug("tco_base %lx = %x\n", (ulong)&regs->tco1_cnt, TCO1_CNT_HLT);
	setio_32(&regs->tco1_cnt, TCO1_CNT_HLT);

	return 0;
}

int pmc_init(struct udevice *dev)
{
	const struct acpi_pmc_ops *ops = acpi_pmc_get_ops(dev);
	int ret;

	pmc_fill_pm_reg_info(dev);
	if (!ops->init)
		return -ENOSYS;

	ret = ops->init(dev);
	if (ret)
		return log_msg_ret("Failed to init pmc", ret);

#ifdef DEBUG
	pmc_dump_info(dev);
#endif

	return 0;
}

int pmc_prev_sleep_state(struct udevice *dev)
{
	struct acpi_pmc_upriv *upriv = dev_get_uclass_priv(dev);
	const struct acpi_pmc_ops *ops = acpi_pmc_get_ops(dev);
	int prev_sleep_state = ACPI_S0;	/* Default to S0 */

	if (upriv->pm1_sts & WAK_STS) {
		switch (acpi_sleep_from_pm1(upriv->pm1_cnt)) {
		case ACPI_S3:
			if (IS_ENABLED(HAVE_ACPI_RESUME))
				prev_sleep_state = ACPI_S3;
			break;
		case ACPI_S5:
			prev_sleep_state = ACPI_S5;
			break;
		default:
			break;
		}

		/* Clear SLP_TYP */
		outl(upriv->pm1_cnt & ~SLP_TYP, upriv->acpi_base + PM1_CNT);
	}

	if (!ops->prev_sleep_state)
		return prev_sleep_state;

	return ops->prev_sleep_state(dev, prev_sleep_state);
}

int pmc_disable_tco(struct udevice *dev)
{
	const struct acpi_pmc_ops *ops = acpi_pmc_get_ops(dev);

	pmc_fill_pm_reg_info(dev);
	if (!ops->disable_tco)
		return -ENOSYS;

	return ops->disable_tco(dev);
}

int pmc_global_reset_set_enable(struct udevice *dev, bool enable)
{
	const struct acpi_pmc_ops *ops = acpi_pmc_get_ops(dev);

	if (!ops->global_reset_set_enable)
		return -ENOSYS;

	return ops->global_reset_set_enable(dev, enable);
}

void pmc_dump_info(struct udevice *dev)
{
	struct acpi_pmc_upriv *upriv = dev_get_uclass_priv(dev);
	int i;

	printf("Device: %s\n", dev->name);
	printf("ACPI base %x, pmc_bar0 %p, pmc_bar2 %p, gpe_cfg %p\n",
	       upriv->acpi_base, upriv->pmc_bar0, upriv->pmc_bar2,
	       upriv->gpe_cfg);
	printf("pm1_sts: %04x pm1_en: %04x pm1_cnt: %08x\n",
	       upriv->pm1_sts, upriv->pm1_en, upriv->pm1_cnt);

	for (i = 0; i < GPE0_REG_MAX; i++) {
		printf("gpe0_sts[%d]: %08x gpe0_en[%d]: %08x\n", i,
		       upriv->gpe0_sts[i], i, upriv->gpe0_en[i]);
	}

	printf("prsts: %08x\n", upriv->prsts);
	printf("tco_sts:   %04x %04x\n", upriv->tco1_sts, upriv->tco2_sts);
	printf("gen_pmcon1: %08x gen_pmcon2: %08x gen_pmcon3: %08x\n",
	       upriv->gen_pmcon1, upriv->gen_pmcon2, upriv->gen_pmcon3);
}

int pmc_ofdata_to_uc_platdata(struct udevice *dev)
{
	struct acpi_pmc_upriv *upriv = dev_get_uclass_priv(dev);
	int ret;

	ret = dev_read_u32(dev, "gpe0-dwx-mask", &upriv->gpe0_dwx_mask);
	if (ret)
		return log_msg_ret("no gpe0-dwx-mask", ret);
	ret = dev_read_u32(dev, "gpe0-dwx-shift-base",
			   &upriv->gpe0_dwx_shift_base);
	if (ret)
		return log_msg_ret("no gpe0-dwx-shift-base", ret);
	ret = dev_read_u32(dev, "gpe0-sts", &upriv->gpe0_sts_reg);
	if (ret)
		return log_msg_ret("no gpe0-sts", ret);
	upriv->gpe0_sts_reg += upriv->acpi_base;
	ret = dev_read_u32(dev, "gpe0-en", &upriv->gpe0_en_reg);
	if (ret)
		return log_msg_ret("no gpe0-en", ret);
	upriv->gpe0_en_reg += upriv->acpi_base;

	return 0;
}

UCLASS_DRIVER(acpi_pmc) = {
	.id		= UCLASS_ACPI_PMC,
	.name		= "power-mgr",
	.per_device_auto_alloc_size	= sizeof(struct acpi_pmc_upriv),
};

/*
 * Copyright (C) 2014 Samsung Electronics
 * Przemyslaw Marczak <p.marczak@samsung.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/power.h>
#include <asm/arch/clock.h>
#include <asm/arch/gpio.h>
#include <asm/gpio.h>
#include <asm/arch/cpu.h>
#include <power/pmic.h>
#include <power/max77686_pmic.h>
#include <errno.h>
#include <mmc.h>
#include <usb.h>
#include <usb/s3c_udc.h>
#include <samsung/misc.h>
#include "setup.h"

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_BOARD_TYPES
/* Odroid board types */
enum {
	ODROID_TYPE_U3,
	ODROID_TYPE_X2,
	ODROID_TYPES,
};

void set_board_type(void)
{
	/* Set GPA1 pin 1 to HI - enable XCL205 output */
	writel(XCL205_EN_GPIO_CON_CFG, XCL205_EN_GPIO_CON);
	writel(XCL205_EN_GPIO_DAT_CFG, XCL205_EN_GPIO_CON + 0x4);
	writel(XCL205_EN_GPIO_PUD_CFG, XCL205_EN_GPIO_CON + 0x8);
	writel(XCL205_EN_GPIO_DRV_CFG, XCL205_EN_GPIO_CON + 0xc);

	/* Set GPC1 pin 2 to IN - check XCL205 output state */
	writel(XCL205_STATE_GPIO_CON_CFG, XCL205_STATE_GPIO_CON);
	writel(XCL205_STATE_GPIO_PUD_CFG, XCL205_STATE_GPIO_CON + 0x8);

	/* XCL205 - needs some latch time */
	sdelay(200000);

	/* Check GPC1 pin2 - LED supplied by XCL205 - X2 only */
	if (readl(XCL205_STATE_GPIO_DAT) & (1 << XCL205_STATE_GPIO_PIN))
		gd->board_type = ODROID_TYPE_X2;
	else
		gd->board_type = ODROID_TYPE_U3;
}

const char *get_board_type(void)
{
	const char *board_type[] = {"u3", "x2"};

	return board_type[gd->board_type];
}
#endif

#ifdef CONFIG_SET_DFU_ALT_INFO
char *get_dfu_alt_system(char *interface, char *devstr)
{
	return getenv("dfu_alt_system");
}

char *get_dfu_alt_boot(char *interface, char *devstr)
{
	struct mmc *mmc;
	char *alt_boot;
	int dev_num;

	dev_num = simple_strtoul(devstr, NULL, 10);

	mmc = find_mmc_device(dev_num);
	if (!mmc)
		return NULL;

	if (mmc_init(mmc))
		return NULL;

	alt_boot = IS_SD(mmc) ? CONFIG_DFU_ALT_BOOT_SD :
				CONFIG_DFU_ALT_BOOT_EMMC;

	return alt_boot;
}
#endif

static void board_clock_init(void)
{
	unsigned int set, clr, clr_src_cpu, clr_pll_con0, clr_src_dmc;
	struct exynos4x12_clock *clk = (struct exynos4x12_clock *)
						samsung_get_base_clock();

	/*
	 * CMU_CPU clocks src to MPLL
	 * Bit values:                 0  ; 1
	 * MUX_APLL_SEL:        FIN_PLL   ; FOUT_APLL
	 * MUX_CORE_SEL:        MOUT_APLL ; SCLK_MPLL
	 * MUX_HPM_SEL:         MOUT_APLL ; SCLK_MPLL_USER_C
	 * MUX_MPLL_USER_SEL_C: FIN_PLL   ; SCLK_MPLL
	*/
	clr_src_cpu = MUX_APLL_SEL(1) | MUX_CORE_SEL(1) |
		      MUX_HPM_SEL(1) | MUX_MPLL_USER_SEL_C(1);
	set = MUX_APLL_SEL(0) | MUX_CORE_SEL(1) | MUX_HPM_SEL(1) |
	      MUX_MPLL_USER_SEL_C(1);

	clrsetbits_le32(&clk->src_cpu, clr_src_cpu, set);

	/* Wait for mux change */
	while (readl(&clk->mux_stat_cpu) & MUX_STAT_CPU_CHANGING)
		continue;

	/* Set APLL to 1000MHz */
	clr_pll_con0 = SDIV(7) | PDIV(63) | MDIV(1023) | FSEL(1);
	set = SDIV(0) | PDIV(3) | MDIV(125) | FSEL(1);

	clrsetbits_le32(&clk->apll_con0, clr_pll_con0, set);

	/* Wait for PLL to be locked */
	while (!(readl(&clk->apll_con0) & PLL_LOCKED_BIT))
		continue;

	/* Set CMU_CPU clocks src to APLL */
	set = MUX_APLL_SEL(1) | MUX_CORE_SEL(0) | MUX_HPM_SEL(0) |
	      MUX_MPLL_USER_SEL_C(1);
	clrsetbits_le32(&clk->src_cpu, clr_src_cpu, set);

	/* Wait for mux change */
	while (readl(&clk->mux_stat_cpu) & MUX_STAT_CPU_CHANGING)
		continue;

	set = CORE_RATIO(0) | COREM0_RATIO(2) | COREM1_RATIO(5) |
	      PERIPH_RATIO(0) | ATB_RATIO(4) | PCLK_DBG_RATIO(1) |
	      APLL_RATIO(0) | CORE2_RATIO(0);
	/*
	 * Set dividers for MOUTcore = 1000 MHz
	 * coreout =      MOUT / (ratio + 1) = 1000 MHz (0)
	 * corem0 =     armclk / (ratio + 1) = 333 MHz (2)
	 * corem1 =     armclk / (ratio + 1) = 166 MHz (5)
	 * periph =     armclk / (ratio + 1) = 1000 MHz (0)
	 * atbout =       MOUT / (ratio + 1) = 200 MHz (4)
	 * pclkdbgout = atbout / (ratio + 1) = 100 MHz (1)
	 * sclkapll = MOUTapll / (ratio + 1) = 1000 MHz (0)
	 * core2out = core_out / (ratio + 1) = 1000 MHz (0) (armclk)
	*/
	clr = CORE_RATIO(7) | COREM0_RATIO(7) | COREM1_RATIO(7) |
	      PERIPH_RATIO(7) | ATB_RATIO(7) | PCLK_DBG_RATIO(7) |
	      APLL_RATIO(7) | CORE2_RATIO(7);

	clrsetbits_le32(&clk->div_cpu0, clr, set);

	/* Wait for divider ready status */
	while (readl(&clk->div_stat_cpu0) & DIV_STAT_CPU0_CHANGING)
		continue;

	/*
	 * For MOUThpm = 1000 MHz (MOUTapll)
	 * doutcopy = MOUThpm / (ratio + 1) = 200 (4)
	 * sclkhpm = doutcopy / (ratio + 1) = 200 (4)
	 * cores_out = armclk / (ratio + 1) = 200 (4)
	 */
	clr = COPY_RATIO(7) | HPM_RATIO(7) | CORES_RATIO(7);
	set = COPY_RATIO(4) | HPM_RATIO(4) | CORES_RATIO(4);

	clrsetbits_le32(&clk->div_cpu1, clr, set);

	/* Wait for divider ready status */
	while (readl(&clk->div_stat_cpu1) & DIV_STAT_CPU1_CHANGING)
		continue;

	/*
	 * Set CMU_DMC clocks src to APLL
	 * Bit values:             0  ; 1
	 * MUX_C2C_SEL:      SCLKMPLL ; SCLKAPLL
	 * MUX_DMC_BUS_SEL:  SCLKMPLL ; SCLKAPLL
	 * MUX_DPHY_SEL:     SCLKMPLL ; SCLKAPLL
	 * MUX_MPLL_SEL:     FINPLL   ; MOUT_MPLL_FOUT
	 * MUX_PWI_SEL:      0110 (MPLL); 0111 (EPLL); 1000 (VPLL); 0(XXTI)
	 * MUX_G2D_ACP0_SEL: SCLKMPLL ; SCLKAPLL
	 * MUX_G2D_ACP1_SEL: SCLKEPLL ; SCLKVPLL
	 * MUX_G2D_ACP_SEL:  OUT_ACP0 ; OUT_ACP1
	*/
	clr_src_dmc = MUX_C2C_SEL(1) | MUX_DMC_BUS_SEL(1) |
		      MUX_DPHY_SEL(1) | MUX_MPLL_SEL(1) |
		      MUX_PWI_SEL(15) | MUX_G2D_ACP0_SEL(1) |
		      MUX_G2D_ACP1_SEL(1) | MUX_G2D_ACP_SEL(1);
	set = MUX_C2C_SEL(1) | MUX_DMC_BUS_SEL(1) | MUX_DPHY_SEL(1) |
	      MUX_MPLL_SEL(0) | MUX_PWI_SEL(0) | MUX_G2D_ACP0_SEL(1) |
	      MUX_G2D_ACP1_SEL(1) | MUX_G2D_ACP_SEL(1);

	clrsetbits_le32(&clk->src_dmc, clr_src_dmc, set);

	/* Wait for mux change */
	while (readl(&clk->mux_stat_dmc) & MUX_STAT_DMC_CHANGING)
		continue;

	/* Set MPLL to 800MHz */
	set = SDIV(0) | PDIV(3) | MDIV(100) | FSEL(0) | PLL_ENABLE(1);

	clrsetbits_le32(&clk->mpll_con0, clr_pll_con0, set);

	/* Wait for PLL to be locked */
	while (!(readl(&clk->mpll_con0) & PLL_LOCKED_BIT))
		continue;

	/* Switch back CMU_DMC mux */
	set = MUX_C2C_SEL(0) | MUX_DMC_BUS_SEL(0) | MUX_DPHY_SEL(0) |
	      MUX_MPLL_SEL(1) | MUX_PWI_SEL(8) | MUX_G2D_ACP0_SEL(0) |
	      MUX_G2D_ACP1_SEL(0) | MUX_G2D_ACP_SEL(0);

	clrsetbits_le32(&clk->src_dmc, clr_src_dmc, set);

	/* Wait for mux change */
	while (readl(&clk->mux_stat_dmc) & MUX_STAT_DMC_CHANGING)
		continue;

	/* CLK_DIV_DMC0 */
	clr = ACP_RATIO(7) | ACP_PCLK_RATIO(7) | DPHY_RATIO(7) |
	      DMC_RATIO(7) | DMCD_RATIO(7) | DMCP_RATIO(7);
	/*
	 * For:
	 * MOUTdmc = 800 MHz
	 * MOUTdphy = 800 MHz
	 *
	 * aclk_acp = MOUTdmc / (ratio + 1) = 200 (3)
	 * pclk_acp = aclk_acp / (ratio + 1) = 100 (1)
	 * sclk_dphy = MOUTdphy / (ratio + 1) = 400 (1)
	 * sclk_dmc = MOUTdmc / (ratio + 1) = 400 (1)
	 * aclk_dmcd = sclk_dmc / (ratio + 1) = 200 (1)
	 * aclk_dmcp = aclk_dmcd / (ratio + 1) = 100 (1)
	 */
	set = ACP_RATIO(3) | ACP_PCLK_RATIO(1) | DPHY_RATIO(1) |
	      DMC_RATIO(1) | DMCD_RATIO(1) | DMCP_RATIO(1);

	clrsetbits_le32(&clk->div_dmc0, clr, set);

	/* Wait for divider ready status */
	while (readl(&clk->div_stat_dmc0) & DIV_STAT_DMC0_CHANGING)
		continue;

	/* CLK_DIV_DMC1 */
	clr = G2D_ACP_RATIO(15) | C2C_RATIO(7) | PWI_RATIO(15) |
	      C2C_ACLK_RATIO(7) | DVSEM_RATIO(127) | DPM_RATIO(127);
	/*
	 * For:
	 * MOUTg2d = 800 MHz
	 * MOUTc2c = 800 Mhz
	 * MOUTpwi = 108 MHz
	 *
	 * sclk_g2d_acp = MOUTg2d / (ratio + 1) = 200 (3)
	 * sclk_c2c = MOUTc2c / (ratio + 1) = 400 (1)
	 * aclk_c2c = sclk_c2c / (ratio + 1) = 200 (1)
	 * sclk_pwi = MOUTpwi / (ratio + 1) = 18 (5)
	 */
	set = G2D_ACP_RATIO(3) | C2C_RATIO(1) | PWI_RATIO(5) |
	      C2C_ACLK_RATIO(1) | DVSEM_RATIO(1) | DPM_RATIO(1);

	clrsetbits_le32(&clk->div_dmc1, clr, set);

	/* Wait for divider ready status */
	while (readl(&clk->div_stat_dmc1) & DIV_STAT_DMC1_CHANGING)
		continue;

	/* CLK_SRC_PERIL0 */
	clr = UART0_SEL(15) | UART1_SEL(15) | UART2_SEL(15) |
	      UART3_SEL(15) | UART4_SEL(15);
	/*
	 * Set CLK_SRC_PERIL0 clocks src to MPLL
	 * src values: 0(XXTI); 1(XusbXTI); 2(SCLK_HDMI24M); 3(SCLK_USBPHY0);
	 *             5(SCLK_HDMIPHY); 6(SCLK_MPLL_USER_T); 7(SCLK_EPLL);
	 *             8(SCLK_VPLL)
	 *
	 * Set all to SCLK_MPLL_USER_T
	 */
	set = UART0_SEL(6) | UART1_SEL(6) | UART2_SEL(6) | UART3_SEL(6) |
	      UART4_SEL(6);

	clrsetbits_le32(&clk->src_peril0, clr, set);

	/* CLK_DIV_PERIL0 */
	clr = UART0_RATIO(15) | UART1_RATIO(15) | UART2_RATIO(15) |
	      UART3_RATIO(15) | UART4_RATIO(15);
	/*
	 * For MOUTuart0-4: 800MHz
	 *
	 * SCLK_UARTx = MOUTuartX / (ratio + 1) = 100 (7)
	*/
	set = UART0_RATIO(7) | UART1_RATIO(7) | UART2_RATIO(7) |
	      UART3_RATIO(7) | UART4_RATIO(7);

	clrsetbits_le32(&clk->div_peril0, clr, set);

	while (readl(&clk->div_stat_peril0) & DIV_STAT_PERIL0_CHANGING)
		continue;

	/* CLK_DIV_FSYS1 */
	clr = MMC0_RATIO(15) | MMC0_PRE_RATIO(255) | MMC1_RATIO(15) |
	      MMC1_PRE_RATIO(255);
	/*
	 * For MOUTmmc0-3 = 800 MHz (MPLL)
	 *
	 * DOUTmmc1 = MOUTmmc1 / (ratio + 1) = 100 (7)
	 * sclk_mmc1 = DOUTmmc1 / (ratio + 1) = 50 (1)
	 * DOUTmmc0 = MOUTmmc0 / (ratio + 1) = 100 (7)
	 * sclk_mmc0 = DOUTmmc0 / (ratio + 1) = 50 (1)
	*/
	set = MMC0_RATIO(7) | MMC0_PRE_RATIO(1) | MMC1_RATIO(7) |
	      MMC1_PRE_RATIO(1);

	clrsetbits_le32(&clk->div_fsys1, clr, set);

	/* Wait for divider ready status */
	while (readl(&clk->div_stat_fsys1) & DIV_STAT_FSYS1_CHANGING)
		continue;

	/* CLK_DIV_FSYS2 */
	clr = MMC2_RATIO(15) | MMC2_PRE_RATIO(255) | MMC3_RATIO(15) |
	      MMC3_PRE_RATIO(255);
	/*
	 * For MOUTmmc0-3 = 800 MHz (MPLL)
	 *
	 * DOUTmmc3 = MOUTmmc3 / (ratio + 1) = 100 (7)
	 * sclk_mmc3 = DOUTmmc3 / (ratio + 1) = 50 (1)
	 * DOUTmmc2 = MOUTmmc2 / (ratio + 1) = 100 (7)
	 * sclk_mmc2 = DOUTmmc2 / (ratio + 1) = 50 (1)
	*/
	set = MMC2_RATIO(7) | MMC2_PRE_RATIO(1) | MMC3_RATIO(7) |
	      MMC3_PRE_RATIO(1);

	clrsetbits_le32(&clk->div_fsys2, clr, set);

	/* Wait for divider ready status */
	while (readl(&clk->div_stat_fsys2) & DIV_STAT_FSYS2_CHANGING)
		continue;

	/* CLK_DIV_FSYS3 */
	clr = MMC4_RATIO(15) | MMC4_PRE_RATIO(255);
	/*
	 * For MOUTmmc4 = 800 MHz (MPLL)
	 *
	 * DOUTmmc4 = MOUTmmc4 / (ratio + 1) = 100 (7)
	 * sclk_mmc4 = DOUTmmc4 / (ratio + 1) = 100 (0)
	*/
	set = MMC4_RATIO(7) | MMC4_PRE_RATIO(0);

	clrsetbits_le32(&clk->div_fsys3, clr, set);

	/* Wait for divider ready status */
	while (readl(&clk->div_stat_fsys3) & DIV_STAT_FSYS3_CHANGING)
		continue;

	return;
}

static void board_gpio_init(void)
{
	/* eMMC Reset Pin */
	gpio_request(EXYNOS4X12_GPIO_K12, "eMMC Reset");

	gpio_cfg_pin(EXYNOS4X12_GPIO_K12, S5P_GPIO_FUNC(0x1));
	gpio_set_pull(EXYNOS4X12_GPIO_K12, S5P_GPIO_PULL_NONE);
	gpio_set_drv(EXYNOS4X12_GPIO_K12, S5P_GPIO_DRV_4X);

	/* Enable FAN (Odroid U3) */
	gpio_request(EXYNOS4X12_GPIO_D00, "FAN Control");

	gpio_set_pull(EXYNOS4X12_GPIO_D00, S5P_GPIO_PULL_UP);
	gpio_set_drv(EXYNOS4X12_GPIO_D00, S5P_GPIO_DRV_4X);
	gpio_direction_output(EXYNOS4X12_GPIO_D00, 1);

	/* OTG Vbus output (Odroid U3+) */
	gpio_request(EXYNOS4X12_GPIO_L20, "OTG Vbus");

	gpio_set_pull(EXYNOS4X12_GPIO_L20, S5P_GPIO_PULL_NONE);
	gpio_set_drv(EXYNOS4X12_GPIO_L20, S5P_GPIO_DRV_4X);
	gpio_direction_output(EXYNOS4X12_GPIO_L20, 0);

	/* OTG INT (Odroid U3+) */
	gpio_request(EXYNOS4X12_GPIO_X31, "OTG INT");

	gpio_set_pull(EXYNOS4X12_GPIO_X31, S5P_GPIO_PULL_UP);
	gpio_set_drv(EXYNOS4X12_GPIO_X31, S5P_GPIO_DRV_4X);
	gpio_direction_input(EXYNOS4X12_GPIO_X31);

	/* Blue LED (Odroid X2/U2/U3) */
	gpio_request(EXYNOS4X12_GPIO_C10, "Blue LED");

	gpio_direction_output(EXYNOS4X12_GPIO_C10, 0);

#ifdef CONFIG_CMD_USB
	/* USB3503A Reference frequency */
	gpio_request(EXYNOS4X12_GPIO_X30, "USB3503A RefFreq");

	/* USB3503A Connect */
	gpio_request(EXYNOS4X12_GPIO_X34, "USB3503A Connect");

	/* USB3503A Reset */
	gpio_request(EXYNOS4X12_GPIO_X35, "USB3503A Reset");
#endif
}

static int pmic_init_max77686(void)
{
	struct pmic *p = pmic_get("MAX77686_PMIC");

	if (pmic_probe(p))
		return -ENODEV;

	/* Set LDO Voltage */
	max77686_set_ldo_voltage(p, 20, 1800000);	/* LDO20 eMMC */
	max77686_set_ldo_voltage(p, 21, 2800000);	/* LDO21 SD */
	max77686_set_ldo_voltage(p, 22, 2800000);	/* LDO22 eMMC */

	return 0;
}

int exynos_early_init_f(void)
{
	board_clock_init();

	return 0;
}

int exynos_init(void)
{
	/* The last MB of memory is reserved for secure firmware */
	gd->ram_size -= SZ_1M;
	gd->bd->bi_dram[CONFIG_NR_DRAM_BANKS - 1].size -= SZ_1M;

	board_gpio_init();

	return 0;
}

int exynos_power_init(void)
{
	pmic_init(0);
	pmic_init_max77686();

	return 0;
}

#ifdef CONFIG_USB_GADGET
static int s5pc210_phy_control(int on)
{
	struct pmic *p_pmic;

	p_pmic = pmic_get("MAX77686_PMIC");
	if (!p_pmic)
		return -ENODEV;

	if (pmic_probe(p_pmic))
		return -1;

	if (on)
		return max77686_set_ldo_mode(p_pmic, 12, OPMODE_ON);
	else
		return max77686_set_ldo_mode(p_pmic, 12, OPMODE_LPM);
}

struct s3c_plat_otg_data s5pc210_otg_data = {
	.phy_control	= s5pc210_phy_control,
	.regs_phy	= EXYNOS4X12_USBPHY_BASE,
	.regs_otg	= EXYNOS4X12_USBOTG_BASE,
	.usb_phy_ctrl	= EXYNOS4X12_USBPHY_CONTROL,
	.usb_flags	= PHY0_SLEEP,
};
#endif

#if defined(CONFIG_USB_GADGET) || defined(CONFIG_CMD_USB)

int board_usb_init(int index, enum usb_init_type init)
{
#ifdef CONFIG_CMD_USB
	struct pmic *p_pmic;

	/* Set Ref freq 0 => 24MHz, 1 => 26MHz*/
	/* Odroid Us have it at 24MHz, Odroid Xs at 26MHz */
	if (gd->board_type == ODROID_TYPE_U3)
		gpio_direction_output(EXYNOS4X12_GPIO_X30, 0);
	else
		gpio_direction_output(EXYNOS4X12_GPIO_X30, 1);

	/* Disconnect, Reset, Connect */
	gpio_direction_output(EXYNOS4X12_GPIO_X34, 0);
	gpio_direction_output(EXYNOS4X12_GPIO_X35, 0);
	gpio_direction_output(EXYNOS4X12_GPIO_X35, 1);
	gpio_direction_output(EXYNOS4X12_GPIO_X34, 1);

	/* Power off and on BUCK8 for LAN9730 */
	debug("LAN9730 - Turning power buck 8 OFF and ON.\n");

	p_pmic = pmic_get("MAX77686_PMIC");
	if (p_pmic && !pmic_probe(p_pmic)) {
		max77686_set_buck_voltage(p_pmic, 8, 750000);
		max77686_set_buck_voltage(p_pmic, 8, 3300000);
	}

#endif

	debug("USB_udc_probe\n");
	return s3c_udc_probe(&s5pc210_otg_data);
}
#endif

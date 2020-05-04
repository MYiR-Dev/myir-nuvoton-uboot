/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2018 NXP
 */

#ifndef _SC_SCI_H
#define _SC_SCI_H

#include <asm/arch/sci/types.h>
#include <asm/arch/sci/svc/misc/api.h>
#include <asm/arch/sci/svc/pad/api.h>
#include <asm/arch/sci/svc/pm/api.h>
#include <asm/arch/sci/svc/rm/api.h>
#include <asm/arch/sci/svc/seco/api.h>
#include <asm/arch/sci/rpc.h>
#include <dt-bindings/soc/imx_rsrc.h>
#include <linux/errno.h>

static inline int sc_err_to_linux(sc_err_t err)
{
	int ret;

	switch (err) {
	case SC_ERR_NONE:
		return 0;
	case SC_ERR_VERSION:
	case SC_ERR_CONFIG:
	case SC_ERR_PARM:
		ret = -EINVAL;
		break;
	case SC_ERR_NOACCESS:
	case SC_ERR_LOCKED:
	case SC_ERR_UNAVAILABLE:
		ret = -EACCES;
		break;
	case SC_ERR_NOTFOUND:
	case SC_ERR_NOPOWER:
		ret = -ENODEV;
		break;
	case SC_ERR_IPC:
		ret = -EIO;
		break;
	case SC_ERR_BUSY:
		ret = -EBUSY;
		break;
	case SC_ERR_FAIL:
		ret = -EIO;
		break;
	default:
		ret = 0;
		break;
	}

	debug("%s %d %d\n", __func__, err, ret);

	return ret;
}

/* PM API*/
int sc_pm_set_resource_power_mode(sc_ipc_t ipc, sc_rsrc_t resource,
				  sc_pm_power_mode_t mode);
int sc_pm_get_resource_power_mode(sc_ipc_t ipc, sc_rsrc_t resource,
				  sc_pm_power_mode_t *mode);
int sc_pm_set_clock_rate(sc_ipc_t ipc, sc_rsrc_t resource, sc_pm_clk_t clk,
			 sc_pm_clock_rate_t *rate);
int sc_pm_get_clock_rate(sc_ipc_t ipc, sc_rsrc_t resource, sc_pm_clk_t clk,
			 sc_pm_clock_rate_t *rate);
int sc_pm_clock_enable(sc_ipc_t ipc, sc_rsrc_t resource, sc_pm_clk_t clk,
		       sc_bool_t enable, sc_bool_t autog);
int sc_pm_set_clock_parent(sc_ipc_t ipc, sc_rsrc_t resource, sc_pm_clk_t clk,
			   sc_pm_clk_parent_t parent);
int sc_pm_cpu_start(sc_ipc_t ipc, sc_rsrc_t resource, sc_bool_t enable,
		    sc_faddr_t address);
sc_bool_t sc_pm_is_partition_started(sc_ipc_t ipc, sc_rm_pt_t pt);
int sc_pm_resource_reset(sc_ipc_t ipc, sc_rsrc_t resource);

/* MISC API */
int sc_misc_set_control(sc_ipc_t ipc, sc_rsrc_t resource,
			sc_ctrl_t ctrl, u32 val);
int sc_misc_get_control(sc_ipc_t ipc, sc_rsrc_t resource, sc_ctrl_t ctrl,
			u32 *val);
void sc_misc_get_boot_dev(sc_ipc_t ipc, sc_rsrc_t *boot_dev);
void sc_misc_boot_status(sc_ipc_t ipc, sc_misc_boot_status_t status);
void sc_misc_build_info(sc_ipc_t ipc, u32 *build, u32 *commit);
int sc_misc_otp_fuse_read(sc_ipc_t ipc, u32 word, u32 *val);
int sc_misc_get_temp(sc_ipc_t ipc, sc_rsrc_t resource, sc_misc_temp_t temp,
		     s16 *celsius, s8 *tenths);

/* RM API */
sc_bool_t sc_rm_is_memreg_owned(sc_ipc_t ipc, sc_rm_mr_t mr);
int sc_rm_find_memreg(sc_ipc_t ipc, sc_rm_mr_t *mr, sc_faddr_t addr_start,
		      sc_faddr_t addr_end);
int sc_rm_set_memreg_permissions(sc_ipc_t ipc, sc_rm_mr_t mr,
				 sc_rm_pt_t pt, sc_rm_perm_t perm);
int sc_rm_get_memreg_info(sc_ipc_t ipc, sc_rm_mr_t mr, sc_faddr_t *addr_start,
			  sc_faddr_t *addr_end);
sc_bool_t sc_rm_is_resource_owned(sc_ipc_t ipc, sc_rsrc_t resource);
int sc_rm_partition_alloc(sc_ipc_t ipc, sc_rm_pt_t *pt, sc_bool_t secure,
			  sc_bool_t isolated, sc_bool_t restricted,
			  sc_bool_t grant, sc_bool_t coherent);
int sc_rm_partition_free(sc_ipc_t ipc, sc_rm_pt_t pt);
int sc_rm_get_partition(sc_ipc_t ipc, sc_rm_pt_t *pt);
int sc_rm_set_parent(sc_ipc_t ipc, sc_rm_pt_t pt, sc_rm_pt_t pt_parent);
int sc_rm_assign_resource(sc_ipc_t ipc, sc_rm_pt_t pt, sc_rsrc_t resource);
int sc_rm_assign_pad(sc_ipc_t ipc, sc_rm_pt_t pt, sc_pad_t pad);
sc_bool_t sc_rm_is_pad_owned(sc_ipc_t ipc, sc_pad_t pad);
int sc_rm_get_resource_owner(sc_ipc_t ipc, sc_rsrc_t resource,
			     sc_rm_pt_t *pt);

/* PAD API */
int sc_pad_set(sc_ipc_t ipc, sc_pad_t pad, u32 val);
int sc_pad_get(sc_ipc_t ipc, sc_pad_t pad, uint32_t *val);

/* SMMU API */
int sc_rm_set_master_sid(sc_ipc_t ipc, sc_rsrc_t resource, sc_rm_sid_t sid);

/* SECO API */
int sc_seco_authenticate(sc_ipc_t ipc, sc_seco_auth_cmd_t cmd,
			 sc_faddr_t addr);
int sc_seco_forward_lifecycle(sc_ipc_t ipc, u32 change);
int sc_seco_chip_info(sc_ipc_t ipc, u16 *lc, u16 *monotonic, u32 *uid_l,
		      u32 *uid_h);
void sc_seco_build_info(sc_ipc_t ipc, u32 *version, u32 *commit);
int sc_seco_get_event(sc_ipc_t ipc, u8 idx, u32 *event);
int sc_seco_gen_key_blob(sc_ipc_t ipc, u32 id, sc_faddr_t load_addr,
			 sc_faddr_t export_addr, u16 max_size);
int sc_seco_get_mp_key(sc_ipc_t ipc, sc_faddr_t dst_addr, u16 dst_size);
int sc_seco_update_mpmr(sc_ipc_t ipc, sc_faddr_t addr, u8 size, u8 lock);
int sc_seco_get_mp_sign(sc_ipc_t ipc, sc_faddr_t msg_addr,
			u16 msg_size, sc_faddr_t dst_addr, u16 dst_size);
int sc_seco_secvio_dgo_config(sc_ipc_t ipc, u8 id, u8 access, u32 *data);
int sc_seco_secvio_config(sc_ipc_t ipc, u8 id, u8 access,
			  u32 *data0, u32 *data1, u32 *data2, u32 *data3,
			  u32 *data4, u8 size);

#endif

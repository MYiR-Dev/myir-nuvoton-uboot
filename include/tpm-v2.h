/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2018 Bootlin
 * Author: Miquel Raynal <miquel.raynal@bootlin.com>
 */

#ifndef __TPM_V2_H
#define __TPM_V2_H

#include <tpm-common.h>

#define TPM2_DIGEST_LEN		32

/**
 * TPM2 Structure Tags for command/response buffers.
 *
 * @TPM2_ST_NO_SESSIONS: the command does not need an authentication.
 * @TPM2_ST_SESSIONS: the command needs an authentication.
 */
enum tpm2_structures {
	TPM2_ST_NO_SESSIONS	= 0x8001,
	TPM2_ST_SESSIONS	= 0x8002,
};

/**
 * TPM2 type of boolean.
 */
enum tpm2_yes_no {
	TPMI_YES		= 1,
	TPMI_NO			= 0,
};

/**
 * TPM2 startup values.
 *
 * @TPM2_SU_CLEAR: reset the internal state.
 * @TPM2_SU_STATE: restore saved state (if any).
 */
enum tpm2_startup_types {
	TPM2_SU_CLEAR		= 0x0000,
	TPM2_SU_STATE		= 0x0001,
};

/**
 * TPM2 permanent handles.
 *
 * @TPM2_RH_OWNER: refers to the 'owner' hierarchy.
 * @TPM2_RS_PW: indicates a password.
 * @TPM2_RH_LOCKOUT: refers to the 'lockout' hierarchy.
 * @TPM2_RH_ENDORSEMENT: refers to the 'endorsement' hierarchy.
 * @TPM2_RH_PLATFORM: refers to the 'platform' hierarchy.
 */
enum tpm2_handles {
	TPM2_RH_OWNER		= 0x40000001,
	TPM2_RS_PW		= 0x40000009,
	TPM2_RH_LOCKOUT		= 0x4000000A,
	TPM2_RH_ENDORSEMENT	= 0x4000000B,
	TPM2_RH_PLATFORM	= 0x4000000C,
};

/**
 * TPM2 command codes used at the beginning of a buffer, gives the command.
 *
 * @TPM2_CC_STARTUP: TPM2_Startup().
 * @TPM2_CC_SELF_TEST: TPM2_SelfTest().
 * @TPM2_CC_CLEAR: TPM2_Clear().
 * @TPM2_CC_CLEARCONTROL: TPM2_ClearControl().
 * @TPM2_CC_HIERCHANGEAUTH: TPM2_HierarchyChangeAuth().
 * @TPM2_CC_PCR_SETAUTHPOL: TPM2_PCR_SetAuthPolicy().
 * @TPM2_CC_DAM_RESET: TPM2_DictionaryAttackLockReset().
 * @TPM2_CC_DAM_PARAMETERS: TPM2_DictionaryAttackParameters().
 * @TPM2_CC_GET_CAPABILITY: TPM2_GetCapibility().
 * @TPM2_CC_PCR_READ: TPM2_PCR_Read().
 * @TPM2_CC_PCR_EXTEND: TPM2_PCR_Extend().
 * @TPM2_CC_PCR_SETAUTHVAL: TPM2_PCR_SetAuthValue().
 */
enum tpm2_command_codes {
	TPM2_CC_STARTUP		= 0x0144,
	TPM2_CC_SELF_TEST	= 0x0143,
	TPM2_CC_CLEAR		= 0x0126,
	TPM2_CC_CLEARCONTROL	= 0x0127,
	TPM2_CC_HIERCHANGEAUTH	= 0x0129,
	TPM2_CC_DAM_RESET	= 0x0139,
	TPM2_CC_DAM_PARAMETERS	= 0x013A,
	TPM2_CC_GET_CAPABILITY	= 0x017A,
	TPM2_CC_PCR_READ	= 0x017E,
	TPM2_CC_PCR_EXTEND	= 0x0182,
};

/**
 * TPM2 return codes.
 */
enum tpm2_return_codes {
	TPM2_RC_SUCCESS		= 0x0000,
	TPM2_RC_BAD_TAG		= 0x001E,
	TPM2_RC_FMT1		= 0x0080,
	TPM2_RC_HASH		= TPM2_RC_FMT1 + 0x0003,
	TPM2_RC_VALUE		= TPM2_RC_FMT1 + 0x0004,
	TPM2_RC_SIZE		= TPM2_RC_FMT1 + 0x0015,
	TPM2_RC_BAD_AUTH	= TPM2_RC_FMT1 + 0x0022,
	TPM2_RC_HANDLE		= TPM2_RC_FMT1 + 0x000B,
	TPM2_RC_VER1		= 0x0100,
	TPM2_RC_INITIALIZE	= TPM2_RC_VER1 + 0x0000,
	TPM2_RC_FAILURE		= TPM2_RC_VER1 + 0x0001,
	TPM2_RC_DISABLED	= TPM2_RC_VER1 + 0x0020,
	TPM2_RC_AUTH_MISSING	= TPM2_RC_VER1 + 0x0025,
	TPM2_RC_COMMAND_CODE	= TPM2_RC_VER1 + 0x0043,
	TPM2_RC_AUTHSIZE	= TPM2_RC_VER1 + 0x0044,
	TPM2_RC_AUTH_CONTEXT	= TPM2_RC_VER1 + 0x0045,
	TPM2_RC_NEEDS_TEST	= TPM2_RC_VER1 + 0x0053,
	TPM2_RC_WARN		= 0x0900,
	TPM2_RC_TESTING		= TPM2_RC_WARN + 0x000A,
	TPM2_RC_REFERENCE_H0	= TPM2_RC_WARN + 0x0010,
	TPM2_RC_LOCKOUT		= TPM2_RC_WARN + 0x0021,
};

/**
 * TPM2 algorithms.
 */
enum tpm2_algorithms {
	TPM2_ALG_XOR		= 0x0A,
	TPM2_ALG_SHA256		= 0x0B,
	TPM2_ALG_SHA384		= 0x0C,
	TPM2_ALG_SHA512		= 0x0D,
	TPM2_ALG_NULL		= 0x10,
};

/**
 * Issue a TPM2_Startup command.
 *
 * @mode	TPM startup mode
 *
 * @return code of the operation
 */
u32 tpm2_startup(enum tpm2_startup_types mode);

/**
 * Issue a TPM2_SelfTest command.
 *
 * @full_test	Asking to perform all tests or only the untested ones
 *
 * @return code of the operation
 */
u32 tpm2_self_test(enum tpm2_yes_no full_test);

/**
 * Issue a TPM2_Clear command.
 *
 * @handle	Handle
 * @pw		Password
 * @pw_sz	Length of the password
 *
 * @return code of the operation
 */
u32 tpm2_clear(u32 handle, const char *pw, const ssize_t pw_sz);

#endif /* __TPM_V2_H */

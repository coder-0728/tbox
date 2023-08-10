/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ApplicationActivation"
 * 	found in "Application_Activation.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_OtaNewPubKeyReq_H_
#define	_OtaNewPubKeyReq_H_


#include <asn_application.h>

/* Including external dependencies */
#include "VinCode.h"
#include "Iccid.h"
#include "SimInfo.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* OtaNewPubKeyReq */
typedef struct OtaNewPubKeyReq {
	VinCode_t	 vinCode;
	Iccid_t	 iccid;
	SimInfo_t	 simInfo;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} OtaNewPubKeyReq_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_OtaNewPubKeyReq;

#ifdef __cplusplus
}
#endif

#endif	/* _OtaNewPubKeyReq_H_ */
#include <asn_internal.h>

/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "ApplicationActivation"
 * 	found in "Application_Activation.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_TboxID_H_
#define	_TboxID_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TboxID */
typedef OCTET_STRING_t	 TboxID_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TboxID;
asn_struct_free_f TboxID_free;
asn_struct_print_f TboxID_print;
asn_constr_check_f TboxID_constraint;
ber_type_decoder_f TboxID_decode_ber;
der_type_encoder_f TboxID_encode_der;
xer_type_decoder_f TboxID_decode_xer;
xer_type_encoder_f TboxID_encode_xer;
per_type_decoder_f TboxID_decode_uper;
per_type_encoder_f TboxID_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _TboxID_H_ */
#include <asn_internal.h>

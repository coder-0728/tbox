/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DispatcherBody"
 * 	found in "Dispatcher_Body.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_EventID_H_
#define	_EventID_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EventID */
typedef long	 EventID_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EventID;
asn_struct_free_f EventID_free;
asn_struct_print_f EventID_print;
asn_constr_check_f EventID_constraint;
ber_type_decoder_f EventID_decode_ber;
der_type_encoder_f EventID_encode_der;
xer_type_decoder_f EventID_decode_xer;
xer_type_encoder_f EventID_encode_xer;
per_type_decoder_f EventID_decode_uper;
per_type_encoder_f EventID_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _EventID_H_ */
#include <asn_internal.h>
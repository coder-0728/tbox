/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DispatcherHeader"
 * 	found in "Dispatcher_Header.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_TspTboxSn_H_
#define	_TspTboxSn_H_


#include <asn_application.h>

/* Including external dependencies */
#include <IA5String.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TspTboxSn */
typedef IA5String_t	 TspTboxSn_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TspTboxSn;
asn_struct_free_f TspTboxSn_free;
asn_struct_print_f TspTboxSn_print;
asn_constr_check_f TspTboxSn_constraint;
ber_type_decoder_f TspTboxSn_decode_ber;
der_type_encoder_f TspTboxSn_encode_der;
xer_type_decoder_f TspTboxSn_decode_xer;
xer_type_encoder_f TspTboxSn_encode_xer;
per_type_decoder_f TspTboxSn_decode_uper;
per_type_encoder_f TspTboxSn_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _TspTboxSn_H_ */
#include <asn_internal.h>

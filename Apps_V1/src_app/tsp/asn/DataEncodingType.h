/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DispatcherBody"
 * 	found in "Dispatcher_Body.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_DataEncodingType_H_
#define	_DataEncodingType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DataEncodingType {
	DataEncodingType_asn1PER	= 0,
	DataEncodingType_asn1DER	= 1,
	DataEncodingType_asn1BER	= 2
} e_DataEncodingType;

/* DataEncodingType */
typedef long	 DataEncodingType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DataEncodingType;
asn_struct_free_f DataEncodingType_free;
asn_struct_print_f DataEncodingType_print;
asn_constr_check_f DataEncodingType_constraint;
ber_type_decoder_f DataEncodingType_decode_ber;
der_type_encoder_f DataEncodingType_encode_der;
xer_type_decoder_f DataEncodingType_decode_xer;
xer_type_encoder_f DataEncodingType_encode_xer;
per_type_decoder_f DataEncodingType_decode_uper;
per_type_encoder_f DataEncodingType_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _DataEncodingType_H_ */
#include <asn_internal.h>

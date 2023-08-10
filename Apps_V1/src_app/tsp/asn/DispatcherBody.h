/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DispatcherBody"
 * 	found in "Dispatcher_Body.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_DispatcherBody_H_
#define	_DispatcherBody_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ApplicationId.h"
#include "MessageId.h"
#include "EventCreationTime.h"
#include "EventID.h"
#include "UlMessageCounter.h"
#include "DlMessageCounter.h"
#include "AckMessageCounter.h"
#include "AckRequired.h"
#include "ApplicationDataLength.h"
#include "DataEncodingType.h"
#include "ApplicationDataProtocolVersion.h"
#include "TestFlag.h"
#include "Result.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DispatcherBody */
typedef struct DispatcherBody {
	ApplicationId_t	 applicationId;
	MessageId_t	 messageId;
	EventCreationTime_t	 eventCreationTime;
	EventID_t	*eventID	/* OPTIONAL */;
	UlMessageCounter_t	*ulMessageCounter	/* OPTIONAL */;
	DlMessageCounter_t	*dlMessageCounter	/* OPTIONAL */;
	AckMessageCounter_t	*ackMessageCounter	/* OPTIONAL */;
	AckRequired_t	*ackRequired	/* OPTIONAL */;
	ApplicationDataLength_t	*applicationDataLength	/* OPTIONAL */;
	DataEncodingType_t	*dataEncodingType	/* OPTIONAL */;
	ApplicationDataProtocolVersion_t	*applicationDataProtocolVersion	/* OPTIONAL */;
	TestFlag_t	*testFlag	/* OPTIONAL */;
	Result_t	*result	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DispatcherBody_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DispatcherBody;

#ifdef __cplusplus
}
#endif

#endif	/* _DispatcherBody_H_ */
#include <asn_internal.h>

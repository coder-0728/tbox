#ifndef _ASN_PACK_H_
#define _ASN_PACK_H_

#include "DispatcherBody.h"
#include "OtaNewPubKeyReq.h"

/*************** DispatcherBody ***************/
int CreateDispatcherBody(DispatcherBody_t **body);
int FreeDispatcherBody(DispatcherBody_t *body);

int CreateApplicationId(ApplicationId_t *application_id, unsigned char *aid, int length);
int FreeApplicationId(ApplicationId_t *application_id);

int CreateUlMessageCounter(UlMessageCounter_t **ul_message_counter, long message_counter);
int FreeUlMessageCounter(UlMessageCounter_t *ul_message_counter);

int CreateDlMessageCounter(DlMessageCounter_t **dl_message_counter, long message_counter);
int FreeDlMessageCounter(DlMessageCounter_t *dl_message_counter);

int CreateAckMessageCounter(AckMessageCounter_t **ack_message_counter, long message_counter);
int FreeAckMessageCounter(AckMessageCounter_t *ack_message_counter);

int CreateAckRequired(AckRequired_t **ack_required, int required);
int FreeAckRequired(AckRequired_t *ack_required);

int CreateApplicationDataLength(ApplicationDataLength_t **application_data_length, long data_length);
int FreeApplicationDataLength(ApplicationDataLength_t *application_data_length);

int CreateDataEncodingType(DataEncodingType_t **data_encoding_type, long encoding_type);
int FreeDataEncodingType(DataEncodingType_t *data_encoding_type);

int CreateApplicationDataProtocolVersion(ApplicationDataProtocolVersion_t **application_data_protocol_version, long protocol_version);
int FreeApplicationDataProtocolVersion(ApplicationDataProtocolVersion_t *application_data_protocol_version);

int CreateTestFlag(TestFlag_t **test_flag, long flag);
int FreeTestFlag(TestFlag_t *test_flag);

int CreateResult(Result_t **result,long result_code);
int FreeResult(Result_t *result);
/**********************************************/

/*************** OtaNewPubKeyReq ***************/
int CreateOtaNewPubKeyReq(OtaNewPubKeyReq_t **ota_new_pub_key_req);
int FreeOtaNewPubKeyReq(OtaNewPubKeyReq_t *ota_new_pub_key_req);

int CreateVinCode(VinCode_t *vin_code, unsigned char *vin, int length);
int FreeVinCode(VinCode_t *vin_code);

int CreateIccid(Iccid_t *pub_key, unsigned char *iccid, int length);
int FreeIccid(Iccid_t *iccid);

int CreateSimInfo(SimInfo_t *sim_info, unsigned char *sim_info_str, int length);
int FreeSimInfo(SimInfo_t *sim_info);
/***********************************************/




#endif
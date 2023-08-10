#include "asn_pack.h"

int CreateApplicationId(ApplicationId_t *application_id, unsigned char *aid, int length)
{
    if(application_id == NULL || aid == NULL || length <= 0)
    {
        return -1;
    }

    application_id->buf = (unsigned char *)calloc(1,sizeof(unsigned char) * length);
    if(application_id->buf == NULL)
    {
        return -1;
    }

    memcpy(application_id->buf,aid,length);
    application_id->size = length;

    return 0;
}

int FreeApplicationId(ApplicationId_t *application_id)
{
    if(application_id == NULL)
    {
        return -1;
    }

    if(application_id->buf != NULL)
    {
        free(application_id->buf);
        application_id->buf = NULL;
    }

    application_id = NULL;

    return 0;
}

int CreateUlMessageCounter(UlMessageCounter_t **ul_message, long message_counter)
{
    *ul_message = (UlMessageCounter_t *)calloc(1,sizeof(UlMessageCounter_t));
    if(*ul_message == NULL)
    {
        return -1;
    }

    **ul_message = message_counter;

    return 0;
}

int CreateDlMessageCounter(DlMessageCounter_t **dl_message, long message_counter)
{
    *dl_message = (DlMessageCounter_t *)calloc(1,sizeof(DlMessageCounter_t));
    if(*dl_message == NULL)
    {
        return -1;
    }

    **dl_message = message_counter;

    return 0;
}

int CreateAckMessageCounter(AckMessageCounter_t **ack_message, long message_counter)
{
    *ack_message = (AckMessageCounter_t *)calloc(1,sizeof(AckMessageCounter_t));
    if(*ack_message == NULL)
    {
        return -1;
    }

    **ack_message = message_counter;

    return 0;
}

int CreateAckRequired(AckRequired_t **ack_required, int required)
{
    *ack_required = (AckRequired_t *)calloc(1,sizeof(AckRequired_t));
    if(*ack_required == NULL)
    {
        return -1;
    }

    **ack_required = required;

    return 0;
}

int CreateApplicationDataLength(ApplicationDataLength_t **application_data_length, long data_length)
{
    *application_data_length = (ApplicationDataLength_t *)calloc(1,sizeof(ApplicationDataLength_t));
    if(*application_data_length == NULL)
    {
        return -1;
    }

    **application_data_length = data_length;

    return 0;
}

int CreateDataEncodingType(DataEncodingType_t **data_encoding_type, long encoding_type)
{
    *data_encoding_type = (DataEncodingType_t *)calloc(1,sizeof(DataEncodingType_t));
    if(*data_encoding_type == NULL)
    {
        return -1;
    }

    **data_encoding_type = encoding_type;

    return 0;
}

int CreateApplicationDataProtocolVersion(ApplicationDataProtocolVersion_t **application_data_protocol_version, long protocol_version)
{
    *application_data_protocol_version = (ApplicationDataProtocolVersion_t *)calloc(1,sizeof(ApplicationDataProtocolVersion_t));
    if(*application_data_protocol_version == NULL)
    {
        return -1;
    }

    **application_data_protocol_version = protocol_version;

    return 0;
}

int CreateTestFlag(TestFlag_t **test_flag, long flag)
{
    *test_flag = (TestFlag_t *)calloc(1,sizeof(TestFlag_t));
    if(*test_flag == NULL)
    {
        return -1;
    }

    **test_flag = flag;

    return 0;
}

int CreateResult(Result_t **result, long result_code)
{
    *result = (Result_t *)calloc(1,sizeof(Result_t));
    if(*result == NULL)
    {
        return -1;
    }

    **result = result_code;

    return 0;
}

int CreateDispatcherBody(DispatcherBody_t **body)
{
    if(*body != NULL)
    {
        return -1;
    }

    *body = (DispatcherBody_t *)calloc(1,sizeof(DispatcherBody_t));
    if(*body == NULL)
    {
        return -1;
    }

    return 0;
}

int CreateOtaNewPubKeyReq(OtaNewPubKeyReq_t **ota_new_pub_key_req)
{
    *ota_new_pub_key_req = (OtaNewPubKeyReq_t *)calloc(1,sizeof(OtaNewPubKeyReq_t));
    if(*ota_new_pub_key_req == NULL)
    {
        return -1;
    }

    return 0;
}

int FreeUlMessageCounter(UlMessageCounter_t *ul_message)
{
    if(ul_message != NULL)
    {
        free(ul_message);
        ul_message = NULL;
    }

    return 0;
}

int FreeDlMessageCounter(DlMessageCounter_t *dl_message)
{
    if(dl_message != NULL)
    {
        free(dl_message);
        dl_message = NULL;
    }

    return 0;
}

int FreeAckMessageCounter(AckMessageCounter_t *ack_message)
{
    if(ack_message != NULL)
    {
        free(ack_message);
        ack_message = NULL;
    }

    return 0;
}

int FreeAckRequired(AckRequired_t *ack_required)
{
    if(ack_required != NULL)
    {
        free(ack_required);
        ack_required = NULL;
    }

    return 0;
}

int FreeApplicationDataLength(ApplicationDataLength_t *application_data_length)
{
    if(application_data_length != NULL)
    {
        free(application_data_length);
        application_data_length = NULL;
    }

    return 0;
}

int FreeDataEncodingType(DataEncodingType_t *data_encoding_type)
{
    if(data_encoding_type != NULL)
    {
        free(data_encoding_type);
        data_encoding_type = NULL;
    }

    return 0;
}

int FreeApplicationDataProtocolVersion(ApplicationDataProtocolVersion_t *application_data_protocol_version)
{
    if(application_data_protocol_version != NULL)
    {
        free(application_data_protocol_version);
        application_data_protocol_version = NULL;
    }

    return 0;
}

int FreeTestFlag(TestFlag_t *test_flag)
{
    if(test_flag != NULL)
    {
        free(test_flag);
        test_flag = NULL;
    }

    return 0;
}

int FreeResult(Result_t *result)
{
    if(result != NULL)
    {
        free(result);
        result = NULL;
    }

    return 0;
}

int FreeDispatcherBody(DispatcherBody_t *body)
{
    if(body != NULL)
    {
        FreeApplicationId(&body->applicationId);
        FreeUlMessageCounter(body->ulMessageCounter);
        FreeDlMessageCounter(body->dlMessageCounter);
        FreeAckMessageCounter(body->ackMessageCounter);
        FreeAckRequired(body->ackRequired);
        FreeApplicationDataLength(body->applicationDataLength);
        FreeDataEncodingType(body->dataEncodingType);
        FreeApplicationDataProtocolVersion(body->applicationDataProtocolVersion);
        FreeTestFlag(body->testFlag);
        FreeResult(body->result);

        free(body);
        body = NULL;
    }

    return 0;
}


int CreateVinCode(VinCode_t *vin_code, unsigned char *vin, int length)
{
    vin_code->buf = (unsigned char *)calloc(1,sizeof(unsigned char) * length);
    if(vin_code->buf == NULL)
    {
        return -1;
    }

    memcpy(vin_code->buf, vin, length);
    vin_code->size = length;

    return 0;
}

int CreateIccid(Iccid_t *iccid, unsigned char *iccid_code, int length)
{
    iccid->buf = (unsigned char *)calloc(1,sizeof(unsigned char) * length);
    if(iccid->buf == NULL)
    {
        return -1;
    }

    memcpy(iccid->buf, iccid_code, length);
    iccid->size = length;

    return 0;
}

int CreateSimInfo(SimInfo_t *sim_info, unsigned char *sim_code, int length)
{
    sim_info->buf = (unsigned char *)calloc(1,sizeof(unsigned char) * length);
    if(sim_info->buf == NULL)
    {
        return -1;
    }

    memcpy(sim_info->buf, sim_code, length);
    sim_info->size = length;

    return 0;
}

int FreeVinCode(VinCode_t *vin_code)
{

    if(vin_code->buf != NULL)
    {
        free(vin_code->buf);
        vin_code->buf = NULL;
    }

    return 0;
}

int FreeIccid(Iccid_t *iccid)
{
    if(iccid->buf != NULL)
    {
        free(iccid->buf);
        iccid->buf = NULL;
    }
    return 0;
}

int FreeSimInfo(SimInfo_t *sim_info)
{
    if(sim_info->buf != NULL)
    {
        free(sim_info->buf);
        sim_info->buf = NULL;
    }

    return 0;
}

int FreeOtaNewPubKeyReq(OtaNewPubKeyReq_t *ota_new_pub_key_req)
{
    if(ota_new_pub_key_req != NULL)
    {
        FreeVinCode(&ota_new_pub_key_req->vinCode);
        FreeIccid(&ota_new_pub_key_req->iccid);
        FreeSimInfo(&ota_new_pub_key_req->simInfo);

        free(ota_new_pub_key_req);
        ota_new_pub_key_req = NULL;
    }

    return 0;
}
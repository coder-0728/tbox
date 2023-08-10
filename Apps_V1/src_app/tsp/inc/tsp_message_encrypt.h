#ifndef _TSP_MESSAGE_ENCRYPT_H_
#define _TSP_MESSAGE_ENCRYPT_H_

#include "tsp_message_protocol.h"

int Tsp_Message_Encrypt(T_TspMessage *data, unsigned char *key, unsigned char *iv, void *transmit_data);

int Tsp_Message_Decrypt(void *data_in, int data_len, unsigned char *key, unsigned char *iv, T_TspMessage *data_out);


#endif
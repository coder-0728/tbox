#ifndef _HTTP_BIND_SIM_H_
#define _HTTP_BIND_SIM_H_

#include <stdio.h>

int http_post_bind_sim(unsigned char *vin, unsigned char *sim_info, unsigned char *country_code, unsigned char *iccid, int *support_ipv6=NULL, unsigned char *ipv6_address=NULL);

int http_post_bind_sim_verification(unsigned char *vin, unsigned char *sim_info, unsigned char verification_code);
#endif // _HTTP_BIND_SIM_H_
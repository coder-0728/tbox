/*************************************************************************
	> File Name: at_sirun.h
	> Author: 
	> Mail: 
	> Created Time: 2018年12月09日 星期日 17时06分49秒
 ************************************************************************/

#ifndef _AT_SIRUN_H
#define _AT_SIRUN_H

#ifdef __cplusplus
extern "C" {
#endif
#include <glib.h>
/*
#define RESPONSE_ZERO_LINE (0)
#define RESPONSE_ONE_LINE (1)
#define RESPONSE_MULTI_LINE (2)
  */

#define AT_CMD_EXECUTE_SUCCESS (0)
#define AT_CMD_EXECUTE_FAILURE (-1)

typedef enum 
{
    RESPONSE_ZERO_LINE = 0, 
    RESPONSE_ONE_LINE,
    RESPONSE_MULTI_LINE,
    RESPONSE_NUMERIC,
   
}ResponseLines;
#ifdef __cplusplus
}
#endif

#endif

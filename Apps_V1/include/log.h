/**
 * @file log.h
 * @author lujiefeng
 * @brief 
 * @version 0.1
 * @date 2018-11-28
 * 
 * @copyright Copyright (SiRun) 2018
 * 
 */
#ifndef _LOG_H
#define _LOG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <libgen.h>

#include <syslog.h>
#include <glib.h>

    /*app start ,first call LOG_INIT(name)*/

#define LOG_INIT(name) openlog(name, LOG_CONS | LOG_PID, LOG_LOCAL7)
#define LOG_CLOSE() closelog()

#define LOG(Priority, format, ...) syslog(Priority, format, ##__VA_ARGS__)

#define LOG_Debug(format, ...) syslog(LOG_DEBUG, " [debug] %s-%d:" format, basename((char *)__FILE__), __LINE__, ##__VA_ARGS__)

#define LOG_DebugHex(hex, len, text)                                      \
    do                                                                    \
    {                                                                     \
        GString *hexData = g_string_new(NULL);                            \
        for (gint i = 0; i < len; i++)                                    \
        {                                                                 \
            g_string_append_printf(hexData, "%02x ", hex[i]);             \
        }                                                                 \
        syslog(LOG_DEBUG, " [debug] %s-%d:%s-data[%s]\n",                 \
               basename((char *)__FILE__), __LINE__, text, hexData->str); \
        g_string_free(hexData, TRUE);                                     \
    } while (0)

#define LOG_Info(format, ...) syslog(LOG_INFO, " [info] %s-%d:" format, basename((char *)__FILE__), __LINE__, ##__VA_ARGS__)

#define LOG_Notice(format, ...) syslog(LOG_NOTICE, " [notice] %s-%d:" format, basename((char *)__FILE__), __LINE__, ##__VA_ARGS__)

#define LOG_Warn(format, ...) syslog(LOG_WARNING, " [warn] %s-%d:" format, basename((char *)__FILE__), __LINE__, ##__VA_ARGS__)

#define LOG_Error(format, ...) syslog(LOG_ERR, " [err] %s-%d:" format, basename((char *)__FILE__), __LINE__, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
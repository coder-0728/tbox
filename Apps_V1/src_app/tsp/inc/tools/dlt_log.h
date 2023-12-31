#ifndef _DLT_LOG_H_
#define _DLT_LOG_H_

#include "dlt/dlt.h"

#define DLT_LOG_ID "TSP"
#define DLT_LOG_CONTEXT "TSP"

#define DLT_LOG_INIT() \
    DLT_REGISTER_APP(DLT_LOG_ID, DLT_LOG_CONTEXT); \
    DLT_REGISTER_CONTEXT(DLT_LOG_ID, DLT_LOG_CONTEXT, "TSP"); \
    DLT_ENABLE_LOCAL_PRINT(); \
    DLT_ENABLE_LOCAL_FILE_PRINT(); \
    DLT_SET_LOG_LEVEL(DLT_LOG_ID, DLT_LOG_CONTEXT, DLT_LOG_DEBUG); \
    DLT_LOG(DLT_LOG_ID, DLT_LOG_CONTEXT, DLT_LOG_DEBUG, "DLT_LOG_INIT")

#define DLT_LOG_DEINIT() \
    DLT_UNREGISTER_CONTEXT(DLT_LOG_ID, DLT_LOG_CONTEXT); \
    DLT_UNREGISTER_APP();

#define DLT_LOG_DEBUG(...) \
    DLT_LOG(DLT_LOG_ID, DLT_LOG_CONTEXT, DLT_LOG_DEBUG, __VA_ARGS__)

#define DLT_LOG_INFO(...) \
    DLT_LOG(DLT_LOG_ID, DLT_LOG_CONTEXT, DLT_LOG_INFO, __VA_ARGS__)

#define DLT_LOG_WARN(...) \
    DLT_LOG(DLT_LOG_ID, DLT_LOG_CONTEXT, DLT_LOG_WARN, __VA_ARGS__)

#define DLT_LOG_ERROR(...) \
    DLT_LOG(DLT_LOG_ID, DLT_LOG_CONTEXT, DLT_LOG_ERROR, __VA_ARGS__)

#define DLT_LOG_FATAL(...) \
    DLT_LOG(DLT_LOG_ID, DLT_LOG_CONTEXT, DLT_LOG_FATAL, __VA_ARGS__)

#endif // _DLT_LOG_H_
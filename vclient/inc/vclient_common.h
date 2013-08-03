/* encoding: UTF-8 */

#ifndef VCLIENT_COMMON_H
#define VCLIENT_COMMON_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum vclient_error_code_e {
    VCLIENT_SUCCESS = 0,
    VCLIENT_LOGINIT_RTS_PROJ_ROOT_ENV_VAR_NOT_SET_ERROR,
    VCLIENT_LOGINIT_OPEN_LOG_FILE_ERROR,
    VCLIENT_WRONG_NUMBER_CLI_PARAMS_ERROR,
    VCLIENT_CANT_OPEN_VECTOR_FILE_ERROR,
    VCLIENT_CREATE_IPC_SOCKET_ERROR,
    VCLIENT_CONNECT_IPC_SOCKET_ERROR,
    VCLIENT_SEND_IPC_SOCKET_ERROR,
    VCLIENT_RECV_IPC_SOCKET_ERROR,
    VCLIENT_RECV_WRONG_DATA_FORMAT_ERROR
} vclient_error_code_t;

/* do not change numbers of types of clients */
typedef enum vclients_types_e {
    VCLIENT_b = 0x2,
    VCLIENT_d = 0x3
} vclients_types_t;

#define VCLIENT_COMMON_ASSERT(condition) assert(condition)

#define VCLIENT_COMMON_MEM_ALLOC(pointer, bytes_count)          \
    do {                                                        \
        if (NULL == pointer)                                    \
        {                                                       \
            pointer = malloc(bytes_count);                      \
            if (NULL == pointer)                                \
            {                                                   \
                VCLIENT_COMMON_ASSERT(0);                       \
            }                                                   \
        }                                                       \
        else                                                    \
        {                                                       \
            VCLIENT_COMMON_ASSERT(0);                           \
        }                                                       \
    } while (0)

#define VCLIENT_COMMON_DEALLOC_MEM(pointer)                     \
    do {                                                        \
        if (pointer)                                            \
        {                                                       \
            free(pointer);                                      \
            pointer = NULL;                                     \
        }                                                       \
        else                                                    \
        {                                                       \
            VCLIENT_COMMON_ASSERT(0);                           \
        }                                                       \
    } while (0)

#endif /* VCLIENT_COMMON_H */
/* encoding: UTF-8 */

#ifndef MCLIENT_COMMON_H
#define MCLIENT_COMMON_H

#include <assert.h>
#include <stdlib.h>

typedef enum mclient_error_code_e {
    MCLIENT_SUCCESS = 0,
    MCLIENT_LOGINIT_RTS_PROJ_ROOT_ENV_VAR_NOT_SET_ERROR,
    MCLIENT_LOGINIT_OPEN_LOG_FILE_ERROR,
    MCLIENT_WRONG_NUMBER_CLI_PARAMS_ERROR,
    MCLIENT_CANT_OPEN_MATRIX_FILE_ERROR,
    MCLIENT_CREATE_IPC_SOCKET_ERROR,
    MCLIENT_CONNECT_IPC_SOCKET_ERROR,
    MCLIENT_SEND_IPC_SOCKET_ERROR,
    MCLIENT_RECV_IPC_SOCKET_ERROR,
    MCLIENT_RECV_WRONG_DATA_FORMAT_ERROR
} mclient_error_code_t;

/* do not change numbers of types of clients */
typedef enum mclients_types_e {
    MCLIENT_A = 0x0,
    MCLIENT_C = 0x1
} mclients_types_t;

#define MCLIENT_COMMON_ASSERT(condition) assert(condition)

#define MCLIENT_COMMON_MEM_ALLOC(pointer, bytes_count)          \
    do {                                                        \
        if (NULL == pointer)                                    \
        {                                                       \
            pointer = malloc(bytes_count);                      \
            if (NULL == pointer)                                \
            {                                                   \
                MCLIENT_COMMON_ASSERT(0);                       \
            }                                                   \
        }                                                       \
        else                                                    \
        {                                                       \
            MCLIENT_COMMON_ASSERT(0);                           \
        }                                                       \
    } while (0)

#define MCLIENT_COMMON_DEALLOC_MEM(pointer)                     \
    do {                                                        \
        if (pointer)                                            \
        {                                                       \
            free(pointer);                                      \
            pointer = NULL;                                     \
        }                                                       \
        else                                                    \
        {                                                       \
            MCLIENT_COMMON_ASSERT(0);                           \
        }                                                       \
    } while (0)


#endif /* MCLIENT_COMMON_H */
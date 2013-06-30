/* encoding: UTF-8 */

#ifndef GATEWAY_COMMON_H
#define GATEWAY_COMMON_H

typedef enum gateway_error_code_e {
    GATEWAY_SUCCESS = 0,
    GATEWAY_CREATE_IPC_SOCKET_ERROR,
    GATEWAY_BIND_IPC_SOCKET_ERROR,
    GATEWAY_LISTEN_IPC_SOCKET_ERROR,
    GATEWAY_ACCEPT_IPC_SOCKET_ERROR,
    GATEWAY_RECV_IPC_SOCKET_ERROR
} gateway_error_code_t;

#endif
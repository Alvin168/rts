/* encoding: UTF-8 */

#ifndef MCLIENT_CLIENT_H
#define MCLIENT_CLIENT_H

#define IPC_SOCKET_NAME "rtos_sock"

int mclient_socket_create_socket(void);
int mclient_socket_connect_socket(int socket_fd_remote);
void mclient_socket_close_socket(int socket_fd);

#endif /* MCLIENT_CLIENT_H */
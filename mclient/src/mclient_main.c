/* encoding: UTF-8 */

#include <sys/types.h>

#include <unistd.h>

#include <stdio.h>
#include <stddef.h>

#include <string.h>

#include <time.h>
#include <math.h>

#include "mclient_common.h"
#include "mclient_log.h"
#include "mclient_main.h"
#include "mclient_socket.h"

#define MICROSECONDS_IN_MILLISECONDS 1000

static int g_mtrx[ROWS][COLUMNS];
static int g_mtrx_col_index[COLUMNS];
static int g_mtrx_col_ind_size;

static void print_mtrx(void)
{
    int row;
    for (row = 0; row < ROWS; row++)
    {
        int col;
        for (col = 0; col < COLUMNS; col++)
        {
            MCLIENT_LOG_LOG("%d ", g_mtrx[row][col]);
        }
        MCLIENT_LOG_LOG("\n");
    }
}

static enum mclient_error_code_e read_mtrx_from_file(char *mtrx_fp)
{
    mclient_error_code_t err_code = MCLIENT_SUCCESS;
    FILE *p_mtrx_f;

    p_mtrx_f = fopen(mtrx_fp, "r");
    if (NULL == p_mtrx_f)
    {
        err_code = MCLIENT_CANT_OPEN_MATRIX_FILE_ERROR;
    }
    else
    {
        int row = 0, col = 0;
        while (!(feof(p_mtrx_f) || row == ROWS))
        {
            fscanf(p_mtrx_f, "%d", &g_mtrx[row][col]);

            ++col;
            if (col == COLUMNS)
            {
                ++row;
                col = 0;
            }
        }
        fclose(p_mtrx_f);
    }

    return err_code;
}

static int get_random_time(void)
{
    return MIN_SLEEP_TIME_MLS + rand() % (MAX_SLEEP_TIME_MLS - MIN_SLEEP_TIME_MLS);
}

static void init_random(void)
{
    srand(time(0));
}

static int get_col_mtrx_index(void)
{
    int index_of_mtrx_col_index = rand() % g_mtrx_col_ind_size;
    int mtrx_col_index = g_mtrx_col_index[index_of_mtrx_col_index];
    g_mtrx_col_index[index_of_mtrx_col_index] = g_mtrx_col_index[g_mtrx_col_ind_size - 1];
    return mtrx_col_index;
}

static enum mclient_error_code_e begin_process(int socket_fd_remote, mclients_types_t mclient_type)
{
    mclient_error_code_t err_code = MCLIENT_SUCCESS;
    send_client_type_data_t send_client_type_data = { 
        .header = "typ",
        .mclient_type = mclient_type
    };
    recv_mtrx_row_data_request_t recv_mtrx_row_request;

    /* Send type of client */
    if (-1 == mclient_socket_send(socket_fd_remote,
                                  &send_client_type_data,
                                  sizeof(send_client_type_data)))
    {
        err_code = MCLIENT_SEND_IPC_SOCKET_ERROR;
        goto error;
    }
    MCLIENT_LOG_LOG("Type 'MClient' has been successfully sent to GateWay\n");

    init_random();

    /* endless loop for recieving commands from GateWay until 'end' will be occured */
    while (1)
    {
        int ind;

        MCLIENT_LOG_LOG("Wait new commands from GateWay...\n");
        fflush(stdout);
        /* Receive command from GateWay */
        if (-1 == mclient_socket_receive(socket_fd_remote,
                                         &recv_mtrx_row_request,
                                         sizeof(recv_mtrx_row_request)))
        {
            err_code = MCLIENT_RECV_IPC_SOCKET_ERROR;
            goto error;
        }

        /* If command if "end" then finish processing */
        if (!strcmp(recv_mtrx_row_request.header, "end") &&
            -1 == recv_mtrx_row_request.row)
        {
            MCLIENT_LOG_LOG("Received command 'end' from GateWay. Finish processing and close remote connection\n");
            break;
        }
        else
        {
            /* If command is not "mtr" then generate error */
            if (strcmp(recv_mtrx_row_request.header, "mtr"))
            {
                MCLIENT_LOG_LOG("Received unknown command from GateWay. Exit with error\n");
                err_code = MCLIENT_RECV_WRONG_DATA_FORMAT_ERROR;
                goto error;
            }
        }

        MCLIENT_LOG_LOG("*****************\n");
        MCLIENT_LOG_LOG("Received command 'mtr' with request number of row %d\n", recv_mtrx_row_request.row);

        for (ind = 0; ind < COLUMNS; ind++)
        {
            g_mtrx_col_index[ind] = ind;
        }

        g_mtrx_col_ind_size = COLUMNS;

        while (g_mtrx_col_ind_size > 0)
        {
            int col_mtrx_ind;
            send_mtrx_data_t send_mtrx_data = {
                .header = "mtr"
            };

            col_mtrx_ind = get_col_mtrx_index();

            send_mtrx_data.mtrx = (mtrx_fmt_t) {
                .col_pos = col_mtrx_ind,
                .value = g_mtrx[recv_mtrx_row_request.row][col_mtrx_ind]
            };

            MCLIENT_LOG_LOG("Data for sending was configured. HEAD = %s COL = %d, VAL = %d\n. Wait for sending...\n",
                   send_mtrx_data.header,
                   send_mtrx_data.mtrx.col_pos,
                   send_mtrx_data.mtrx.value);
            fflush(stdout);

            usleep(get_random_time() * MICROSECONDS_IN_MILLISECONDS);

            /* Send matrix value */
            if (-1 == mclient_socket_send(socket_fd_remote, &send_mtrx_data, sizeof(send_mtrx_data_t)))
            {
                MCLIENT_COMMON_ASSERT(0);
            }
            MCLIENT_LOG_LOG("Data has been successfully sent\n");

            --g_mtrx_col_ind_size;
        }

        MCLIENT_LOG_LOG("\nAll elements of matrix row %d was successfully sent\n", recv_mtrx_row_request.row);
    }

    error:

    MCLIENT_LOG_LOG("Finishing processing of 'MClient'\n");

    return err_code;
}

int main(int const argc, char const *argv[])
{
    char *mtrx_fp = NULL;
    size_t mtrx_fp_len;
    mclient_error_code_t err_code = MCLIENT_SUCCESS;
    mclients_types_t mclient_type;
    int i;

    if (3 != argc)
    {
        err_code = MCLIENT_WRONG_NUMBER_CLI_PARAMS_ERROR;
        goto error;
    }

    mclient_type = (mclients_types_t)atoi(argv[2]);

    err_code = mclient_log_init_log(argv[2]);
    if (MCLIENT_SUCCESS != err_code)
    {
        goto error_log;
    }

    MCLIENT_LOG_LOG("MClient parameters:\n");

    for (i = 0; i < argc; i++)
    {
        MCLIENT_LOG_LOG("argv[%d] = %s\n", i, argv[i]);
    }

    mtrx_fp_len = strlen(argv[1]);
    MCLIENT_COMMON_MEM_ALLOC(mtrx_fp, (mtrx_fp_len + 1) * sizeof(char));
    strcpy(mtrx_fp, argv[1]);

    MCLIENT_LOG_LOG("Try reading matrix from the file path '%s'...\n", mtrx_fp);
    err_code = read_mtrx_from_file(mtrx_fp);
    MCLIENT_COMMON_DEALLOC_MEM(mtrx_fp);
    if (MCLIENT_SUCCESS == err_code)
    {
        int socket_fd_remote;

        MCLIENT_LOG_LOG("The next matrix has been read:\n");
        print_mtrx();

        MCLIENT_LOG_LOG("Try to create remote socket for connection with RTS GateWay...\n");
        socket_fd_remote = mclient_socket_create_socket();
        if (-1 == socket_fd_remote)
        {
            err_code = MCLIENT_CREATE_IPC_SOCKET_ERROR;
            goto error;
        }
        MCLIENT_LOG_LOG("Socket for connection with GateWay has been created. SOCKET_FD = %d\n", socket_fd_remote);
        
        MCLIENT_LOG_LOG("Try to connect to remote RTS GateWay...\n");
        if (-1 == mclient_socket_connect_socket(socket_fd_remote))
        {
            err_code = MCLIENT_CONNECT_IPC_SOCKET_ERROR;
            goto error;
        }
        MCLIENT_LOG_LOG("MClient has been successfully connected to remote GateWay server. SOCKET_FD = %d\n", socket_fd_remote);

        MCLIENT_LOG_LOG("Begin processing...\n");
        err_code = begin_process(socket_fd_remote, mclient_type);

        mclient_socket_close_socket(socket_fd_remote);
        MCLIENT_LOG_LOG("Socket for remote connection with GateWay has just been closed. SOCKET_FD = %d\n", socket_fd_remote);
    }

    error:

    MCLIENT_LOG_LOG("Finishing 'MClient'\n");

    error_log:

    mclient_log_deinit_log();

    return err_code;
}
/* encoding: UTF-8 */

#ifndef MCLIENT_MAIN_H
#define MCLIENT_MAIN_H

#define MIN_SLEEP_TIME_MLS 400
#define MAX_SLEEP_TIME_MLS 500

#define ROWS 7
#define COLUMNS 7

typedef struct mtrx_fmt_s {
    int col_pos;
    int value;
} mtrx_fmt_t;

typedef struct send_mtrx_data_s {
    char header[4];
    mtrx_fmt_t mtrx;
} send_mtrx_data_t;

typedef struct send_client_type_data_s {
    char header[4];
    mclients_types_t mclient_type;
} send_client_type_data_t;

typedef struct recv_mtrx_row_data_request_s {
    char header[4];
    int row;
} recv_mtrx_row_data_request_t;

#endif /* MCLIENT_MAIN_H */
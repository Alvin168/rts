#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vclient_common.h"
#include "vclient_log.h"

#define LOGING_VCLIENT_LOG_FILE 1
#define PROJECT_ROOT_PATH_ENV_VARIABLE "RTS_PROJ_ROOT"

#define VCLIENT_LOG_FILE_DIRECTORY_PATH "log"
#define VCLIENT_LOG_FILE_NAME "vclient"
#define VCLIENT_LOG_FILE_EXTENSION "log"

FILE *vclient_log_f;

void vclient_log_deinit_log(void)
{
    if (LOGING_VCLIENT_LOG_FILE && NULL != vclient_log_f)
    {
        fclose(vclient_log_f);
        vclient_log_f = NULL;
    }
}

enum vclient_error_code_e vclient_log_init_log(char const *vclient_type_str)
{
    vclient_error_code_t err_code = VCLIENT_SUCCESS;

    if (LOGING_VCLIENT_LOG_FILE)
    {
        char *project_root_path;
        char log_file_path[100];

        project_root_path = getenv("RTS_PROJ_ROOT");
        if (NULL == project_root_path)
        {
            err_code = VCLIENT_LOGINIT_RTS_PROJ_ROOT_ENV_VAR_NOT_SET_ERROR;
            goto error;
        }

        strcpy(log_file_path, project_root_path);
        strcat(log_file_path, "/");
        strcat(log_file_path, VCLIENT_LOG_FILE_DIRECTORY_PATH);
        strcat(log_file_path, "/");
        strcat(log_file_path, VCLIENT_LOG_FILE_NAME);
        strcat(log_file_path, vclient_type_str);
        strcat(log_file_path, ".");
        strcat(log_file_path, VCLIENT_LOG_FILE_EXTENSION);

        vclient_log_f = fopen(log_file_path, "w");
        if (NULL == vclient_log_f)
        {
            err_code = VCLIENT_LOGINIT_OPEN_LOG_FILE_ERROR;
            goto error;
        }
    }
    else
    {
        vclient_log_f = stdout;
    }

    error:

    return err_code;
}
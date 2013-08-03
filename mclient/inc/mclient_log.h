/* encoding: UTF-8 */

#ifndef MCLIENT_LOG_H_
#define MCLIENT_LOG_H_

#include <stdio.h>

extern FILE *mclient_log_f;

#define MCLIENT_LOG_LOG(fmt, ...) fprintf(mclient_log_f, fmt, ##__VA_ARGS__)

enum mclient_error_code_e mclient_log_init_log(char const *mclient_type_str);
void mclient_log_deinit_log(void);

#endif /* MCLIENT_LOG_H_ */
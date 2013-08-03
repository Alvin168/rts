/* encoding: UTF-8 */

#ifndef VCLIENT_LOG_H_
#define VCLIENT_LOG_H_

#include <stdio.h>

extern FILE *vclient_log_f;

#define VCLIENT_LOG_LOG(fmt, ...) fprintf(vclient_log_f, fmt, ##__VA_ARGS__)

enum vclient_error_code_e vclient_log_init_log(char const *vclient_type_str);
void vclient_log_deinit_log(void);

#endif /* VCLIENT_LOG_H_ */
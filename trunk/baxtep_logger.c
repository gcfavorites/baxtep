#include "php_baxtep.h"
#include <time.h>
#include <stdlib.h>


void php_baxtep_log_write(char *message TSRMLS_DC)
{
	if (BAXTEP_G(logfile) == 0) {
		return;
	}

	php_stream *stream = php_stream_open_wrapper(BAXTEP_G(logfile), "a", IGNORE_URL_WIN | ENFORCE_SAFE_MODE, NULL);

	if (stream) {
		php_stream_write(stream, message, strlen(message));
		php_stream_close(stream);
	}
}

int php_baxtep_log(char *exec_str, char *fname TSRMLS_DC)
{
	char *log_str;
	char *date = (char *) emalloc(20);
	char *uri;
	time_t t;
	struct tm *tmp;

	// format current time
	t = time(NULL);
	tmp = localtime(&t);
	strftime(date, 20, "%F %T", tmp);

	// get query string
	uri = SG(request_info).query_string;

	// create message
	spprintf(&log_str, 0, "%s BAXTEP: %s CMDLINE: `%s` FILE: %s on line %i URI: %s\n",
		date,
		fname,
		exec_str,
		zend_get_executed_filename(TSRMLS_C),
		zend_get_executed_lineno(TSRMLS_C),
		uri
	);

	php_baxtep_log_write(log_str TSRMLS_CC);

	efree(log_str);
	efree(date);

	return SUCCESS;
}

void php_baxtep_logger_init()
{
	char *ini_str = INI_STR("baxtep.logfile");

	// TODO: if file doesn't exist, check folder for write
	BAXTEP_G(logfile) = (VCWD_ACCESS(ini_str, W_OK) == 0)?ini_str:0;
}

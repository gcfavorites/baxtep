#include "php_baxtep.h"

PHP_NAMED_FUNCTION(php_baxtep_execution_interceptor)
{
	php_baxtep_zif *fe;
	zval **exec_str;

	char *fname = get_active_function_name(TSRMLS_C);

	if (ZEND_NUM_ARGS() >= 1 && zend_get_parameters_ex(1, &exec_str) != FAILURE) {
                convert_to_string_ex(exec_str);
		if (php_baxtep_trap(Z_STRVAL_PP(exec_str), fname TSRMLS_CC) == FAILURE) {
                        // make fatal error if needed
                        RETURN_FALSE;
                }
        }

	if (zend_hash_find(BAXTEP_G(stolen_functions), fname, strlen(fname) + 1, (void**)&fe) != FAILURE) {
		(*fe)(INTERNAL_FUNCTION_PARAM_PASSTHRU);
	}
}

int php_baxtep_trap(char *exec_str, char *fname TSRMLS_DC)
{
	php_baxtep_log(exec_str, fname);

	return SUCCESS;
}

int php_baxtep_substitute_function(char *fname TSRMLS_DC)
{
	zend_internal_function *fe;
	int fname_len = strlen(fname);

	// check steal function is exists
	if (zend_hash_find(CG(function_table), fname, fname_len + 1, (void**)&fe) == FAILURE) {
		return FAILURE;
	}

	// check steal function is normal
	if (fe->type != ZEND_INTERNAL_FUNCTION) {
		return FAILURE;
	}

	// add to stolen_hash
	if (PHP_BAXTEP_HASH_ADD(BAXTEP_G(stolen_functions), fname, (void*)&(fe->handler), sizeof(php_baxtep_zif)) == FAILURE) {
		return FAILURE;
	}

	fe->handler = php_baxtep_execution_interceptor;

	return SUCCESS;
}



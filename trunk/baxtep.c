#include "php_baxtep.h"
ZEND_DECLARE_MODULE_GLOBALS(baxtep)

zend_module_entry baxtep_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"baxtep",
	NULL,
	PHP_MINIT(baxtep),
	PHP_MSHUTDOWN(baxtep),
	PHP_RINIT(baxtep),
	PHP_RSHUTDOWN(baxtep),
	PHP_MINFO(baxtep),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_BAXTEP_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

PHP_INI_BEGIN()
	PHP_INI_ENTRY("baxtep.logfile", "/tmp/baxtep_messages", PHP_INI_SYSTEM|PHP_INI_PERDIR, NULL)
	//STD_PHP_INI_BOOLEAN("baxtep.internal_override", "0", PHP_INI_SYSTEM, OnUpdateBool, internal_override, zend_baxtep_globals, baxtep_globals)
PHP_INI_END()

#ifdef COMPILE_DL_BAXTEP
ZEND_GET_MODULE(baxtep)
#endif

static void php_baxtep_globals_ctor(zend_baxtep_globals *baxtep_global TSRMLS_DC)
{
	baxtep_global->stolen_functions = NULL;
}

PHP_MINIT_FUNCTION(baxtep)
{
#ifdef ZTS
	ts_allocate_id(&baxtep_globals_id, sizeof(zend_baxtep_globals), php_baxtep_globals_ctor, NULL);
#else
	php_baxtep_globals_ctor(&baxtep_globals);
#endif

	REGISTER_INI_ENTRIES();

	BAXTEP_G(stolen_functions) = (HashTable *) malloc(sizeof(HashTable));
	zend_hash_init(BAXTEP_G(stolen_functions), 4, NULL, NULL, 1);

	// make spies :)
	php_baxtep_substitute_function("exec" TSRMLS_CC);
	php_baxtep_substitute_function("system" TSRMLS_CC);
	php_baxtep_substitute_function("shell_exec" TSRMLS_CC);
	php_baxtep_substitute_function("passthru" TSRMLS_CC);

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(baxtep)
{
	UNREGISTER_INI_ENTRIES();

	if (BAXTEP_G(stolen_functions)) {
		zend_hash_destroy(BAXTEP_G(stolen_functions));
		pefree(BAXTEP_G(stolen_functions), 1);
		BAXTEP_G(stolen_functions) = NULL;
	}

	return SUCCESS;
}

PHP_RINIT_FUNCTION(baxtep)
{
	php_baxtep_logger_init();

	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(baxtep)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(baxtep)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "baxtep support", "enabled");
	php_info_print_table_header(2, "version", PHP_BAXTEP_VERSION);
	php_info_print_table_header(2, "logfile", BAXTEP_G(logfile)?BAXTEP_G(logfile):"Not writable");
	php_info_print_table_end();

}

#ifndef PHP_BAXTEP_H
#define PHP_BAXTEP_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "main/SAPI.h"

#define PHP_BAXTEP_VERSION	"0.1"

typedef void (*php_baxtep_zif)(INTERNAL_FUNCTION_PARAMETERS);

extern zend_module_entry baxtep_module_entry;
#define phpext_baxtep_ptr &baxtep_module_entry

PHP_MINIT_FUNCTION(baxtep);
PHP_MSHUTDOWN_FUNCTION(baxtep);
PHP_RINIT_FUNCTION(baxtep);
PHP_RSHUTDOWN_FUNCTION(baxtep);
PHP_MINFO_FUNCTION(baxtep);

ZEND_BEGIN_MODULE_GLOBALS(baxtep)
	HashTable *stolen_functions;
	char *logfile;
ZEND_END_MODULE_GLOBALS(baxtep)

extern ZEND_DECLARE_MODULE_GLOBALS(baxtep);

// from core
int php_baxtep_steal_function(char * TSRMLS_DC);
//int php_baxtep_restore_functions(void *fe, int num_args, va_list args, zend_hash_key *hash_key);

// from logger
void php_baxtep_logger_init();
int php_baxtep_log(char *exec_str, char *fname TSRMLS_DC);

#ifdef ZTS
#define		BAXTEP_G(v)		TSRMG(baxtep_globals_id, zend_baxtep_globals *, v)
#define BAXTEP_TSRMLS_C		TSRMLS_C
#else
#define		BAXTEP_G(v)		(baxtep_globals.v)
#define BAXTEP_TSRMLS_C		NULL
#endif

#if defined(ZEND_ENGINE_2) && !defined(zend_hash_add_or_update)
/* Why doesn't ZE2 define this? */
#define zend_hash_add_or_update(ht, arKey, nKeyLength, pData, pDataSize, pDest, flag) \
        _zend_hash_add_or_update((ht), (arKey), (nKeyLength), (pData), (pDataSize), (pDest), (flag) ZEND_FILE_LINE_CC)
#endif

#if PHP_MAJOR_VERSION >= 6
#define PHP_BAXTEP_DECL_STRING_PARAM(param)		void *param; int32_t param##_len; zend_uchar param##_type;
#define PHP_BAXTEP_STRING_SPEC					"t"
#define PHP_BAXTEP_STRING_PARAM(param)			&param, &param##_len, &param##_type
#define PHP_BAXTEP_STRING_LEN(param,addtl)		(param##_type == IS_UNICODE ? UBYTES(param##_len + (addtl)) : (param##_len + (addtl)))
#define PHP_BAXTEP_STRING_TYPE(param)			(param##_type)
#define PHP_BAXTEP_HASH_FIND(hash,param,ppvar)	zend_u_hash_find(hash, param##_type, (UChar *)param, param##_len + 1, (void**)ppvar)
#define PHP_BAXTEP_HASH_EXISTS(hash,param)		zend_u_hash_exists(hash, param##_type, (UChar *)param, param##_len + 1)
#define PHP_BAXTEP_HASH_KEY(hash_key)			((hash_key)->type == HASH_KEY_IS_UNICODE ? (hash_key)->u.unicode : (hash_key)->u.string)
#define PHP_BAXTEP_HASH_KEYLEN(hash_key)		((hash_key)->type == HASH_KEY_IS_UNICODE ? UBYTES((hash_key)->nKeyLength) : (hash_key)->nKeyLength)
#define PHP_BAXTEP_HASH_ADD(hash, param, el, el_size)	zend_u_hash_add(hash, IS_STRING, (UChar *)param, param##_len + 1, el, el_size, NULL)

#elif PHP_MAJOR_VERSION >= 5
#define PHP_BAXTEP_DECL_STRING_PARAM(p)			char *p; int p##_len;
#define PHP_BAXTEP_STRING_SPEC					"s"
#define PHP_BAXTEP_STRING_PARAM(p)				&p, &p##_len
#define PHP_BAXTEP_STRING_LEN(param,addtl)		(param##_len + (addtl))
#define PHP_BAXTEP_STRING_TYPE(param)			IS_STRING
#define PHP_BAXTEP_HASH_FIND(hash,param,ppvar)	zend_hash_find(hash, param, param##_len + 1, (void**)ppvar)
#define PHP_BAXTEP_HASH_EXISTS(hash,param)		zend_hash_exists(hash, param##_type, param, param##_len + 1)
#define PHP_BAXTEP_HASH_KEY(hash_key)			((hash_key)->arKey)
#define PHP_BAXTEP_HASH_KEYLEN(hash_key)		((hash_key)->nKeyLength)
#define PHP_BAXTEP_HASH_ADD(hash, param, el, el_size)	zend_hash_add(hash, param, param##_len + 1, el, el_size, NULL)

#else /* PHP4 */
#define PHP_BAXTEP_DECL_STRING_PARAM(p)			char *p; int p##_len;
#define PHP_BAXTEP_STRING_SPEC					"s"
#define PHP_BAXTEP_STRING_PARAM(p)				&p, &p##_len
#define PHP_BAXTEP_STRING_LEN(param,addtl)		(param##_len + (addtl))
#define PHP_BAXTEP_STRING_TYPE(param)			IS_STRING
#define PHP_BAXTEP_HASH_FIND(hash,param,ppvar)	zend_hash_find(hash, param, param##_len + 1, (void**)ppvar)
#define PHP_BAXTEP_HASH_EXISTS(hash,param)		zend_hash_exists(hash, param##_type, param, param##_len + 1)
#define PHP_BAXTEP_HASH_KEY(hash_key)			((hash_key)->arKey)
#define PHP_BAXTEP_HASH_KEYLEN(hash_key)		((hash_key)->nKeyLength)
#define PHP_BAXTEP_HASH_ADD(hash, param, el, el_size)	zend_hash_add(hash, param, param##_len + 1, el, el_size, NULL)
#define zend_function_dtor						destroy_zend_function

#endif /* Version Agnosticism */

#endif	/* PHP_BAXTEP_H */

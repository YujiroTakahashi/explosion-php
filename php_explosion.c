
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/json/php_json.h"
#include "php_explosion.h"

/* If you declare any globals in php_explosion.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(explosion)
*/

/* True global resources - no need for thread safety here */
static int le_explosion;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("explosion.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_explosion_globals, explosion_globals)
    STD_PHP_INI_ENTRY("explosion.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_explosion_globals, explosion_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto array croco_explosion(string haystack, string file[, string regfile]) */
PHP_FUNCTION(croco_explosion)
{
	char *haystack = NULL;
	size_t haystack_len;
	char *file = NULL;
	size_t file_len;
	char *regfile = NULL;
	size_t regfile_len = 0;

	ExplosionHandle handle;
	EPStr json;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss|s", &haystack, &haystack_len, &file, &file_len, &regfile, &regfile_len) == FAILURE) {
		return;
	}

	handle = ExplosionCreate(haystack);

	if (file_len != 0) {
		ExplosionFindMatch(handle, file);
	}

	if (regfile_len != 0) {
		ExplosionRegexMatch(handle, regfile);
	}

	json = ExplosionExplode(handle);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);

	ExplosionFreeText(json);
	ExplosionFree(handle);
}
/* }}} */

/* {{{ proto array croco_explosionRe(string haystack, string pattern) */
PHP_FUNCTION(croco_explosionRe)
{
	char *haystack = NULL;
	size_t haystack_len;
	char *pattern = NULL;
	size_t pattern_len;

	ExplosionHandle handle;
	EPStr json;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss|s", &haystack, &haystack_len, &pattern, &pattern_len) == FAILURE) {
		return;
	}

	handle = ExplosionCreate(haystack);
	ExplosionRegexSearch(handle, pattern);
	json = ExplosionExplode(handle);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);

	ExplosionFreeText(json);
	ExplosionFree(handle);
}
/* }}} */


/* {{{ php_explosion_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_explosion_init_globals(zend_explosion_globals *explosion_globals)
{
	explosion_globals->global_value = 0;
	explosion_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(explosion)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	REGISTER_LONG_CONSTANT("EXPLOSION_TYPE_NONE",  0, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("EXPLOSION_TYPE_FIND",  1, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("EXPLOSION_TYPE_REGEX",  2, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(explosion)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(explosion)
{
#if defined(COMPILE_DL_EXPLOSION) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(explosion)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(explosion)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "explosion support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_explosion, 2)
	ZEND_ARG_INFO(0, haystack)
	ZEND_ARG_INFO(0, file)
	ZEND_ARG_INFO(0, reges_file)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_explosion_re, 2)
	ZEND_ARG_INFO(0, haystack)
	ZEND_ARG_INFO(0, pattern)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ explosion_functions[]
 *
 * Every user visible function must have an entry in explosion_functions[].
 */
const zend_function_entry explosion_functions[] = {
	PHP_FE(croco_explosion,	  arginfo_explosion)
	PHP_FE(croco_explosionRe, arginfo_explosion_re)
	PHP_FE_END	/* Must be the last line in explosion_functions[] */
};
/* }}} */

/* {{{ explosion_module_entry
 */
zend_module_entry explosion_module_entry = {
	STANDARD_MODULE_HEADER,
	"explosion",
	explosion_functions,
	PHP_MINIT(explosion),
	PHP_MSHUTDOWN(explosion),
	PHP_RINIT(explosion),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(explosion),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(explosion),
	PHP_EXPLOSION_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_EXPLOSION
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(explosion)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

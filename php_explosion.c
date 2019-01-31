#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/json/php_json.h"
#include "php_explosion.h"
#include "main/SAPI.h"

#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "SAPI.h"

ZEND_DECLARE_MODULE_GLOBALS(explosion)

/* {{{ PHP_INI
*/
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("explosion.model_dir",  NULL, PHP_INI_SYSTEM, OnUpdateString, model_dir, zend_explosion_globals, explosion_globals)
PHP_INI_END()
/* }}} */

/* Handlers */
static zend_object_handlers explosion_object_handlers;

/* Class entries */
zend_class_entry *php_explosion_sc_entry;

/* {{{ proto void explosion::__construct()
 */
PHP_METHOD(croco_explosion, __construct)
{
	php_explosion_object *ex_obj;
	zval *object = getThis();

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	ex_obj = Z_EXPLOSION_P(object);
	ex_obj->explosion = ExplosionCreate();
}
/* }}} */

/* {{{ proto void explosion::load(String key, String filename)
 */
PHP_METHOD(croco_explosion, load)
{
	php_explosion_object *ex_obj;
	zval *object = getThis();
	char *key;
	size_t key_len;
	char *filename;
	size_t filename_len;

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ss", &key, &key_len, &filename, &filename_len)) {
		return;
	}

	ex_obj = Z_EXPLOSION_P(object);
	ExplosionLoad(ex_obj->explosion, key, filename);
}
/* }}} */

/* {{{ proto array explosion::explode(String haystack, String find_key[, String regex_key])
 */
PHP_METHOD(croco_explosion, explode)
{
	php_explosion_object *ex_obj;
	zval *object = getThis();
	char *haystack = NULL;
	size_t haystack_len;
	char *find_key = NULL;
	size_t find_key_len;
	char *regex_key = NULL;
	size_t regex_key_len = 0;
	EPStr json;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss|s", &haystack, &haystack_len, &find_key, &find_key_len, &regex_key, &regex_key_len) == FAILURE) {
		return;
	}

	ex_obj = Z_EXPLOSION_P(object);
	ExplosionSetHaystack(ex_obj->explosion, haystack);

	if (find_key_len != 0) {
		ExplosionFindMatch(ex_obj->explosion, find_key);
	}

	if (regex_key_len != 0) {
		ExplosionRegexMatch(ex_obj->explosion, regex_key);
	}

	json = ExplosionExplode(ex_obj->explosion);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);

	ExplosionFreeText(json);
}
/* }}} */

/* {{{ proto array explosion::explodeRe(String haystack, String pattern)
 */
PHP_METHOD(croco_explosion, explodeRe)
{
	php_explosion_object *ex_obj;
	zval *object = getThis();
	char *haystack = NULL;
	size_t haystack_len;
	char *pattern = NULL;
	size_t pattern_len;
	EPStr json;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &haystack, &haystack_len, &pattern, &pattern_len) == FAILURE) {
		return;
	}

	ex_obj = Z_EXPLOSION_P(object);
	ExplosionSetHaystack(ex_obj->explosion, haystack);

	ExplosionRegexSearch(ex_obj->explosion, pattern);
	json = ExplosionExplode(ex_obj->explosion);

	array_init(return_value);
	php_json_decode(return_value, json->buff, json->len, 1, PHP_JSON_PARSER_DEFAULT_DEPTH);

	ExplosionFreeText(json);
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_explosion_void, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_explosion_load, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, file)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_explosion_explode, 0, 0, 2)
	ZEND_ARG_INFO(0, haystack)
	ZEND_ARG_INFO(0, find_key)
	ZEND_ARG_INFO(0, regex_key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_explosion_explode_re, 0, 0, 2)
	ZEND_ARG_INFO(0, haystack)
	ZEND_ARG_INFO(0, pattern)
ZEND_END_ARG_INFO()

/* }}} */


/* {{{ php_sexplosion_class_methods */
static zend_function_entry php_explosion_class_methods[] = {
	PHP_ME(croco_explosion, __construct, arginfo_explosion_void,       ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(croco_explosion, load,        arginfo_explosion_load,       ZEND_ACC_PUBLIC)
	PHP_ME(croco_explosion, explode,     arginfo_explosion_explode,    ZEND_ACC_PUBLIC)
	PHP_ME(croco_explosion, explodeRe,   arginfo_explosion_explode_re, ZEND_ACC_PUBLIC)

	PHP_FE_END
};
/* }}} */

static void php_explosion_object_free_storage(zend_object *object) /* {{{ */
{
	php_explosion_object *intern = php_explosion_from_obj(object);

	if (!intern) {
		return;
	}

	if (intern->explosion) {
		ExplosionFree(intern->explosion);
		intern->explosion = NULL;
	}

	zend_object_std_dtor(&intern->zo);
}
/* }}} */

static zend_object *php_explosion_object_new(zend_class_entry *class_type) /* {{{ */
{
	php_explosion_object *intern;

	/* Allocate memory for it */
	int ftxtsize = ExplosionSize();
	intern = ecalloc(1, sizeof(php_explosion_object) + zend_object_properties_size(class_type) + ftxtsize);

	zend_object_std_init(&intern->zo, class_type);
	object_properties_init(&intern->zo, class_type);

	intern->zo.handlers = &explosion_object_handlers;

	return &intern->zo;
}
/* }}} */


/* {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(explosion)
{
	zend_class_entry ce;

	memcpy(&explosion_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	/* Register Explosion Class */
	INIT_CLASS_ENTRY(ce, "Croco\\Explosion", php_explosion_class_methods);
	ce.create_object = php_explosion_object_new;
	explosion_object_handlers.offset = XtOffsetOf(php_explosion_object, zo);
	explosion_object_handlers.clone_obj = NULL;
	explosion_object_handlers.free_obj = php_explosion_object_free_storage;
	php_explosion_sc_entry = zend_register_internal_class(&ce);

	REGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(explosion)
{
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(explosion)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "Explosion support", "enabled");
	php_info_print_table_row(2, "Explosion module version", PHP_EXPLOSION_VERSION);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ PHP_GINIT_FUNCTION
*/
static PHP_GINIT_FUNCTION(explosion)
{
	memset(explosion_globals, 0, sizeof(*explosion_globals));
}
/* }}} */

/* {{{ explosion_module_entry
*/
zend_module_entry explosion_module_entry = {
	STANDARD_MODULE_HEADER,
	"explosion",
	NULL,
	PHP_MINIT(explosion),
	PHP_MSHUTDOWN(explosion),
	NULL,
	NULL,
	PHP_MINFO(explosion),
	PHP_EXPLOSION_VERSION,
	PHP_MODULE_GLOBALS(explosion),
	PHP_GINIT(explosion),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
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

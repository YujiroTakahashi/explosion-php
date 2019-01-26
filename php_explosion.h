
#ifndef PHP_EXPLOSION_H
#define PHP_EXPLOSION_H

extern zend_module_entry explosion_module_entry;
#define phpext_explosion_ptr &explosion_module_entry

#define PHP_EXPLOSION_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_EXPLOSION_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_EXPLOSION_API __attribute__ ((visibility("default")))
#else
#	define PHP_EXPLOSION_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(explosion)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(explosion)
*/

/* Always refer to the globals in your function as EXPLOSION_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define EXPLOSION_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(explosion, v)

#if defined(ZTS) && defined(COMPILE_DL_EXPLOSION)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_EXPLOSION_H */


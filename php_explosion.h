#ifndef PHP_EXPLOSION_H
#define PHP_EXPLOSION_H

#include "explosion_api.h"

#define PHP_EXPLOSION_VERSION	"0.1.0"

extern zend_module_entry explosion_module_entry;
#define phpext_explosion_ptr &explosion_module_entry

ZEND_BEGIN_MODULE_GLOBALS(explosion)
	char *model_dir;
ZEND_END_MODULE_GLOBALS(explosion)

#ifdef ZTS
# define EXPLOSION_G(v) TSRMG(explosion_globals_id, zend_explosion_globals *, v)
# ifdef COMPILE_DL_EXPLOSION
ZEND_TSRMLS_CACHE_EXTERN()
# endif
#else
# define EXPLOSION_G(v) (explosion_globals.v)
#endif

typedef struct {
    zend_object zo;
	zval error;
    ExplosionHandle explosion;
} php_explosion_object;

static inline php_explosion_object *php_explosion_from_obj(zend_object *obj) {
	return (php_explosion_object*)((char*)(obj) - XtOffsetOf(php_explosion_object, zo));
}

#define Z_EXPLOSION_P(zv) php_explosion_from_obj(Z_OBJ_P((zv)))


#endif  /* PHP_EXPLOSION_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
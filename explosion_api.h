#ifndef __CROCO_EXPLOSION_API_H
#define __CROCO_EXPLOSION_API_H

#include <stdint.h>

#ifdef __cplusplus

#include "explosion.h"

extern "C" {

#endif /* __cplusplus */

#ifndef EXPLOSION_API
#   if defined(_WIN32) || defined(_WIN64)
#       define EXPLOSION_API __declspec(dllimport)
#   else
#       define EXPLOSION_API extern
#   endif /* defined(_WIN32) || defined(_WIN64) */
#endif /* EXPLOSION_API */

#ifndef EXPLOSION_VERSION
#    define EXPLOSION_VERSION 1
#endif /* EXPLOSION_VERSION */

#define EXPLOSION_TRUE           (1)
#define EXPLOSION_FALSE          (0)

struct _EPStr {
    size_t len;
    char *buff;
};

typedef struct _EPStr *EPStr;
typedef void *ExplosionHandle;

EXPLOSION_API int ExplosionSize();
EXPLOSION_API ExplosionHandle ExplosionCreate(const char *haystack, const char *file);
EXPLOSION_API void ExplosionFree(ExplosionHandle handle);
EXPLOSION_API void ExplosionFreeText(EPStr handle);
EXPLOSION_API EPStr ExplosionExplode(ExplosionHandle handle);

#ifdef __cplusplus
}

inline EPStr EPStrVal(std::string word) {
    EPStr retval = new struct _EPStr;
    retval->len = word.length();
    retval->buff = new char[retval->len + 1];
    strcpy(retval->buff, word.c_str());
    return retval;
}

#endif /* __cplusplus */

#endif /* __CROCO_EXPLOSION_API_H */

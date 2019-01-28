#include <string>

#include "json.hpp"
#include "explosion_api.h"

using namespace croco;

/**
 * create a Explosion handle
 *
 * @access public
 * @param  const char *haystack
 * @return ExplosionHandle
 */
ExplosionHandle ExplosionCreate(const char *haystack)
{
    ExplosionHandle handle = new explosion(std::string(haystack));
    return handle;
}

/**
 * free a Explosion handle
 *
 * @access public
 * @param  ExplosionHandle handle
 * @return void
 */
void ExplosionFree(ExplosionHandle handle)
{
    delete static_cast<explosion*>(handle);
}

/**
 * free a char handle
 *
 * @access public
 * @param  EPStr handle
 * @return void
 */
void ExplosionFreeText(EPStr handle)
{
    if (nullptr != handle->buff) {
        delete[] handle->buff;
    }

    if (nullptr != handle) {
        delete handle;
    }
}

/**
 * 正規表現検索
 *
 * @access public
 * @param  ExplosionHandle handle
 * @param  const char *file
 * @return void
 */
void ExplosionRegexMatch(ExplosionHandle handle, const char *file)
{
    explosion *explode = static_cast<explosion*>(handle);

    explode->regexMatch(std::string(file));
}

/**
 * 文字列検索
 *
 * @access public
 * @param  ExplosionHandle handle
 * @param  const char *file
 * @return void
 */
void ExplosionFindMatch(ExplosionHandle handle, const char *file)
{
    explosion *explode = static_cast<explosion*>(handle);

    explode->findMatch(std::string(file));
}

/**
 * Explode
 *
 * @access public
 * @param  ExplosionHandle handle
 * @return char *
 */
EPStr ExplosionExplode(ExplosionHandle handle)
{
    explosion *explode = static_cast<explosion*>(handle);
    
    nlohmann::json retj = explode->explode();

    std::string word = retj.dump();
    EPStr retval = new struct _EPStr;
    retval->len = word.length();
    retval->buff = new char[retval->len + 1];
    strcpy(retval->buff, word.c_str());

    return retval;
}

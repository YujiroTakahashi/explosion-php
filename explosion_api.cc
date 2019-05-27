#include <string>

#include "json.hpp"
#include "explosion_api.h"

using namespace croco;

/**
 * get Explosion size
 *
 * @access public
 * @return int
 */
int ExplosionSize()
{
    return sizeof(explosion);
}

/**
 * create a Explosion handle
 *
 * @access public
 * @return ExplosionHandle
 */
ExplosionHandle ExplosionCreate()
{
    ExplosionHandle handle = new explosion();
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
 * 対象文字列の設定
 *
 * @access public
 * @param  ExplosionHandle handle
 * @param  const char *haystack
 * @return void
 */
void ExplosionSetHaystack(ExplosionHandle handle, const char *haystack)
{
    explosion *explode = static_cast<explosion*>(handle);

    explode->setHaystack(std::string(haystack));
}

/**
 * 辞書の読み込み設定
 *
 * @access public
 * @param  ExplosionHandle handle
 * @param  const char *pattern
 * @return void
 */
void ExplosionLoad(ExplosionHandle handle, const char *key, const char *file)
{
    explosion *explode = static_cast<explosion*>(handle);

    explode->load(std::string(key), std::string(file));
}

/**
 * 正規表現検索
 *
 * @access public
 * @param  ExplosionHandle handle
 * @param  const char *pattern
 * @return void
 */
void ExplosionRegexSearch(ExplosionHandle handle, const char *pattern)
{
    explosion *explode = static_cast<explosion*>(handle);

    explode->regexSearch(std::string(pattern));
}

/**
 * 正規表現検索
 *
 * @access public
 * @param  ExplosionHandle handle
 * @param  const char *key
 * @return void
 */
void ExplosionRegexMatch(ExplosionHandle handle, const char *key)
{
    explosion *explode = static_cast<explosion*>(handle);

    explode->regexMatch(std::string(key));
}

/**
 * 文字列検索
 *
 * @access public
 * @param  ExplosionHandle handle
 * @param  const char *key
 * @return void
 */
void ExplosionFindMatch(ExplosionHandle handle, const char *key)
{
    explosion *explode = static_cast<explosion*>(handle);

    explode->findMatch(std::string(key));
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

/**
 * Ngram
 *
 * @access public
 * @param  ExplosionHandle handle
 * @param  const char *data
 * @param  size_t minn
 * @param  size_t maxn
 * @param  size_t step
 * @return EPStr
 */
EPStr ExplosionNgram(ExplosionHandle handle, const char *data, size_t minn, size_t maxn, size_t step)
{
    explosion *explode = static_cast<explosion*>(handle);
    
    nlohmann::json retj = explode->ngram(std::string(data), minn, maxn, step);

    std::string word = retj.dump();
    EPStr retval = new struct _EPStr;
    retval->len = word.length();
    retval->buff = new char[retval->len + 1];
    strcpy(retval->buff, word.c_str());

    return retval;
}
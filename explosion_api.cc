#include <string>

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
 * 文字列一覧の検索
 *
 * @access public
 * @param  ExplosionHandle handle
 * @param  const char *key
 * @return void
 */
void ExplosionFindAll(ExplosionHandle handle, HashTable *ht)
{
    explosion *explode = static_cast<explosion*>(handle);

    size_t size = zend_hash_num_elements(ht);
    std::vector<std::string> finds;

    for (size_t idx=0; idx<size; idx++) {
        zval *value = zend_hash_get_current_data(ht);
        zend_string *str = zval_get_string(value);
        finds.push_back(std::string(ZSTR_VAL(str)));
        zend_hash_move_forward(ht);
    }

    explode->findAll(finds);
}

/**
 * Explode
 *
 * @access public
 * @param  ExplosionHandle handle
 * @return char *
 */
void ExplosionExplode(ExplosionHandle handle, zval *retval)
{
    explosion *explode = static_cast<explosion*>(handle);
    
    std::vector<explosion::NodeFormat> nodes = explode->explode();

    array_init(retval);
    zend_ulong idx = 0;

    for (auto node : nodes) {
        zval rowVal, surfaceVal, typeVal, fromVal, fromLineVal, fromChVal
           , toVal, toLineVal, toChVal;

        ZVAL_STRINGL(&surfaceVal, node.surface.c_str(), node.surface.length());
        ZVAL_LONG(&typeVal, node.type);
        ZVAL_LONG(&fromLineVal, node.from.line);
        ZVAL_LONG(&fromChVal, node.from.ch);
        ZVAL_LONG(&toLineVal, node.to.line);
        ZVAL_LONG(&toChVal, node.to.ch);

        array_init(&fromVal);
        zend_hash_str_add(Z_ARRVAL_P(&fromVal), "line", sizeof("line")-1, &fromChVal);
        zend_hash_str_add(Z_ARRVAL_P(&fromVal), "ch", sizeof("ch")-1, &fromChVal);

        array_init(&toVal);
        zend_hash_str_add(Z_ARRVAL_P(&toVal), "line", sizeof("line")-1, &toLineVal);
        zend_hash_str_add(Z_ARRVAL_P(&toVal), "ch", sizeof("ch")-1, &toChVal);

        array_init(&rowVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "surface", sizeof("surface")-1, &surfaceVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "type", sizeof("type")-1, &typeVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "from", sizeof("from")-1, &fromVal);
        zend_hash_str_add(Z_ARRVAL_P(&rowVal), "to", sizeof("to")-1, &toVal);

        add_index_zval(retval, idx, &rowVal);
        idx++;
    }
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
void ExplosionNgram(ExplosionHandle handle, zval *retval, const char *data, size_t minn, size_t maxn, size_t step)
{
    explosion *explode = static_cast<explosion*>(handle);
    std::vector<std::string> ngrams = explode->ngram(std::string(data), minn, maxn, step);

    array_init(retval);
    zend_ulong idx = 0;
    for (auto ngram : ngrams) {
        add_index_string(retval, idx, ngram.c_str());
        idx++;
    }
}
#include <string>

#include "json.hpp"
#include "explosion_api.h"

using namespace croco;

/**
 * get markov size
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
 * @param  const std::string str
 * @param  const int nsize
 * @return ExplosionHandle
 */
ExplosionHandle ExplosionCreate(const char *haystack, const char *file)
{
    ExplosionHandle handle = new explosion(std::string(haystack), std::string(file));
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
 * fgenerate the text
 *
 * @access public
 * @param  ExplosionHandle handle
 * @return char *
 */
EPStr ExplosionExplode(ExplosionHandle handle)
{
    explosion *explode = static_cast<explosion*>(handle);
    nlohmann::json retj;

    std::vector<std::string> list = explode->explode();
    nlohmann::json retj = nlohmann::json::parse(list.begin(), list.end());

    return EPStrVal(retj.dump());
}

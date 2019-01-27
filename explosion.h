#ifndef __CROCO_EXPLOSION_H__
#define __CROCO_EXPLOSION_H__

#ifdef __cplusplus

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "json.hpp"

namespace croco {

class explosion {
public:
	typedef std::map<std::size_t, std::size_t> delimiters_t;

private:
    delimiters_t _delimiters;
    std::string _haystack;

public:
	explosion(const std::string haystack, const std::string file);
	nlohmann::json explode();

private:
    void _find(const std::string needle);
}; // class explosion

} // namespace croco

#endif /* __cplusplus */

#endif // #ifndef __CROCO_EXPLOSION_H__
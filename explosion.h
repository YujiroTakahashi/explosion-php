#ifndef __CROCO_EXPLOSION_H__
#define __CROCO_EXPLOSION_H__

#ifdef __cplusplus

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <re2/re2.h>
#include "json.hpp"

namespace croco {

class explosion {
public:
    enum {
        TYPE_NONE,
        TYPE_FIND,
        TYPE_REGEX
    };
    typedef struct _Node {
        std::string str;
        std::size_t position;
        std::size_t length;
        int type;
    } Node;
	typedef std::map<std::size_t, Node> pieces_t;

private:
    pieces_t _pieces;
    std::string _haystack;

public:
	explosion(const std::string haystack);
    void regexSearch(const std::string pattern);
	void regexMatch(const std::string file);
    void findMatch(const std::string file);
	nlohmann::json explode();

private:
    void _regexSearch(const std::string pattern);
}; // class explosion

} // namespace croco

#endif /* __cplusplus */

#endif // #ifndef __CROCO_EXPLOSION_H__
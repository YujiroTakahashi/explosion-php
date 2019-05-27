#ifndef __CROCO_EXPLOSION_H__
#define __CROCO_EXPLOSION_H__

#ifdef __cplusplus

#include <fstream>
#include <map>
#include <string>
#include <unordered_map>
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
    std::unordered_map<std::string, std::vector<std::string>> _dictionaries;

public:
    void setHaystack(const std::string haystack);
    void load(const std::string key, const std::string file);
    void regexSearch(const std::string pattern);
	void regexMatch(const std::string key);
    void findMatch(const std::string key);
	nlohmann::json explode();
    nlohmann::json ngram(
        const std::string input, 
        size_t minn = 3, 
        size_t maxn = 6, 
        size_t step = 1
    );

private:
    void _regexSearch(const std::string pattern);
    int _utf8_strlen(const std::string word);
    nlohmann::json _getNode(const std::string surface, const int type, int &no, std::string &text);
    std::vector<std::string> _explode(const std::string str);
}; // class explosion

} // namespace croco

#endif /* __cplusplus */

#endif // #ifndef __CROCO_EXPLOSION_H__

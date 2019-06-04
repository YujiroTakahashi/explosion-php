#ifndef __CROCO_EXPLOSION_H__
#define __CROCO_EXPLOSION_H__

#ifdef __cplusplus

#include <fstream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <re2/re2.h>

namespace croco {

class explosion {
public:
    enum node_type_t {
        TYPE_NONE,
        TYPE_FIND,
        TYPE_REGEX
    };
    typedef struct _position_t {
        std::size_t line;
        std::size_t ch;
    } position_t;
    typedef struct _Node {
        std::string str;
        std::size_t position;
        std::size_t length;
        node_type_t type;
    } Node;
    typedef struct _NodeFormat {
        std::string surface;
        node_type_t type;
        position_t from;
        position_t to;
    } NodeFormat;

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
    void findAll(std::vector<std::string> dictionary);
	std::vector<NodeFormat> explode();
    std::vector<std::string> ngram(
        const std::string input, 
        size_t minn = 3, 
        size_t maxn = 6, 
        size_t step = 1
    );

private:
    void _regexSearch(const std::string pattern);
    int _utf8_strlen(const std::string word);
    NodeFormat _getNode(const std::string surface, const node_type_t type, int &no, std::string &text);
    std::vector<std::string> _explode(const std::string str);
}; // class explosion

} // namespace croco

#endif /* __cplusplus */

#endif // #ifndef __CROCO_EXPLOSION_H__

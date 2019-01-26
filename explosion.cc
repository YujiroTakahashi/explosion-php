#include "explosion.h"

namespace croco {

/**
 * コンストラクタ
 *
 * @access public
 * @param  const std::string haystack
 * @param  const std::string file
 */
explosion::explosion(const std::string haystack, const std::string file)
{
    std::ifstream ifs(file);
    if (ifs.fail()) {
        return ;
    }
    _haystack = haystack;

    std::string needle;
    while (std::getline(ifs, needle)) {
        _find(needle);
    }
}

/**
 * データの抽出
 *
 * @access public
 * @return std::vector<std::string>
 */
std::vector<std::string> explosion::explode()
{
    std::vector<std::string> pieces;

    std::size_t pos = 0;
    for (auto &node :_delimiters) {
        if (pos < node.first) {
            pieces.push_back(_haystack.substr(
                pos, node.first - pos
            ));

            pieces.push_back(_haystack.substr(
                node.first, node.second
            ));
        }
        pos = node.first + node.second;
    }

    if (_haystack.length() > pos) {
        pieces.push_back(_haystack.substr(pos));
    }

    return pieces;
}


/**
 * 文字の位置検索
 *
 * @access prviate
 * @param  const std::string needle
 * @return void
 */
void explosion::_find(const std::string needle)
{ 
    std::size_t length = needle.length();
    std::size_t pos = _haystack.find(needle);
 
    while (pos != std::string::npos) {
        _delimiters.insert(std::make_pair(pos, length));
        pos = _haystack.find(needle, pos + length);
    }
}


} // namespace croco
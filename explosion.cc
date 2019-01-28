#include <iostream>
#include "explosion.h"

namespace croco {

/**
 * コンストラクタ
 *
 * @access public
 * @param  const std::string haystack
 * @param  const std::string file
 */
explosion::explosion(const std::string haystack)
{
    _haystack = haystack;
}

/**
 * 正規表現
 *
 * @access public
 * @param  const std::string file
 */
void explosion::regexMatch(const std::string file)
{
    std::ifstream ifs(file);
    if (ifs.fail()) {
        return ;
    }

    std::string pattern;
    while (std::getline(ifs, pattern)) {
        if (pattern.length()) {
            std::regex reg(pattern);
            std::sregex_iterator it(std::begin(_haystack), std::end(_haystack), reg);
            std::sregex_iterator end;

            for (; it != end; ++it) {
                auto&& match = *it;
                std::size_t position = static_cast<std::size_t>(match.position());
                std::size_t length = static_cast<std::size_t>(match.length());
                Node node = {
                    match.str(),
                    position,
                    length,
                    TYPE_REGEX
                };
                _pieces.insert(std::make_pair(position, node));
            }
        } // if (pattern.length())
    } // while (std::getline(ifs, pattern))
}

/**
 * 文字列検索
 *
 * @access public
 * @param  const std::string file
 */
void explosion::findMatch(const std::string file)
{
    std::ifstream ifs(file);
    if (ifs.fail()) {
        return ;
    }

    std::string needle;
    while (std::getline(ifs, needle)) {
        if (needle.length()) {
            std::size_t length = needle.length();
            std::size_t position = _haystack.find(needle);
            while (position != std::string::npos) {
                Node node = {
                    needle, 
                    position, 
                    length,
                    TYPE_FIND
                };
                _pieces.insert(std::make_pair(position, node));
                position = _haystack.find(needle, position + length);
            }
        } // if (needle.length())
    } // while (std::getline(ifs, needle))
}

/**
 * データの抽出
 *
 * @access public
 * @return std::vector<std::string>
 */
nlohmann::json explosion::explode()
{
    nlohmann::json pieces;

    int idx = 0;
    std::size_t position = 0;
    for (auto &node :_pieces) {
        if (position < node.first) {
            pieces[idx]["sentence"] = _haystack.substr(
                position, node.first - position
            );
            pieces[idx]["type"] = TYPE_NONE;
            idx++;

            pieces[idx]["sentence"] = _haystack.substr(
                node.first, node.second.length
            );
            pieces[idx]["type"] = node.second.type;
            idx++;
        }
        position = node.first + node.second.length;
    } // for (auto &node :_pieces)

    if (_haystack.length() > position) {
        pieces[idx]["sentence"] = _haystack.substr(position);
        pieces[idx]["type"] = TYPE_NONE;
    }

    return pieces;
}

} // namespace croco

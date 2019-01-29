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
 * 直接正規表現
 *
 * @access public
 * @param  const std::string pattern
 */
void explosion::regexSearch(const std::string pattern)
{
    _regexSearch(pattern);
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
        _regexSearch(pattern);
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
        if (position <= node.first) {
            if (node.first - position) {
                pieces[idx]["sentence"] = _haystack.substr(
                    position, node.first - position
                );
                pieces[idx]["type"] = TYPE_NONE;
                idx++;
            }

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

/**
 * 直接正規表現
 *
 * @access private
 * @param  const std::string pattern
 */
void explosion::_regexSearch(const std::string pattern)
{
    if (0 >= pattern.length()) {
        return ;
    }

    re2::RE2 reg(pattern);
    re2::StringPiece input(_haystack);
    std::string sentence;
    std::size_t position = 0;

    while (re2::RE2::FindAndConsume(&input, reg, &sentence)) {
        std::size_t length = sentence.length();
        position = _haystack.find(sentence, position);

        Node node = {
            sentence,
            position,
            length,
            TYPE_REGEX
        };
        _pieces.insert(std::make_pair(position, node));

        position = position + length;
    }
}


} // namespace croco

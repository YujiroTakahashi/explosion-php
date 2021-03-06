#include <iostream>
#include "explosion.h"

namespace croco {

/**
 * 直接正規表現
 *
 * @access public
 * @param  const std::string pattern
 */
void explosion::setHaystack(const std::string haystack)
{
    _haystack = haystack;
    _pieces.clear();
}

/**
 * 直接正規表現
 *
 * @access public
 * @param  const std::string pattern
 */
void explosion::load(const std::string key, const std::string file)
{
    if (_dictionaries.find(key) != _dictionaries.end()) {
        return ;
    }

    std::ifstream ifs(file);
    if (ifs.fail()) {
        return ;
    }

    std::vector<std::string> dictionary;
    std::string node;
    while (std::getline(ifs, node)) {
        if (node.length()) {
            dictionary.push_back(node);
        } // if (node.length())
    } // while (std::getline(ifs, node))

    if (!dictionary.size()) {
        return ;
    }

    _dictionaries.insert(std::make_pair(key, dictionary));
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
void explosion::regexMatch(const std::string key)
{
    if (_dictionaries.find(key) == _dictionaries.end()) {
        return ;
    }
    std::vector<std::string> dictionary = _dictionaries.at(key);

    for (auto &pattern : dictionary) {
        _regexSearch(pattern);
    } // while (std::getline(ifs, pattern))
}

/**
 * 文字列検索
 *
 * @access public
 * @param  const std::string file
 */
void explosion::findMatch(const std::string key)
{
    if (_dictionaries.find(key) == _dictionaries.end()) {
        return ;
    }
    std::vector<std::string> dictionary = _dictionaries.at(key);

    for (auto &needle : dictionary) {
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
    } // for (auto &needle : dictionary)
}

/**
 * 文字列一覧の検索
 *
 * @access public
 * @param  const std::vector<std::string> dictionary
 */
void explosion::findAll(std::vector<std::string> dictionary)
{
    for (auto &needle : dictionary) {
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
    } // for (auto &needle : dictionary)
}

/**
 * データの抽出
 *
 * @access public
 * @return std::vector<NodeFormat>
 */
std::vector<explosion::NodeFormat> explosion::explode()
{
    std::vector<NodeFormat> pieces;

    int idx = 0, no = 0;
    std::string text("");
    std::size_t position = 0;
    for (auto &node :_pieces) {
        if (position <= node.first) {
            if (node.first - position) {
                std::string surface = _haystack.substr(
                    position, node.first - position
                );
                pieces.push_back(_getNode(
                    surface, 
                    TYPE_NONE, 
                    no, 
                    text
                ));
                idx++;
            }

            {
                std::string surface = _haystack.substr(
                    node.first, node.second.length
                );
                pieces.push_back(_getNode(
                    surface, 
                    node.second.type, 
                    no, 
                    text
                ));
                idx++;
            }
        }
        position = node.first + node.second.length;
    } // for (auto &node :_pieces)

    if (_haystack.length() > position) {
        std::string surface = _haystack.substr(position);
        pieces.push_back(_getNode(surface, TYPE_NONE, no, text));
    }

    return pieces;
}

/**
 * N-gram文字列リスト取得
 *
 * @access private
 * @param  const std::string input
 * @param  size_t minn
 * @param  size_t maxn
 * @param  size_t step
 * @return std::vector<std::string>
 */
std::vector<std::string> explosion::ngram(const std::string input, size_t minn, size_t maxn, size_t step)
{
    std::vector<std::string> ngrams;
    std::vector<std::string> list = _explode(input);

    size_t max = list.size();

    for (size_t row=0; row < max; row += step) {
        size_t nsize = (row + maxn > max) ? max - row : maxn;

        std::string str("");
        size_t idx = 0;
        for (; idx < nsize - 1; idx++) {
            str = str + list.at(row + idx) + " ";
        }
        str = str + list.at(row + idx);

        if (nsize >= minn) {
            ngrams.push_back(str);
        }
    } // for (size_t row=0; row < max; row++)

    return ngrams;
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

/**
 * utf8文字数カウンタ
 *
 * @access private
 * @param  const std::string pattern
 * @return int
 */
int explosion::_utf8_strlen(const std::string word)
{
    int length = 0;
    for (int pos = 0; pos < word.size();) {
        uint8_t hex = static_cast<uint8_t>(word[pos]);
        pos += (hex < 0x80) ? 1 :
               (hex < 0xE0) ? 2 :
               (hex < 0xF0) ? 3 : 4;
        length += 1;
    }

    return length;
}

/**
 * ノード情報の成形取得
 *
 * @access private
 * @param  const std::string surface
 * @param  const int type
 * @param  int &no
 * @return int
 */
explosion::NodeFormat explosion::_getNode(const std::string surface, const node_type_t type, int &no, std::string &text)
{
    NodeFormat node;

    node.surface = surface;
    node.type = type;
    node.from.line = no;
    node.from.ch = _utf8_strlen(text);

    std::string::size_type last, pos = surface.find('\n');
    if (pos != std::string::npos) {
        while (pos != std::string::npos) {
            last = pos;
            pos = surface.find('\n', pos + 1);
            no++;
        }
        text = surface.substr(last + 1);
    } else {
        text = text + surface;
    }

    node.to.line = no;
    node.to.ch = _utf8_strlen(text);

    return node;
}

/**
 * スペース区切りの分割
 *
 * @access private
 * @param  const std::string str
 * @return std::vector<std::string>
 */
std::vector<std::string> explosion::_explode(const std::string str)
{
    std::vector<std::string> result;

    size_t pos = str.find(" ");
    size_t last = 0;

    while (pos != std::string::npos) {
        size_t size = pos - last;
        result.push_back(str.substr(last, size));

        last = pos + 1;
        pos = str.find(" ", pos + 1);
    }
    if (str.length() > last) {
        result.push_back(str.substr(last));
    }

    return result;
}

} // namespace croco

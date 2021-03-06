# explosion-php
Explosive analysis tool

## Index

---
+ [Requirements](#doc_001)
+ [Building explosion for PHP](#doc_002)
+ [Class synopsis](#doc_003)
+ [\Croco\Explosion::explode](#doc_004)
+ [\Croco\Explosion::explodeRe](#doc_005)
+ [\Croco\Explosion::ngram](#doc_008)
+ [Predefined Constants](#doc_006)
+ [Return Values](#doc_007)

---

## <a name="doc_001">Requirements

* PHP 7.x
* [RE2 shard object](https://github.com/google/re2)

```
$ sudo apt install libre2-4 libre2-dev
```

## <a name="doc_002">Building explosion for PHP

```
$ cd explosion-php
$ phpize
$ ./configure
$ make
$ sudo make install
```


edit your php.ini and add:

-----

## <a name="doc_003">Class synopsis

```php
\Croco\Explosion {
    public __construct ( void )
    public load ( string key, string filename )
    public array explode ( string haystack, string load_key_find[, string load_key_pattern] )
    public array explodeRe ( string haystack, string pattern )
}
```
---

## <a name="doc_004">array \Croco\Explosion::explode(string haystack, string findKey[, string regexKey])

*kaomoji.txt*
```php
(ﾟДﾟ)ﾉ ｧｨ
(　´∀｀)つ≡≡≡愛愛愛)Д｀)ｸﾞｼｬ
<丶｀Д´>ｱｲｺﾞｰ
(＿´Д｀) ｱｲｰﾝ
(^_-)-☆
```

*regex.txt*
```php
(\([^\)]+\))
```
(RE2 style regular expression)

*sample.php*

```php
$haystack = "日本語の(^^;)(^_-)-☆中にある(^_-)-☆顔文字を(ﾟДﾟ)ﾉ ｧｨ爆裂";

$explosion = new \Croco\Explosion();

/* 同じキーによる再読込は行われない */
$explosion->load('find:Kaomoji', 'kaomoji.txt');
$explosion->load('regex:Kaomoji', 'regex.txt');

/* エクスプロージョンによる文字列分解 */
$pieces = $explosion->explode(
    $haystack,
    'find:Kaomoji',
    'regex:Kaomoji'
);

foreach ($pieces as $node){
    echo $node['surface'];
    if (\Croco\EXPLOSION_TYPE_NONE == $node['type']) {
        echo " <<< マッチしていない文字列";
    } else if(\Croco\EXPLOSION_TYPE_FIND == $node['type']) {
        echo " <<< 完全一致した文字列";
    } else if(\Croco\EXPLOSION_TYPE_REGEX == $node['type']) {
        echo " <<< 正規表現で一致した文字列";
    }
    echo "\n";
}
echo "\n";

print_r($pieces);
```


```
日本語の <<< マッチしていない文字列
(^^;) <<< 正規表現で一致した文字列
(^_-)-☆ <<< 完全一致した文字列
中にある <<< マッチしていない文字列
(^_-)-☆ <<< 完全一致した文字列
顔文字を <<< マッチしていない文字列
(ﾟДﾟ)ﾉ ｧｨ <<< 完全一致した文字列
爆裂 <<< マッチしていない文字列

Array(
    [0] => Array(
        [from] => Array(
            [ch] => 0
            [line] => 0
        )
        [surface] => 日本語の
        [to] => Array(
            [ch] => 4
            [line] => 0
        )
        [type] => 0
    )
    [1] => Array(
        [from] => Array(
            [ch] => 4
            [line] => 0
        )
        [surface] => (^^;)
        [to] => Array(
            [ch] => 9
            [line] => 0
        )
        [type] => 2
    )
    [2] => Array(
        [from] => Array(
            [ch] => 9
            [line] => 0
        )
        [surface] => (^_-)-☆
        [to] => Array(
            [ch] => 16
            [line] => 0
        )
        [type] => 1
    )
    [3] => Array(
        [from] => Array(
            [ch] => 16
            [line] => 0
        )
        [surface] => 中にある
        [to] => Array(
            [ch] => 20
            [line] => 0
        )
        [type] => 0
    )
    [4] => Array(
        [from] => Array(
            [ch] => 20
            [line] => 0
        )
        [surface] => (^_-)-☆
        [to] => Array(
            [ch] => 27
            [line] => 0
        )
        [type] => 1
    )
    [5] => Array(
        [from] => Array(
            [ch] => 27
            [line] => 0
        )
        [surface] => 顔文字を
        [to] => Array(
            [ch] => 31
            [line] => 0
        )
        [type] => 0
    )
    [6] => Array(
        [from] => Array(
            [ch] => 31
            [line] => 0
        )
        [surface] => (ﾟДﾟ)ﾉ ｧｨ
        [to] => Array(
            [ch] => 40
            [line] => 0
        )
        [type] => 1
    )
    [7] => Array(
        [from] => Array(
            [ch] => 40
            [line] => 0
        )
        [surface] => 爆裂
        [to] => Array(
            [ch] => 42
            [line] => 0
        )
        [type] => 0
    )
)
```
-----


## <a name="doc_005">array \Croco\Explosion::explodeRe(string haystack, string pattern)

```php
$haystack = "いろはにほへと、ちりぬるを";

$explosion = new \Croco\Explosion();
$pieces = $explosion->explodeRe($haystack, "([^、]{4})");

foreach ($pieces as $piece) {
    echo $piece['surface']."\n";
}
```

```
いろはに
ほへと、
ちりぬる
を
```
-----

## <a name="doc_008">array \Croco\Explosion::ngram(string wakati[, int minn = 3, int maxn = 6, int step = 1])
```php
$data = "これは 推 論 の 形 をと って は いる が 、 その 示 す ところ は 、 思 惟 する 私 が 存在する という 自己 意識 の 直 覚 である 。";

$explosion = new \Croco\Explosion();

$list = $explosion->ngram($data, , 12, 3);
foreach ($list as $idx => $row) {
    echo $idx .':'. var_export($row, true) ."\n";
}
```

```
0:'これは 推 論 の 形 をと って は いる が 、 その'
1:'の 形 をと って は いる が 、 その 示 す ところ'
2:'って は いる が 、 その 示 す ところ は 、 思'
3:'が 、 その 示 す ところ は 、 思 惟 する 私'
4:'示 す ところ は 、 思 惟 する 私 が 存在する という'
5:'は 、 思 惟 する 私 が 存在する という 自己 意識 の'
6:'惟 する 私 が 存在する という 自己 意識 の 直 覚 である'
7:'が 存在する という 自己 意識 の 直 覚 である 。'
8:'自己 意識 の 直 覚 である 。'
```
-----



## <a name="doc_006">Predefined Constants

  * `\Croco\EXPLOSION_TYPE_NONE =  0` 　　マッチしていない文字列
  * `\Croco\EXPLOSION_TYPE_FIND =  1` 　　完全一致した文字列
  * `\Croco\EXPLOSION_TYPE_REGEX =  2` 　　正規表現で一致した文字列


## <a name="doc_007">Return Values

| Column  | Description   |
|:-------:|:--------------|
| surface | 文字列         |
| type    | 判定番号        |
| from    | 発見した開始位置 |
| to      | 発見した終始位置 |

```javascript
Array(
    [0] => Array(
        [surface] => 日本語の
        [type] => 0
        [from] => Array(
            [ch] => 0
            [line] => 0
        )
        [to] => Array(
            [ch] => 4
            [line] => 0
        )
    )
    :
    :
    :
)
```

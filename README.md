# explosion-php
エクスプロージョン！

## Building explosion for PHP

```
$ cd explosion-php
$ phpize
$ ./configure
$ make
$ sudo make install
```


edit your php.ini and add:

-----

### array croco_explosion(string haystack, string file[, string regex_file])

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
\([^\)]+\)
```

*sample.php*

```php
<?php
$haystack = "日本語の(^^;)(^_-)-☆中にある(^_-)-☆顔文字を(ﾟДﾟ)ﾉ ｧｨ爆裂";
$file = "kaomoji.txt";
$refile = "regex.txt";

$array = croco_explosion($haystack, $file, $refile);

foreach ($array as $node){
    echo $node['sentence'];
    if (EXPLOSION_TYPE_NONE == $node['type']) {
        echo " <<< マッチしていない文字列";
    } else if(EXPLOSION_TYPE_FIND == $node['type']) {
        echo " <<< 完全一致した文字列";
    } else if(EXPLOSION_TYPE_REGEX == $node['type']) {
        echo " <<< 正規表現で一致した文字列";
    }
    echo "\n";
}
echo "\n";

print_r($array);
```


```
日本語の <<< マッチしていない文字列
(^^;) <<< 正規表現で一致した文字列
中にある <<< マッチしていない文字列
(^_-)-☆ <<< 完全一致した文字列
顔文字を <<< マッチしていない文字列
(ﾟДﾟ)ﾉ ｧｨ <<< 完全一致した文字列
爆裂 <<< マッチしていない文字列

Array
(
    [0] => Array
        (
            [sentence] => 日本語の
            [type] => 0
        )

    [1] => Array
        (
            [sentence] => (^^;)
            [type] => 2
        )

    [2] => Array
        (
            [sentence] => 中にある
            [type] => 0
        )

    [3] => Array
        (
            [sentence] => (^_-)-☆
            [type] => 1
        )

    [4] => Array
        (
            [sentence] => 顔文字を
            [type] => 0
        )

    [5] => Array
        (
            [sentence] => (ﾟДﾟ)ﾉ ｧｨ
            [type] => 1
        )

    [6] => Array
        (
            [sentence] => 爆裂
            [type] => 0
        )

)
```
-----


### 定数

  * `EXPLOSION_TYPE_NONE =  0` 　　マッチしていない文字列
  * `EXPLOSION_TYPE_FIND =  1` 　　完全一致した文字列
  * `EXPLOSION_TYPE_REGEX =  2` 　　正規表現で一致した文字列

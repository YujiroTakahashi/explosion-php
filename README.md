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

*kaomoji.txt*
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

foreach ($array as $line){
    echo $line['sentence'];
    if (EXPLOSION_TYPE_NONE == $line['type']) {
        echo " <<< マッチしていない文字列"
    } else if(EXPLOSION_TYPE_FIND == $line['type']) {
        echo " <<< 完全一致した文字列"
    } else if(EXPLOSION_TYPE_REGEX == $line['type']) {
        echo " <<< 正規表現で一致した文字列"
    }
    echo "\n";
}

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
```
-----


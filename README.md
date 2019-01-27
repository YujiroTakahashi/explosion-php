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

### array croco_explosion(string haystack, string file)

*kaomoji.txt*
```php
(ﾟДﾟ)ﾉ ｧｨ
(　´∀｀)つ≡≡≡愛愛愛)Д｀)ｸﾞｼｬ
<丶｀Д´>ｱｲｺﾞｰ
(＿´Д｀) ｱｲｰﾝ
(^_-)-☆
```

*sample.php*

```php
<?php
$haystack = "日本語の(^_-)-☆中にある(^_-)-☆顔文字を(ﾟДﾟ)ﾉ ｧｨ爆裂";
$file = "kaomoji.txt";

$array = croco_explosion($haystack, $file);

foreach ($array as $line){
    echo $line['sentence'];
    echo $line['needed'] ? '  (*)':'';
    echo "\n";
}

```


```
日本語の
(^_-)-☆  (*)
中にある
(^_-)-☆  (*)
顔文字を
(ﾟДﾟ)ﾉ ｧｨ  (*)
爆裂
```
-----


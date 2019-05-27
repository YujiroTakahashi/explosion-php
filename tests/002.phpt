--TEST--
Check for explosion presence
--SKIPIF--
<?php if (!extension_loaded("json")) print "skip"; ?>
<?php if (!extension_loaded("explosion")) print "skip"; ?>
--FILE--
<?php
$exp = new \Croco\Explosion();
$haystack = "日本語の(^_-)-☆中にある(^_-)-☆顔文字を(ﾟДﾟ)ﾉ ｧｨ爆裂";
$pieces = $exp->explodeRe($haystack, "(\([^\)]+\))");

foreach ($pieces as $piece){
    echo $piece['type'].' ';
    echo $piece['from']['line'].' ';
    echo $piece['from']['ch'].' ';
    echo $piece['to']['line'].' ';
    echo $piece['to']['ch'].' ';
    echo $piece['surface']."\n";
}
?>
--EXPECT--
0 0 0 0 4 日本語の
2 0 4 0 9 (^_-)
0 0 9 0 15 -☆中にある
2 0 15 0 20 (^_-)
0 0 20 0 26 -☆顔文字を
2 0 26 0 31 (ﾟДﾟ)
0 0 31 0 37 ﾉ ｧｨ爆裂

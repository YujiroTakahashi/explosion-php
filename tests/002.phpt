--TEST--
Check for explosion presence
--SKIPIF--
<?php if (!extension_loaded("explosion")) print "skip"; ?>
--FILE--
<?php 
$haystack = "日本語の(^_-)-☆中にある(^_-)-☆顔文字を(ﾟДﾟ)ﾉ ｧｨ爆裂";
$file = "kaomoji.txt";

$array = croco_explosion($haystack, $file);

foreach ($array as $line){
    echo $line."\n";
}
?>
--EXPECT--
日本語の
(^_-)-☆
中にある
(^_-)-☆
顔文字を
(ﾟДﾟ)ﾉ ｧｨ
爆裂

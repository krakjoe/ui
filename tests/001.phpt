--TEST--
Test size/point
--ENV--
XDG_RUNTIME_DIR=/tmp
DISPLAY=:0
NO_AT_BRIDGE=1
--DESCRIPTION--
This test verifies size/point functionality
--FILE--
<?php
$size = new UI\Size(640, 480);
$point = new UI\Point(10, 10);

var_dump($size + $point);

var_dump($size, $point);

var_dump($point + $size);

var_dump($size, $point);

var_dump($size + 10);

var_dump($point * 2);

var_dump($size += $point);

var_dump($size);
?>
--EXPECT--
object(UI\Size)#3 (2) {
  ["width"]=>
  float(650)
  ["height"]=>
  float(490)
}
object(UI\Size)#1 (2) {
  ["width"]=>
  float(640)
  ["height"]=>
  float(480)
}
object(UI\Point)#2 (2) {
  ["x"]=>
  float(10)
  ["y"]=>
  float(10)
}
object(UI\Point)#3 (2) {
  ["x"]=>
  float(650)
  ["y"]=>
  float(490)
}
object(UI\Size)#1 (2) {
  ["width"]=>
  float(640)
  ["height"]=>
  float(480)
}
object(UI\Point)#2 (2) {
  ["x"]=>
  float(10)
  ["y"]=>
  float(10)
}
object(UI\Size)#3 (2) {
  ["width"]=>
  float(650)
  ["height"]=>
  float(490)
}
object(UI\Point)#3 (2) {
  ["x"]=>
  float(20)
  ["y"]=>
  float(20)
}
object(UI\Size)#1 (2) {
  ["width"]=>
  float(650)
  ["height"]=>
  float(490)
}
object(UI\Size)#1 (2) {
  ["width"]=>
  float(650)
  ["height"]=>
  float(490)
}

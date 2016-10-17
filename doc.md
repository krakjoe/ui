Documentation
=============
*This is how it looks ...*

UI\Point
========
*A value object representing a point (co-ordinates) on the screen (x,y)*

```
final class UI\Point {
	public double $x;
	public double $y;

	public function setX(double $x) : void;
	public function setY(double $y) : void;
	public function getX() : double;
	public function getY() : double;

	public function __construct(double $x, double $y);
}
```

UI\Size
======
*A value object representing the dimensions of an area (width, height)*

```
final class UI\Size {
	public double $width;
	public double $height;
	
	public function setWidth(double $width) : void;
	public function setHeight(double $height) : void;
	public function getWidth() : double;
	public function getHeight() : double;

	public function __construct(double $width, double $height);
}
```

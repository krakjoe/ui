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

UI\Control
=========
*An object representing a uiControl*

```
final class UI\Control {
	public function getParent() : UI\Control;
	public function setParent(UI\Control $parent) : void;
	public function getTopLevel() : int;
	public function isVisible() : bool;
	public function show() : void;
	public function hide() : void;
	public function isEnabled() : bool;
	public function enable() : void;
	public function disable() : void;
	public function destroy() : void;
}
```

UI\Window
========
*An object representing a uiWindow*

```
final class UI\Window extends UI\Control {
	public function setTitle(string $title) : void;
	public function getTitle() : string;
	public function setSize(UI\Size $size) : void;
	public function getSize() : UI\Size;
	public function setPosition(UI\Point $position) : void;
	public function getPosition() : UI\Point;
	public function setFullScreen(bool $fullScreen) : void;
	public function isFullScreen() : bool;
	public function setBorders(bool $borders) : void;
	public function hasBorders() : bool;
	public function setMargin(bool $margin) : void;
	public function hasMargin() : bool;
	public function center() : void;
	public function add(UI\Control $control) : void;
	public function msg(string $title, string $message) : void;
	public function error(string $title, string $message) : void;

	public function __construct(string $title, UI\Size $size, boolean $menu);
}
```

UI\Form
======
*An object representing a uiForm*

```
final class UI\Form extends UI\Control {
	public function setPadded(bool $padded) : void;
	public function isPadded() : bool;
	public function append(string $label, UI\Control $control, bool $stretchy = false) : bool;
	public function delete(int $index) : void;
}
```

UI\Grid
=======
*An object representing a uiGrid*

```
final class UI\Grid extends UI\Control {
	public function setPadded(bool $padded) : void;
	public function isPadded() : bool;
	public function append(UI\Control $control, int $left, int $top, int $xspan, int $yspan, bool $hexpand, int $haign, bool $vexpand, int $valign);
}
```

UI\Tab
======
*An object representing a uiTab*

```
final class UI\Tab extends UI\Control {
	public function append(string $name, UI\Control $control) : void;
	public function delete(int $index) : void;
	public function pages() : int;
	public function insertAt(string $name, bool $before, UI\Control $control) : void;
	public function setMargin(bool $margin) : void;
	public function hasMargin() : bool;
}
```

UI\Box
=====
*An object representing a uiBox*

```
final class UI\Box extends UI\Control {
	public function getOrientation() : int;
	public function append(UI\Control $control, bool $stretchy = false) : void;
	public function delete(int $index) : void;
	public function setPadded(bool $padded) : void;
	public function isPadded() : bool;

	public function __construct(int $orientation = UI\BOX::HORIZONTAL);

	const VERTICAL;
	const HORIZONTAL;
}
```

UI\Check
=======
*An object represnting a uiCheckbox*

```
final class UI\Check extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;
	public function setChecked(bool $checked) : void;
	public function isChecked() : bool;
	public function onToggle(callable $handler) : void;

	public function __construct(string $text);
}
```

```UI\Check::onToggle``` handler should be ```callable(UI\Check $toggled)```

UI\Button
========
*An object representing a uiButton*

```
final class UI\Button extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;
	public function onClick(callable $handler) : void;

	public function __construct(string $text);
}
```

```UI\Button::onClick``` handler should be ```callable(UI\Button $clicked)```


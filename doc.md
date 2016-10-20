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

UI\ColorButton
=============
*An object representing a uiColorButton*

```
final class UI\ColorButton extends UI\Control {
	public function setColor(int $r, int $r, int $b) : void;
	public function setColorFromBrush(UI\DrawBrush $brush) : void;
	public function getColor() : array;
	public function getBrush() : UI\DrawBrush;
	public function onChange(callable $handler) : void;
	
	public function __construct(string $text);
}
```

```UI\ColorButton::onChange``` handler should be ```callable(UI\ColorButton $changed)```

UI\Label
=======
*An object representing a uiLabel*

```
final class UI\Label extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;

	public function __construct(string $text);
}
```

UI\Entry
=======
*An object representing a uiEntry*

```
final class UI\Entry extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;
	public function setReadOnly(bool $readOnly) : void;
	public function isReadOnly() : bool;
	public function onChange(callable $handler);

	public function __construct(int $type = UI\ENTRY::NORMAL);

	const NORMAL;
	const PASSWORD;
	const SEARCH;
}
```

```UI\Entry::onChange``` handler should be ```callable(UI\Entry $changed)```

UI\Multi
=======
*An object represneting a uiMultilineEntry*

```
final class UI\Multi extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;
	public function setReadOnly(bool $readOnly) : void;
	public function isReadOnly() : bool;
	public function append(string $text) : void;
	public function onChange(callable $handler);

	public function __construct(int $type = UI\MULTI::WRAP);

	const NOWRAP;
	const WRAP;
}
```

```UI\Multi::onChange``` handler should be ```callable(UI\Multi $changed)```

UI\Group
=======
*An object representing a uiGroup*

```
final class UI\Group extends UI\Control {
	public function setTitle(string $text) : void;
	public function getTitle() : string;
	public function setMargin(bool $margin) : void;
	public function hasMargin() : bool;
	public function add(UI\Control $control) : void;

	public function __construct(string $title);
}
```

UI\Spin
======
*An object representing a uiSpinbox*

```
final class UI\Spin extends UI\Control {
	public function setValue(int $value) : void;
	public function getValue() : int;
	public function onChange(callable $handler) : void;

	public function __construct(int $min, int $max);
}
```

```UI\Spin::onChange``` handler should be ```callable(UI\Spin $changed)```

UI\Slider
========
*An object representing a uiSlider*

```
final class UI\Slider extends UI\Control {
	public function setValue(int $value) : void;
	public function getValue() : int;
	public function onChange(callable $handler) : void;

	public function __construct(int $min, int $max);
}
```

```UI\Slider::onChange``` handler should be ```callable(UI\Slider $changed)```

UI\Progress
==========

```
final class UI\Progress extends UI\Control {
	public function setValue(int $value) : void;
	public function getValue() : int;
}
```

UI\Separator
===========
*An object representing a uiSeparator*

```
final class UI\Separator extends UI\Control {

	public function __construct(int $type == UI\SEPARATOR::HORIZONTAL);

	const HORIZONTAL;
	const VERTICAL;
}
```

UI\Combo
=======
*An object representing a uiCombobox*

```
final class UI\Combo extends UI\Control {
	public function setSelected(int $selected) : void;
	public function getSelected() : int;
	public function append(string $text) : void;
	public function onSelected(callable $handler) : void;
}
```

```UI\Combo::onSelected``` handler should be ```callable(UI\Combo $selected)```


UI\EditableCombo
==============
*An object representing a uiEditableCombobox*

```
final class UI\EditableCombo extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;
	public function append(string $text) : void;
	public function onChange(callable $handler) : void;
}
```

```UI\EditableCombo::onChange``` handler should be ```callable(UI\EditableCombo $changed)```

UI\Radio
=======
*An object representing a uiRadioButton*

```
final class UI\Radio extends UI\Control {
	public function setSelected(int $selected) : void;
	public function getSelected() : int;
	public function append(string $text) : void;
	public function onSelected(callable $handler) : void;
}
```

```UI\Radio::onSelected``` handler should be ```callable(UI\Radio $changed)```

UI\Picker
========
*An object representing a uiDatePicker*

```
final class UI\Picker extends UI\Control {
	public function __construct(int $type = UI\PICKER::DATE);

	const DATE;
	const TIME;
	const DATETIME;
}
```

UI\Menu
======
*An object representing a uiMenu*

```
final class UI\Menu {
	public function append(string $name) : UI\MenuItem;
	public function appendCheck(string $name) : UI\MenuItem;
	public function appendQuit() : UI\MenuItem;
	public function appendPreferences() : UI\MenuItem;
	public function appendAbout() : UI\MenuItem;
	public function appendSeparator() : void;

	public function __construct(string $name);
}
```

UI\MenuItem
==========
*An object representing a uiMenuItem*

```
final class UI\MenuItem {
	public function enable() : void;
	public function disable() : void;
	public function isChecked() : bool;
	public function setChecked(bool $checked) : void;
	public function onClick(callable $handler) : void;
}
```

```UI\MenuItem::onClick``` handler should be ```callable(UI\MenuItem $clicked)```

UI\Area
=======
*An object representing a uiArea*

```
final class UI\Area extends UI\Control {
	public function redraw() : void;
	public function setSize(UI\Size $size) : void;
	public function scrollTo(UI\Point $point, UI\Size $size) : void;
	public function onDraw(callable $handler) : void;
	public function onMouse(callable $handler) : void;
	public function onKey(callable $handler) : void;	

	const CTRL;
	const ALT;
	const SHIFT;
	const SUPER;
	const UP;
	const DOWN;
}
```

```UI\Area::onDraw``` handler should be ```callable(UI\Area $area, UI\DrawContext $context, UI\Size $areaSize, UI\Point $clipPoint, UI\Size $clipSize)```

```UI\Area::onMouse``` handler should be ```callable(UI\Area $area, UI\Point $areaPoint, UI\Size $areaSize, int $flags)```

UI\DrawPath
==========
*An object representing a uiDrawPath*

```
final class UI\DrawPath {
	public function newFigure(UI\Point $point) : void;
	public function newFigureWithArc(UI\Point $point, double $radius, double $angle, double $sweep, double $negative) : void;
	public function lineTo(UI\Point $point, double $radius, double $angle, double $sweep, double $negative) : void;
	public function arcTo(UI\Point $point, double $radius, double $angle, double $sweep, double $negative) : void;
	public function bezierTo(UI\Point $point, double $radius, double $angle, double $sweep, double $negative) : void;
	public function closeFigure() : void;
	public function addRectangle(UI\Point $point, UI\Size $size) : void;
	public function end() : void;

	public function __construct(int $mode = UI\DRAWPATH::WINDING);

	const WINDING;
	const ALTERANATE;
}
```

UI\DrawContext
=============
*An object representing a uiDrawContext*

```
final class UI\DrawContext
```

UI\DrawBrush
===========
*An object representing a uiDrawBrush*

```
final class UI\DrawBrush {
	public function getType() : int;
	public function setRGB(int $color) : int;
	public function setAlpha(double $alpha) : void;
	public function getAlpha() : double;
	public function 
	public function __construct(int $type, double $r = 0, double $g = 0, double $b = 0, double $a = 0, double $X0 = 0, double $Y0 = 0, double $X1 = 0, double $Y1 = 0, double $radius = 0);

	const SOLID;
	const LGRADIENT;
	const RGRADIENT;
	const IMAGE;
}
```

UI\DrawStroke
============
*An object representing uiDrawStrokeParams*

```
final class UI\DrawStroke {
	
	public function __construct(int $cap, int $join, double $thickness, float $miterLimit);

	const CAP_FLAT;
	const CAP_ROUND;
	const CAP_SQUARE;
	
	const JOIN_MITER;
	const JOIN_ROUND;
	const JOIN_BEVEL;
}
```

UI\DrawMatrix
============
*An object representing a uiDrawMatrix*

```
final class UI\DrawMatrix {
	public function translate(UI\Point $point) : void;
	public function scale(UI\Point $center, UI\Point $point) : void;
	public function rotate(UI\Point $point, double $amount) : void;
	public function skew(UI\Point $point, UI\Point $amount) : void;
	public function multiply(UI\DrawMatrix $matrix) : UI\DrawMatrix;
	public function isInvertible() : bool;
	public function invert() : void;
}
```

UI\Draw
======
*Static drawing functions from ui*

```
final class UI\Draw {
	public function fill(UI\DrawContext $context, UI\DrawPath $path, UI\DrawBrush $brush) : void;
	public function stroke(UI\DrawContext $context, UI\DrawPath $path, UI\DrawBrush $brush, UI\DrawStroke $stroke) : void;
	public function transform(UI\DrawContext $context, UI\DrawMatrix $matrix) : void;	
	public function clip(UI\DrawContext $context, UI\DrawPath $path) : void;
	public function save(UI\DrawContext $context) : void;
	public function restore(UI\DrawContext $context) : void;
}
```
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

UI\Control\Window
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

UI\Control\Form
======
*An object representing a uiForm*

```
final class UI\Control\Form extends UI\Control {
	public function setPadded(bool $padded) : void;
	public function isPadded() : bool;
	public function append(string $label, UI\Control $control, bool $stretchy = false) : bool;
	public function delete(int $index) : void;
}
```

UI\Control\Grid
=======
*An object representing a uiGrid*

```
final class UI\Control\Grid extends UI\Control {
	public function setPadded(bool $padded) : void;
	public function isPadded() : bool;
	public function append(UI\Control $control, int $left, int $top, int $xspan, int $yspan, bool $hexpand, int $haign, bool $vexpand, int $valign);
}
```

UI\Control\Tab
======
*An object representing a uiTab*

```
final class UI\Control\Tab extends UI\Control {
	public function append(string $name, UI\Control $control) : void;
	public function delete(int $index) : void;
	public function pages() : int;
	public function insertAt(string $name, bool $before, UI\Control $control) : void;
	public function setMargin(bool $margin) : void;
	public function hasMargin() : bool;
}
```

UI\Control\Box
=====
*An object representing a uiBox*

```
final class UI\Control\Box extends UI\Control {
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

UI\Control\Check
=======
*An object represnting a uiCheckbox*

```
final class UI\Control\Check extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;
	public function setChecked(bool $checked) : void;
	public function isChecked() : bool;
	public function onToggle(callable $handler) : void;

	public function __construct(string $text);
}
```

```UI\Control\Check::onToggle``` handler should be ```callable(UI\Check $toggled)```

UI\Control\Button
========
*An object representing a uiButton*

```
final class UI\Control\Button extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;
	public function onClick(callable $handler) : void;

	public function __construct(string $text);
}
```

```UI\Control\Button::onClick``` handler should be ```callable(UI\Button $clicked)```

UI\Control\ColorButton
=============
*An object representing a uiColorButton*

```
final class UI\Control\ColorButton extends UI\Control {
	public function setColor(UI\Draw\Color color) : void;
	public function getColor() : UI\Draw\Color;
	public function onChange(callable $handler) : void;
	
	public function __construct(string $text);
}
```

```UI\Control\ColorButton::onChange``` handler should be ```callable(UI\ColorButton $changed)```

UI\Control\Label
=======
*An object representing a uiLabel*

```
final class UI\Control\Label extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;

	public function __construct(string $text);
}
```

UI\Control\Entry
=======
*An object representing a uiEntry*

```
final class UI\Control\Entry extends UI\Control {
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

```UI\Control\Entry::onChange``` handler should be ```callable(UI\Control\Entry $changed)```

UI\Control\Multi
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

```UI\Control\Multi::onChange``` handler should be ```callable(UI\Control\Multi $changed)```

UI\Control\Group
=======
*An object representing a uiGroup*

```
final class UI\Control\Group extends UI\Control {
	public function setTitle(string $text) : void;
	public function getTitle() : string;
	public function setMargin(bool $margin) : void;
	public function hasMargin() : bool;
	public function add(UI\Control $control) : void;

	public function __construct(string $title);
}
```

UI\Control\Spin
======
*An object representing a uiSpinbox*

```
final class UI\Control\Spin extends UI\Control {
	public function setValue(int $value) : void;
	public function getValue() : int;
	public function onChange(callable $handler) : void;

	public function __construct(int $min, int $max);
}
```

```UI\Control\Spin::onChange``` handler should be ```callable(UI\Control\Spin $changed)```

UI\Control\Slider
========
*An object representing a uiSlider*

```
final class UI\Control\Slider extends UI\Control {
	public function setValue(int $value) : void;
	public function getValue() : int;
	public function onChange(callable $handler) : void;

	public function __construct(int $min, int $max);
}
```

```UI\Control\Slider::onChange``` handler should be ```callable(UI\Control\Slider $changed)```

UI\Control\Progress
==========

```
final class UI\Control\Progress extends UI\Control {
	public function setValue(int $value) : void;
	public function getValue() : int;
}
```

UI\Control\Separator
===========
*An object representing a uiSeparator*

```
final class UI\Control\Separator extends UI\Control {

	public function __construct(int $type == SEPARATOR::HORIZONTAL);

	const HORIZONTAL;
	const VERTICAL;
}
```

UI\Control\Combo
=======
*An object representing a uiCombobox*

```
final class UI\Control\Combo extends UI\Control {
	public function setSelected(int $selected) : void;
	public function getSelected() : int;
	public function append(string $text) : void;
	public function onSelected(callable $handler) : void;
}
```

```UI\Control\Combo::onSelected``` handler should be ```callable(UI\Control\Combo $selected)```


UI\Control\EditableCombo
==============
*An object representing a uiEditableCombobox*

```
final class UI\Control\EditableCombo extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;
	public function append(string $text) : void;
	public function onChange(callable $handler) : void;
}
```

```UI\Control\EditableCombo::onChange``` handler should be ```callable(UI\Control\EditableCombo $changed)```

UI\Control\Radio
=======
*An object representing a uiRadioButton*

```
final class UI\Control\Radio extends UI\Control {
	public function setSelected(int $selected) : void;
	public function getSelected() : int;
	public function append(string $text) : void;
	public function onSelected(callable $handler) : void;
}
```

```UI\Control\Radio::onSelected``` handler should be ```callable(UI\Control\Radio $changed)```

UI\Control\Picker
========
*An object representing a uiDatePicker*

```
final class UI\Control\Picker extends UI\Control {
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

```UI\Area::onDraw``` handler should be ```callable(UI\Area $area, UI\Draw\Pen $pen, UI\Size $areaSize, UI\Point $clipPoint, UI\Size $clipSize)```

```UI\Area::onMouse``` handler should be ```callable(UI\Area $area, UI\Point $areaPoint, UI\Size $areaSize, int $flags)```

UI\Draw\Pen
==========
*A drawing object*

```
final class UI\Draw\Pen {
	public function fill(UI\Draw\Path $path, UI\Draw\Brush $brush) : void;
	public function stroke(UI\Draw\Path $path, UI\Draw\Brush $brush, UI\Draw\Stroke $stroke) : void;
	public function transform(UI\Draw\Matrix $matrix) : void;	
	public function clip(UI\Draw\Path $path) : void;
	public function save() : void;
	public function restore() : void;
}
```

UI\Draw\Path
==========
*An object representing a uiDrawPath*

```
final class UI\Draw\Path {
	public function newFigure(UI\Point $point) : void;
	public function newFigureWithArc(UI\Point $point, double $radius, double $angle, double $sweep, double $negative) : void;
	public function lineTo(UI\Point $point, double $radius, double $angle, double $sweep, double $negative) : void;
	public function arcTo(UI\Point $point, double $radius, double $angle, double $sweep, double $negative) : void;
	public function bezierTo(UI\Point $point, double $radius, double $angle, double $sweep, double $negative) : void;
	public function closeFigure() : void;
	public function addRectangle(UI\Point $point, UI\Size $size) : void;
	public function end() : void;

	public function __construct(int $mode = UI\DRAW\PATH::WINDING);

	const WINDING;
	const ALTERANATE;
}
```

UI\Draw\Color
============
*An object representing RGBA colors*

```
final class UI\Draw\Color {
	public function setChannel(int $channel, double $value) : void;
	public function getChannel(int $channel) : double;

	public function __construct(int $rgb [, double $alpha]); 

	const RED;
	const GREEN;
	const BLUE;
	const ALPHA;
}
```

UI\Draw\Brush
===========
*An object representing a uiDrawBrush*

```
final class UI\Draw\Brush {
	public function getType() : int;
	public function setType(int $type) : void;
	public function getColor() : UI\Draw\Color;
	public function setColor(UI\Draw\Color $color) : void;

	public function __construct(int $type, UI\Draw\Color $color, double $X0 = 0, double $Y0 = 0, double $X1 = 0, double $Y1 = 0, double $radius = 0);

	const SOLID;
	const LGRADIENT;
	const RGRADIENT;
	const IMAGE;
}
```

UI\Draw\Stroke
============
*An object representing uiDrawStrokeParams*

```
final class UI\Draw\Stroke {
	
	public function __construct(int $cap, int $join, double $thickness, float $miterLimit);

	const CAP_FLAT;
	const CAP_ROUND;
	const CAP_SQUARE;
	
	const JOIN_MITER;
	const JOIN_ROUND;
	const JOIN_BEVEL;
}
```

UI\Draw\Matrix
============
*An object representing a uiDrawMatrix*

```
final class UI\Draw\Matrix {
	public function translate(UI\Point $point) : void;
	public function scale(UI\Point $center, UI\Point $point) : void;
	public function rotate(UI\Point $point, double $amount) : void;
	public function skew(UI\Point $point, UI\Point $amount) : void;
	public function multiply(UI\Draw\Matrix $matrix) : UI\Draw\Matrix;
	public function isInvertible() : bool;
	public function invert() : void;
}
```

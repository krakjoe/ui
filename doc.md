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

UI\App
======
*An object representing the libui context*

```
class UI\App {
	public function run(bool $loop = false, bool $block = false);
	public function quit();

	protected function onShouldQuit(array $windows) : void;
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
class UI\Window extends UI\Control {
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

	protected function onClosing() : void;

	public function __construct(string $title, UI\Size $size, boolean $menu);
}
```

UI\Control\Form
======
*An object representing a uiForm*

```
class UI\Control\Form extends UI\Control {
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
class UI\Control\Grid extends UI\Control {
	public function setPadded(bool $padded) : void;
	public function isPadded() : bool;
	public function append(UI\Control $control, int $left, int $top, int $xspan, int $yspan, bool $hexpand, int $haign, bool $vexpand, int $valign);

	const Fill;
	const Start;
	const Center;
	const End;

	const Leading;
	const Trailing;
	const Top;
	const Bottom;
}
```

UI\Control\Tab
======
*An object representing a uiTab*

```
class UI\Control\Tab extends UI\Control {
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
class UI\Control\Box extends UI\Control {
	public function getOrientation() : int;
	public function append(UI\Control $control, bool $stretchy = false) : void;
	public function delete(int $index) : void;
	public function setPadded(bool $padded) : void;
	public function isPadded() : bool;

	public function __construct(int $orientation = Box::Horizontal);

	const Vertical;
	const Horizontal;
}
```

UI\Control\Check
=======
*An object represnting a uiCheckbox*

```
class UI\Control\Check extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;
	public function setChecked(bool $checked) : void;
	public function isChecked() : bool;

	protected function onToggle() : void;

	public function __construct(string $text);
}
```

UI\Control\Button
========
*An object representing a uiButton*

```
class UI\Control\Button extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;

	protected function onClick() : void;

	public function __construct(string $text);
}
```

UI\Control\ColorButton
=============
*An object representing a uiColorButton*

```
class UI\Control\ColorButton extends UI\Control {
	public function setColor(UI\Draw\Color color) : void;
	public function getColor() : UI\Draw\Color;

	protected function onChange() : void;
}
```

UI\Control\Label
=======
*An object representing a uiLabel*

```
class UI\Control\Label extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;

	public function __construct(string $text);
}
```

UI\Control\Entry
=======
*An object representing a uiEntry*

```
class UI\Control\Entry extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;
	public function setReadOnly(bool $readOnly) : void;
	public function isReadOnly() : bool;

	protected function onChange() : void;

	public function __construct(int $type = Entry::Normal);

	const Normal;
	const Password;
	const Search;
}
```

UI\Control\MultilineEntry
=======
*An object represneting a uiMultilineEntry*

```
class UI\Control\MultilineEntry extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;
	public function setReadOnly(bool $readOnly) : void;
	public function isReadOnly() : bool;
	public function append(string $text) : void;

	protected function onChange();

	public function __construct(int $type = MultilineEntry::Wrap);

	const NoWrap;
	const Wrap;
}
```

UI\Control\Group
=======
*An object representing a uiGroup*

```
class UI\Control\Group extends UI\Control {
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
class UI\Control\Spin extends UI\Control {
	public function setValue(int $value) : void;
	public function getValue() : int;

	protected function onChange() : void;

	public function __construct(int $min, int $max);
}
```

UI\Control\Slider
========
*An object representing a uiSlider*

```
class UI\Control\Slider extends UI\Control {
	public function setValue(int $value) : void;
	public function getValue() : int;

	protected function onChange() : void;

	public function __construct(int $min, int $max);
}
```

UI\Control\Progress
==========

```
class UI\Control\Progress extends UI\Control {
	public function setValue(int $value) : void;
	public function getValue() : int;
}
```

UI\Control\Separator
===========
*An object representing a uiSeparator*

```
class UI\Control\Separator extends UI\Control {

	public function __construct(int $type == Separator::Horizontal);

	const Horizontal;
	const Vertical;
}
```

UI\Control\Combo
=======
*An object representing a uiCombobox*

```
class UI\Control\Combo extends UI\Control {
	public function setSelected(int $selected) : void;
	public function getSelected() : int;
	public function append(string $text) : void;

	protected function onSelected() : void;
}
```


UI\Control\EditableCombo
==============
*An object representing a uiEditableCombobox*

```
class UI\Control\EditableCombo extends UI\Control {
	public function setText(string $text) : void;
	public function getText() : string;
	public function append(string $text) : void;

	protected function onChange() : void;
}
```

UI\Control\Radio
=======
*An object representing a uiRadioButton*

```
class UI\Control\Radio extends UI\Control {
	public function setSelected(int $selected) : void;
	public function getSelected() : int;
	public function append(string $text) : void;

	protected function onSelected() : void;
}
```

UI\Control\Picker
========
*An object representing a uiDatePicker*

```
class UI\Control\Picker extends UI\Control {
	public function __construct(int $type = Picker::Date);

	const Date;
	const Time;
	const DateTime;
}
```

UI\Menu
======
*An object representing a uiMenu*

```
class UI\Menu {
	public function append(string $name [, string $type = MenuItem::class]) : UI\MenuItem;
	public function appendCheck(string $name [, string $type = MenuItem::class]) : UI\MenuItem;
	public function appendQuit([string $type = MenuItem::class]) : UI\MenuItem;
	public function appendPreferences([string $type = MenuItem::class]) : UI\MenuItem;
	public function appendAbout([string $type = MenuItem::class]) : UI\MenuItem;
	public function appendSeparator() : void;

	public function __construct(string $name);
}
```

UI\MenuItem
==========
*An object representing a uiMenuItem*

```
class UI\MenuItem {
	public function enable() : void;
	public function disable() : void;
	public function isChecked() : bool;
	public function setChecked(bool $checked) : void;

	protected function onClick(Window $window) : void;
}
```

UI\Area
=======
*An object representing a uiArea*

```
class UI\Area extends UI\Control {
	public function redraw() : void;
	public function setSize(UI\Size $size) : void;
	public function scrollTo(UI\Point $point, UI\Size $size) : void;

	protected function onDraw(UI\Draw\Pen $pen, UI\Size $areaSize, UI\Point $clipPoint, UI\Size $clipSize) : void;
	protected function onMouse(UI\Point $areaPoint, UI\Size $areaSize, int $flags) : void;
	protected function onKey(string key, int ext, int modifiers) : void;	

	const Ctrl;
	const Alt;
	const Shift;
	const Super;
	const Up;
	const Down;
}
```

UI\Draw\Pen
==========
*A drawing object*

```
final class UI\Draw\Pen {
	public function fill(UI\Draw\Path $path, UI\Draw\Brush $brush) : void;
	public function stroke(UI\Draw\Path $path, UI\Draw\Brush $brush, UI\Draw\Stroke $stroke) : void;
	public function transform(UI\Draw\Matrix $matrix) : void;	
	public function clip(UI\Draw\Path $path) : void;
	public function write(UI\Point $point, UI\Draw\Text\Layout $layout) : void;
	public function save() : void;
	public function restore() : void;
}
```

UI\Draw\Path
==========
*An object representing a uiDrawPath*

```
class UI\Draw\Path {
	public function newFigure(UI\Point $point) : void;
	public function newFigureWithArc(UI\Point $point, double $radius, double $angle, double $sweep, double $negative) : void;
	public function lineTo(UI\Point $point, double $radius, double $angle, double $sweep, double $negative) : void;
	public function arcTo(UI\Point $point, double $radius, double $angle, double $sweep, double $negative) : void;
	public function bezierTo(UI\Point $point, double $radius, double $angle, double $sweep, double $negative) : void;
	public function closeFigure() : void;
	public function addRectangle(UI\Point $point, UI\Size $size) : void;
	public function end() : void;

	public function __construct(int $mode = Path::Winding);

	const Winding;
	const Alternate;
}
```

UI\Draw\Color
============
*An object representing RGBA colors*

```
class UI\Draw\Color {
	public function setChannel(int $channel, double $value) : void;
	public function getChannel(int $channel) : double;

	public function __construct(int $rgb [, double $alpha]); 

	const Red;
	const Green;
	const Blue;
	const Alpha;
}
```

UI\Draw\Brush
===========
*An object representing a uiDrawBrush*

```
class UI\Draw\Brush {
	public function getType() : int;
	public function setType(int $type) : void;
	public function getColor() : UI\Draw\Color;
	public function setColor(UI\Draw\Color $color) : void;

	public function __construct(int $type [
		UI\Draw\Color $color, 
		double $X0 = 0, 
		double $Y0 = 0, 
		double $X1 = 0, 
		double $Y1 = 0, 
		double $radius = 0
	]);

	const Solid;
	const LinearGradient;
	const RadialGradient;
	const Image;
}
```

UI\Draw\Stroke
============
*An object representing uiDrawStrokeParams*

```
class UI\Draw\Stroke {
	public function setCap(int $cap) : void;
	public function getCap() : int;
	public function setJoin(int $join) : void;
	public function getJoin() : int;
	public function setThickness(double $thickness) : void;
	public function getThickness() : double;
	public function setMiterLimit(double $limit) : void;
	public function getMiterLimit() : double;

	public function __construct([
		int $cap = UI\Draw\Line\Cap::Flat, 
		int $join = UI\Draw\Line\Join::Miter, 
		double $thickness = 1, 
		float $miterLimit = 10
	]);
}
```

UI\Draw\Line\Cap
===============
*An enum representing uiDrawLineCap*

```
final class UI\Draw\Line\Cap {
	const Flat;
	const Round;
	const Square;
}
```

UI\Draw\Line\Join
===============
*An enum representing uiDrawLineJoin*

```
final class UI\Draw\Line\Join {
	const Miter;
	const Round;
	const Bevel;
}
```

UI\Draw\Matrix
============
*An object representing a uiDrawMatrix*

```
class UI\Draw\Matrix {
	public function translate(UI\Point $point) : void;
	public function scale(UI\Point $center, UI\Point $point) : void;
	public function rotate(UI\Point $point, double $amount) : void;
	public function skew(UI\Point $point, UI\Point $amount) : void;
	public function multiply(UI\Draw\Matrix $matrix) : UI\Draw\Matrix;
	public function isInvertible() : bool;
	public function invert() : void;
}
```

UI\Draw\Text\Font\Descriptor
=========================
*An object representing a uiDrawTextFontDescriptor*

```
class UI\Draw\Text\Font\Descriptor {
	public function __construct(string family, double size [, 
		int weight = UI\Draw\Text\Font\Weight::Normal, 
		int italic = UI\Draw\Text\Font\Italic::Normal,
		int stretch = UI\Draw\Text\Font\Stretch::Normal
	]);
}
```

UI\Draw\Text\Font\Weight
======================
*An enum representing uiDrawTextFontWeight*

```
final class UI\Draw\Text\Font\Weight {
	const Thin;
	const Light;
	const UltraLight;
	const Book;
	const Normal;
	const Medium;
	const SemiBold;
	const Bold;
	const UtraBold; /* typo here in libui */
	const Heavy;
	const UltraHeavy;
}
```

UI\Draw\Text\Font\Italic
======================
*An enum representing uiDrawTextFontItalic*

```
final class UI\Draw\Text\Font\Italic {
	const Normal;
	const Italic;
	const Oblique;
}
```

UI\Draw\Text\Font\Stretch
======================
*An enum representing uiDrawTextFontStretch*

```
final class UI\Draw\Text\Font\Stretch {
	const UltraCondensed;
	const ExtraCondensed;
	const Condensed;
	const SemiCondensed;
	const Normal;
	const SemiExpanded;
	const Expanded;
	const ExtraExpanded;
	const UltraExpanded;
}
```


UI\Draw\Text\Layout
=================
*An object representing uiDrawTextLayout*

```
class UI\Draw\Text\Layout {
	public function __construct(string text, UI\Draw\Text\Font font, double width);

	public function setWidth(double width) : void;

	public function setColor(UI\Draw\Color color) : void;
}
```

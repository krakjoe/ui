<?php
use UI\App;
use UI\Point;
use UI\Size;
use UI\Window;
use UI\Controls\Box;
use UI\Controls\Spin;
use UI\Controls\ColorButton;
use UI\Controls\Button;
use UI\Controls\Entry;
use UI\Controls\Label;
use UI\Controls\Combo;
use UI\Area;
use UI\Draw\Pen;
use UI\Draw\Path;
use UI\Draw\Color;
use UI\Draw\Brush;
use UI\Draw\Stroke;
use UI\Draw\Matrix;

use UI\Draw\Text\Font\Descriptor;
use UI\Draw\Text\Font;
use UI\Draw\Text\Layout;

$app = new App();

$window = new Window($app, "libui Histogram Example", new Size(640, 480), true);

$window->setMargin(true);

$hBox = new Box(Box::Horizontal);
$hBox->setPadded(true);

$vBox = new Box(Box::Vertical);
$vBox->setPadded(true);

$window->add($hBox);

$hBox->append($vBox);

$dataSources = [];

$histogram = new class($dataSources) extends Area {

	private function getGraphPoints(Size $size) : array {
		$xincr = $size->width / 9;
		$yincr = $size->height / 100;

		$points = [];

		foreach ($this->sources as $i => $source) {
			$points[$i] = new Point($xincr * $i, $yincr * (100 - $source->getValue()));
		}

		return $points;
	}

	private function getGraphPath(array $locations, Size $size, bool $extend = false) : Path {
		$path = new Path(Path::Winding);

		$path->newFigure(array_shift($locations));

		foreach ($locations as $location) {
			$path->lineTo($location);
		}

		if ($extend) {
			$path->lineTo(new Point($size->width, $size->height));
			$path->lineTo(new Point(0, $size->height));
			$path->closeFigure();
		}

		$path->end();

		return $path;
	}

	protected function onDraw(Pen $pen, Size $areaSize, Point $clipPoint, Size $clipSize) {
		$start = microtime(true);

		$path = new Path(Path::Winding);
		$path->addRectangle($clipPoint, $areaSize);
		$path->end();

		$pen->fill($path, new Brush(Brush::Solid, new Color(0xFFFFFF, 1)));
	
		$graphSize = new Size($areaSize->width - 40, $areaSize->height - 40);

		$zero = new Point(20, 20);

		$path = new Path(Path::Winding);
		$path->newFigure($zero);
		$path->lineTo(new Point(20, 20 + $graphSize->height));
		$path->lineTo(new Point(20 + $graphSize->width, 20 + $graphSize->height));	
		$path->end();

		$stroke = new Stroke();
		$stroke->setThickness(2);
		
		$black = new Color(0x000000, 1);

		$pen->stroke($path, new Brush(Brush::Solid, $black), $stroke);

		$matrix = new Matrix();
		$matrix->translate($zero);

		$pen->transform($matrix);

		$points = $this->getGraphPoints($graphSize);

		$path = $this->getGraphPath($points, $graphSize, true);

		$brush = new Brush(Brush::Solid, $this->color->getColor());

		$pen->fill($path, $brush);

		$path = $this->getGraphPath($points, $graphSize, false);

		$strokeColor = $brush->getColor();
		$strokeColor
			->setChannel(Color::Alpha, 
				$strokeColor->getChannel(Color::Alpha)/2);
		$brush->setColor($strokeColor);

		$pen->stroke($path, $brush, $stroke);

		$layout = new Layout(sprintf(
			"Drawn in %.5f seconds", 
				microtime(true) - $start),
			$this->font->getFont(),
			$clipSize->width
		);
		$layout->setColor($black);

		$pen->write(new Point(10, $graphSize->height - 30), $layout);
	}

	public function setColorSource(ColorButton $source) {
		$this->color = $source;
	}

	public function getColorSource() {
		return $this->color;
	}

	public function setFontSource(Combo $font) {
		$this->font = $font;
	}

	public function getFontSource() {
		return $this->font;
	}

	public function __construct(array &$sources, ColorButton $color = null, Font $font = null) {
		$this->sources =& $sources;
		$this->color  = $color;
		$this->font = $font;
	}

	private $sources;
	private $color;
	private $font;
};

$colorBox = new Entry();
$colorBox->setText("0x8892BF");

$colorButton = new class($histogram, $colorBox, new Color(0x8892BF, 1)) extends ColorButton {

	protected function onChange() {
		$redrawColor = $this->getColor();

		$this->entry->setText(sprintf(
			"0x%02X%02X%02X",
				$redrawColor->getChannel(Color::Red) * 255,
				$redrawColor->getChannel(Color::Green) * 255,
				$redrawColor->getChannel(Color::Blue) * 255));

		$this->histogram->redraw();
	}

	public function __construct(Area $histogram, Entry $entry, Color $color) {
		$this->histogram = $histogram;
		$this->entry = $entry;

		$this->setColor($color);

		$this->histogram->setColorSource($this);
	}

	private $histogram;
};

$redrawHistogram = function() use($histogram, $colorBox, $colorButton) {
	$redrawColor = $colorButton->getColor();

	$colorBox->setText(sprintf(
		"0x%02X%02X%02X",
			$redrawColor->getChannel(Color::Red) * 255,
			$redrawColor->getChannel(Color::Green) * 255,
			$redrawColor->getChannel(Color::Blue) * 255));

	$histogram->redraw();
};

$vBox->append(new Label("Change Data:"));
for ($i = 0; $i < 10; $i++) {

	$dataSources[$i] = new class(0, 100, $redrawHistogram) extends Spin {
		protected function onChange() {
			($this->redraw)();
		}

		public function __construct($min, $max, Closure $redraw) {
			parent::__construct($min, $max);

			$this->redraw = $redraw;
		}

		private $redraw;
	};

	$dataSources[$i]->setValue(mt_rand(0, 100));

	$vBox->append($dataSources[$i]);
}

$vBox->append(new Label("Choose Color:"));
$vBox->append($colorButton);

$colorBoxButton = new class("Set Color", $colorButton, $colorBox, $redrawHistogram) extends Button {

	protected function onClick() {
		$this->button->setColor(
			new Color(
				hexdec($this->entry->getText()), 1));
		($this->redraw)();
	}

	public function __construct(string $text, ColorButton $button, Entry $entry, Closure $redraw) {
		$this->button = $button;
		$this->entry = $entry;
		$this->redraw = $redraw;

		parent::__construct($text);
	}

	private $button;
	private $entry;
	private $redraw;
};

$vBox->append($colorBox);
$vBox->append($colorBoxButton);

$vBox->append(new Label("Choose Font:"));
$fontCombo = new class($histogram) extends Combo {
	
	public function onSelected() {
		$this->histogram->redraw();
	}

	public function __construct(Area $histogram) {
		$this->families  = UI\Draw\Text\Font\fontFamilies();
		$this->histogram = $histogram;

		sort($this->families);

		foreach ($this->families as $family) {
			$this->append($family);
		}

		$this->setSelected(0);

		$this->histogram->setFontSource($this);
	}

	public function getFont(int $selected = -1, int $size = 12) {
		return new Font(
			new Descriptor($this->families[
				$selected > -1 ? $families : $this->getSelected()
			], $size));
	}

	private $items;
	private $families;
};

$vBox->append($fontCombo);

$hBox->append($histogram, true);

$window->show();

$app->run();
?>

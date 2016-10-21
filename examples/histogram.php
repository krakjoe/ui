<?php
use UI\App;
use UI\Point;
use UI\Size;
use UI\Window;
use UI\Control\Box;
use UI\Control\Spin;
use UI\Control\ColorButton;
use UI\Control\Button;
use UI\Control\Entry;
use UI\Area;
use UI\Draw\Pen;
use UI\Draw\Path;
use UI\Draw\Color;
use UI\Draw\Brush;
use UI\Draw\Stroke;
use UI\Draw\Matrix;

$app = new App();

$window = new Window($app, "libui Histogram Example", new Size(640, 480), true);

$window->setMargin(true);

$hBox = new Box(BOX::HORIZONTAL);
$hBox->setPadded(true);

$vBox = new Box(BOX::VERTICAL);
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
		$path = new Path(PATH::WINDING);

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
		$path = new Path(PATH::WINDING);
		$path->addRectangle($clipPoint, $areaSize);
		$path->end();

		$pen->fill($path, new Brush(BRUSH::SOLID, new Color(0xFFFFFF, 1)));
	
		$graphSize = new Size($areaSize->width - 40, $areaSize->height - 40);

		$zero = new Point(20, 20);

		$path = new Path(PATH::WINDING);
		$path->newFigure($zero);
		$path->lineTo(new Point(20, 20 + $graphSize->height));
		$path->lineTo(new Point(20 + $graphSize->width, 20 + $graphSize->height));	
		$path->end();

		$stroke = new Stroke(STROKE::CAP_FLAT, STROKE::JOIN_MITER, 2, 10);

		$pen->stroke($path, new Brush(BRUSH::SOLID, new Color(0x000000, 1)), $stroke);

		$matrix = new Matrix();
		$matrix->translate($zero);

		$pen->transform($matrix);

		$points = $this->getGraphPoints($graphSize);

		$path = $this->getGraphPath($points, $graphSize, true);

		$brush = new Brush(BRUSH::SOLID, $this->button->getColor());

		$pen->fill($path, $brush);

		$path = $this->getGraphPath($points, $graphSize, false);

		$strokeColor = $brush->getColor();
		$strokeColor
			->setChannel(COLOR::ALPHA, 
				$strokeColor->getChannel(COLOR::ALPHA)/2);
		$brush->setColor($strokeColor);

		$pen->stroke($path, $brush, $stroke);
	}

	public function setColorSource(ColorButton $button) {
		$this->button = $button;
	}

	public function getColorSource() {
		return $this->button;
	}

	public function __construct(array &$sources, ColorButton $button = null) {
		$this->sources =& $sources;
		$this->button  = $button;
	}

	private $sources;
	private $button;
};

$colorBox = new Entry();
$colorBox->setText("0x8892BF");

$colorButton = new class($histogram, $colorBox, new Color(0x8892BF, 1)) extends ColorButton {

	protected function onChange() {
		$redrawColor = $this->getColor();

		$this->entry->setText(sprintf(
			"0x%02X%02X%02X",
				$redrawColor->getChannel(COLOR::RED) * 255,
				$redrawColor->getChannel(COLOR::GREEN) * 255,
				$redrawColor->getChannel(COLOR::BLUE) * 255));

		$this->histogram->redraw();
	}

	public function __construct(Area $histogram, Entry $entry, Color $color) {
		$this->histogram = $histogram;
		$this->entry = $entry;

		$this->setColor($color);
	}

	private $histogram;
};

$histogram->setColorSource($colorButton);

$redrawHistogram = function() use($histogram, $colorBox, $colorButton) {
	$redrawColor = $colorButton->getColor();

	$colorBox->setText(sprintf(
		"0x%02X%02X%02X",
			$redrawColor->getChannel(COLOR::RED) * 255,
			$redrawColor->getChannel(COLOR::GREEN) * 255,
			$redrawColor->getChannel(COLOR::BLUE) * 255));

	$histogram->redraw();
};

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
$hBox->append($histogram, true);

$window->show();

$app->run();
?>

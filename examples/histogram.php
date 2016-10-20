<?php
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

$window = new Window("libui Histogram Example", new Size(640, 480), true);

$window->setMargin(true);

$hBox = new Box(BOX::HORIZONTAL);
$hBox->setPadded(true);

$vBox = new Box(BOX::VERTICAL);
$vBox->setPadded(true);

$window->add($hBox);

$hBox->append($vBox);

$dataSources = [];

function getGraphPoints(array $dataSources, Size $size) : array {
	$width = $size->getWidth();
	$height = $size->getHeight();

	$xincr = $width / 9;
	$yincr = $height / 100;

	$points = [];

	foreach ($dataSources as $i => $source) {
		$points[$i] = new Point(
						$xincr * $i, 
						$yincr * (100 - $source->getValue()));
	}

	return $points;
}

function getGraphPath(array $locations, Size $size, bool $extend = false) : Path {
	$path = new Path(PATH::WINDING);

	$path->newFigure(array_shift($locations));

	foreach ($locations as $location) {
		$path->lineTo($location);
	}

	if ($extend) {
		$path->lineTo(new Point(
						$size->width, 
						$size->height));
		$path->lineTo(new Point(
						0, 
						$size->height));
		$path->closeFigure();
	}

	$path->end();
	return $path;
}

$colorButton = new ColorButton();

$white = new Brush(BRUSH::SOLID, new Color(0xFFFFFF, 1));
$black = new Brush(BRUSH::SOLID, new Color(0x000000, 1));

$histogram = new Area();

$histogram->onDraw(function(Area $area, Pen $pen, Size $areaSize, Point $clipPoint, Size $clipSize) use($white, $black, &$dataSources, $colorButton) {
	$path = new Path(PATH::WINDING);

	$path
		->addRectangle($clipPoint, $areaSize);

	$path->end();

	$pen->fill($path, $white);

	$graphSize = new Size(
		$areaSize->width - 40, 
		$areaSize->height - 40);

	$path = new Path(PATH::WINDING);

	$zero = new Point(20, 20);

	$path->newFigure($zero);

	$path->lineTo(new Point(
		20, 
		20 + $graphSize->height));

	$path->lineTo(new Point(
		20 + $graphSize->width, 
		20 + $graphSize->height));
	
	$path->end();

	$stroke = new Stroke(STROKE::CAP_FLAT, STROKE::JOIN_MITER, 2, 10);

	$pen->stroke($path, $black, $stroke);

	$matrix = new Matrix();
	$matrix->translate($zero);

	$pen->transform($matrix);

	$points = 
		getGraphPoints($dataSources, $graphSize);

	$path = getGraphPath($points, $graphSize, true);

	$brush = new Brush(BRUSH::SOLID, $colorButton->getColor());

	$pen->fill($path, $brush);

	$path = getGraphPath($points, $graphSize, false);

	$strokeColor = $brush->getColor();
	$strokeColor
		->setChannel(COLOR::ALPHA, 
			$strokeColor->getChannel(COLOR::ALPHA)/2);
	$brush->setColor($strokeColor);

	$pen->stroke($path, $brush, $stroke);
});

$colorBox = new Entry();
$colorBox->setText("0x8892BF");

$redrawHistogram = function() use($histogram, $colorBox, $colorButton) {
	$redrawColor = $colorButton->getColor();

	$colorBox->setText(sprintf(
		"0x%02X%02X%02X",
			$redrawColor->getChannel(COLOR::RED) * 255,
			$redrawColor->getChannel(COLOR::GREEN) * 255,
			$redrawColor->getChannel(COLOR::BLUE) * 255));

	$histogram->redraw();
};

$colorButton->setColor(new Color(0x8892BF, 1));
$colorButton->onChange($redrawHistogram);

for ($i = 0; $i < 10; $i++) {
	$dataSources[$i] = new Spin(0, 100);
	$dataSources[$i]->setValue(mt_rand(0, 100));
	$dataSources[$i]->onChange($redrawHistogram);
	$vBox->append($dataSources[$i]);
}

$vBox->append($colorButton);

$colorBoxButton = new Button("Set Color");
$colorBoxButton->onClick(function() use($colorBox, $colorButton, $redrawHistogram) {
	$colorButton->setColor(
		new Color(hexdec($colorBox->getText()), 1));
	$redrawHistogram();
});

$vBox->append($colorBox);
$vBox->append($colorBoxButton);
$hBox->append($histogram, true);

$window->show();

UI\main();
?>

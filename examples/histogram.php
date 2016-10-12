<?php
use UI\Window;
use UI\Box;
use UI\Spin;
use UI\ColorButton;
use UI\Area;
use UI\DrawPath;
use UI\DrawBrush;
use UI\Draw;
use UI\DrawStroke;
use UI\DrawMatrix;

$window = new Window("libui Histogram Example", 640, 480, true);

$window->setMargin(true);

$hBox = new Box(BOX::HORIZONTAL);
$hBox->setPadded(true);

$vBox = new Box(BOX::VERTICAL);
$vBox->setPadded(true);

$window->add($hBox);

$hBox->append($vBox);

$dataPoints = [];

$xs = [];
$ys = [];

function getGraphSize($clientWidth, $clientHeight, &$graphWidth, &$graphHeight) {
	$graphWidth = $clientWidth - 40;
	$graphHeight = $clientHeight - 40;
}

function getGraphLocations($dataPoints, $width, $height) {
	$xincr = $width / 9;
	$yincr = $height / 100;

	$xs = [];
	$ys = [];

	for ($i = 0; $i < count($dataPoints); $i++) {
		$xs[$i] = $xincr * $i;
		$ys[$i] = $yincr * (100 - $dataPoints[$i]->getValue());
	}

	return [
		"xs" => $xs,
		"ys" => $ys
	];
}

function getGraphPath($locations, $width, $height, $extend) {
	$path = new DrawPath(DRAWPATH::WINDING);

	$path->newFigure($locations["xs"][0], $locations["ys"][0]);
	for ($i = 1; $i < 10; $i++)
		$path->lineTo($locations["xs"][$i], $locations["ys"][$i]);
	
	if ($extend) {
		$path->lineTo($width, $height);
		$path->lineTo(0, $height);
		$path->closeFigure();
	}

	$path->end();
	return $path;
}

$colorButton = new ColorButton();

$white = new DrawBrush(DRAWBRUSH::SOLID, 1, 1, 1, 1);
$black = new DrawBrush(DRAWBRUSH::SOLID, 0, 0, 0, 1);

$histogram = new Area(function($area, $context, $areaWidth, $areaHeight, $clipX, $clipY, $clipWidth, $clipHeight) use($white, $black, &$dataPoints, $colorButton) {
	$path = new DrawPath(DRAWPATH::WINDING);

	$path
		->addRectangle($clipX, $clipY, $areaWidth, $areaHeight);

	$path->end();

	Draw::fill($context, $path, $white);

	getGraphSize($areaWidth, $areaHeight, $graphWidth, $graphHeight);

	$path = new DrawPath(DRAWPATH::WINDING);

	$path->newFigure(20, 20);
	$path->lineTo(20, 20 + $graphHeight);
	$path->lineTo(20 + $graphWidth, 20 + $graphHeight);
	
	$path->end();

	Draw::stroke($context, $path, $black, new DrawStroke(DRAWSTROKE::CAP_FLAT, DRAWSTROKE::JOIN_MITER, 2, 10));

	$matrix = new DrawMatrix();
	$matrix->translate(20, 20);
	
	Draw::transform($context, $matrix);

	$locations = getGraphLocations(
		$dataPoints, $graphWidth, $graphHeight);	

	$path = getGraphPath($locations, $graphWidth, $graphHeight, true);
	
	$brush = $colorButton->getBrush();
	
	Draw::fill($context, $path, $brush);

	$path = getGraphPath($locations, $graphWidth, $graphHeight, false);

	$brush->setAlpha($brush->getAlpha()/2);

	Draw::stroke($context, $path, $brush, new DrawStroke(DRAWSTROKE::CAP_FLAT, DRAWSTROKE::JOIN_MITER, 2, 10));
});

$redrawHistogram = function() use($histogram) {
	$histogram->redraw();
};

$brush = new DrawBrush(DRAWBRUSH::SOLID);
$brush->setAlpha(1);
$brush->setRGB(0x8892BF); # this is the color of PHP, internally ...

$colorButton->setColorFromBrush($brush);
$colorButton->onChange($redrawHistogram);

for ($i = 0; $i < 10; $i++) {
	$dataPoints[$i] = new Spin(0, 100);
	$dataPoints[$i]->setValue(mt_rand(0, 100));
	$dataPoints[$i]->onChange($redrawHistogram);
	$vBox->append($dataPoints[$i]);
}

$vBox->append($colorButton);

$hBox->append($histogram, true);

$window->show();

uiMain();
?>

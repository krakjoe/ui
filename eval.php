<?php
use UI\Window;
use UI\Box;
use UI\Label;
use UI\Button;
use UI\Entry;
use UI\Multi;

$window = new Window("PHP - evil", 640, 480);

$inputBox = new Box(BOX::HORIZONTAL);

$inputLabel = new Label("Select a file to load:");
$file = new Entry();
$openButton = new Button("Open");

$inputBox->append($inputLabel, true);
$inputBox->append($file);
$inputBox->append($openButton);

$outputBox = new Box(BOX::VERTICAL);
$fileContents = new Multi();

$outputBox->append($fileContents, true);

$openButton->onClick(function() use($window, $file, $outputBox) {
	$fileName = $window->open();
	$file->setText($fileName);
	$outputBox->setText(
		file_get_contents($fileName));
});


$window->add($inputBox);
$window->add($outputBox);

$window->show();

uiMain();
?>

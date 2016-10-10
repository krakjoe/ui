<?php
use UI\Window;
use UI\Button;
use UI\Box;
use UI\Label;
use UI\Entry;
use UI\Spin;
use UI\Slider;
use UI\Progress;
use UI\Separator;
use UI\Combo;
use UI\EditableCombo;
use UI\Radio;
use UI\Picker;
use UI\Menu;

$window = new Window("PHP-GTK is not dead", 480, 240);

$box = new UI\Box(UI\Box::VERTICAL);

$box->append(new Label("Username:"));

$user = new Entry();

$box->append($user);

$box->append(new Label("Password:"));

$pass = new Entry(ENTRY::PASSWORD);

$box->append($pass);

$button = new Button("OK");

$button->onClick(function() use($user, $pass){
	var_dump($user->getText(), $pass->getText());
});
$button->disable();

$box->append($button);

$spin = new Spin(0, 100);

$spin->setValue(50);

$spin->onChange(function() use($spin) {
	var_dump($spin->getValue());
});

$box->append($spin);

$box->append(new Separator(SEPARATOR::HORIZONTAL));

$progress = new Progress();

$progress->setValue(50);

$box->append($progress);

$slider = new Slider(0, 100);

$slider->setValue(50);

$slider->onChange(function() use($slider, $progress) {
	var_dump($slider->getValue());
	$progress->setValue($slider->getValue());
});

$box->append($slider);

$combo = new Combo();
$combo->append("Oranges");
$combo->append("Apples");
$combo->onSelected(function() use($combo) {
	var_dump($combo->getSelected());
});

$box->append($combo);

$ecombo = new EditableCombo();
$ecombo->append("Oranges");
$ecombo->append("Apples");
$ecombo->onChange(function() use($ecombo) {
	var_dump($ecombo->getText());
});

$box->append($ecombo);

$radio = new Radio();
$radio->append("Oranges");
$radio->append("Apples");
$radio->onSelected(function() use($radio) {
	var_dump($radio->getSelected());
});

$box->append($radio);

$picker = new Picker(PICKER::DATE);

$box->append($picker);

$window->add($box);

$file = new UI\Menu("File");
$file->append("Open");

$window->show();

uiMain();
?>

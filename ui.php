<?php
use UI\Window;
use UI\Tab;
use UI\Box;
use UI\Group;
use UI\Form;
use UI\Button;
use UI\Entry;
use UI\Multi;
use UI\Spin;
use UI\Slider;
use UI\Progress;
use UI\Combo;
use UI\EditableCombo;
use UI\Radio;

$window = new UI\Window("libui Control Gallery", 640, 480);

$window->setMargin(true);

$tab = new Tab();

$window->add($tab);

$tab->append("Basic Controls", (function(){
	$vbox = new Box(BOX::VERTICAL);
	$vbox->setPadded(true);

	$hbox = new Box(BOX::HORIZONTAL);
	$hbox->setPadded(true);

	$vbox->append($hbox);

	$group = new Group("Entries");
	$group->setMargins(true);

	$vbox->append($group);

	$entryForm = new Form();
	$entryForm->setPadded(true);

	$entryForm->append("Entry", new Entry(ENTRY::NORMAL), false);
	$entryForm->append("Password Entry", new Entry(ENTRY::PASSWORD), false);
	$entryForm->append("Search Entry", new Entry(ENTRY::SEARCH), false);
	$entryForm->append("Multiline Entry", new Multi(MULTI::WRAP), true);
	$entryForm->append("Multiline Entry Without Wrapping", new Multi(MULTI::NOWRAP), true);

	$group->add($entryForm);

	return $vbox;
})());

$tab->append("Numbers and Lists", (function(){
	$hbox = new Box(BOX::HORIZONTAL);
	$hbox->setPadded(true);

	$group = new Group("Numbers");
	$group->setMargins(true);
	
	$hbox->append($group, true);

	$vbox = new Box(BOX::VERTICAL);
	$vbox->setPadded(true);

	$group->add($vbox);
	
	$spin = new Spin(0, 100);
//	$spin->onChange(function(){
//
//	});
	$slider = new Slider(0, 100);
//	$slider->onChange(function(){
//
//	});	
	$progress = new Progress();
	$vbox->append($spin);
	$vbox->append($slider);
	$vbox->append($progress);
	
	$ip = new Progress();
	$ip->setValue(-1);
	$vbox->append($ip);

	$group = new Group("Lists");
	$group->setMargins(true);
	$hbox->append($group);
	
	$vbox = new Box(BOX::VERTICAL);
	$vbox->setPadded(true);
	$group->add($vbox);

	$combo = new Combo();
	$combo->append("Item 1");
	$combo->append("Item 2");
	$combo->append("Item 3");
	$vbox->append($combo);

	$ecombo = new EditableCombo();
	$ecombo->append("Editable Item 1");
	$ecombo->append("Editable Item 2");
	$ecombo->append("Editable Item 3");
	$vbox->append($ecombo);

	$radio = new Radio();
	$radio->append("Radio Button 1");
	$radio->append("Radio Button 2");
	$radio->append("Radio Button 3");
	$vbox->append($radio);
	
	return $hbox;
})());

$tab->setMargin(0, true);

$window->show();

uiMain();
?>

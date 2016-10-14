<?php
use UI\Window;
use UI\Size;
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
use UI\Grid;
use UI\Picker;
use UI\Check;
use UI\Label;
use UI\Separator;
use UI\ColorButton;
use UI\Menu;

$menu = new Menu("File");
$quitButton = $menu->append("Quit");
$quitButton->onClick(function($item, $window){
	//$window->close();
});

$window = new UI\Window("libui Control Gallery", new Size(640, 480), true);

$window->setMargin(true);

$tab = new Tab();

$window->add($tab);

$basicControlsVbox = new Box(BOX::VERTICAL);
$basicControlsVbox->setPadded(true);

$basicControlsHbox = new Box(BOX::HORIZONTAL);
$basicControlsHbox->setPadded(true);

$basicControlsVbox->append($basicControlsHbox);

$basicControlsHbox->append(new Button("Button"));
$basicControlsHbox->append(new Check("Checkbox"));
$basicControlsVbox->append(
	new Label("This is a label. Right now, labels can only span one line."));
$basicControlsVbox->append(new Separator(SEPARATOR::HORIZONTAL));

$entriesGroup = new Group("Entries");
$entriesGroup->setMargin(true);

$basicControlsVbox->append($entriesGroup, true);

$entryForm = new Form();
$entryForm->setPadded(true);

$entryForm->append("Entry", new Entry(ENTRY::NORMAL), false);
$entryForm->append("Password Entry", new Entry(ENTRY::PASSWORD), false);
$entryForm->append("Search Entry", new Entry(ENTRY::SEARCH), false);
$entryForm->append("Multiline Entry", new Multi(MULTI::WRAP), true);
$entryForm->append("Multiline Entry Without Wrapping", new Multi(MULTI::NOWRAP), true);

$entriesGroup->add($entryForm);

$tab->append("Basic Controls", $basicControlsVbox);

$numbersHbox = new Box(BOX::HORIZONTAL);
$numbersHbox->setPadded(true);

$numbersGroup = new Group("Numbers");
$numbersGroup->setMargin(true);

$numbersHbox->append($numbersGroup, true);

$numbersVbox = new Box(BOX::VERTICAL);
$numbersVbox->setPadded(true);

$numbersGroup->add($numbersVbox);

$spin = new Spin(0, 100);
$slider = new Slider(0, 100);
$progress = new Progress();

$spin->onChange(function() use ($slider, $spin, $progress) {
	$slider->setValue($spin->getValue());
	$progress->setValue($spin->getValue());
});
$slider->onChange(function() use($slider, $spin, $progress) {
	$spin->setValue($slider->getValue());
	$progress->setValue($slider->getValue());
});	

$numbersVbox->append($spin);
$numbersVbox->append($slider);
$numbersVbox->append($progress);

$ip = new Progress();
$ip->setValue(-1);
$numbersVbox->append($ip);

$listsGroup = new Group("Lists");
$listsGroup->setMargin(true);
$numbersHbox->append($listsGroup);

$otherBox = new Box(BOX::VERTICAL);
$otherBox->setPadded(true);
$listsGroup->add($otherBox);

$combo = new Combo();
$combo->append("Item 1");
$combo->append("Item 2");
$combo->append("Item 3");
$otherBox->append($combo);

$ecombo = new EditableCombo();
$ecombo->append("Editable Item 1");
$ecombo->append("Editable Item 2");
$ecombo->append("Editable Item 3");
$otherBox->append($ecombo);

$radio = new Radio();
$radio->append("Radio Button 1");
$radio->append("Radio Button 2");
$radio->append("Radio Button 3");
$otherBox->append($radio);

$tab->append("Numbers and Lists", $numbersHbox);

$dataHbox = new Box(BOX::HORIZONTAL);
$dataHbox->setPadded(true);

$dataVbox = new Box(BOX::VERTICAL);
$dataVbox->setPadded(true);

$dataHbox->append($dataVbox);

$dataVbox->append(new Picker(PICKER::DATE));
$dataVbox->append(new Picker(PICKER::TIME));
$dataVbox->append(new Picker(PICKER::DATETIME));
$dataVbox->append(new ColorButton());

$dataHbox->append(new Separator(SEPARATOR::VERTICAL));	

$filesVbox = new Box(BOX::VERTICAL);
$filesVbox->setPadded(true);
$dataHbox->append($filesVbox, true);

$filesGrid = new Grid();
$filesGrid->setPadded(true);
$filesVbox->append($filesGrid);

$openButton = new Button("Open File");
$openEntry  = new Entry();
$openEntry->setReadOnly(true);

$openButton->onClick(function() use($openEntry, $window) {
	$openFile = $window->open();

	if (!$openFile) {
		return;
	}

	$openEntry->setText($openFile);
});

$filesGrid->append($openButton, 0, 0, 1, 1, 
	false, GRID::FILL, false, GRID::FILL);
$filesGrid->append($openEntry, 1, 0, 1, 1, 
	true, GRID::FILL, false, GRID::FILL);

$saveButton = new Button("Save File");
$saveEntry = new Entry();
$saveEntry->setReadOnly(true);

$saveButton->onClick(function() use($saveEntry, $window) {
	$saveFile = $window->save();

	if (!$saveFile) {
		return;
	}

	$saveEntry->setText($saveFile);
});

$filesGrid->append($saveButton, 0, 1, 1, 1,
	false, GRID::FILL, false, GRID::FILL);
$filesGrid->append($saveEntry, 1, 1, 1, 1,
	true, GRID::FILL, false, GRID::FILL);

$messageGrid = new Grid();
$messageGrid->setPadded(true);
$filesGrid->append($messageGrid, 0, 2, 2, 1,
	false, GRID::CENTER, false, GRID::START);

$messageButton = new Button("Message Box");
$messageButton->onClick(function() use($window) {
	$window->msg(
		"This is a normal message box", 
		"More detailed information can be shown here.");
});
$messageGrid->append($messageButton, 0, 0, 1, 1,
	false, GRID::FILL, false, GRID::FILL);
$errorButton = new Button("Error Box");
$errorButton->onClick(function() use($window) {
	$window->error(
		"This message box describes an error",
		"More detailed information can be shown here.");
});
$messageGrid->append($errorButton, 1, 0, 1, 1,
	false, GRID::FILL, false, GRID::FILL);

$tab->append("Data Choosers", $dataHbox);

$tab->setMargin(0, true);
$tab->setMargin(1, true);
$tab->setMargin(2, true);

$window->show();

UI\main();
?>

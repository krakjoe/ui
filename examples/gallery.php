<?php
use UI\Size;
use UI\Menu;
use UI\MenuItem;
use UI\Window;
use UI\Control\Tab;
use UI\Control\Box;
use UI\Control\Group;
use UI\Control\Form;
use UI\Control\Button;
use UI\Control\Entry;
use UI\Control\MultilineEntry;
use UI\Control\Spin;
use UI\Control\Slider;
use UI\Control\Progress;
use UI\Control\Combo;
use UI\Control\EditableCombo;
use UI\Control\Radio;
use UI\Control\Grid;
use UI\Control\Picker;
use UI\Control\Check;
use UI\Control\Label;
use UI\Control\Separator;
use UI\Control\ColorButton;

class QuitButton extends MenuItem {
	public function onClick() {
		UI\quit();
	}
}

$menu = new Menu("File");
$quitButton = $menu->append("Quit", QuitButton::class);

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
$entryForm->append("Multiline Entry", new MultilineEntry(MultilineEntry::WRAP), true);
$entryForm->append("Multiline Entry Without Wrapping", new MultilineEntry(MultilineEntry::NOWRAP), true);

$entriesGroup->add($entryForm);

$tab->append("Basic Controls", $basicControlsVbox);
$tab->setMargin(0, true);

$numbersHbox = new Box(BOX::HORIZONTAL);
$numbersHbox->setPadded(true);

$numbersGroup = new Group("Numbers");
$numbersGroup->setMargin(true);

$numbersHbox->append($numbersGroup, true);

$numbersVbox = new Box(BOX::VERTICAL);
$numbersVbox->setPadded(true);

$numbersGroup->add($numbersVbox);

$progress = new Progress();

$spin = new class(0, 100) extends Spin {

	public function setSlider(Slider $slider) {
		$this->slider = $slider;
	}

	public function setProgress(Progress $progress) {
		$this->progress = $progress;	
	}

	protected function onChange() {
		$this->slider->setValue($this->getValue());
		$this->progress->setValue($this->getValue());
	}

	private $slider;
	private $progress;
};
$spin->setProgress($progress);

$slider = new class(0, 100) extends Slider {
	public function setSpin(Spin $spin) {
		$this->spin = $spin;
	}

	public function setProgress(Progress $progress) {
		$this->progress = $progress;
	}

	protected function onChange() {
		$this->spin->setValue($this->getValue());
		$this->progress->setValue($this->getValue());
	}

	private $spin;
	private $progress;
};

$slider->setProgress($progress);

$slider->setSpin($spin);
$spin->setSlider($slider);

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
$tab->setMargin(1, true);

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

$openEntry  = new Entry();
$openEntry->setReadOnly(true);

$openButton = new class("Open File", $openEntry, $window) extends Button {
	public function __construct(string $text, Entry $entry, Window $window) {
		$this->entry = $entry;
		$this->window = $window;

		parent::__construct($text);
	}
	
	protected function onClick() {
		$openFile = $this->window->open();

		if (!$openFile) {
			return;
		}

		$this->entry->setText($openFile);
	}

	private $entry;
	private $window;
};

$filesGrid->append($openButton, 0, 0, 1, 1, 
	false, GRID::FILL, false, GRID::FILL);
$filesGrid->append($openEntry, 1, 0, 1, 1, 
	true, GRID::FILL, false, GRID::FILL);

$saveEntry = new Entry();
$saveEntry->setReadOnly(true);
$saveButton = new class("Save File", $saveEntry, $window) extends Button {
	public function __construct(string $text, Entry $entry, Window $window) {
		$this->entry = $entry;
		$this->window = $window;

		parent::__construct($text);
	}
	
	protected function onClick() {
		$saveFile = $this->window->save();

		if (!$saveFile) {
			return;
		}

		$this->entry->setText($saveFile);
	}

	private $entry;
	private $window;
};

$filesGrid->append($saveButton, 0, 1, 1, 1,
	false, GRID::FILL, false, GRID::FILL);
$filesGrid->append($saveEntry, 1, 1, 1, 1,
	true, GRID::FILL, false, GRID::FILL);

$messageGrid = new Grid();
$messageGrid->setPadded(true);
$filesGrid->append($messageGrid, 0, 2, 2, 1,
	false, GRID::CENTER, false, GRID::START);

$messageButton = new class("Msg Box", $window) extends Button {
	public function __construct(string $text, Window $window) {
		$this->window = $window;

		parent::__construct($text);
	}
	
	public function onClick() {
		$this->window->msg(
			"This is a normal message box", 
			"More detailed information can be shown here.");
	}

	private $window;
};
$messageGrid->append($messageButton, 0, 0, 1, 1,
	false, GRID::FILL, false, GRID::FILL);

$errorButton = new class("Error Box", $window) extends Button {
	public function __construct(string $text, Window $window) {
		$this->window = $window;

		parent::__construct($text);
	}
	
	public function onClick() {
		$this->window->error(
			"This message box describes an error",
			"More detailed information can be shown here.");
	}

	private $window;
};

$messageGrid->append($errorButton, 1, 0, 1, 1,
	false, GRID::FILL, false, GRID::FILL);

$tab->append("Data Choosers", $dataHbox);
$tab->setMargin(2, true);

$window->show();

UI\main();
?>

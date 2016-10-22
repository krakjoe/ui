<?php
use UI\Size;
use UI\App;
use UI\Menu;
use UI\MenuItem;
use UI\Window;

use UI\Controls\Tab;
use UI\Controls\Box;
use UI\Controls\Group;
use UI\Controls\Form;
use UI\Controls\Button;
use UI\Controls\Entry;
use UI\Controls\MultilineEntry;
use UI\Controls\Spin;
use UI\Controls\Slider;
use UI\Controls\Progress;
use UI\Controls\Combo;
use UI\Controls\EditableCombo;
use UI\Controls\Radio;
use UI\Controls\Grid;
use UI\Controls\Picker;
use UI\Controls\Check;
use UI\Controls\Label;
use UI\Controls\Separator;
use UI\Controls\ColorButton;

$app = new App();

$window = new Window($app, "libui Control Gallery", new Size(640, 480), true);
$window->setMargin(true);

$tab = new Tab();

$window->add($tab);

$basicControlsVbox = new Box(Box::Vertical);
$basicControlsVbox->setPadded(true);

$basicControlsHbox = new Box(Box::Horizontal);
$basicControlsHbox->setPadded(true);

$basicControlsVbox->append($basicControlsHbox);

$basicControlsHbox->append(new Button("Button"));
$basicControlsHbox->append(new Check("Checkbox"));
$basicControlsVbox->append(
	new Label("This is a label. Right now, labels can only span one line."));
$basicControlsVbox->append(new Separator(Separator::Horizontal));

$entriesGroup = new Group("Entries");
$entriesGroup->setMargin(true);

$basicControlsVbox->append($entriesGroup, true);

$entryForm = new Form();
$entryForm->setPadded(true);

$entryForm->append("Entry", new Entry(Entry::Normal), false);
$entryForm->append("Password Entry", new Entry(Entry::Password), false);
$entryForm->append("Search Entry", new Entry(Entry::Search), false);
$entryForm->append("Multiline Entry", new MultilineEntry(MultilineEntry::Wrap), true);
$entryForm->append("Multiline Entry Without Wrapping", new MultilineEntry(MultilineEntry::NoWrap), true);

$entriesGroup->add($entryForm);

$tab->append("Basic Controls", $basicControlsVbox);
$tab->setMargin(0, true);

$numbersHbox = new Box(Box::Horizontal);
$numbersHbox->setPadded(true);

$numbersGroup = new Group("Numbers");
$numbersGroup->setMargin(true);

$numbersHbox->append($numbersGroup, true);

$numbersVbox = new Box(Box::Vertical);
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

$otherBox = new Box(Box::Vertical);
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

$dataHbox = new Box(Box::Horizontal);
$dataHbox->setPadded(true);

$dataVbox = new Box(Box::Vertical);
$dataVbox->setPadded(true);

$dataHbox->append($dataVbox);

$dataVbox->append(new Picker(Picker::Date));
$dataVbox->append(new Picker(Picker::Time));
$dataVbox->append(new Picker(Picker::DateTime));
$dataVbox->append(new ColorButton());

$dataHbox->append(new Separator(Separator::Vertical));	

$filesVbox = new Box(Box::Vertical);
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
	false, Grid::Fill, false, Grid::Fill);
$filesGrid->append($openEntry, 1, 0, 1, 1, 
	true, Grid::Fill, false, Grid::Fill);

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
	false, Grid::Fill, false, Grid::Fill);
$filesGrid->append($saveEntry, 1, 1, 1, 1,
	true, Grid::Fill, false, Grid::Fill);

$messageGrid = new Grid();
$messageGrid->setPadded(true);
$filesGrid->append($messageGrid, 0, 2, 2, 1,
	false, Grid::Center, false, Grid::Start);

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
	false, Grid::Fill, false, Grid::Fill);

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
	false, Grid::Fill, false, Grid::Fill);

$tab->append("Data Choosers", $dataHbox);
$tab->setMargin(2, true);

$window->show();

$app->run();
?>

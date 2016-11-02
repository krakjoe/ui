<?php
define ("PHP_UI_SECOND",    1000000);
define ("PHP_UI_SNAKE_FPS", 30);

use UI\Window;
use UI\Point;
use UI\Size;
use UI\Area;
use UI\Key;

use UI\Controls\Box;

use UI\Draw\Pen;
use UI\Draw\Brush;
use UI\Draw\Brush\LinearGradient;
use UI\Draw\Path;
use UI\Draw\Color;
use UI\Draw\Stroke;
use UI\Draw\Matrix;

use UI\Draw\Text\Font;
use UI\Draw\Text\Font\Descriptor;

use UI\Executor;

$win = new class("Snake", new Size(640, 480), false) extends Window {
	public function addExecutor(Executor $executor) {
		$this->executors[] = $executor;
	}

	protected function onClosing() {
		foreach ($this->executors as $executor) {
			$executor->kill();
		}

		$this->destroy();

		UI\quit();
	}
};

$box = new Box(Box::Vertical);
$win->add($box);

class Cell {

	public function __construct(Point $point, $color) {
		$this->point = $point;
		$this->color = $color;
	}

	public function __clone() {
		$this->point = clone $this->point;
	}

	public $point;
	public $color;
}

$snake = new class($box) extends Area{

	public function __construct(Box $box) {
		$box->append($this, true);
	}

	public function setExecutor(Executor $executor) {
		$this->executor = $executor;
	}

	protected function onKey(string $char, int $key, int $flags) {
		if ($flags & Area::Down) {
			switch ($key) {
				case Key::Up:
					if ($this->direction == Key::Down)
						return;
					$this->direction = $key;
				break;

				case Key::Down:
					if ($this->direction == Key::Up)
						return;
					$this->direction = $key;
				break;

				case Key::Right:
					if ($this->direction == Key::Left)
						return;
					$this->direction = $key;
				break;

				case Key::Left:
					if ($this->direction == Key::Right)
						return;

					$this->direction = $key;
				break;

				default:
					if ($char == " ") {
						$this->pause = !$this->pause;

						if ($this->pause) {
							/* this allows the CPU to idle while paused */
							$this->executor->setInterval(0);
						} else {
							/* this will (re)start the game */
							$this->executor->setInterval(PHP_UI_SECOND/PHP_UI_SNAKE_FPS);
						}
					}
				break;
			}
		}
	}

	protected function onDraw(Pen $pen, Size $size, Point $clip, Size $clipSize) {
		$zero = new Point(0, 0);
		$frame = $zero + 40;
		$frameSize = $size - 80;

		$path = new Path();
		$path->addRectangle($zero, $size);
		$path->end();

		$pen->fill($path, 0xf5f5f5ff);

		$stroke = new Stroke();	

		$pen->stroke($path, 0x000000FF, $stroke);

		$path = new Path();
		$path->addRectangle($frame, $frameSize);
		$path->end();

		$gradient = new LinearGradient(
			Point::at(0), new Point(0, $frameSize->height));

		$gradient->addStop(0, 0x000000FF);
		$gradient->addStop(1.0, 0x303030F1);

		$pen->fill($path, $gradient);

		$pen->stroke($path, 0x000000FF, $stroke);

		$matrix = new Matrix();
		$matrix->translate($frame);

		$pen->transform($matrix);

		if (!$this->snake) {
			$this->newSnake($frameSize);
		}

		if (!$this->food) {
			$this->newFood($frameSize);
		}

		if (!$this->pause && ($run = microtime(true)) - $this->run > 0.2) {
			$this->run = $run;

			$head = clone $this->snake[0];

			switch ($this->direction) {
				case Key::Right: $head->x++; break;
				case Key::Left:  $head->x--; break;
				case Key::Up: $head->y--; break;
				case Key::Down: $head->y++; break;
			}

			if ($head->x < 0 || $head->x >= ($frameSize->width)/20 || 
				$head->y < 0 || $head->y >= ($frameSize->height)/20) {
				$this->newSnake($frameSize);
				$this->newFood($frameSize);

				foreach ($this->snake as $color => $body) {
					$this->newCell($pen, $body, $color);
				}

				$this->newCell($pen, $this->food, count($this->snake)-1);

				$this->pause = true;
				$this->direction = Key::Right;
				$this->score = 0;
				$this->level = 1;
				$this->executor->setInterval(0);
				return;
			}

			if ($this->food == $head) {
				$this->snake[] = $this->food;
				$this->score += 10;
				$this->level = ceil($this->score / 100);
				$this->newFood($frameSize);
			}

			$tail = array_pop($this->snake);
			$tail->x = $head->x;
			$tail->y = $head->y;

			array_unshift($this->snake, $tail);		
		}

		foreach ($this->snake as $color => $body) {
			$this->newCell($pen, $body, $color);
		}
		
		$this->newCell($pen, $this->food, count($this->snake));

		$matrix = new Matrix();
		$matrix->translate($zero - 40);
		$pen->transform($matrix);

		if ($this->pause) {
			$this->drawPause($pen, $size);
		} else $this->drawScore($pen, $size);
	}

	private function newColor() {
		if (self::$nextColor == count(self::$availableColors))
			self::$nextColor = 0;

		return self::$availableColors[self::$nextColor++];
	}

	private function newSnake($size) {
		self::$nextColor = 0;
		shuffle(self::$availableColors);

		$this->snake = [];
		$this->colors = [];

		for ($i = 4; $i >= 0; $i--) {
			$this->snake[] = new Point($i, 0);
			$this->colors[] = $this->newColor();
		}
	}

	private function newFood(Size $size) {
		$this->food = new Point(
			floor(mt_rand(40, ($size->width ) - 20) / 20), 
			floor(mt_rand(40, ($size->height) - 20) / 20));

		$this->colors[count($this->snake)] = $this->newColor();
	}

	private function newCell(Pen $pen, Point $point, int $color) {
		$path = new Path();
		$path->addRectangle(
			$point * 20, new Size(20, 20));
		$path->end();

		$pen->fill($path, $this->colors[$color]);

		$stroke = new Stroke();
		$stroke->setThickness(3);

		$pen->stroke($path, 0xFFFFFFFF, $stroke);
	}

	private function drawPause(Pen $pen, Size $size) {
		$layout = new UI\Draw\Text\Layout(sprintf(
			"Press space bar to play ...",
			$this->level,
			$this->score
		), new Font(new Descriptor("arial", 12)), $size->width);

		$layout->setColor(0x000000FF);

		$pen->write(new Point(20, 10), $layout);
	}

	private function drawScore(Pen $pen, Size $size) {
		$layout = new UI\Draw\Text\Layout(sprintf(
			"Level: %d Score: %d",
			$this->level,
			$this->score
		), new Font(new Descriptor("arial", 12)), $size->width);

		$layout->setColor(0x000000FF);
	
		$pen->write(new Point(20, 10), $layout);
	}
	
	private $snake;
	private $food;
	private $direction = Key::Right;
	private $level = 1;
	private $score = 0;
	private $pause = true;
	private $run = 0;
	private $colors = [];

	private static $availableColors = [
		0x8F3AA4FF,
		0xCD116AFF,
		0x119ACDFF,
		0x65CD11FF,
		0xE64C00FF,
		0xE2B500FF
	];
	private static $nextColor = 0;
};

$animator = new class ($snake) extends Executor {

	public function __construct(Area $area) {
		$this->area = $area;

		/* construct executor with infinite timeout */
		parent::__construct();
	}

	protected function onExecute() {
		$this->area->redraw();
	}
};

$win->addExecutor($animator);

$snake->setExecutor($animator);

$win->show();

UI\run();

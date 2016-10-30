<?php
use UI\App;
use UI\Window;
use UI\Point;
use UI\Size;
use UI\Area;
use UI\Key;

use UI\Controls\Box;

use UI\Draw\Pen;
use UI\Draw\Brush;
use UI\Draw\Path;
use UI\Draw\Color;
use UI\Draw\Stroke;
use UI\Draw\Matrix;

use UI\Draw\Text\Font;
use UI\Draw\Text\Font\Descriptor;

$app = new class extends App {

	public function setGame(Area $game) {
		$this->game = $game;
	}

	protected function onTick() {
		if ($this->game) {
			$this->game->redraw();
		}
	}
};

$win = new Window($app, "Snake", new Size(640, 480), false);
$box = new Box(Box::Vertical);
$win->add($box);

$app->setGame(new class($box) extends Area{

	public function __construct(Box $box) {
		$this->newSnake();

		$box->append($this, true);
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
					}
				break;
			}
		}
	}

	protected function onDraw(Pen $pen, Size $size, Point $clip, Size $clipSize) {
		$zero = new Point(0, 0);
		$frame = $zero + 40;
		$frameSize = $size - 80;

		$path = new Path(Path::Winding);
		$path->addRectangle($zero, $size);
		$path->end();

		$brush = new Brush(0xf5f5f5ff);

		$pen->fill($path, $brush);

		$stroke = new Stroke();	
		$brush->setColor(0x000000FF);
		$pen->stroke($path, $brush, $stroke);

		$path = new Path(Path::Winding);
		$path->addRectangle($frame, $frameSize);
		$path->end();

		$pen->stroke($path, $brush, $stroke);

		$matrix = new Matrix();
		$matrix->translate($frame);

		$pen->transform($matrix);

		if (!$this->food) {
			$this->newFood($frameSize);
		}

		if (!$this->pause && ($run = microtime(true)) - $this->run > 0.1 / $this->level * 2) {
			$this->run = $run;

			$next = clone $this->snake[0];

			switch ($this->direction) {
				case Key::Right: $next->x++; break;
				case Key::Left:  $next->x--; break;
				case Key::Up: $next->y--; break;
				case Key::Down: $next->y++; break;
			}

			if ($next->x < 0 || $next->x >= ($frameSize->width)/10 || 
				$next->y < 0 || $next->y >= ($frameSize->height)/10) {
				$this->newSnake();
				$this->newFood($frameSize);

				foreach ($this->snake as $body) {
					$this->newCell($pen, $body);
				}

				$this->newCell($pen, $this->food);

				$this->pause = true;
				$this->direction = Key::Right;
				$this->score = 0;
				$this->level = 1;
				return;
			}

			if ($this->food == $next) {
				$tail = $next;
				$this->newFood($frameSize);
				$this->score += 10;
				$this->level = ceil($this->score / 100);
			} else {
				$tail = array_pop($this->snake);
				$tail->x = $next->x;
				$tail->y = $next->y;
			}

			array_unshift($this->snake, $tail);
		}

		foreach ($this->snake as $body) {
			$this->newCell($pen, $body);
		}
		
		$this->newCell($pen, $this->food);

		$matrix = new Matrix();
		$matrix->translate($zero - 40);
		$pen->transform($matrix);

		if ($this->pause) {
			$this->drawPause($pen, $size);
		} else $this->drawScore($pen, $size);
	}

	private function newSnake() {
		$this->snake = [];
		for ($i = 0; $i < 5; $i++)
			$this->snake[$i] = new Point($i, 0);
	}

	private function newFood(Size $size) {
		$this->food = new Point(
			floor(mt_rand(40, ($size->width ) - 10) / 10), 
			floor(mt_rand(40, ($size->height) - 10) / 10));
	}

	private function newCell(Pen $pen, Point $point) {
		$path = new Path(Path::Winding);
		$path->addRectangle($point * 10, new Size(10, 10));
		$path->end();

		$brush = new Brush(0x0000FFFF);
		$pen->fill($path, $brush);
		
		$stroke = new Stroke();
		$stroke->setThickness(2);
		$brush->setColor(0x000000FF);

		$pen->stroke($path, $brush, $stroke);
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
});

$win->show();

do {
	$app->run(App::Loop);
} while($win->isVisible());

$app->quit();

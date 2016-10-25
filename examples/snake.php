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

				case Key::Escape:
					$this->pause = !$this->pause;
				break;
			}
		}
	}

	protected function onDraw(Pen $pen, Size $size, Point $clip, Size $clipSize) {
		$path = new Path(Path::Winding);
		$path->addRectangle(new Point(0, 0), $size);
		$path->end();

		$brush = new Brush(Brush::Solid, new Color(0xFFFFFF, 1));
		$pen->fill($path, $brush);

		$stroke = new Stroke();	
		$brush->setColor(new Color(0, 1));
		$pen->stroke($path, $brush, $stroke);

		if (!$this->food) {
			$this->newFood($size);
		}

		if (!$this->pause && ($run = microtime(true)) - $this->run > 0.1 / $this->level) {
			$this->run = $run;

			$next = clone $this->snake[0];

			switch ($this->direction) {
				case Key::Right: $next->x++; break;
				case Key::Left:  $next->x--; break;
				case Key::Up: $next->y--; break;
				case Key::Down: $next->y++; break;
			}

			if ($next->x < 0 || $next->x >= $size->width/10 || 
				$next->y < 0 || $next->y >= $size->height/10) {
				$this->newSnake();
				$this->newFood($size);

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
				$this->newFood($size);
				$this->score++;
				$this->level = ceil($this->score / 10);
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

		$this->newScoreBoard($pen, $size, $clip, $clipSize);
	}

	private function newSnake() {
		$this->snake = [];
		for ($i = 0; $i < 5; $i++)
			$this->snake[$i] = new Point($i, 0);
	}

	private function newFood(Size $size) {
		$this->food = new Point(
			floor(mt_rand(0, $size->width - 10) / 10), 
			floor(mt_rand(0, $size->height - 10) / 10));
	}

	private function newCell(Pen $pen, Point $point) {
		$path = new Path(Path::Winding);
		$path->addRectangle(
			$point->x * 10,
			new Size(10, 10));
		$path->end();

		$brush = new Brush(Brush::Solid, new Color(0x0000FF, 1));
		$pen->fill($path, $brush);
		
		$stroke = new Stroke();
		$stroke->setThickness(2);
		$brush->setColor(new Color(0x000000, 1));

		$pen->stroke($path, $brush, $stroke);
	}

	private function newScoreBoard(Pen $pen, Size $size, Point $clip, Size $clipSize) {

		$layout = new UI\Draw\Text\Layout(sprintf(
			"Level: %d Score: %d",
			$this->level,
			$this->score
		), new Font(new Descriptor("arial", 12)), $size->width);

		$layout->setColor(new Color(0x000000, 1));
	
		$pen->write(new Point(20, $size->height - 20), $layout);
	}
	
	private $pause = true;
	private $snake;
	private $food;
	private $direction = Key::Right;
	private $level = 1;
	private $score = 0;
});

$win->show();

do {
	$app->run(App::Loop | App::Wait);
} while($win->isVisible());

$app->quit();

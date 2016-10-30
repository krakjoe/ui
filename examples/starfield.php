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

$app = new class extends App {
	public function setStars(Area $stars) {
		$this->stars = $stars;

	}

	protected function onTick() {
		$this->stars->redraw();	
	}

	private $stars;
};

$win = new Window($app, "Starfield", new Size(640, 480), false);
$box = new Box(Box::Vertical);
$win->add($box);

$font = new UI\Draw\Text\Font(
	new UI\Draw\Text\Font\Descriptor("arial", 12)			
);

$app->setStars(new class($box, 1024, 64, $font) extends Area {

	protected function onKey(string $key, int $ext, int $flags) {
		if ($flags & Area::Down) {
			switch ($ext) {
				case Key::Up: if ($this->velocity < 40) {
					$this->velocity++;;
				} break;

				case Key::Down: if ($this->velocity) {
					$this->velocity--;
				} break;
			}
		}
	}

	protected function onDraw(UI\Draw\Pen $pen, UI\Size $size, UI\Point $clip, UI\Size $clipSize) {
		$hSize = $size / 2;

		$path = new Path();
		$path->addRectangle(Point::at(0), $size);
		$path->end();
		$pen->fill($path, new Brush(0x000000FF));

		foreach ($this->stars as $idx => &$star) {
			$star[1] -= $this->velocity / 10;

			if ($star[1] <= 0) {
				$star[0]->x = mt_rand(-25, 25);
				$star[0]->y = mt_rand(-25, 25);
				$star[1] = $this->depth;
			}

			$pos = $star[0] * (128 / $star[1]) + $hSize;

			if ($pos->x >= 0 && $pos->x <= $size->width && $pos->y >= 0 && $pos->y <= $size->height) {
				$starSize = (1 - $star[1] / 32) * 5;

				$path = new Path();
				if (PHP_OS == "WINNT") {
					$path->addRectangle($pos, new Size($starSize, $starSize));
				} else {
					$path->newFigureWithArc($pos, $starSize/2, 0, M_PI*2, 0);
				}
				$path->end();

				$color = new Color();
				$color->r = $starSize;
				$color->g = $starSize;
				$color->b = $starSize;
				
				if ($star[2] && $star[3]++ % 3 == 0) {
					$color->a = mt_rand(0,10) / 10;
				}

				$pen->fill($path, new Brush($color));
			}
		}

		$this->writeRenderSpeed($pen, $size);
	}

	private function writeRenderSpeed(UI\Draw\Pen $pen, UI\Size $size) {
		$now = time();
		@$this->frames[$now]++;

		$layout = new UI\Draw\Text\Layout(sprintf(
			"%d fps",
			isset($this->frames[$now - 1]) ? 
				$this->frames[$now-1] : $this->frames[$now]
		), $this->font, $size->width);

		$layout->setColor(0xFFFFFFFF);
	
		$pen->write(new Point(20, 20), $layout);

		unset($this->frames[$now-2]);
	}

	public function __construct($box, $max, $depth, $font, $velocity = 2) {
		$this->box = $box;
		$this->max = $max;
		$this->depth = $depth;
		$this->font = $font;
		$this->velocity = $velocity;

		for ($i = 0; $i < $this->max; $i++) {
			$this->stars[] = [
				new Point(mt_rand(-25, 25), mt_rand(-25, 25)),
				mt_rand(1, $this->depth),
				mt_rand(0, 1),
				0
			];
		}

		$this->box->append($this, true);
	}
});

$win->show();

do {
	$app->run(App::Loop);
} while($win->isVisible());

$app->quit();
?>

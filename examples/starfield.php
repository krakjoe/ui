<?php
use UI\App;
use UI\Window;
use UI\Point;
use UI\Size;
use UI\Area;
use UI\Controls\Box;
use UI\Draw\Pen;
use UI\Draw\Brush;
use UI\Draw\Path;
use UI\Draw\Color;

$app = new App();
$win = new Window($app, "Starfield", new Size(640, 480), false);
$box = new Box(Box::Vertical);
$win->add($box);
$stars = new class(512, 32) extends Area {

	public function onDraw(UI\Draw\Pen $pen, UI\Size $size, UI\Point $clip, UI\Size $clipSize) {
		$hWidth = $size->width / 2;
		$hHeight = $size->height / 2;
		
		$path = new Path(Path::Winding);
		$path->addRectangle(
			new Point(0, 0), $size);
		$path->end();
		
		$pen->fill($path, new Brush(Brush::Solid, new Color(0, 1)));

		foreach ($this->stars as $idx => &$star) {
			$star[1] -= 0.2;

			if ($star[1] <= 0) {
				$star[0]->setX(mt_rand(-25, 25));
				$star[0]->setY(mt_rand(-25, 25));
				$star[1] = $this->depth;
			}

			$k = 128 / $star[1];
			$px = $star[0]->getX() * $k + $hWidth;
			$py = $star[0]->getY() * $k + $hHeight;
			
			if ($px >= 0 && $px <= $size->width && $py >= 0 && $py <= $size->height) {
				$starSize = (1 - $star[1] / 32) * 5;

				$path = new Path(Path::Winding);
				$path->addRectangle(new Point($px, $py), new Size($starSize, $starSize));
				$path->end();

				$color = new Color(0, 1);
				$color->setChannel(Color::Red, $starSize);
				$color->setChannel(Color::Green, $starSize);
				$color->setChannel(Color::Blue, $starSize);

				$pen->fill($path, new Brush(Brush::Solid, $color));
			}
		}
	}

	public function __construct($max, $depth) {
		$this->max = $max;
		$this->depth = $depth;
		for ($i = 0; $i < $this->max; $i++) {
			$this->stars[] = [
				new Point(mt_rand(-25, 25), mt_rand(-25, 25)),
				mt_rand(1, $this->depth)
			];
		}
	}
};
$box->append($stars, true);

$win->show();

do {
	$app->run(true, false);

	$stars->redraw();
} while($win->isVisible());

$app->quit();
?>

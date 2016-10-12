/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe                                                      |
  +----------------------------------------------------------------------+
*/
#ifndef HAVE_PHP_UI_STROKE_H
#define HAVE_PHP_UI_STROKE_H

zend_class_entry *uiDrawStroke_ce;

typedef struct _php_ui_stroke_t {
	uiDrawStrokeParams s;
	zend_object std;
} php_ui_stroke_t;

#define php_ui_stroke_from(o) ((php_ui_stroke_t*) ((char*) o - XtOffsetOf(php_ui_stroke_t, std)))
#define php_ui_stroke_fetch(z) php_ui_stroke_from(Z_OBJ_P(z))

#define PHP_UI_STROKE_CAP_FLAT uiDrawLineCapFlat
#define PHP_UI_STROKE_CAP_ROUND uiDrawLineCapRound
#define PHP_UI_STROKE_CAP_SQUARE uiDrawLineCapSquare

#define PHP_UI_STROKE_JOIN_MITER uiDrawLineJoinMiter
#define PHP_UI_STROKE_JOIN_ROUND uiDrawLineJoinRound
#define PHP_UI_STROKE_JOIN_BEVEL uiDrawLineJoinBevel

PHP_MINIT_FUNCTION(UI_DrawStroke);
#endif

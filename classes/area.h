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
#ifndef HAVE_PHP_UI_AREA_H
#define HAVE_PHP_UI_AREA_H

zend_class_entry *uiArea_ce;

typedef struct _php_ui_area_t {
	uiArea *a;
	zval draw;
	zval mouse;
	zval key;
	uiAreaHandler h;
	zend_object std;
} php_ui_area_t;

#define php_ui_area_from(o) ((php_ui_area_t*) ((char*) (o) - XtOffsetOf(php_ui_area_t, std)))
#define php_ui_area_fetch(z) php_ui_area_from(Z_OBJ_P(z))

#define PHP_UI_AREA_HANDLER_OFFSET ((sizeof(zval) * 3) + sizeof(void*))

#define PHP_UI_AREA_CTRL	1<<0
#define PHP_UI_AREA_ALT		1<<1
#define PHP_UI_AREA_SHIFT	1<<2
#define PHP_UI_AREA_SUPER	1<<3
#define PHP_UI_AREA_UP		1<<4
#define PHP_UI_AREA_DOWN	1<<5

#define php_ui_area_from_handler(a) (php_ui_area_t*) ((char *) (a) - XtOffsetOf(php_ui_area_t, h))

PHP_MINIT_FUNCTION(UI_Area);
#endif

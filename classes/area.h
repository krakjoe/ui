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
	zend_object std;
	zval draw;
	zval mouse;
	zval key;
	uiAreaHandler h;
} php_ui_area_t;

#define php_ui_area_from(o) ((php_ui_area_t*) ((char*) (o) - XtOffsetOf(php_ui_area_t, std)))
#define php_ui_area_fetch(z) php_ui_area_from(Z_OBJ_P(z))

#define PHP_UI_AREA_HANDLER_OFFSET ((sizeof(zval) * 3) + sizeof(zend_object) + sizeof(void*))

#define php_ui_area_from_handler(h) (php_ui_area_t*) ((char *) (h) - PHP_UI_AREA_HANDLER_OFFSET)

PHP_MINIT_FUNCTION(UI_Area);
#endif

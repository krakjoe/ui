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
#ifndef HAVE_PHP_UI_WINDOW_H
#define HAVE_PHP_UI_WINDOW_H

extern zend_class_entry *uiWindow_ce;

typedef struct _php_ui_window_t {
	uiWindow *w;
	zend_object *parent;
	HashTable *controls;
	struct php_ui_window_closing_t {
		zend_fcall_info fci;
		zend_fcall_info_cache fcc;
	} closing;
	struct php_ui_window_resized_t {
		zend_fcall_info fci;
		zend_fcall_info_cache fcc;
	} resized;
	struct php_ui_window_uncaught_t {
		zend_fcall_info fci;
		zend_fcall_info_cache fcc;
	} uncaught;
	zend_object std;
} php_ui_window_t;

#define php_ui_window_from(o) ((php_ui_window_t*) ((char*) o - XtOffsetOf(php_ui_window_t, std)))
#define php_ui_window_fetch(z) php_ui_window_from(Z_OBJ_P(z))

#define PHP_UI_WINDOW_MESSAGEBOX_INFO 1
#define PHP_UI_WINDOW_MESSAGEBOX_ERROR 2

zval *php_ui_window_construct(zval *object, uiWindow *w);

PHP_MINIT_FUNCTION(UI_Window);
#endif

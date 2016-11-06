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
#ifndef HAVE_PHP_UI_ITEM_H
#define HAVE_PHP_UI_ITEM_H

extern zend_class_entry *uiItem_ce;

typedef struct _php_ui_item_t {
	uiMenuItem *i;
	struct php_ui_item_click_t {
		zend_fcall_info fci;
		zend_fcall_info_cache fcc;
	} click;
	zend_object std;
} php_ui_item_t;

#define php_ui_item_from(o) ((php_ui_item_t*) ((char*) o - XtOffsetOf(php_ui_item_t, std)))
#define php_ui_item_fetch(z) php_ui_item_from(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(UI_MenuItem);
#endif

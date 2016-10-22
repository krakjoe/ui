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
#ifndef HAVE_PHP_UI_SLIDER
#define HAVE_PHP_UI_SLIDER
#include <ui.h>

#include "php.h"

#include <classes/control.h>
#include <classes/slider.h>

zend_object_handlers php_ui_slider_handlers;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);

zend_object* php_ui_slider_create(zend_class_entry *ce) {
	php_ui_slider_t *slider = 
		(php_ui_slider_t*) ecalloc(1, sizeof(php_ui_slider_t) + zend_object_properties_size(ce));

	zend_object_std_init(&slider->std, ce);

	object_properties_init(&slider->std, ce);

	slider->std.handlers = &php_ui_slider_handlers;

	php_ui_set_call(&slider->std, ZEND_STRL("onchange"), &slider->change.fci, &slider->change.fcc);

	return &slider->std;
}

void php_ui_slider_change_handler(uiSlider *m,  void *_slider) {
	php_ui_slider_t *slider = (php_ui_slider_t*) _slider;

	if (slider->change.fci.size) {
		zval rv;

		ZVAL_UNDEF(&rv);

		slider->change.fci.retval = &rv;

		if (zend_call_function(&slider->change.fci, &slider->change.fcc) != SUCCESS) {
			return;
		}

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}
	}
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_slider_construct_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Slider Slider::__construct(int min, int max) */
PHP_METHOD(Slider, __construct) 
{
	php_ui_slider_t *slider = php_ui_slider_fetch(getThis());
	zend_long min = 0, max = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ll", &min, &max) != SUCCESS) {
		return;
	}

	slider->s = uiNewSlider((int) min, (int) max);

	uiSliderOnChanged(slider->s, php_ui_slider_change_handler, slider);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_slider_set_value_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Slider::setValue(int value) */
PHP_METHOD(Slider, setValue) 
{
	php_ui_slider_t *slider = php_ui_slider_fetch(getThis());
	zend_long value = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &value) != SUCCESS) {
		return;
	}

	uiSliderSetValue(slider->s, value);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_slider_get_value_info, 0, 0, IS_LONG, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto string Slider::getValue(void) */
PHP_METHOD(Slider, getValue) 
{
	php_ui_slider_t *slider = php_ui_slider_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(uiSliderValue(slider->s));	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_slider_on_change_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Slider::onChange() */
PHP_METHOD(Slider, onChange)
{
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_slider_methods[] = {
	PHP_ME(Slider, __construct, php_ui_slider_construct_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Slider, setValue,    php_ui_slider_set_value_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Slider, getValue,    php_ui_slider_get_value_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Slider, onChange,    php_ui_slider_on_change_info,  ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Slider) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Controls", "Slider", php_ui_slider_methods);

	uiSlider_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiSlider_ce->create_object = php_ui_slider_create;

	memcpy(&php_ui_slider_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_slider_handlers.offset = XtOffsetOf(php_ui_slider_t, std);

	return SUCCESS;
} /* }}} */
#endif

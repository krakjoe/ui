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
#ifndef HAVE_PHP_UI_LABEL
#define HAVE_PHP_UI_LABEL
#include <ui.h>

#include "php.h"

#include <classes/size.h>

zend_object_handlers php_ui_size_handlers;

zend_class_entry *uiSize_ce;

zval *php_ui_size_construct(zval *size, double width, double height)
{
	php_ui_size_t *s;

	object_init_ex(size, uiSize_ce);

	s = php_ui_size_fetch(size);

	s->width = width;
	s->height = height;

	return size;
}

zend_object* php_ui_size_create(zend_class_entry *ce) {
	php_ui_size_t *size = 
		(php_ui_size_t*) ecalloc(1, sizeof(php_ui_size_t) + zend_object_properties_size(ce));

	zend_object_std_init(&size->std, ce);

	object_properties_init(&size->std, ce);

	size->std.handlers = &php_ui_size_handlers;

	return &size->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_size_construct_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Size Size::__construct(double width, double height) */
PHP_METHOD(Size, __construct) 
{
	php_ui_size_t *size = php_ui_size_fetch(getThis());
	double width = 0, height = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dd", &width, &height) != SUCCESS) {
		return;
	}

	size->width = width;
	size->height = height;
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_size_get_size_info, 0, 0, IS_DOUBLE, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto double Size::getWidth(void) */
PHP_METHOD(Size, getWidth) 
{
	php_ui_size_t *size = php_ui_size_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(size->width);	
} /* }}} */

/* {{{ proto double Size::getHeight(void) */
PHP_METHOD(Size, getHeight) 
{
	php_ui_size_t *size = php_ui_size_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(size->height);	
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_size_set_size_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, size, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Size::setWidth(double size) */
PHP_METHOD(Size, setWidth)
{
	php_ui_size_t *size = php_ui_size_fetch(getThis());
	double width = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "d", &width) != SUCCESS) {
		return;
	}

	size->width = width;
} /* }}} */

/* {{{ proto void Size::setHeight(double size) */
PHP_METHOD(Size, setHeight)
{
	php_ui_size_t *size = php_ui_size_fetch(getThis());
	double height = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "d", &height) != SUCCESS) {
		return;
	}

	size->height = height;
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_size_methods[] = {
	PHP_ME(Size, __construct,     php_ui_size_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(Size, getWidth,        php_ui_size_get_size_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Size, getHeight,       php_ui_size_get_size_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Size, setWidth,        php_ui_size_set_size_info,  ZEND_ACC_PUBLIC)
	PHP_ME(Size, setHeight,       php_ui_size_set_size_info,  ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
static int php_ui_size_operate(zend_uchar opcode, zval *result, zval *op1, zval *op2) {
	php_ui_size_t *size = php_ui_size_fetch(op1);

#define php_ui_size_do_operation(operator) do { \
	if (Z_TYPE_P(op2) == IS_OBJECT && instanceof_function(Z_OBJCE_P(op2), uiSize_ce)) {  \
		php_ui_size_t *operand = php_ui_size_fetch(op2);                                 \
		php_ui_size_t *retval;                                                           \
						                                                                 \
		object_init_ex(result, uiSize_ce);                                               \
                                                                                         \
		retval = php_ui_size_fetch(result);                                             \
		retval->width = size->width operator operand->width;                             \
		retval->height = size->height operator operand->height;                          \
                                                                                         \
		return SUCCESS;                                                                  \
	}                                                                                    \
                                                                                         \
	if (Z_TYPE_P(op2) == IS_LONG || Z_TYPE_P(op2) == IS_DOUBLE) {                        \
		php_ui_size_t *retval;                                                           \
                                                                                         \
		object_init_ex(result, uiSize_ce);                                               \
		                                                                                 \
		retval = php_ui_size_fetch(result);                                              \
		retval->width = size->width operator zval_get_double(op2);                       \
		retval->height = size->height operator zval_get_double(op2);                     \
                                                                                         \
		return SUCCESS;                                                                  \
	}                                                                                    \
} while(0)

	switch (opcode) {
		case ZEND_MUL:
			php_ui_size_do_operation(*);
		break;

		case ZEND_DIV:
			php_ui_size_do_operation(/);
		break;

		case ZEND_SUB:
			php_ui_size_do_operation(-);
		break;

		case ZEND_ADD:
			php_ui_size_do_operation(+);
		break;
	}

#undef php_ui_size_do_operation

	return FAILURE;
} /* }}} */

/* {{{ */
static zend_object* php_ui_size_clone(zval *o) {
	php_ui_size_t *object = php_ui_size_fetch(o);

	zend_object *cloned = 
		php_ui_size_create(object->std.ce);

	php_ui_size_t *clone = php_ui_size_from(cloned);

	clone->width = object->width;
	clone->height = object->height;

	return cloned;	
} /* }}} */

/* {{{ */
static int php_ui_size_compare(zval *op1, zval *op2) {
	php_ui_size_t *l = php_ui_size_fetch(op1);
	php_ui_size_t *r = php_ui_size_fetch(op2);

	return l->width == r->width && l->height == r->height ? SUCCESS : FAILURE;
} /* }}} */

/* {{{ */
static zval* php_ui_size_read(zval *object, zval *member, int type, void **cache, zval *rv) {
	php_ui_size_t *size = php_ui_size_fetch(object);

	if (Z_TYPE_P(member) != IS_STRING) {
		return &EG(uninitialized_zval);
	}

	if (type == BP_VAR_RW || type == BP_VAR_W) {
		zend_throw_exception_ex(NULL, 0, 
			"Failed to fetch reference to %s, not allowed", Z_STRVAL_P(member));
		return &EG(uninitialized_zval);	
	}

	if (zend_string_equals_literal_ci(Z_STR_P(member), "width")) {
		ZVAL_DOUBLE(rv, size->width);
		return rv;
	}

	if (zend_string_equals_literal_ci(Z_STR_P(member), "height")) {
		ZVAL_DOUBLE(rv, size->height);
		return rv;
	}

	zend_throw_exception_ex(NULL, 0,
		"Failed to fetch %s, does not exist", Z_STRVAL_P(member));

	return &EG(uninitialized_zval);
} /* }}} */

/* {{{ */
zval* php_ui_size_noref(zval *object, zval *member, int type, void **cache) {
	return NULL;
} /* }}} */

/* {{{ */
void php_ui_size_write(zval *object, zval *member, zval *value, void **cache) {
	php_ui_size_t *size = php_ui_size_fetch(object);

	if (Z_TYPE_P(member) != IS_STRING) {
		return;
	}

	if (zend_string_equals_literal_ci(Z_STR_P(member), "width")) {
		size->width = zval_get_double(value);
		return;
	}

	if (zend_string_equals_literal_ci(Z_STR_P(member), "height")) {
		size->height = zval_get_double(value);
		return;
	}

	zend_throw_exception_ex(NULL, 0,
		"Failed to write %s, does not exist", Z_STRVAL_P(member));
} /* }}} */

/* {{{ */
HashTable* php_ui_size_debug(zval *object, int *is_temp) {
	php_ui_size_t *size = php_ui_size_fetch(object);
	zval debug;

	array_init(&debug);

	add_assoc_double(&debug, "width", size->width);
	add_assoc_double(&debug, "height", size->height);

	*is_temp = 1;

	return Z_ARRVAL(debug);
} /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Size) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Size", php_ui_size_methods);

	uiSize_ce = zend_register_internal_class(&ce);
	uiSize_ce->create_object = php_ui_size_create;
	uiSize_ce->ce_flags |= ZEND_ACC_FINAL;

	memcpy(&php_ui_size_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_size_handlers.offset = XtOffsetOf(php_ui_size_t, std);
	php_ui_size_handlers.do_operation = php_ui_size_operate;
	php_ui_size_handlers.clone_obj = php_ui_size_clone;
	php_ui_size_handlers.compare_objects = php_ui_size_compare;
	php_ui_size_handlers.read_property = php_ui_size_read;
	php_ui_size_handlers.get_property_ptr_ptr = php_ui_size_noref;
	php_ui_size_handlers.write_property = php_ui_size_write;
	php_ui_size_handlers.get_debug_info = php_ui_size_debug;

	return SUCCESS;
} /* }}} */
#endif

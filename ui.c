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

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_ui.h"

#include <ui.h>

#ifdef HAVE_UI_X_THREADS
#	include <X11/Xlib.h>
#endif

#include <pthread.h>

#include <classes/exceptions.h>
#include <classes/executor.h>

#include <classes/point.h>
#include <classes/size.h>

#include <classes/control.h>
#include <classes/window.h>
#include <classes/tab.h>
#include <classes/form.h>
#include <classes/grid.h>
#include <classes/box.h>
#include <classes/check.h>
#include <classes/button.h>
#include <classes/cbutton.h>
#include <classes/label.h>
#include <classes/entry.h>
#include <classes/multi.h>
#include <classes/group.h>
#include <classes/spin.h>
#include <classes/slider.h>
#include <classes/progress.h>
#include <classes/separator.h>
#include <classes/combo.h>
#include <classes/ecombo.h>
#include <classes/radio.h>
#include <classes/picker.h>
#include <classes/menu.h>
#include <classes/item.h>

#include <classes/area.h>
#include <classes/pen.h>
#include <classes/path.h>
#include <classes/color.h>
#include <classes/brush.h>
#include <classes/stroke.h>
#include <classes/matrix.h>

#include <classes/descriptor.h>
#include <classes/font.h>
#include <classes/layout.h>

#define PHP_UI_LOOP	1<<0
#define PHP_UI_WAIT	1<<1

void php_ui_set_controls(zend_object *std, const char *name, size_t nlength, HashTable *table) {
	zval obj;
	zval *controls, stacked;

	ZVAL_OBJ(&obj, std);
	
	controls = zend_read_property(std->ce, &obj, name, nlength, 1, &stacked);

	if (!controls) {
		return;
	}

	ZVAL_ARR(controls, table);
}

void php_ui_set_parent(zval *child, zval *control) {
	zval *parent, stacked;

	parent = zend_read_property(Z_OBJCE_P(child), child, ZEND_STRL("parent"), 1, &stacked);

	if (!parent) {
		return;
	}

	ZVAL_COPY(parent, control);

	{
		php_ui_control_t *set = 
			php_ui_control_fetch(child);

		set->parent = Z_OBJ_P(control);
	}	
}

void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc) {
	zend_function *function = zend_hash_str_find_ptr(&object->ce->function_table, name, nlength);

	if (!function || function->type == ZEND_INTERNAL_FUNCTION) {
		return;
	}

	fci->size = sizeof(zend_fcall_info);
	fci->object = object;
	fci->no_separation = 1;

	fcc->initialized = 1;
	fcc->object = object;
	fcc->function_handler = function;
	fcc->calling_scope = object->ce;
	fcc->called_scope = object->ce;
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ui)
{
#ifdef HAVE_UI_X_THREADS
	XInitThreads();
#endif

	REGISTER_NS_LONG_CONSTANT("UI", "Loop", PHP_UI_LOOP, CONST_CS|CONST_PERSISTENT);
	REGISTER_NS_LONG_CONSTANT("UI", "Wait", PHP_UI_WAIT, CONST_CS|CONST_PERSISTENT);

	PHP_MINIT(UI_Exceptions)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Executor)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(UI_Point)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Size)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(UI_Control)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Window)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Form)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Grid)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Tab)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Box)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Check)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Button)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_ColorButton)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Label)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Entry)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Multi)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Group)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Spin)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Slider)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Progress)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Separator)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Combo)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_EditableCombo)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Radio)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Picker)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Menu)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_MenuItem)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(UI_Area)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawPen)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawPath)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawColor)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawBrush)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawStroke)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawMatrix)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(UI_DrawTextFontDescriptor)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawTextFont)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawTextLayout)(INIT_FUNC_ARGS_PASSTHRU);

	php_ui_control_finalize();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(ui)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(ui)
{
	uiInitOptions options;

	const char *initError = NULL;

#if defined(COMPILE_DL_UI) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	memset(&options, 0, sizeof(uiInitOptions));

	initError = uiInit(&options);

	if (initError) {
		zend_error(E_ERROR,
			"Cannot initialize libui: %s", initError);
		uiFreeInitError(initError);

		return FAILURE;
	}

	uiMainSteps();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(ui)
{
#if 0
	uiUninit();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(ui)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ui support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ nothing happening here ... ignore this ... */
typedef struct _php_ui_object_t {
	void *p;
	zend_object std;
} php_ui_object_t;

int php_ui_serialize(zval *object, unsigned char **buffer, size_t *buflen, zend_serialize_data *data) {
	void *address = ((char*) Z_OBJ_P(object) - XtOffsetOf(php_ui_object_t, std));

#ifdef _WIN64
	(*buflen) = snprintf(NULL, 0, ":%I64u:", (unsigned __int64) address);
#else
	(*buflen) = snprintf(NULL, 0, ":%lu:", (long unsigned int) address);
#endif
	(*buffer) = emalloc((*buflen) + 1);
	sprintf(
#ifdef _WIN64
		(char*) (*buffer), ":%I64u:", (unsigned __int64) address);
#else
		(char*) (*buffer), ":%lu:", (long unsigned int) address);
#endif
	(*buffer)[(*buflen)] = 0;

	return SUCCESS;
}

#define php_ui_object_from(o) ((php_ui_object_t*) ((char*) o - XtOffsetOf(php_ui_object_t, std)))
#define php_ui_object_fetch(o) php_ui_object_from(Z_OBJ_P(o))

int php_ui_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t blen, zend_unserialize_data *data) {
	php_ui_object_t *address = NULL;
	php_ui_object_t *o;

#ifdef _WIN64
	if (!sscanf((const char*) buffer, ":%I64u:", (unsigned __int64*)&address)) {
#else
	if (!sscanf((const char*) buffer, ":%lu:", (long unsigned int*)&address)) {
#endif
		return FAILURE;
	}

	object_init_ex(object, ce);

	o = php_ui_object_fetch(object);
	o->p = address->p;

	return SUCCESS;
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_run_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void UI\run([int flags = 0])*/
PHP_FUNCTION(UI_run)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|l", &flags) != SUCCESS) {
		return;
	}
	
	if (!(flags & PHP_UI_LOOP)) {
		uiMain();
		return;
	}
	
	uiMainStep((flags & PHP_UI_WAIT) == PHP_UI_WAIT);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_quit_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ */
PHP_FUNCTION(UI_quit)
{
	uiQuit();
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_draw_font_families_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto array UI\Draw\Text\fontFamilies(void) */
PHP_FUNCTION(fontFamilies)
{
	uiDrawFontFamilies* families;
	int familiy = 0;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	array_init(return_value);

	families = uiDrawListFontFamilies();
	for (familiy = 0; familiy < uiDrawFontFamiliesNumFamilies(families); familiy++) {
		add_next_index_string(return_value, uiDrawFontFamiliesFamily(families, familiy));
	}

	uiDrawFreeFontFamilies(families);
} /* }}} */

/* {{{ ui_functions[]
 */
const zend_function_entry ui_functions[] = {
	ZEND_NS_NAMED_FE("UI", run, zif_UI_run, php_ui_run_info)
	ZEND_NS_NAMED_FE("UI", quit, zif_UI_quit, php_ui_quit_info)
	ZEND_NS_FE("UI\\Draw\\Text\\Font", fontFamilies, php_ui_draw_font_families_info)
	PHP_FE_END
};
/* }}} */

/* {{{ ui_module_entry
 */
zend_module_entry ui_module_entry = {
	STANDARD_MODULE_HEADER,
	"ui",
	ui_functions,
	PHP_MINIT(ui),
	PHP_MSHUTDOWN(ui),
	PHP_RINIT(ui),
	PHP_RSHUTDOWN(ui),
	PHP_MINFO(ui),
	PHP_UI_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_UI
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(ui)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

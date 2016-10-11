PHP_ARG_WITH(ui, for ui support,
[  --with-ui             Include ui support])

if test "$PHP_UI" != "no"; then
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/ui.h"  # you most likely want to change this
  if test -r $PHP_UI/$SEARCH_FOR; then # path given as parameter
    UI_DIR=$PHP_UI
  else
    AC_MSG_CHECKING([for ui files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        UI_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi

  if test -z "$UI_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the ui distribution])
  fi

  # --with-ui -> add include path
  PHP_ADD_INCLUDE($UI_DIR/include)

  # --with-ui -> check for lib and symbol presence
  LIBNAME=ui # you may want to change this
  LIBSYMBOL=uiInit # you most likely want to change this 

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $UI_DIR/$PHP_LIBDIR, UI_SHARED_LIBADD)
    AC_DEFINE(HAVE_UILIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong libui version or libui not found])
  ],[
    -L$UI_DIR/$PHP_LIBDIR -lm
  ])
  dnl

  PHP_ADD_BUILD_DIR($ext_builddir/classes, 1)
  PHP_ADD_INCLUDE($ext_builddir)

  PHP_SUBST(UI_SHARED_LIBADD)

  PHP_NEW_EXTENSION(ui, ui.c classes/form.c classes/grid.c classes/window.c classes/control.c classes/tab.c classes/box.c classes/check.c classes/button.c classes/cbutton.c classes/label.c classes/entry.c classes/multi.c classes/group.c classes/spin.c classes/slider.c classes/progress.c classes/separator.c classes/combo.c classes/ecombo.c classes/radio.c classes/picker.c classes/menu.c classes/item.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi

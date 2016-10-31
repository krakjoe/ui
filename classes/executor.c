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
#ifndef HAVE_PHP_UI_EXECUTOR
#define HAVE_PHP_UI_EXECUTOR
#include <ui.h>

#include "php.h"

#include <pthread.h>

#include <classes/exceptions.h>
#include <classes/control.h>
#include <classes/executor.h>

zend_object_handlers php_ui_executor_handlers;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);

zend_class_entry *uiExecutor_ce;

static inline void php_ui_monitor_init(php_ui_monitor_t *monitor) {
	if (pthread_mutex_init(&monitor->m, NULL) != SUCCESS) {
		php_ui_exception("failed to initialize mutex for monitor, panic");
	}

	if (pthread_cond_init(&monitor->c, NULL) != SUCCESS) {
		php_ui_exception("failed to initialize condition for monitor, panic");
	}
}

static inline void php_ui_monitor_destroy(php_ui_monitor_t *monitor) {
	pthread_mutex_destroy(&monitor->m);
	pthread_cond_destroy(&monitor->c);
}

zend_object* php_ui_executor_create(zend_class_entry *ce) {
	php_ui_executor_t *executor = 
		(php_ui_executor_t*) ecalloc(1, sizeof(php_ui_executor_t) + zend_object_properties_size(ce));

	zend_object_std_init(&executor->std, ce);

	object_properties_init(&executor->std, ce);

	executor->std.handlers = &php_ui_executor_handlers;

	php_ui_monitor_init(&executor->monitors[0]);
	php_ui_monitor_init(&executor->monitors[1]);

	php_ui_set_call(&executor->std, ZEND_STRL("onexecute"), &executor->execute.fci, &executor->execute.fcc);

	return &executor->std;
}

void php_ui_executor_free(zend_object *o) {
	php_ui_executor_t *executor = php_ui_executor_from(o);

	if (pthread_mutex_lock(&executor->monitors[0].m) == SUCCESS) {
		executor->monitors[0].flag = 1;
		pthread_cond_signal(&executor->monitors[0].c);
		pthread_mutex_unlock(&executor->monitors[0].m);
	}
	pthread_join(executor->thread, NULL);

	php_ui_monitor_destroy(&executor->monitors[0]);
	php_ui_monitor_destroy(&executor->monitors[1]);

	zend_object_std_dtor(o);
}

void php_ui_executor_handler(void *o) {
    php_ui_executor_t *executor = (php_ui_executor_t *) o;
    zval rv;

    if (pthread_mutex_lock(&executor->monitors[1].m) != SUCCESS) {
            return;
    }

    if (!executor->execute.fci.size) {
            goto php_ui_executor_handler_leave;
    }

    ZVAL_UNDEF(&rv);
    executor->execute.fci.retval = &rv;

    if (zend_call_function(&executor->execute.fci, &executor->execute.fcc) != SUCCESS) {
            goto php_ui_executor_handler_leave;
    }

    if (Z_TYPE(rv) != IS_UNDEF) {
            zval_ptr_dtor(&rv);
    }

php_ui_executor_handler_leave:
    executor->monitors[1].flag = 1;

    pthread_cond_signal(&executor->monitors[1].c);
    pthread_mutex_unlock(&executor->monitors[1].m);
}

static inline void php_ui_executor_time_add(struct timespec *a, struct timespec *b, struct timespec *result) {
	result->tv_sec = a->tv_sec + b->tv_sec;
	result->tv_nsec = a->tv_nsec + b->tv_nsec;

	while (result->tv_nsec >= 1000000000L) {
		result->tv_sec++ ; 
		result->tv_nsec -= 1000000000L ;
	}
}

void* php_ui_executor_thread(void *arg) {
    php_ui_executor_t *executor = (php_ui_executor_t*) arg;
    struct timeval time;
    struct timespec spec;

    if (executor->gap) {
            if (gettimeofday(&time, NULL) != SUCCESS) {
                    pthread_exit(NULL);
            }

			spec.tv_sec = time.tv_sec;
			spec.tv_nsec = time.tv_usec * 1000;
    }

    if (pthread_mutex_lock(&executor->monitors[0].m) != SUCCESS) {
            pthread_exit(NULL);
    }

    while(!executor->monitors[0].flag) {
            if (executor->gap) {
					struct timespec next;

					next.tv_sec = 0;
					next.tv_nsec = executor->gap * 1000;

                    php_ui_executor_time_add(&spec, &next, &spec);

                    switch (pthread_cond_timedwait(&executor->monitors[0].c, &executor->monitors[0].m, &spec)) {
 						case SUCCESS:
							/* we were notified to wake up */
							continue;
					}
            } else {
                    switch (pthread_cond_wait(&executor->monitors[0].c, &executor->monitors[0].m)) {
						case SUCCESS:
							if (executor->gap) { /* new gap was set */
								gettimeofday(&time, NULL);

								spec.tv_sec = time.tv_sec;
								spec.tv_nsec = time.tv_usec * 1000;
								continue;
							}
                    }
            }

            pthread_mutex_lock(&executor->monitors[1].m);

            executor->monitors[1].flag = 0;

            uiQueueMain(php_ui_executor_handler, executor);

            while (!executor->monitors[1].flag) {
                    if (pthread_cond_wait(&executor->monitors[1].c, &executor->monitors[1].m) != SUCCESS) {
                            break;
                    }
            }

            pthread_mutex_unlock(&executor->monitors[1].m);
    }

    pthread_mutex_unlock(&executor->monitors[0].m);
    pthread_exit(NULL);
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_executor_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, gap, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Executor Executor::__construct(integer gap) */
PHP_METHOD(Executor, __construct) 
{
	php_ui_executor_t *executor = php_ui_executor_fetch(getThis());
	zend_long gap = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &gap) != SUCCESS) {
		return;
	}

	executor->gap = gap;

	if (pthread_create(&executor->thread, NULL, php_ui_executor_thread, executor) != SUCCESS) {
		php_ui_exception("failed to create executor thread, panic");
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_executor_set_gap_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, gap, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Executor::setGap(integer gap) */
PHP_METHOD(Executor, setGap)
{
	php_ui_executor_t *executor = php_ui_executor_fetch(getThis());
	zend_long gap = 0;
	zend_bool wakeup = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &gap) != SUCCESS) {
		return;
	}

	if (!executor->gap) {
		wakeup = 1;
	}

	executor->gap = gap;

	if (wakeup) {
		pthread_cond_signal(&executor->monitors[0].c);
	}
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_executor_get_gap_info, 0, 0, IS_LONG, NULL, 0)
ZEND_END_ARG_INFO();

/* {{{ proto integer Executor::getGap(void) */
PHP_METHOD(Executor, getGap)
{
	php_ui_executor_t *executor = php_ui_executor_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(executor->gap);
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_executor_stop_info, 0, 0, _IS_BOOL, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Executor::stop(void) */
PHP_METHOD(Executor, stop)
{
	php_ui_executor_t *executor = php_ui_executor_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (executor->monitors[0].flag) {
		RETURN_FALSE;
	}

	executor->monitors[0].flag = 1;

	RETURN_BOOL(pthread_cond_signal(&executor->monitors[0].c) == SUCCESS);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_executor_methods[] = {
	PHP_ME(Executor, __construct, php_ui_executor_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(Executor, setGap,     php_ui_executor_set_gap_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Executor, getGap,     php_ui_executor_get_gap_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Executor, stop,       php_ui_executor_stop_info,       ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Executor) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Executor", php_ui_executor_methods);

	uiExecutor_ce = zend_register_internal_class(&ce);
	uiExecutor_ce->create_object = php_ui_executor_create;

	memcpy(&php_ui_executor_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_executor_handlers.offset = XtOffsetOf(php_ui_executor_t, std);
	php_ui_executor_handlers.free_obj = php_ui_executor_free;

	return SUCCESS;
} /* }}} */
#endif

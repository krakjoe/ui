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

#ifdef _WIN32
#	define HAVE_STRUCT_TIMESPEC
#endif

#include <pthread.h>

#include <classes/exceptions.h>
#include <classes/control.h>
#include <classes/executor.h>

zend_object_handlers php_ui_executor_handlers;

zend_class_entry *uiExecutor_ce;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);
extern int php_ui_call(zend_fcall_info *fci, zend_fcall_info_cache *fcc);

#define php_ui_executor_has_interval(e)   ((e)->interval.tv_sec || (e)->interval.tv_nsec)
#define php_ui_executor_has_flag(e, m)    ((e)->monitors.m.flag)
#define php_ui_executor_set_flag(e, m, v) ((e)->monitors.m.flag = v)

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

	php_ui_monitor_init(&executor->monitors.main);
	php_ui_monitor_init(&executor->monitors.queue);

	php_ui_set_call(&executor->std, ZEND_STRL("onexecute"), &executor->execute.fci, &executor->execute.fcc);

	return &executor->std;
}

void php_ui_executor_free(zend_object *o) {
	php_ui_executor_t *executor = php_ui_executor_from(o);

	if (pthread_mutex_lock(&executor->monitors.main.m) == SUCCESS) {
		php_ui_executor_set_flag(executor, main, 1);
		pthread_cond_signal(&executor->monitors.main.c);
		pthread_mutex_unlock(&executor->monitors.main.m);
	}
	pthread_join(executor->thread, NULL);

	php_ui_monitor_destroy(&executor->monitors.main);
	php_ui_monitor_destroy(&executor->monitors.queue);

	zend_object_std_dtor(o);
}

void php_ui_executor_handler(void *o) {
    php_ui_executor_t *executor = (php_ui_executor_t *) o;
    zval rv;

    if (pthread_mutex_lock(&executor->monitors.queue.m) != SUCCESS) {
            return;
    }

    if (!executor->execute.fci.size) {
            goto php_ui_executor_handler_leave;
    }

    ZVAL_UNDEF(&rv);
    executor->execute.fci.retval = &rv;

    if (php_ui_call(&executor->execute.fci, &executor->execute.fcc) != SUCCESS) {
            goto php_ui_executor_handler_leave;
    }

    if (Z_TYPE(rv) != IS_UNDEF) {
            zval_ptr_dtor(&rv);
    }

php_ui_executor_handler_leave:
	php_ui_executor_set_flag(executor, queue, 1);

    pthread_cond_signal(&executor->monitors.queue.c);
    pthread_mutex_unlock(&executor->monitors.queue.m);
}

static inline void php_ui_executor_time_set(struct timespec *result, zend_long seconds, zend_long microseconds) {
	while (microseconds >= 1000000L) {
		seconds++;
		microseconds -= 1000000L;
	}

	result->tv_sec = seconds;
	result->tv_nsec = microseconds * 1000;
}

static inline void php_ui_executor_time_add(struct timespec *a, struct timespec *b, struct timespec *result) {
	result->tv_sec = a->tv_sec + b->tv_sec;
	result->tv_nsec = a->tv_nsec + b->tv_nsec;

	if (result->tv_nsec >= 1000000000L) {
		result->tv_sec++ ;
		result->tv_nsec -= 1000000000L ;
	}
}

void* php_ui_executor_thread(void *arg) {
    php_ui_executor_t *executor = (php_ui_executor_t*) arg;
    struct timeval time;
    struct timespec spec;

    if (php_ui_executor_has_interval(executor)) {
        if (gettimeofday(&time, NULL) != SUCCESS) {
                pthread_exit(NULL);
        }

		php_ui_executor_time_set(&spec, time.tv_sec, time.tv_usec);
    }

    if (pthread_mutex_lock(&executor->monitors.main.m) != SUCCESS) {
            pthread_exit(NULL);
    }

    while(!php_ui_executor_has_flag(executor, main)) {
        if (php_ui_executor_has_interval(executor)) {
            php_ui_executor_time_add(&spec, &executor->interval, &spec);

            switch (pthread_cond_timedwait(&executor->monitors.main.c, &executor->monitors.main.m, &spec)) {
				case SUCCESS:
					if (!php_ui_executor_has_interval(executor) ||  /* new interval was set */
						php_ui_executor_has_flag(executor, main)) { /* killed */
						continue;
					}
			}
        } else {
            switch (pthread_cond_wait(&executor->monitors.main.c, &executor->monitors.main.m)) {
				case SUCCESS:
					if (php_ui_executor_has_interval(executor) ||   /* new interval was set */
						php_ui_executor_has_flag(executor, main)) { /* killed */
						if (gettimeofday(&time, NULL) != SUCCESS) {
							break;
						}

						php_ui_executor_time_set(&spec, time.tv_sec, time.tv_usec);
						continue;
					}
            }
        }

        if (pthread_mutex_lock(&executor->monitors.queue.m)  != SUCCESS) {
			break;
		}

		php_ui_executor_set_flag(executor, queue, 0);

        uiQueueMain(php_ui_executor_handler, executor);

        while (!php_ui_executor_has_flag(executor, queue)) {
                if (pthread_cond_wait(&executor->monitors.queue.c, &executor->monitors.queue.m) != SUCCESS) {
                        break;
                }
        }

        pthread_mutex_unlock(&executor->monitors.queue.m);
    }

    pthread_mutex_unlock(&executor->monitors.main.m);
    pthread_exit(NULL);
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_executor_construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, seconds, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, microseconds, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ 
       proto Executor Executor::__construct()
       proto Executor Executor::__construct(integer microseconds)
       proto Executor Executor::__construct(integer seconds, integer microseconds) */
PHP_METHOD(Executor, __construct) 
{
	php_ui_executor_t *executor = php_ui_executor_fetch(getThis());
	zend_long seconds = 0;
	zend_long microseconds = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|ll", &seconds, &microseconds) != SUCCESS) {
		return;
	}

	if (ZEND_NUM_ARGS() == 1) {
		microseconds = seconds;
		seconds      = 0;
	}

	if (seconds < 0 || microseconds < 0) {
		php_ui_exception("seconds and microseconds are not allowed to be negative");
		return;
	}

	php_ui_executor_time_set(&executor->interval, seconds, microseconds);

	if (pthread_create(&executor->thread, NULL, php_ui_executor_thread, executor) != SUCCESS) {
		php_ui_exception("failed to create executor thread, panic");
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_executor_set_interval_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, seconds, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, microseconds, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void Executor::setInterval(integer microseconds)
       proto void Executor::setInterval(integer seconds, integer microseconds) */
PHP_METHOD(Executor, setInterval)
{
	php_ui_executor_t *executor = php_ui_executor_fetch(getThis());
	zend_long seconds = 0;
	zend_long microseconds = 0;
	zend_bool wakeup = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l|l", &seconds, &microseconds) != SUCCESS) {
		return;
	}

	if (ZEND_NUM_ARGS() == 1) {
		microseconds = seconds;
		seconds      = 0;
	}

	if (seconds < 0 || microseconds < 0) {
		php_ui_exception("seconds and microseconds are not allowed to be negative");
		return;
	}

	if (!php_ui_executor_has_interval(executor)) {
		wakeup = 1;
	}

	php_ui_executor_time_set(&executor->interval, seconds, microseconds);

	if (wakeup) {
		pthread_cond_signal(&executor->monitors.main.c);
	}
} /* }}} */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_executor_kill_info, 0, 0, _IS_BOOL, NULL, 0)
ZEND_END_ARG_INFO()

/* {{{ proto bool Executor::kill(void) */
PHP_METHOD(Executor, kill)
{
	php_ui_executor_t *executor = php_ui_executor_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (php_ui_executor_has_flag(executor, main)) {
		RETURN_FALSE;
	}

	php_ui_executor_set_flag(executor, main, 1);

	RETURN_BOOL(pthread_cond_signal(&executor->monitors.main.c) == SUCCESS);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_executor_execute_info, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Executor, onExecute) {}

/* {{{ */
const zend_function_entry php_ui_executor_methods[] = {
	PHP_ME(Executor, __construct,     php_ui_executor_construct_info,       ZEND_ACC_PUBLIC)
	PHP_ME(Executor, setInterval,     php_ui_executor_set_interval_info,    ZEND_ACC_PUBLIC)
	PHP_ME(Executor, kill,            php_ui_executor_kill_info,            ZEND_ACC_PUBLIC)
	PHP_ME(Executor, onExecute,       php_ui_executor_execute_info,         ZEND_ACC_PROTECTED|ZEND_ACC_ABSTRACT)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_Executor) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI", "Executor", php_ui_executor_methods);

	uiExecutor_ce = zend_register_internal_class(&ce);
	uiExecutor_ce->create_object = php_ui_executor_create;
	uiExecutor_ce->ce_flags |= ZEND_ACC_ABSTRACT;

	memcpy(&php_ui_executor_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_executor_handlers.offset = XtOffsetOf(php_ui_executor_t, std);
	php_ui_executor_handlers.free_obj = php_ui_executor_free;

	return SUCCESS;
} /* }}} */
#endif

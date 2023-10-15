/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */
 
#include <elog.h>


/*Include HeadFile*/
#include <stdio.h>
#include "usart.h"
 #include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


QueueHandle_t xElog_MutexSemaphore;

/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;
    /* add your code here */
    xElog_MutexSemaphore = xSemaphoreCreateMutex();
    if( NULL == xElog_MutexSemaphore){
       result = ELOG_SEM_FAIL;
    }
    return result;
    
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void) {

    /* add your code here */

}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
    /* add your code here */
        printf("%.*s",size,log);
    
}

/**
 * output lock
 */
void elog_port_output_lock(void) {

    if(NULL != xElog_MutexSemaphore){
        xSemaphoreTake(xElog_MutexSemaphore,portMAX_DELAY);
    }
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    
    if(NULL != xElog_MutexSemaphore){
        xSemaphoreGive(xElog_MutexSemaphore);
    }
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    
    static char cur_system_time[] = "";

    TickType_t tick = xTaskGetTickCount();
    tick = tick/portTICK_PERIOD_MS;
    snprintf(cur_system_time,12,"%lu",(unsigned long)tick);
    return cur_system_time;  
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {
    
    /* add your code here */
    return "";
    
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {
    
    /* add your code here */
    return "";//pcTaskGetName(xTaskGetCurrentTaskHandle());
    
}
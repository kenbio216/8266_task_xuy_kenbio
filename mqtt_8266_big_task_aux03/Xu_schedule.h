#ifndef _XU_SCHEDULE_H_
#define _XU_SCHEDULE_H_

#include <Arduino.h> 

void Scheduler_init(void); // 初始化任务调度器，必须放在setup中
void Scheduler_run(void);  // 任务调度器，必须放在loop中

#endif _XU_SCHEDULE_H_



#include "TaskManager.h"


TICK TaskManager::tick = 0;
u16 TaskManager::subTick = 0;
i16 TaskManager::subTickMAX = -1;
u8 TaskManager::subTickMod = -1;
Task* TaskManager::taskList[TASK_MAX];
Task* TaskManager::stackTask[TASK_STACK_MAX];
Task TaskManager::taskMemory[TASK_MAX];
u8 TaskManager::stackTaskSize=0;
//新しい割り込み処理を行います
void TaskManager::interrupt(void(*func)(void*),  u8 priority, void* param) {
    Task* newTask=nullptr;
    for(u8 i=0;i<TASK_MAX;i++){
        if(taskMemory[i].priority==0){
            newTask=&taskMemory[i];
            break;
        }
    }
    if(newTask== nullptr){
        //error 新しくTaskを開始出来なかった
        return;
    }
    newTask->func = func;
    newTask->param = param;
    newTask->priority = priority;
    newTask->startTick = tick;
    interrupt(newTask);


}
void TaskManager::interrupt(Task* newTask) {
    if (stackTask[stackTaskSize-1]->priority < newTask->priority) {
        /*
        todo
        実行中のメモリ不足が危惧される場合どうするべき？
        */
        //newTaskを優先させる
        stackTask[stackTaskSize]=newTask;
        stackTaskSize++;
        runTask();
    }
    else {
        //newTaskはtaskリストに入れる
        for (u8 i = 0; i < TASK_MAX; i++) {
            if (taskList[i] == nullptr) {
                taskList[i] = newTask;
                break;
            }
        }
    }
}


//stackTaskの一番上のタスクを実行する
void TaskManager::runTask() {
    //タスクを実行

    stackTask[stackTaskSize-1]->func(stackTask[stackTaskSize-1]->param);
    //タスクを終了
    //stackTaskの削除
    stackTask[stackTaskSize-1]->priority=0;
    stackTask[stackTaskSize-1]= nullptr;
    stackTaskSize--;
    checkTask();
}


ISR(TIMER0_COMPA_vect)  //タイマ割り込み
{
    if(TaskManager::subTickMAX>0){
        TaskManager::subTick++;
        if(TaskManager::subTick==TaskManager::subTickMAX){
            TaskManager::subTick=-1;
            OCR0A = TaskManager::subTickMod-1;

        }
        if(TaskManager::subTick==0) {
            TaskManager::update();
            OCR0A =255;
        }


    }else{
        TaskManager::update();

    }
}

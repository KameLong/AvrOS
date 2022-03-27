#pragma once
#include "def.h"
#include "avr/iom328p.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include <stdlib.h>


/**
 * このクラスはTIMER0を使用します。
 *
 */
#ifndef USE_TIMER0
#define USE_TIMER0
#else
#error TIMER0は既に使用されています(__FILE__ __LINE__)
#endif
typedef u32 TICK;
struct Task {
public:
    /**
    実行すべき関数
    引数と戻り値はポインター型で渡すこと
    */
    void(*func)(void*);
    /**
    関数に渡す引数ポインター
    */
    void* param;
    /**
    優先度
    この数字が高い方が優先度が高い
    priorityの値は1以上にすること
    */
    u8 priority = 0;
    /**
    開始するタイミング
    */
    TICK startTick = 0;
};


/**
Arduinoのタスク管理を行うクラス
静的クラスなので、コンストラクタ、デストラクタはなし
*/

class TaskManager {
public:
    TaskManager() = delete;

    ~TaskManager() = delete;

public:
    constexpr static u8 TASK_STACK_MAX = 8;

    /**
    最大タスク保持数
    */
    constexpr static u8 TASK_MAX = 16;
    /**
    tick間隔(INTERVAL * 0.1ms間隔で実行される)
    */
    constexpr static u16 INTERVAL = 1000;

    static Task *taskList[TASK_MAX];
    static Task *stackTask[TASK_STACK_MAX];
    static u8 stackTaskSize;

    /**
    現在のtick
    */
    static TICK tick;
    static u16 subTick;
    static i16 subTickMAX;
    static u8 subTickMod;



    static void init(){
        TCCR0A = 0b00000010; //標準動作
        u32 waitClock=F_CPU/10000*INTERVAL;


        if(waitClock<256){
            TCCR0B=0b00000001;
            OCR0A=waitClock-1;
        }else if(waitClock/8<256){
            TCCR0B=0b00000010;
            OCR0A=waitClock/8-1;
        }else if(waitClock/64<256){
        TCCR0B=0b00000011;
        OCR0A=waitClock/64-1;
        }else if(waitClock/256<256){
            TCCR0B=0b00000100;
            OCR0A=waitClock/256-1;
        }else if(waitClock/1024<256) {
            TCCR0B = 0b00000101;
            OCR0A = waitClock / 1024 - 1;
        }else {
            TCCR0B = 0b00000101;
            OCR0A = 255;
            subTickMAX=waitClock/1024/256;
            subTickMod=waitClock/1024%256;
        }
        TIMSK0 = 0b0000010;
        sei();
    }
    /**
    1tick進めます
    INTERVAL ミリ秒毎に呼び出してください。
    */
    static void update() {
        tick++;
        checkTask();
    }

    //新しい割り込み処理を行います
    static void interrupt(void(*func)(void *), u8 priority = 128, void *param = nullptr);

    static void interrupt(Task *);

    //時間差で割り込み処理を行います
    static void setTimeout(void(*func)(void *), TICK delay, u8 priority = 128, void *param = nullptr) {
        Task* newTask = (Task*)malloc(sizeof(Task));
        newTask->func = func;
        newTask->param = param;
        newTask->priority = priority;
        newTask->startTick = tick + delay;
        for (u8 i = 0; i < TASK_MAX; i++) {
            if (taskList[i] == nullptr) {
                taskList[i] = newTask;
                break;
            }
        }
    }

    //stackTaskの一番上のタスクを実行する
    static void runTask();

    //タスク一覧を確認し、現在実行しているものよりも優先度が高いものがあれば実行する
    static void checkTask() {
        if (stackTaskSize >= TASK_STACK_MAX) {
            //STACKの最大数を超えていたら、新規タスクの実行をブロックします。
            return;
        }
        u8 maxP = 0;
        i8 index = -1;
        for (int i = 0; i < TASK_MAX; i++) {
            if (taskList[i] != nullptr && (tick - taskList[i]->startTick) < 2000 && taskList[i]->priority > maxP) {
                maxP = taskList[i]->priority;
                index = i;
            }
        }
        //ここから割り込み禁止
        if (index >= 0 && taskList[index] != nullptr) {
            if (stackTaskSize == 0) {
                stackTask[stackTaskSize]=taskList[index];
                stackTaskSize++;
                taskList[index] = nullptr;
                //割り込み許可
                runTask();
            } else if (stackTask[stackTaskSize-1]->priority < maxP) {
                stackTask[stackTaskSize]=taskList[index];
                stackTaskSize++;
                taskList[index] = nullptr;
                //割り込み許可
                runTask();
            }
        }
        //割り込み許可


    }


};


#include "stdint.h"

#define TIMER_NUM   10


static softTimer  timer[TIMER_NUM];
static volatile uint32_t tickCnt = 0;   // 当前滴答计数，硬件定时器中断中进行滴答计数

//定时器状态
typedef enum tmrState {
 SOFT_TIMER_STOPPED = 0,  //停止
 SOFT_TIMER_RUNNING,      //运行
 SOFT_TIMER_TIMEOUT       //超时
}tmrState;

//定时器模式
typedef enum tmrMode {
 MODE_ONE_SHOT = 0,       //单次模式
 MODE_PERIODIC,           //周期模式
}tmrMode;

//回调函数类型
typedef void callback(void *argv, uint16_t argc);

//软件定时器结构体
typedef struct 
{
    uint8_t state;       //状态
    uint8_t mode;        //模式
    uint32_t match;      //到期时间
    uint32_t period;     //定时周期
    callback *cb;        //回调函数
    void *argv;          //回调函数参数
    uint16_t argc;       //回调函数参数个数
}softTimer;

//软件定时器初始化
void softTimer_Init(void)
{
    uint16_t i;

    for(i = 0; i < TIMER_NUM; i++)
    {
        timer[i].state = SOFT_TIMER_STOPPED;
        timer[i].mode = MODE_ONE_SHOT;
        timer[i].match = 0;
        timer[i].period = 0;
        timer[i].cb = NULL;
        timer[i].argv = NULL;
        timer[i].argc = 0;
    }
}

//获取当前滴答计数
uint32_t tickCnt_Get(void)
{
    return tickCnt;
}

//启动定时器
void softTimer_Start(uint16_t id, tmrMode mode, uint32_t delay, callback *cb, void *argv, uint16_t argc)
{
    assert_param(id < TIMER_NUM);
    assert_param(mode == MODE_ONE_SHOT || mode == MODE_PERIODIC);

    timer[id].match = tickCnt_Get() + delay;
    timer[id].period = delay;
    timer[id].state = SOFT_TIMER_RUNNING;
    timer[id].mode = mode;
    timer[id].cb = cb;
    timer[id].argv = argv;
    timer[id].argc = argc;
}

//定时器更新
void softTimer_Update(void)
{
    uint16_t i;

    for(i=0; i<TIMER_NUM; i++)
    {
    switch (timer[i].state) {
            case SOFT_TIMER_STOPPED:
      break;

    case SOFT_TIMER_RUNNING:
      if(timer[i].match <= tickCnt_Get()) {
        timer[i].state = SOFT_TIMER_TIMEOUT;
        timer[i].cb(timer[i].argv, timer[i].argc);       //执行回调函数
      }
      break;

    case SOFT_TIMER_TIMEOUT:
      if(timer[i].mode == MODE_ONE_SHOT) {
          timer[i].state = SOFT_TIMER_STOPPED;
      } else {
          timer[i].match = tickCnt_Get() + timer[i].period;
          timer[i].state = SOFT_TIMER_RUNNING;
      }
      break;

    default:
      printf("timer[%d] state error!\r\n", i);
      break;
        }
    }
}

//停止定时器
void softTimer_Stop(uint16_t id)
{
  assert_param(id < TIMER_NUM);
  timer[id].state = SOFT_TIMER_STOPPED;
}

//获取定时器状态
uint8_t softTimer_GetState(uint16_t id)
{
    return timer[id].state;
}

//测试用例
static uint8_t data[] = {1,2,3,4,5,6,7,8,9,0};

int main(void)
{
    USART1_Init(115200);
    TIM4_Init(TIME_BASE_MS);
    TIM4_NVIC_Config();
    LED_Init();

    printf("I just grabbed a spoon.\r\n");
    softTimer_Start(TMR_STRING_PRINT, MODE_ONE_SHOT, 1000, stringPrint, data, 5);
    softTimer_Start(TMR_TWINKLING, MODE_PERIODIC, 500, LED0_Twinkling, NULL, 0);
    softTimer_Start(TMR_DELAY_ON, MODE_ONE_SHOT, 3000, nop, NULL, 0);

    while(1) {
        softTimer_Update();
        if(softTimer_GetState(TMR_DELAY_ON) == SOFT_TIMER_TIMEOUT) {
            LED1_On();
        }
    }
}
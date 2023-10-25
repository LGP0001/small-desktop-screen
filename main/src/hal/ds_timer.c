/************************************************************
 * ds_timer.c : ESP32的硬件定时器处理模块
 * 
 * 此模块提供了ESP32的定时器初始化、中断处理等功能，支持触摸屏和UI更新的计时任务。
 * 
 ************************************************************/

// 标准输入输出头文件
# include <stdio.h>

// ESP32的相关头文件
# include "esp_types.h"
# include "driver/periph_ctrl.h"
# include "driver/timer.h"
# include "esp_log.h"

// FreeRTOS的任务调度和队列相关头文件
# include "freertos/FreeRTOS.h"
# include "freertos/task.h"
# include "freertos/queue.h"

// 与本模块相关的头文件
# include "ds_timer.h"
// # include "ds_system_data.h"
// # include "ds_ft6336.h"
// # include "ds_ui_timepage.h"
// # include "ds_ui_tomatpage.h"

// 定时器的一些预定义参数
# define TIMER_DIVIDER                          16
# define TIMER_SCALE                            (TIMER_BASE_CLK / TIMER_DIVIDER/1000)
# define TIMER_INTERVALO_SEC                    (10)
# define TEST_WITH_RELOAD                       1

// 定时器事件的数据结构
typedef struct {
    uint64_t timer_minute_count;
    uint64_t timer_second_count;
    uint64_t timer_second_count_isr;
}timer_event_t;

// 全局的定时器事件变量
timer_event_t g_timer_event;

// 定义与定时器和UI更新相关的队列
xQueueHandle timer_queue;
// xQueueHandle ui_update_timer_queue;

static const char *TAG = "TIME APP";
/*
 * @description : 定时器中断服务例程，当定时器达到其警报值时触发
 *
 * @param : para - 传递给中断服务例程的参数，这里是定时器的索引
 *
 * @return : 无
 */

void IRAM_ATTR timer_group0_isr(void *para)
{
    // 获取定时器的自旋锁，防止其他任务或中断干扰定时器的配置
    // timer_spinlock_take (TIMER_GROUP_0);

    // 将传递的参数转换为定时器索引
    int timer_idx = (int) para;

    // 准备基本的事件数据，稍后将发送回主程序任务
    timer_event_t evt;

    // 清除定时器的中断状态，这样它不会立即再次触发
    timer_group_clr_intr_status_in_isr (TIMER_GROUP_0, TIMER_0);

    // 警报触发后，我们需要再次启用它，以便下次触发
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, timer_idx);

    // 现在将事件数据发送回主程序任务
    // xQueueSendFromISR 是 FreeRTOS 的函数，用于在中断服务例程中向队列发送数据
    // timer_queue 是之前创建的队列，evt 是要发送的数据，最后一个参数用于接收被挂起任务的标识，这里不需要所以使用 NULL
    xQueueSendFromISR (timer_queue, &evt, NULL);

    // 释放定时器的自旋锁
    // timer_spinlock_give(TIMER_GROUP_0);
}


/*
 * @description : 初始化指定的定时器，并配置其基本参数
 *
 * @param : timer_idx - 要初始化的定时器编号
 * @param : auto_reload - 定时器到达警报值时是否自动重装
 * @param : timer_interval_sec - 要设置的警报间隔
 *
 * @return : 无
 */
static void tg0_timer_init(int timer_idx, bool auto_reload, double timer_interval_sec)
{
    // 定义定时器的配置结构体
    timer_config_t config = {
        .divider = TIMER_DIVIDER,                           // 计数器时钟分频器。分频器的范围从 2 到 65536
        .counter_dir = TIMER_COUNT_UP,                      // 设置计数器递增
        .counter_en = TIMER_PAUSE,                          // 计数器使能
        .alarm_en = TIMER_ALARM_EN,                         // 定时器报警使能
        .auto_reload = auto_reload,                         // 定时器自动重新加载
    };

    // 使用上面的配置初始化定时器
    timer_init (TIMER_GROUP_0, timer_idx, &config);

    // 定时器的计数器将从下面的值开始
    // 此外，如果设置了auto_reload，这个值将在警报时自动重载
    timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0x00000000ULL);

    // 配置警报值和警报中断
    // TIMER_SCALE 是一个宏，用于将秒转换为定时器的内部计数单位
    timer_set_alarm_value (TIMER_GROUP_0, timer_idx, timer_interval_sec * TIMER_SCALE);

    // 使能定时器的中断
    timer_enable_intr (TIMER_GROUP_0, timer_idx);

    // 为定时器注册一个中断服务例程
    // timer_group0_isr 是中断服务例程的函数地址
    // ESP_INTR_FLAG_IRAM 是中断的属性标志
    timer_isr_register(TIMER_GROUP_0, timer_idx, timer_group0_isr, (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);

    // 启动定时器
    timer_start(TIMER_GROUP_0, timer_idx);
}


/*
 * @description :  定时器事件处理任务。该任务监听定时器事件队列并更新分钟和秒的计数器。
 *                每60秒刷新时间，并每1秒执行一次相关操作
 *
 * @param : arg - 任务参数，当前实现未使用
 *
 * @return : 无
 */
static void timer_evt_task(void *arg)
{
    while (1)
    {
        timer_event_t evt;

        // 从队列中接收定时器事件
        xQueueReceive (timer_queue, &evt, portMAX_DELAY);

        // 更新分钟计数器
        g_timer_event.timer_minute_count ++;
        
        // 每6000次事件（相当于60秒）重置分钟计数器并刷新时间
        if (g_timer_event.timer_minute_count >= 6000)
            g_timer_event.timer_second_count = 0;
        
        // 更新秒计数器
        g_timer_event.timer_second_count ++;

        // 每100次事件（相当于1秒）重置秒计数器并执行相关操作
        if(g_timer_event.timer_second_count >= 100)
        {
            g_timer_event.timer_second_count = 0;
            ESP_LOGI (TAG, "is run");
        }
    }
}

/*
 * @description : 示例程序的UI更新任务，每10ms执行一次
 *
 * @param : arg - 任务参数，当前实现未使用
 *
 * @return : 无
 */

/*
 * @description : 初始化定时器相关的资源
 *
 * @return : 无
 */
void ds_timer_init (void)
{
    // 重置定时器的分钟，秒和中断服务例程的秒计数器为0
    g_timer_event.timer_minute_count = 0;
    g_timer_event.timer_second_count = 0;
    g_timer_event.timer_second_count_isr = 0;

    // 创建一个长度为10，每个元素大小为timer_event_t的队列
    // 该队列可能用于存储定时器事件
    timer_queue = xQueueCreate (10, sizeof(timer_event_t));    

    // 初始化定时器，设置其属性如是否自动重载和时间间隔                     
    tg0_timer_init (TIMER_0, TEST_WITH_RELOAD, TIMER_INTERVALO_SEC);

    // 创建一个新的任务来处理定时器事件
    // 任务名为"timer_evt_task"，堆栈大小为2048，优先级为5

    xTaskCreate (timer_evt_task, "timer_evt_task", 2048, NULL, 5, NULL);
}
/**
 * @file ds_gpio.c
 * @brief GPIO配置和控制模块
 * 
 * 该模块主要用于配置和控制与触摸屏和显示屏相关的GPIO（通用输入/输出）引脚。
 */

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include "freertos/FreeRTOS.h"
# include "freertos/task.h"
# include "freertos/queue.h"
# include "driver/gpio.h"

# include "ds_gpio.h"
# include "ds_system_data.h"

/**
 * GPIO状态说明：
 * GPIO5: 输出
 * GPIO4: 输入，上拉，上升沿触发中断
 */


// 定义GPIO引脚号
# define GPIO_OUTPUT_IO_0    5
# define GPIO_INPUT_IO_0     4

// 创建GPIO引脚的位掩码，用于配置函数
# define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0))
# define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0))

// 定义中断标志的默认值
# define ESP_INTR_FLAG_DEFAULT 0

//屏幕片选 0-有效
# define SCREEN_GPIO_OUTPUT_CS 27
# define SCREEN_GPIO_OUTPUT_CS_SEL ((1ULL<<SCREEN_GPIO_OUTPUT_CS))
//屏幕数据/指令选择 1-data 0-cmd
# define SCREEN_GPIO_OUTPUT_DC 14
# define SCREEN_GPIO_OUTPUT_DC_SEL ((1ULL<<SCREEN_GPIO_OUTPUT_DC))
//屏幕复位 0-reset
# define SCREEN_GPIO_OUTPUT_RES 12
# define SCREEN_GPIO_OUTPUT_RES_SEL ((1ULL<<SCREEN_GPIO_OUTPUT_RES))
//屏幕状态 1-busy 
# define SCREEN_GPIO_INTPUT_BUSY 13
# define SCREEN_GPIO_INTPUT_BUSY_SEL ((1ULL<<SCREEN_GPIO_INTPUT_BUSY))

// 定义一个全局的队列句柄，用于在中断服务例程和任务之间传递数据
static xQueueHandle gpio_evt_queue = NULL;

/**
 * @brief GPIO中断服务例程
 * 
 * 当GPIO引脚中断触发时，该函数会被调用，将触发中断的GPIO引脚号发送到队列中。
 * 
 * @param arg 传递给中断服务例程的参数，这里是触发中断的GPIO引脚号。
 */
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

/**
 * @brief GPIO中断服务例程
 * 
 * 当GPIO引脚中断触发时，该函数会被调用，将触发中断的GPIO引脚号发送到队列中。
 * 
 * @param arg 传递给中断服务例程的参数，这里是触发中断的GPIO引脚号。
 */
static void gpio_task_example(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
            
        }
    }
}

/**
 * @brief 触摸屏GPIO初始化函数
 * 
 * 初始化用于触摸屏控制的GPIO引脚，并创建处理GPIO事件的任务和队列。
 */
void ds_touch_gpio_init(){
    // GPIO配置结构体
    gpio_config_t io_conf;
    // 禁用中断
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    // 设置为输出模式
    io_conf.mode = GPIO_MODE_OUTPUT;
    // 设置要配置的引脚的位掩码
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    // 禁用下拉模式
    io_conf.pull_down_en = 0;
    // 禁用上拉模式
    io_conf.pull_up_en = 0;
    // 使用给定的设置配置GPIO
    gpio_config(&io_conf);

    // 配置中断类型为任意边缘触发
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    // 重新设置要配置的引脚的位掩码，这里使用GPIO4
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    // 设置为输入模式      
    io_conf.mode = GPIO_MODE_INPUT;
    // 启用上拉模式
    io_conf.pull_up_en = 1;
    // 重新配置GPIO
    gpio_config(&io_conf);

    //change gpio intrrupt type for one pin
    // gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_NEGEDGE);

    // 创建一个队列，用于处理来自中断服务例程的GPIO事件
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    // 创建一个任务，用于处理来自队列的GPIO事件
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    // 安装GPIO中断服务
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    // 为特定的GPIO引脚挂钩中断服务例程
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);

}

/**
 * @brief 显示屏GPIO初始化函数
 * 
 * 初始化用于显示屏控制的GPIO引脚。
 */
void ds_screen_gpio_init(){
    // GPIO配置结构体
    gpio_config_t io_conf;
    // 禁用中断
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    // 设置为输出模式
    io_conf.mode = GPIO_MODE_OUTPUT;
    // 设置要配置的引脚的位掩码，首先是屏幕片选信号
    io_conf.pin_bit_mask = SCREEN_GPIO_OUTPUT_CS_SEL;
    // 禁用下拉模式
    io_conf.pull_down_en = 0;
    // 禁用上拉模式
    io_conf.pull_up_en = 0;
    // 使用给定的设置配置GPIO
    gpio_config(&io_conf);

    // 重新设置要配置的引脚的位掩码，这次是屏幕数据/指令选择信号
    io_conf.pin_bit_mask = SCREEN_GPIO_OUTPUT_DC_SEL;
    // 重新配置GPIO
    gpio_config(&io_conf);

    // 重新设置要配置的引脚的位掩码，这次是屏幕复位信号
    io_conf.pin_bit_mask = SCREEN_GPIO_OUTPUT_RES_SEL;
    // 重新配置GPIO
    gpio_config(&io_conf);

    // 配置中断类型为负边缘触发
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    // 重新设置要配置的引脚的位掩码，这次是屏幕忙碌状态信号
    io_conf.pin_bit_mask = SCREEN_GPIO_INTPUT_BUSY_SEL;
    // 设置为输入模式    
    io_conf.mode = GPIO_MODE_INPUT;
    // 禁用上拉模式
    io_conf.pull_up_en = 0;
    // 重新配置GPIO
    gpio_config(&io_conf);
    //change gpio intrrupt type for one pin
    // gpio_set_intr_type(SCREEN_GPIO_INTPUT_BUSY, GPIO_INTR_NEGEDGE);

    // 安装GPIO中断服务
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    // 为特定的GPIO引脚挂钩中断服务例程
    gpio_isr_handler_add(SCREEN_GPIO_INTPUT_BUSY, gpio_isr_handler, (void*) SCREEN_GPIO_INTPUT_BUSY);
}


/**
 * @brief GPIO总初始化函数
 * 
 * 调用触摸屏GPIO初始化函数，初始化用于触摸屏控制的GPIO引脚。
 */
void ds_gpio_init(){
    ds_touch_gpio_init(); // 调用触摸屏GPIO初始化函数
}

/**
 * @brief 设置屏幕片选信号的状态
 * 
 * @param level GPIO引脚的状态（高或低）。
 */
void ds_gpio_set_screen_cs(uint32_t level){
    gpio_set_level(SCREEN_GPIO_OUTPUT_CS, level); // 设置GPIO引脚状态
}

/**
 * @brief 设置屏幕数据/指令选择信号的状态
 * 
 * @param level GPIO引脚的状态（高或低）。
 */
void ds_gpio_set_screen_dc(uint32_t level){
    gpio_set_level(SCREEN_GPIO_OUTPUT_DC, level); // 设置GPIO引脚状态
}

/**
 * @brief 设置屏幕复位信号的状态
 * 
 * @param level GPIO引脚的状态（高或低）。
 */
void ds_gpio_set_screen_rst(uint32_t level){
    gpio_set_level(SCREEN_GPIO_OUTPUT_RES, level); // 设置GPIO引脚状态
}

/**
 * @brief 获取屏幕忙碌状态
 * 
 * @return GPIO引脚的状态（高或低）。
 */
int ds_gpio_get_screen_busy(){
    return gpio_get_level(SCREEN_GPIO_INTPUT_BUSY); // 获取GPIO引脚状态
}

/**
 * @brief 设置触摸屏复位信号的状态
 * 
 * @param level GPIO引脚的状态（高或低）。
 */
void ds_gpio_set_touch_rst(uint32_t level){
    gpio_set_level(GPIO_OUTPUT_IO_0, level); // 设置GPIO引脚状态
}


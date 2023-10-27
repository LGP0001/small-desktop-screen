/**
 * @file ds_gpio.h
 * @brief GPIO配置和控制模块
 * 
 * 该模块主要用于配置和控制与触摸屏和显示屏相关的GPIO（通用输入/输出）引脚。
 */
# ifndef _DS_GPIO_H_
# define _DS_GPIO_H_

# include <stdint.h>

/**
 * @brief 触摸屏GPIO初始化函数
 * 
 * 初始化用于触摸屏控制的GPIO引脚，并创建处理GPIO事件的任务和队列。
 */
void ds_touch_gpio_init(void);

/**
 * @brief 显示屏GPIO初始化函数
 * 
 * 初始化用于显示屏控制的GPIO引脚。
 */
void ds_screen_gpio_init(void);

/**
 * @brief 设置屏幕片选信号的状态
 * 
 * @param level GPIO引脚的状态（高或低）。
 */
void ds_gpio_set_screen_cs(uint32_t level);

/**
 * @brief 设置屏幕数据/指令选择信号的状态
 * 
 * @param level GPIO引脚的状态（高或低）。
 */
void ds_gpio_set_screen_dc(uint32_t level);

/**
 * @brief 设置屏幕复位信号的状态
 * 
 * @param level GPIO引脚的状态（高或低）。
 */
void ds_gpio_set_screen_rst(uint32_t level);

/**
 * @brief 获取屏幕忙碌状态
 * 
 * @return GPIO引脚的状态（高或低）。
 */
int ds_gpio_get_screen_busy(void);

/**
 * @brief 设置触摸屏复位信号的状态
 * 
 * @param level GPIO引脚的状态（高或低）。
 */
void ds_gpio_set_touch_rst(uint32_t level);

# endif
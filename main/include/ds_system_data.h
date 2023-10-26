/**
 * @file ds_system_data.h
 * @brief 系统数据管理模块头文件
 * 
 * 该模块提供了与WiFi设置相关的数据结构和功能。
 */

# ifndef _DS_SYSTEM_DATA_H_
# define _DS_SYSTEM_DATA_H_

# include <stdio.h>
# include <string.h>
# include <stdint.h>

// 最大SSID长度定义
# define MAX_SETTING_SSID_LEN 32
// 最大密码长度定义
# define MAX_SETTING_PSW_LEN 64

// 设置数据的枚举类型，描述WiFi信息的状态
typedef enum{
    SETTING_DATA_INIT = 0,              /**< 初始状态，无WiFi信息 */
    SETTING_DATA_HAS_WIFI_INFO,         /**< 有WiFi信息 */
}SETTING_DATA_E;

// 系统数据结构，包含WiFi的SSID和密码
typedef struct
{
    char setting_ssid[MAX_SETTING_SSID_LEN];    /**< SSID字符串 */
    uint8_t setting_ssid_len;                   /**< SSID长度 */
    char setting_psw[MAX_SETTING_PSW_LEN];      /**< 密码字符串 */
    uint8_t setting_psw_len;                    /**< 密码长度 */
}SYSTRM_DATA_T;

//////////////////WIFI INFO MANAGE //////////////////////////////


/**
 * @brief 设置系统的WiFi信息
 * 
 * @param p_ssid SSID字符串
 * @param p_ssidlen SSID长度
 * @param p_psw 密码字符串
 * @param p_pswlen 密码长度
 */
void set_system_data_wifi_info(char * p_ssid,uint8_t p_ssidlen,char *p_psw ,uint8_t p_pswlen);

/**
 * @brief 打印当前系统的WiFi信息
 */
void print_system_data_wifi_info(void);

/**
 * @brief 检查系统的WiFi信息状态
 * 
 * @return SETTING_DATA_E WiFi信息的状态
 */
SETTING_DATA_E check_system_data_wifi_info(void);

/**
 * @brief 获取当前系统的数据
 * 
 * @return SYSTRM_DATA_T 当前的系统数据
 */
SYSTRM_DATA_T get_system_data(void);

/**
 * @brief 初始化系统数据模块
 */
void ds_system_data_init(void);

# endif
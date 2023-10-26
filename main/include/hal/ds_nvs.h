/**
 * @file ds_nvs.h
 * @brief NVS (非易失性存储) 管理模块头文件
 * 
 * 该模块提供了与WiFi信息在NVS中的存储和读取相关的功能。
 */

#ifndef _DS_NVS_H_
#define _DS_NVS_H_

// 定义NVS中WiFi信息的枚举状态
typedef enum{
    NVS_WIFI_INFO_ERROR =0,         /**< 读取或保存WiFi信息时出错 */
    NVS_WIFI_INFO_NULL,             /**< NVS中没有WiFi信息 */
    NVS_WIFI_INFO_SAVE,             /**< WiFi信息已成功保存到NVS */
}NVS_WIFI_INFO_E;

/**
 * @brief 初始化NVS模块
 */
void ds_nvs_init(void);

/**
 * @brief 将当前的WiFi信息保存到NVS
 */
void ds_nvs_save_wifi_info(void);

/**
 * @brief 从NVS读取WiFi信息
 * 
 * @return NVS_WIFI_INFO_E 读取结果的状态，可能是错误、空或成功保存的状态
 */
NVS_WIFI_INFO_E ds_nvs_read_wifi_info(void);


#endif


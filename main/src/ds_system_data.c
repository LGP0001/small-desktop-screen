/**
 * @file ds_system_data.c
 * @brief 系统数据管理模块
 * 
 * 该模块提供了与WiFi设置相关的数据结构和功能。
 */


#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_log.h"

#include "ds_system_data.h"


static const char *TAG = "ds_system_data";


SYSTRM_DATA_T g_system_data;

/**
 * @brief 初始化系统数据模块
 */
void ds_system_data_init(){
    memset(&g_system_data,0,sizeof(SYSTRM_DATA_T));
}

/**
 * @brief 获取当前系统的数据
 * 
 * @return SYSTRM_DATA_T 当前的系统数据
 */
SYSTRM_DATA_T get_system_data(){
    return g_system_data;
}

/**
 * @brief 设置系统的WiFi信息
 * 
 * @param p_ssid SSID字符串
 * @param p_ssidlen SSID长度
 * @param p_psw 密码字符串
 * @param p_pswlen 密码长度
 */
void set_system_data_wifi_info(char * p_ssid,uint8_t p_ssidlen,char *p_psw ,uint8_t p_pswlen){
    if(p_pswlen >= MAX_SETTING_SSID_LEN || p_ssidlen >= MAX_SETTING_PSW_LEN){
        ESP_LOGE(TAG, "MAX_SETTING_SSID_PWD_LEN ERROR");
    }
    g_system_data.setting_ssid_len = p_ssidlen;
    g_system_data.setting_psw_len = p_pswlen;
    memcpy(g_system_data.setting_ssid,p_ssid,p_ssidlen);
    memcpy(g_system_data.setting_psw,p_psw,p_pswlen);
    g_system_data.setting_ssid[p_ssidlen] = '\0';
    g_system_data.setting_psw[p_pswlen] = '\0';
}

/**
 * @brief 打印当前系统的WiFi信息
 */
void print_system_data_wifi_info()
{
    int i;
    printf("\r\nwifi_ssid:");
    for(i = 0;i<g_system_data.setting_ssid_len;i++){
        printf("%c",g_system_data.setting_ssid[i]);
    }

    printf("\r\nwifi_password:");
    for(i = 0;i<g_system_data.setting_psw_len;i++){
        printf("%c",g_system_data.setting_psw[i]);
    }
    printf("\r\n");
}

/**
 * @brief 检查系统的WiFi信息状态
 * 
 * @return SETTING_DATA_E WiFi信息的状态
 */
SETTING_DATA_E check_system_data_wifi_info(){
    if(g_system_data.setting_ssid_len != 0){
        return SETTING_DATA_HAS_WIFI_INFO;
    }
    return SETTING_DATA_INIT;
}
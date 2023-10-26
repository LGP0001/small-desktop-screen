/**
 * @file ds_nvs.c
 * @brief NVS (非易失性存储) 管理模块实现文件
 * 
 * 该模块提供了用于读取和写入WiFi配置到NVS的功能。
 * 
 */

# include <stdio.h>
# include "freertos/FreeRTOS.h"
# include "freertos/task.h"
# include "esp_system.h"
# include "nvs_flash.h"
# include "nvs.h"
# include "esp_log.h"

# include "ds_nvs.h"
# include "ds_system_data.h"

static const char *TAG = "ds_nvs";  // 日志标签

NVS_WIFI_INFO_E wifi_config_flag = NVS_WIFI_INFO_NULL;  // WiFi配置标志

/**
 * @brief 保存WiFi信息到NVS
 */
void ds_nvs_save_wifi_info(){
    esp_err_t err;
    nvs_handle_t nvs_handle;

    // 尝试打开NVS的wificonfig命名空间
    err = nvs_open("wificonfig", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGI(TAG,"Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return ;
    }

    wifi_config_flag = NVS_WIFI_INFO_SAVE;

    // 将WiFi配置标志、SSID和密码保存到NVS
    ESP_ERROR_CHECK(nvs_set_u8(nvs_handle, "wifi_flag", wifi_config_flag));
    ESP_ERROR_CHECK(nvs_set_str(nvs_handle, "ssid", get_system_data().setting_ssid));
    ESP_ERROR_CHECK(nvs_set_str(nvs_handle, "psw",  get_system_data().setting_psw));
    
    // 提交更改到NVS
    ESP_ERROR_CHECK(nvs_commit(nvs_handle));
    nvs_close(nvs_handle);
}

/**
 * @brief 从NVS读取WiFi信息
 * 
 * @return NVS_WIFI_INFO_E 返回WiFi信息的状态，可能是错误、空或成功保存的状态
 */
NVS_WIFI_INFO_E ds_nvs_read_wifi_info(){
    esp_err_t err;
    nvs_handle_t nvs_handle;

    // 尝试打开NVS的wificonfig命名空间
    err = nvs_open("wificonfig", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGI(TAG,"Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return NVS_WIFI_INFO_ERROR;
    }

    uint8_t wifi_config = 0;
    ESP_ERROR_CHECK(nvs_get_u8(nvs_handle, "wifi_flag", &wifi_config));
    wifi_config_flag = wifi_config;

    // 根据保存的标志，读取或处理WiFi信息
    if(wifi_config_flag == NVS_WIFI_INFO_SAVE){
        ESP_LOGI(TAG,"has wifi config info");
        char ssid[32];
        char psw[64];
        size_t ssid_len = sizeof(ssid);
        size_t psw_len = sizeof(psw);
        ESP_ERROR_CHECK(nvs_get_str(nvs_handle, "ssid", ssid, &ssid_len));
        ESP_ERROR_CHECK(nvs_get_str(nvs_handle, "psw", psw, &psw_len));
        set_system_data_wifi_info(ssid,ssid_len,psw,psw_len);
        print_system_data_wifi_info();
    }else{
        ESP_LOGI(TAG,"wifi config info null");
    }

    nvs_close(nvs_handle);
    return wifi_config_flag;
}

/**
 * @brief 初始化NVS模块
 */
void ds_nvs_init()
{
    // 初始化NVS
    esp_err_t err = nvs_flash_init();

    // 如果NVS分区被截断或需要删除，则重试nvs_flash_init
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    
    ESP_ERROR_CHECK(err);
}




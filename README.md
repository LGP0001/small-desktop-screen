# NVS基本介绍
NVS（Non-Volatile Storage，非易失性存储）是ESP32提供的一个持久化存储解决方案，可以用于保存配置信息、状态值等需要在重启后保留的数据。

# ESP32 NVS示例
在这个示例中，我们将展示如何在ESP32上初始化NVS，保存和读取WiFi配置信息。代码文件名为ds_nvs.c，该文件主要包括三个函数：ds_nvs_init、ds_nvs_save_wifi_info和ds_nvs_read_wifi_info。

## 初始化NVS (ds_nvs_init)
- 首先，我们调用nvs_flash_init函数初始化NVS。
- 如果NVS分区被截断或需要删除，我们调用nvs_flash_erase函数删除NVS分区，然后再次调用nvs_flash_init函数。
~~~c
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
~~~
## 保存WiFi信息 (ds_nvs_save_wifi_info)
- 我们使用nvs_open函数打开名为wificonfig的NVS命名空间，以读写模式。
- 然后，我们使用nvs_set_u8和nvs_set_str函数将WiFi配置标志、SSID和密码保存到NVS。
- 最后，我们调用nvs_commit函数提交更改，并使用nvs_close函数关闭NVS句柄。
~~~c
void ds_nvs_save_wifi_info(){
    esp_err_t err;
    nvs_handle_t nvs_handle;

    // 尝试打开NVS的wificonfig命名空间
    err = nvs_open("wificonfig", NVS_READWRITE, &nvs_handle);
    ...
    // 提交更改到NVS
    ESP_ERROR_CHECK(nvs_commit(nvs_handle));
    nvs_close(nvs_handle);
}
~~~
## 从NVS读取WiFi信息 (ds_nvs_read_wifi_info)
- 我们同样使用nvs_open函数打开名为wificonfig的NVS命名空间。
- 使用nvs_get_u8和nvs_get_str函数从NVS读取WiFi配置标志、SSID和密码。
- 最后，我们使用nvs_close函数关闭NVS句柄。
~~~c
NVS_WIFI_INFO_E ds_nvs_read_wifi_info(){
    esp_err_t err;
    nvs_handle_t nvs_handle;

    // 尝试打开NVS的wificonfig命名空间
    err = nvs_open("wificonfig", NVS_READWRITE, &nvs_handle);
    ...
    nvs_close(nvs_handle);
    return wifi_config_flag;
}
~~~
## ESP32 NVS API概览
你的代码中使用了以下NVS相关的API：

- nvs_flash_init: 初始化NVS。
- nvs_flash_erase: 删除NVS分区。
- nvs_open: 打开一个NVS命名空间。
- nvs_set_u8、nvs_set_str: 设置不同类型的值。
- nvs_get_u8、nvs_get_str: 获取不同类型的值。
- nvs_commit: 提交更改。
nvs_close: 关闭NVS句柄。

/************************************************************
 * ds_spiffs.c : ESP32的SPIFFS文件系统处理模块
 * 
 * 此模块提供了ESP32的SPIFFS文件系统的初始化、去初始化及测试功能。
 * 
 ************************************************************/


# include <sys/param.h>
# include <stdio.h>
# include <string.h>
# include <sys/unistd.h>
# include <sys/stat.h>

# include "esp_wifi.h"
# include "esp_event.h"
# include "esp_log.h"
# include "esp_spiffs.h"
# include "esp_err.h"

# include "ds_spiffs.h"

static const char *TAG="spiffs";

esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,   // 这决定了可以在存储上创建的最大文件数量
      .format_if_mount_failed = true
};

/* 初始化SPIFFS的函数 */
/*
 * @description : 初始化SPIFFS文件系统。
 *
 * @return : esp_err_t - 若初始化成功返回ESP_OK，否则返回表示失败的错误码。
 */
esp_err_t init_spiffs(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

    // 注册SPIFFS文件系统
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) 
    {
        if (ret == ESP_FAIL) 
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } 
        else if (ret == ESP_ERR_NOT_FOUND) 
        {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } 
        else 
        {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    // 获取SPIFFS分区信息
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    return ESP_OK;
}

/* 去初始化SPIFFS的函数 */
/*
 * @description : 卸载SPIFFS分区并禁用SPIFFS文件系统。
 *
 * @return : 无
 */
void ds_spiffs_deinit()
{
    // 卸载分区并禁用SPIFFS
    esp_vfs_spiffs_unregister(conf.partition_label);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}


/* 测试SPIFFS的函数 */
/*
 * @description : 用于测试SPIFFS文件系统的操作，包括文件的创建、写入、重命名和读取。
 *
 * @return : 无
 */
void ds_spiffs_test()
{
    // 使用POSIX和C标准库函数操作文件
    ESP_LOGI(TAG, "Opening file");
    FILE* f = fopen("/spiffs/hello.txt", "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fprintf(f, "Hello World!\n");
    fclose(f);
    ESP_LOGI(TAG, "File written");

     // 在重命名之前检查目标文件是否存在
    struct stat st;
    if (stat("/spiffs/foo.txt", &st) == 0) 
    {
        // 如果存在则删除
        unlink("/spiffs/foo.txt");
    }

    // 重命名原始文件
    ESP_LOGI(TAG, "Renaming file");
    if (rename("/spiffs/hello.txt", "/spiffs/foo.txt") != 0) 
    {
        ESP_LOGE(TAG, "Rename failed");
        return;
    }

    // 打开重命名的文件进行读取
    ESP_LOGI(TAG, "Reading file");
    f = fopen("/spiffs/foo.txt", "r");
    if (f == NULL) 
    {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    // 去掉换行符
    char* pos = strchr(line, '\n');
    if (pos) 
    {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);
}
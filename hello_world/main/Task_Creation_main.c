/*************************************************************
* Task_Creation_main.c :
* 
* 1. 应用程序的入口点：包含主要的业务逻辑。
* 2. 任务创建和管理：展示了如何使用 FreeRTOS 创建和处理任务。
* 3. 系统和芯片信息获取：如何检索 ESP32 的系统信息和芯片详情。
* 4. 循环和任务延迟：演示了在 FreeRTOS 任务中如何使用延迟。
* 
*************************************************************/
# include <stdio.h>
# include "sdkconfig.h"
# include "freertos/FreeRTOS.h"
# include "freertos/task.h"
# include "esp_system.h"
# include "esp_spi_flash.h"

# define CHIP_NAME "ESP32"

static const char *TAG = "MAIN APP";

/**
 * @description: 一个简单的 FreeRTOS 任务示例，无限循环打印消息。
 * @param: arg - 传递给任务的参数（在此示例中未使用）。
 */
static void test_task_example (void* arg)
{
    for(;;) 
    {
        // 延迟任务
        vTaskDelay (1000 / portTICK_PERIOD_MS);

        // 打印任务正在运行的消息
        printf ("task run \n");
    }
}

void app_main(void)
{
    printf("Hello world!\n");

    /* 获取并打印芯片信息 */
    esp_chip_info_t chip_info;
    esp_chip_info (&chip_info);
    printf ("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CHIP_NAME,
            chip_info.cores,                                                // CPU 核心数量
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",            // 蓝牙功能支持
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");         // BLE 功能支持

    printf ("silicon revision %d, ", chip_info.revision);                   // 硅片版本

    // 打印闪存大小和类型
    printf ("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    // 显示最小可用堆大小
    printf ("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    // 系统初始化日志消息
    ESP_LOGI (TAG, "system init V1.1");

    // 创建新的 FreeRTOS 任务
    xTaskCreate (test_task_example, "test_task_example", 2048, NULL, 10, NULL);

    while(1)
    {
        printf ("system run ...\n");                    // 打印系统正在运行的消息
        vTaskDelay (1000 / portTICK_PERIOD_MS);         // 延迟循环
    }
}

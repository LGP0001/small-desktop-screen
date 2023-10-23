[English](基础班_EN.md) | 中文
# 任务创建基础版

## 基础知识

### 什么是任务（Task）?

任务就是你想让你的电脑或者说单片机做的一件事。就像你给自己安排了一个任务，比如说做作业。在编程的世界里，任务就是一段程序，它能完成一些特定的工作。

### 为什么要创建任务?

你可能想让你的单片机同时做很多事情，比如说同时检查温度和显示时间。为了能让单片机同时做多件事，你就需要创建不同的任务。

### 什么是FreeRTOS?

FreeRTOS是一个“实时操作系统”。它能帮你管理和运行你创建的任务。就像一个班长，帮你安排和管理班级的事务。

### 任务是怎样运行的？

当你创建了一个任务，FreeRTOS就会帮你运行它。任务运行，就是它开始做它应该做的事情。比如说，打印消息、检查按钮是否被按下等。

### 怎样创建一个任务？

在FreeRTOS里，创建任务很简单，只需要调用一个函数xTaskCreate()就可以了。

### 如何结束一个任务？

如果你想让一个任务停下来，你可以调用vTaskDelete()函数。这个函数告诉FreeRTOS，你想让这个任务停下来。

## 示例代码
~~~c
# include <stdio.h>
# include "sdkconfig.h"
# include "freertos/FreeRTOS.h"
# include "freertos/task.h"
# include "esp_system.h"
# include "esp_spi_flash.h"

# define CHIP_NAME "ESP32"

static const char *TAG = "MAIN APP";

static void test_task_example (void* arg)
{
    for(;;) 
    {
        vTaskDelay (1000 / portTICK_PERIOD_MS);

        printf ("task run \n");
    }
}

void app_main(void)
{
    printf("Hello world!\n");

    esp_chip_info_t chip_info;
    esp_chip_info (&chip_info);
    printf ("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CHIP_NAME,
            chip_info.cores,                                                
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",            
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");         

    printf ("silicon revision %d, ", chip_info.revision);                   

    printf ("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf ("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    ESP_LOGI (TAG, "system init V1.1");

    xTaskCreate (test_task_example, "test_task_example", 2048, NULL, 10, NULL);

    while(1)
    {
        printf ("system run ...\n");                    
        vTaskDelay (1000 / portTICK_PERIOD_MS);        
    }
}
~~~

### 解析

1. **任务创建**：在 app_main 函数中，有一行代码 xTaskCreate(test_task_example, "test_task_example", 2048, NULL, 10, NULL);。这行代码告诉 FreeRTOS 创建一个新的任务，并且运行 test_task_example 函数。
2. **任务函数**：test_task_example 函数是一个简单的示例任务。这个函数包含一个无限循环，它会做两件事情：
- 使用 vTaskDelay 函数让任务休眠1秒。
- 使用 printf 函数打印 "task run" 消息。
3. **任务延迟**：vTaskDelay(1000 / portTICK_PERIOD_MS); 这行代码让任务休眠了1秒。这样，任务会每隔1秒打印一次消息。

通过以上的解释和示例代码的分析，希望你能对任务创建有一个基本的理解。你可以试着运行示例代码，看看代码运行的效果，也可以尝试修改代码，创建你自己的任务，看看会发生什么。
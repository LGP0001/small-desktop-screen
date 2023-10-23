English | [中文](基础版_CN.md)

# Task Creation Basics

## Fundamental Knowledge

### What is a Task?
A task is something you want your computer or microcontroller to do. Just like you assign a task to yourself, for example, doing homework. In the programming world, a task is a piece of code that can accomplish some specific work.

### Why Create Tasks?
You might want your microcontroller to do multiple things at once, like checking the temperature while displaying the time. To let the microcontroller handle multiple things, you need to create different tasks.

### What is FreeRTOS?
FreeRTOS is a "Real-Time Operating System." It helps you manage and run the tasks you create. It's like a class monitor, helping you arrange and manage class affairs.

### How Does a Task Run?
When you create a task, FreeRTOS will help you run it. Running a task means it starts doing what it's supposed to do, like printing messages, checking if a button is pressed, and so on.

### How to Create a Task?
In FreeRTOS, creating a task is simple; you just need to call a function xTaskCreate().

### How to End a Task?
If you want a task to stop, you can call the vTaskDelete() function. This function tells FreeRTOS that you want this task to stop.

## Example Code

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

### Analysis
1. **Task Creation**: In the app_main function, there is a line of code xTaskCreate(test_task_example, "test_task_example", 2048, NULL, 10, NULL);. This line tells FreeRTOS to create a new task and run the test_task_example function.
2. **Task Function**: The test_task_example function is a simple example task. This function contains an infinite loop that does two things:
- Uses the vTaskDelay function to put the task to sleep for 1 second.
- Uses the printf function to print the "task run" message.
3. **Task Delay**: The line vTaskDelay(1000 / portTICK_PERIOD_MS); puts the task to sleep for 1 second. This way, the task will print the message every 1 second.

Through the above explanations and analysis of the example code, I hope you have a basic understanding of task creation. You can try running the example code to see the effect of the code, or you can try modifying the code, create your own tasks, and see what happens.
Remember, it's designed to be easily understood by complete beginners and those with a little C language foundation!

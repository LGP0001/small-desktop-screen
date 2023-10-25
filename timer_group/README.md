# 定时器示例 (Timer Example)

[English](README_EN.md) | 中文

本示例启动一个创建定时器任务，用于打印 "Hello World"。

## 如何使用示例

按照本示例提供的详细说明进行操作。

根据你的开发板上安装的 Espressif 芯片选择相应的说明：

- [ESP32 入门指南](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/index.html)
- [ESP32-S2 入门指南](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/get-started/index.html)

## 示例文件夹内容

项目 **Timer_group** 包含一个用 C 语言编写的源文件 [Task_Creation_main.c](main/Task_Creation_main.c)。文件位于 [main](main) 文件夹中。

ESP-IDF 项目使用 CMake 构建。项目的构建配置包含在 `CMakeLists.txt` 文件中，这些文件提供了一组描述项目源文件和目标（可执行文件、库或两者）的指令和说明。

以下是项目文件夹中其余文件的简短说明。
```plaintext
├── CMakeLists.txt
├── example_test.py 用于自动化示例测试的 Python 脚本
├── main
│ ├── CMakeLists.txt
│ ├── component.mk 组件 make 文件
│ └── Timer_group_example_main.c
├── Makefile 由传统 GNU Make 使用的 Makefile
└── README.md 你当前正在阅读的文件 currently reading
```


有关 ESP-IDF 项目的结构和内容的更多信息，请参阅 ESP-IDF 编程指南的 [构建系统](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html) 部分。

## 故障排除

* 程序上传失败

    * 硬件连接不正确：运行 `idf.py -p PORT monitor`，并重新启动你的板子，看是否有任何输出日志。
    * 下载的波特率太高：在 `menuconfig` 菜单中降低你的波特率，然后重试。

## 技术支持和反馈

请使用以下反馈渠道：

* 对于技术查询，请前往 [esp32.com](https://esp32.com/) 论坛
* 对于功能请求或错误报告，请创建一个 [GitHub 问题](https://github.com/espressif/esp-idf/issues)

我们会尽快回复你。

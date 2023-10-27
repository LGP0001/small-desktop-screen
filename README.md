# GPIO教程

本教程分为两个版本：基础版和专业版。基础版主要面向初学者，通过示例代码解释STM32和ESP32的GPIO如何使用，专业版则是针对有一定基础的学习者，深入讲解GPIO的不同模式和特性。

## 学习目标

- 理解GPIO的基本原理和在嵌入式系统中的应用。
- 学会如何在STM32和ESP32上配置和使用GPIO。
- 掌握中断和事件处理的基本概念和用法。

## 学习资源

- 基础版示例代码：[GitHub仓库链接](https://github.com/LGP0001/small-desktop-screen.git)
- STM32参考资料：[STM32标准外设库](https://www.st.com/en/development-tools/stsw-stm32054.html)
- ESP32参考资料：[ESP-IDF GPIO API文档](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/api-reference/peripherals/gpio.html)
- esp-idf参考资料：[FreeRTOS 免费实时操作系统](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/api-reference/system/freertos.html)

## 学习内容

### [基础版](基础版.md)

1. **什么是GPIO**：简单解释GPIO的基本概念和作用。
2. **实现视频示例代码**：跟随视频教程，直接实现示例代码以掌握STM32和ESP32的GPIO基本操作。（视频示例代码链接待补充）

### [专业版](专业版.md)

1. **GPIO模式和配置**：深入讲解GPIO的不同模式，包括输入、输出、中断等，并通过流程图解释GPIO配置的效果。
2. **使用标准库配置GPIO**：探讨如何使用STM32和ESP32的标准库函数来配置和控制GPIO，使代码更为简洁和可维护。
3. **中断和事件处理**：介绍中断的基本概念，以及如何结合GPIO使用中断实现任务间的通信与同步。
4. **理解GPIO编程背后的原理**：探讨为什么要以特定的方式编写GPIO代码，以及如何写出高效、可维护的GPIO代码。

## 获取帮助

如果您遇到任何问题或需要帮助，请随时在与此仓库关联的 GitHub 问题跟踪器中提出问题。

## 许可和贡献

本项目遵循MIT许可协议。对于原课程的所有内容，请参考原课程的许可证规定。

[查看原课程的 MIT 许可证](LICENSE)

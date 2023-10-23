English | [中文](README.md)

# FreeRTOS Task Creation Tutorial

This tutorial is divided into two versions: Basic and Professional. The Basic version is mainly for beginners, explaining what a task is in a simple and understandable manner, and assisting you in implementing basic task creation through video tutorials. The Professional version is for learners with some foundation, referencing the FreeRTOS Real-Time Kernel Chinese Manual to delve into the theory and practice of task creation.

## Learning Objectives

- Understand what a task is and its importance in FreeRTOS.
- Learn how to create and manage tasks in FreeRTOS.
- Deepen understanding of task creation through practice.

## Learning Resources

- Basic version video tutorial: [Video Tutorial](https://www.bilibili.com/video/BV1wV4y1G7Vk/?p=24&vd_source=43aebb8344792e06500b46f018a5c4b9)
- Professional version reference material: [FreeRTOS Real-Time Kernel Chinese Manual](/home/lgp/Desktop/SDS/FreeRTOS实时内核使用指南-中文.pdf)
- esp-idf reference material: [FreeRTOS Free Real-Time Operating System](https://docs.espressif.com/projects/esp-idf/en/release-v4.1/api-reference/system/freertos.html)

## Learning Content

### [Basic Version](Basic_Version.md)

1. **What is a Task**: Simply explain what a task is and why we need tasks.
2. **Creating Your First Task**: Learn how to create and run your first task through a video tutorial.

### [Professional Version](Professional_Version.md)

1. **Task Creation Theory**: Deeply understand the basics of task management in FreeRTOS, including processing time allocation, task selection and running, how task priority affects system behavior, and the various states of tasks.
2. **Task Creation Practice**: Explore the creation and management of queues through example code, as well as how to implement communication and synchronization between tasks.
3. **Task States and Scheduling**: Discuss interrupt management, including FreeRTOS API that can be used in Interrupt Service Routines (ISR), delayed interrupt schemes, creation and use of binary semaphores and counting semaphores, and how to pass data in ISR through queues.
4. **Resource Management and Control**: Detailed introduction to the necessity of resource management, the concept of critical sections, implementation of mutual exclusion, the significance of suspending the scheduler, and how to create and use daemon tasks. Additionally, discuss the concepts and implementations of priority inversion and priority inheritance.
5. **Memory Management**: Understand when FreeRTOS allocates memory, including task creation, creation of queues/semaphores/mutexes, dynamic memory allocation, and memory allocation in ISR.

## Getting Help

If you encounter any problems or need help, feel free to raise issues in the GitHub issue tracker associated with this repository.

## License and Contributions

This project follows the MIT license. For all original course content, please refer to the license provisions of the original course.

[View the MIT license of the original course](LICENSE)

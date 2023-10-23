# Task_Creation Example

[中文](README.md) | English

This example starts a FreeRTOS task to print "Hello World".

(For more information about examples, see the README.md file in the upper level 'examples' directory.)

## How to use example

Follow detailed instructions provided specifically for this example.

Select the instructions depending on the Espressif chip installed on your development board:

- [ESP32 Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/index.html)
- [ESP32-S2 Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/get-started/index.html)

## Example folder contents

The project **hello_world** contains one source file in C language [Task_Creation_main.c](main/Task_Creation_main.c). The file is located in the [main](main) folder.

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt` files that provide a set of directives and instructions describing the project's source files and targets (executable, library, or both).

Below is a short explanation of the remaining files in the project folder.

```plaintext
├── CMakeLists.txt
├── example_test.py Python script used for automated example testing
├── main
│ ├── CMakeLists.txt
│ ├── component.mk Component make file
│ └── Task_Creation_main.c
├── Makefile Makefile used by legacy GNU Make
└── README.md This is the file you are currently reading
```


For more information on structure and contents of ESP-IDF projects, please refer to the [Build System](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html) section of the ESP-IDF Programming Guide.

## Troubleshooting

* Program upload failure

    * Hardware connection is not correct: run `idf.py -p PORT monitor`, and reboot your board to see if there are any output logs.
    * The baud rate for downloading is too high: lower your baud rate in the `menuconfig` menu, and try again.

## Technical support and feedback

Please use the following feedback channels:

* For technical queries, go to the [esp32.com](https://esp32.com/) forum
* For a feature request or bug report, create a [GitHub issue](https://github.com/espressif/esp-idf/issues)

We will get back to you as soon as possible.

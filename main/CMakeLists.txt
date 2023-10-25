idf_component_register(SRCS 
                            "app_main.c"
                            "src/hal/ds_timer.c"

                            "src/fs/ds_spiffs.c"
                    
                       INCLUDE_DIRS 
                            "."
                            "./include/hal"
                            "./include/fs")

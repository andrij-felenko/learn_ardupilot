set (CATAPULT_SRC
                    "main.c"
                    "led_status.c"
                    "led_status.h"
                    "primitive.c"
                    "primitive.h"
)

idf_component_register(SRCS ${CATAPULT_SRC}
                    INCLUDE_DIRS "."
                    REQUIRES led_strip esp_driver_gpio)

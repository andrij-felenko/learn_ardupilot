#ifndef CATAPULT_LED_STATUS_H
#define CATAPULT_LED_STATUS_H

#include "primitive.h"
#include <stdint.h>

typedef struct led_t led_t;

typedef enum {
    LED_S_Empty,
    LED_S_Prepared,
    LED_S_Loading,
    LED_S_Shooting,
    LED_S_Disconnected,
    LED_S_Uncomponnet,
    LED_S_Testing,
} led_enum_t;

typedef led_t* led_handle_t;
led_handle_t led_init(uint8_t gpio, uint8_t bright);
void led_free(led_handle_t);
void led_task(void *pvParameters);

void led_reset(led_handle_t);
void led_test (led_handle_t);
void led_shoot(led_handle_t);

void    led_bright_set(led_handle_t, uint8_t);
uint8_t led_bright    (led_handle_t);

void    led_gpio_set(led_handle_t, uint8_t);
uint8_t led_gpio    (led_handle_t);

RGB  led_color(led_handle_t);


// Led vtable style call
typedef struct {
    void (*reset)(led_handle_t);
    void (*test) (led_handle_t);
    void (*shoot)(led_handle_t);

    void    (*bright_set)(led_handle_t, uint8_t);
    uint8_t (*bright    )(led_handle_t);

    void    (*gpio_set)(led_handle_t, uint8_t);
    uint8_t (*gpio    )(led_handle_t);

    RGB (*color)(led_handle_t);
} led_w;

extern const led_w led;

#endif // CATAPULT_LED_STATUS_H
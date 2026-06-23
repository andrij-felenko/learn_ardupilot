#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "led_status.h"

// Onboard WS2812 on the Waveshare ESP32-C6-Zero is wired to GPIO8 (no separate
// power-enable pin). On generic C6 devkits it's the same GPIO8.
#define LED_DATA_GPIO  8

// Brightness knob: 1..255, lower = dimmer (try 3, 8, 15). The palette is just
// on/off per channel, scaled by BRIGHT — so one number controls brightness.
#define BRIGHT  1

void app_main(void)
{
    // init LED
    led_handle_t led = led_init(LED_DATA_GPIO, BRIGHT);

    xTaskCreate(led_task, "LedTask", 2048, (void*)led, 1, NULL);
}

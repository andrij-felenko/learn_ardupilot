#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "led_status.h"

// Onboard WS2812 on the Waveshare ESP32-C6-Zero is wired to GPIO8 (no separate
// power-enable pin). On generic C6 devkits it's the same GPIO8.
#define LED_DATA_GPIO  8

// Brightness knob: 1..255, lower = dimmer (try 3, 8, 15). The palette is just
// on/off per channel, scaled by BRIGHT — so one number controls brightness.
#define BRIGHT  1

// static const uint8_t COLORS[5][3] = {
//     { 1, 0, 0 },   // 0 -> red
//     { 0, 1, 0 },   // 1 -> green
//     { 0, 0, 1 },   // 2 -> blue
//     { 1, 0, 1 },   // 3 -> yellow
//     { 1, 1, 1 },   // 4 -> purple
// };

void app_main(void)
{
    // init LED
    led_handle_t led = led_init(LED_DATA_GPIO, BRIGHT);

    xTaskCreate(led_task, "LedTask", 2048, (void*)led, 1, NULL);

    // vTaskStartScheduler();



    // // Create the WS2812 driver on the data pin (backed by the RMT peripheral).
    // led_strip_handle_t led;
    // led_strip_config_t strip_cfg = {
    //     .strip_gpio_num   = LED_DATA_GPIO,
    //     .max_leds         = 1,                       // one LED on the board
    //     .led_pixel_format = LED_PIXEL_FORMAT_GRB,    // WS2812 byte order
    //     .led_model        = LED_MODEL_WS2812,
    //     .flags.invert_out = false,
    // };
    // led_strip_rmt_config_t rmt_cfg = {
    //     .clk_src        = RMT_CLK_SRC_DEFAULT,
    //     .resolution_hz  = 10 * 1000 * 1000,          // 10 MHz timing for WS2812
    //     .flags.with_dma = false,
    // };
    // ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_cfg, &rmt_cfg, &led));

    // int count = 0;
    // while (1) {
    //     int idx = count % 5;     // 0..4 -> which colour this second

    //     // ON: show the colour for the first half-second
    //     led_strip_set_pixel(led, 0,
    //                         COLORS[idx][0] * BRIGHT,
    //                         COLORS[idx][1] * BRIGHT,
    //                         COLORS[idx][2] * BRIGHT);
    //     led_strip_refresh(led);                       // push the bytes to the LED
    //     ESP_LOGI(TAG, "blink %d -> colour %d", count, idx);
    //     vTaskDelay(pdMS_TO_TICKS(1000));

    //     // OFF: dark for the second half -> a visible 1 Hz blink
    //     led_strip_clear(led);
    //     vTaskDelay(pdMS_TO_TICKS(1000));

    //     count++;
    // }
}

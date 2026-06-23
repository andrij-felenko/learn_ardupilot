#include "led_status.h"
#include <stdint.h>

#include "esp_log.h"
#include "led_strip.h" // WS2812 driver (espressif/led_strip component)

const led_w led = {
    .reset = /*(void(*)(led_handle_t))*/ led_reset,
    .test  = /*(void(*)(led_handle_t))*/ led_test,
    .shoot = led_shoot,

    .bright_set = led_bright_set,
    .bright     = led_bright,

    .gpio_set = led_gpio_set,
    .gpio     = led_gpio,

    .color = led_color,
};

struct led_t {
    led_enum_t enum_v;
    uint8_t bright;
    // uint8_t gpio_pin; // move to config.strip_gpio_num

    led_strip_handle_t strip;
    led_strip_config_t config;
    led_strip_rmt_config_t rmt_config;
};

led_handle_t led_init(uint8_t gpio, uint8_t bright){
    ESP_LOGI("RMT", "init led");
    led_handle_t _ = malloc(sizeof(struct led_t));
    memset(_, 0, sizeof(struct led_t));
    _->enum_v = LED_S_Empty;
    _->bright = bright;

    _->config.strip_gpio_num   = gpio;
    _->config.max_leds         = 1;
    _->config.led_pixel_format = LED_PIXEL_FORMAT_GRB;
    _->config.led_model        = LED_MODEL_WS2812;
    _->config.flags.invert_out = false;

    _->rmt_config.clk_src        = RMT_CLK_SRC_DEFAULT;
    _->rmt_config.resolution_hz  = 10 * 1000 * 1000;
    _->rmt_config.mem_block_symbols = 48;
    _->rmt_config.flags.with_dma = false;

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&_->config, &_->rmt_config, &_->strip));
    return _;
}

static const char *TAG = "catapult";
void led_task(void *pvParameters){
    led_handle_t self = (led_handle_t)pvParameters;
    led_enum_t last = -1;

    while (1){
        // ON: show the colour for the first half-second
        if (last != self->enum_v){
            last  = self->enum_v;
            RGB c = led.color(self);
            uint8_t br = led.bright(self);
            led_strip_set_pixel(self->strip, 0,
                                c.red   * br,
                                c.green * br,
                                c.blue  * br);
            led_strip_refresh(self->strip);                       // push the bytes to the LED
            ESP_LOGI(TAG, "colour %d", self->enum_v);
        }
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

void led_free(led_handle_t led){
    // TODO
}

void led_reset(led_handle_t led){ led->enum_v = LED_S_Empty; }
void led_test (led_handle_t led){ led->enum_v = LED_S_Testing; }
void led_shoot(led_handle_t led){ led->enum_v = LED_S_Shooting; }

uint8_t led_bright    (led_handle_t led)             { return led->bright; }
void    led_bright_set(led_handle_t led, uint8_t br) {        led->bright = br; }

uint8_t led_gpio    (led_handle_t led)             { return led->config.strip_gpio_num; }
void    led_gpio_set(led_handle_t led, uint8_t g) {         led->config.strip_gpio_num = g; }

RGB led_color(led_handle_t led){
    switch (led->enum_v){
        case LED_S_Loading:  return rgb_make(1, 1, 0); // red + green
        case LED_S_Prepared: return rgb_make(1, 0, 1); // yellow
        case LED_S_Shooting: return rgb_make(0, 1, 0); // green
        case LED_S_Empty:    return rgb_make(1, 1, 1); // white
        case LED_S_Testing:  return rgb_make(0, 0, 1); // blue
        case LED_S_Uncomponnet:  return rgb_make(0, 1, 1); // green + blue
        case LED_S_Disconnected: return rgb_make(1, 0, 0); // red
    }
    return rgb_make(0, 0, 0);
}
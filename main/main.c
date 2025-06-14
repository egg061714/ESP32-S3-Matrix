#include "led_strip.h"
#include "driver/rmt_tx.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN     14
#define LED_NUM     64
#define LED_BRIGHTNESS 32  // 建議 8~32 依你板子調

static led_strip_handle_t led_strip;

void app_main(void)
{
    // 初始化
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_PIN,
        .max_leds = LED_NUM,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB,
        .led_model = LED_MODEL_WS2812,
        .flags.invert_out = false,
    };
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,
        .mem_block_symbols = 0,
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    led_strip_clear(led_strip);

    // 設定全部為白色
    uint8_t r = 255 / LED_BRIGHTNESS;
    uint8_t g = 255 / LED_BRIGHTNESS;
    uint8_t b = 255 / LED_BRIGHTNESS;
    for (int i = 0; i < LED_NUM; i++) {
        led_strip_set_pixel(led_strip, i, r, g, b);
    }
    led_strip_refresh(led_strip);

    // 進入死迴圈讓燈常亮
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

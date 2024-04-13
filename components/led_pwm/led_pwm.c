/**
 *    ____    __    ____  _______     _______.      ___     ___    ___    _  _
 *    \   \  /  \  /   / |   ____|   /       |     |__ \   / _ \  |__ \  | || |
 *     \   \/    \/   /  |  |__     |   (----` ______ ) | | | | |    ) | | || |_
 *      \            /   |   __|     \   \    |______/ /  | | | |   / /  |__ _|
 *       \    /\    /    |  |____.----)   |         / /_  | |_| |  / /_     | |
 *        \__/  \__/     |_______|_______/         |____|  \___/  |____|    |_|
 *
 */

/*--------------------------- INCLUDES ---------------------------------------*/
#include "led_pwm.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "sdkconfig.h"
#include <string.h>
#include <unistd.h>

/*--------------------------- MACROS AND DEFINES -----------------------------*/

#define LEDC_DUTY_RES  LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY      (8191) // ((2 ** 13) - 1) = 8191 full cycle , 4095 is 50%
#define LEDC_FREQUENCY (5000) // Frequency in Hertz. Set frequency at 5 kHz
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
void vCallbackFunction(TimerHandle_t xTimer);
static int led_timer_init();
uint32_t timer_refresh = 100;
/*--------------------------- VARIABLES --------------------------------------*/
TimerHandle_t periodic_timer;
bool is_timer_inited = false;
static const char *TAG = "ledc";
uint32_t led_on_saved[4];
uint32_t led_off_saved[4];
uint32_t led_on_left[4];
uint32_t led_off_left[4];
uint32_t led_is_on[4];
uint32_t led_pwm[4];
bool is_running_pattern[4];
bool led_is_on_toggle[4];
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
void vCallbackFunction(TimerHandle_t xTimer)
{
    for (int i = 0; i < 4; i++) {
        if (is_running_pattern[i]) {
            ESP_LOGI(TAG, "run pattern %d", i);
            if (led_is_on[i]) {
                if (led_on_left[i] > 0) {
                    led_on_left[i] -= timer_refresh;
                } else {
                    led_off(i);
                    led_on_left[i] = led_on_saved[i];
                    led_is_on[i] = false;
                }
            } else {
                if (led_off_left[i] > 0) {
                    led_off_left[i] -= timer_refresh;
                } else {
                    led_on_pwm(i, led_pwm[i]);
                    led_off_left[i] = led_off_saved[i];
                    led_is_on[i] = true;
                }
            }
        }
    }
}

static int led_timer_init()
{
    periodic_timer = xTimerCreate("periodic", pdMS_TO_TICKS(timer_refresh),
                                  pdTRUE, (void *)0, vCallbackFunction);
    xTimerStart(periodic_timer, 0);
    return 0;
}
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
int led_init(int led_id)
{
    int gpio_pin = 0;
    if (led_id == LEDC_CHANNEL_BLUE) {
        gpio_pin = GPIO_LED_BLUE;
    } else if (led_id == LEDC_CHANNEL_GREEN) {
        gpio_pin = GPIO_LED_GREEN;
    } else if (led_id == LEDC_CHANNEL_RED) {
        gpio_pin = GPIO_LED_RED;
    } else if (led_id == LEDC_CHANNEL_BUZZER) {
        gpio_pin = GPIO_BUZZER;
    }

    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,          // isti
        .timer_num = LEDC_TIMER,          // isti
        .duty_resolution = LEDC_DUTY_RES, // isti
        .freq_hz = LEDC_FREQUENCY,        // Set output frequency at 5 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {.speed_mode = LEDC_MODE,

                                          .channel = led_id,
                                          .timer_sel = LEDC_TIMER,
                                          .intr_type = LEDC_INTR_DISABLE,
                                          .gpio_num = gpio_pin,
                                          .duty = 0, // Set duty to %
                                          .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    is_running_pattern[led_id] = false;
    if (!is_timer_inited) {
        led_timer_init();
        is_timer_inited = true;
    }
    return 0;
}

int led_on_pwm(int led_id, int pwm_percentage)
{
    ledc_set_duty(LEDC_MODE, led_id, LEDC_DUTY * pwm_percentage / 100);
    ledc_update_duty(LEDC_MODE, led_id);
    if (pwm_percentage > 0) {
        led_is_on_toggle[led_id] = true;
    } else {
        led_is_on_toggle[led_id] = false;
    }
    return 0;
}

int led_off(int led_id)
{
    led_on_pwm(led_id, 0);
    return 0;
}

int led_on(int led_id)
{
    led_on_pwm(led_id, 100);
    return 0;
}

int led_on_pwm_pattern(int led_id, int pwm_percentage, int time_on_ms,
                       int time_off_ms)
{
    led_on_saved[led_id] = time_on_ms;
    led_off_saved[led_id] = time_off_ms;
    led_on_left[led_id] = time_on_ms;
    led_off_left[led_id] = time_off_ms;
    led_pwm[led_id] = pwm_percentage;
    is_running_pattern[led_id] = true;
    return 0;
}

int stop_running_pattern(int led_id)
{
    is_running_pattern[led_id] = false;
    led_off(led_id);
    return 0;
}

int led_toggle(int led_id)
{
    if (led_is_on_toggle[led_id]) {
        led_off(led_id);
    } else {
        led_on(led_id);
    }
    return 0;
}

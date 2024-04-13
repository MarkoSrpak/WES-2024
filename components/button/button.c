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
#include "button.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include <stdio.h>

/*--------------------------- MACROS AND DEFINES -----------------------------*/
#define GPIO_BUTTON_1 (36U)
#define GPIO_BUTTON_2 (32U)
#define GPIO_BUTTON_3 (33U)
#define GPIO_BUTTON_4 (25U)

#define NUM_TIMERS     4
#define DEBOUNCE_PAUSE 300

#define DELAY_TIME_MS (1000)

/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
typedef void (*button_pressed_isr_t)(void *p_arg);

/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
static void _btn_1_isr(void *p_arg); // button 1 press
static void _btn_2_isr(void *p_arg); // button 2 press
static void _btn_3_isr(void *p_arg); // button 3 press
static void _btn_4_isr(void *p_arg); // button 4 press

static esp_err_t _button_init(uint8_t pin, button_pressed_isr_t p_isr);
static int button_timer_init();
static void vTimerCallback(TimerHandle_t xTimer);
/*--------------------------- VARIABLES --------------------------------------*/
TimerHandle_t xTimers[NUM_TIMERS];
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
static void IRAM_ATTR _btn_1_isr(void *p_arg)
{
    (void)p_arg; /* Suppress the unused variable warning*/
    if (pdFALSE == xTimerIsTimerActive(xTimers[0])) {
        xTimerStartFromISR(xTimers[0], pdFALSE);
    }
}

static void IRAM_ATTR _btn_2_isr(void *p_arg)
{
    (void)p_arg; /* Suppress the unused variable warning*/
    if (pdFALSE == xTimerIsTimerActive(xTimers[1])) {
        xTimerStartFromISR(xTimers[1], pdFALSE);
    }
}

static void IRAM_ATTR _btn_3_isr(void *p_arg)
{
    (void)p_arg; /* Suppress the unused variable warning*/
    if (pdFALSE == xTimerIsTimerActive(xTimers[2])) {
        xTimerStartFromISR(xTimers[2], pdFALSE);
    }
}

static void IRAM_ATTR _btn_4_isr(void *p_arg)
{
    (void)p_arg; /* Suppress the unused variable warning*/
    if (pdFALSE == xTimerIsTimerActive(xTimers[3])) {
        xTimerStartFromISR(xTimers[3], pdFALSE);
    }
}

static int button_timer_init()
{
    for (int x = 0; x < NUM_TIMERS; x++) {
        xTimers[x] = xTimerCreate("Timer", pdMS_TO_TICKS(DEBOUNCE_PAUSE),
                                  pdFALSE, (void *)0, vTimerCallback);
    }
    return 0;
}

static esp_err_t _button_init(uint8_t pin, button_pressed_isr_t p_isr)
{
    // Configure the GPIO.
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = (GPIO_INTR_POSEDGE),
    };

    esp_err_t esp_err = gpio_config(&io_conf);

    if (ESP_OK == esp_err) {
        // Change gpio interrupt type for a pin.
        esp_err = gpio_set_intr_type(io_conf.pin_bit_mask, io_conf.intr_type);
    }

    if (ESP_OK == esp_err) {
        // Install gpio isr service.
        gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);
        /* esp_err is not assigned to it because it returns invalid
        statuses when called many times (for many buttons)*/
    }

    if (ESP_OK == esp_err) {
        // Hook isr handler for specific gpio pin.
        esp_err = gpio_isr_handler_add(pin, p_isr, (void *)&pin);
    }

    return esp_err;
}

static void vTimerCallback(TimerHandle_t xTimer)
{
    xTimerStop(xTimer, 0);
}
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/

int button_init()
{
    _button_init(GPIO_BUTTON_1, _btn_1_isr);
    _button_init(GPIO_BUTTON_2, _btn_2_isr);
    _button_init(GPIO_BUTTON_3, _btn_3_isr);
    _button_init(GPIO_BUTTON_4, _btn_4_isr);
    button_timer_init();
    return 0;
}

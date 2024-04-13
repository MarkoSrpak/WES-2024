/**
 *    ____    __    ____  _______     _______.      ___     ___    ___    _  _
 *    \   \  /  \  /   / |   ____|   /       |     |__ \   / _ \  |__ \  | || |
 *     \   \/    \/   /  |  |__     |   (----` ______ ) | | | | |    ) | | || |_
 *      \            /   |   __|     \   \    |______/ /  | | | |   / /  |__ _|
 *       \    /\    /    |  |____.----)   |         / /_  | |_| |  / /_     | |
 *        \__/  \__/     |_______|_______/         |____|  \___/  |____|    |_|
 *
 */

#ifndef LED_PWM_LED_PWM_H
#define LED_PWM_LED_PWM_H

/*--------------------------- INCLUDES ---------------------------------------*/
#include "driver/ledc.h"
#include "esp_err.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
#define GPIO_LED_BLUE  14
#define GPIO_LED_GREEN 27
#define GPIO_LED_RED   26
#define GPIO_BUZZER    2

#define LEDC_TIMER          LEDC_TIMER_0        // isti timer za sve
#define LEDC_MODE           LEDC_LOW_SPEED_MODE // isti svima
#define LEDC_CHANNEL_BLUE   LEDC_CHANNEL_0      // led_id
#define LEDC_CHANNEL_GREEN  LEDC_CHANNEL_1      // led_id
#define LEDC_CHANNEL_RED    LEDC_CHANNEL_2      // led_id
#define LEDC_CHANNEL_BUZZER LEDC_CHANNEL_3      // led_id

#define LED_ID_BLUE   0
#define LED_ID_GREEN  1
#define LED_ID_RED    2
#define LED_ID_BUZZER 3

#define LEDC_DUTY_RES  LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY      (8191) // ((2 ** 13) - 1) = 8191 full cycle , 4095 is 50%
#define LEDC_FREQUENCY (5000) // Frequency in Hertz. Set frequency at 5 kHz
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- EXTERN -----------------------------------------*/
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/

// example/peripherals/ledc
int led_init(int led_id);
int led_on_pwm(int led_id, int pwm_percentage);
int led_on(int led_id);
int led_off(int led_id);
int led_toggle(int led_id);

int led_on_pwm_pattern(int led_id, int pwm_percentage, int time_on_ms,
                       int time_off_ms);
int stop_running_pattern(int led_id);

#endif /*LED_PWM_LED_PWM_H*/

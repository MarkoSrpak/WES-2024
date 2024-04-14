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
#include "app.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
// accelerometer
#include "accelerometer.h"

// led_wm
#include "led_pwm.h"

// button
#include "button.h"

// lvgl
#include "lvgl.h"
#include "lvgl_helpers.h"

// squareline
#include "ui.h"

// wifi
#include "mqtt_driver.h"
#include "wifi.h"

// i2c
#include "i2c.h"

// joy
#include "joy.h"

#include "btnqueue.h"
#include "coms.h"
#include "tictac.h"

#include "string.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
// static void _app_task(void *p_parameter);

/*--------------------------- VARIABLES --------------------------------------*/
/*--------------------------- STATIC FUNCTIONS -------------------------------*/

void _app_task(void *p_parameter)
{
    (void)p_parameter;

    double temp;
    double hum;
    float xacc;
    float yacc;
    float zacc;

    char buffer[20];

    for (;;) {
        i2c_start_measurement();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        read_temp(&temp, &hum);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        xacc = SPI_get_X();
        yacc = SPI_get_Y();
        zacc = SPI_get_Z();
        // int Xjoy = adc_read_x();
        // int Yjoy = adc_read_y();
        // printf("x:%d y:%d\n", Xjoy, Yjoy);
        send_sensor_data((int)temp, (int)hum, xacc, yacc, zacc);

        sprintf(buffer, "Temp: %d", (int)temp);
        lv_label_set_text(ui_TempLabel, buffer);
        sprintf(buffer, "Hum: %d", (int)temp);
        lv_label_set_text(ui_HumLabel, buffer);

        sprintf(buffer, "Acc: (%.1f,%.1f,%.1f)", xacc, yacc, zacc);
        lv_label_set_text(ui_AccLabel, buffer);
        // EARTHQUARE DETECTION
        if (zacc > 1 || zacc < -1) {
            send_SOS();
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        // JOYSTICK
        int xadc = adc_read_x();
        int yadc = adc_read_y();
        if (yadc > 90) {
            xQueueSend(btn_queue, &btn_up, 0);
        } else if (yadc < 10) {
            xQueueSend(btn_queue, &btn_down, 0);
        } else if (xadc > 90) {
            xQueueSend(btn_queue, &btn_left, 0);
        } else if (xadc < 10) {
            xQueueSend(btn_queue, &btn_right, 0);
        }
    }
}

/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/

void app_init(void)
{
    ui_init();

    button_init();
    led_init(0);
    led_init(1);
    led_init(2);
    led_init(3);

    led_on_pwm_pattern(LED_ID_BLUE, 100, 800, 200);

    wifi_init();

    i2c_init();

    // send_SOS();
    adc_init();
    SPI_init();
    SPI_write(0x20, 0b01110111);
    SPI_write(0x23, 0b00110000);

    xTaskCreatePinnedToCore(_button_task, "btn", 4096 * 2, NULL, 0, NULL, 0);
}

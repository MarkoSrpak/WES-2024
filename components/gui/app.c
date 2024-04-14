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
#include "tictac.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
// static void _app_task(void *p_parameter);

/*--------------------------- VARIABLES --------------------------------------*/
/*--------------------------- STATIC FUNCTIONS -------------------------------*/

static void _app_task(void *p_parameter)
{
    (void)p_parameter;
    button_init();
    wifi_init();

    // int indexX[] = {0, 4, -1};
    // int indexO[] = {1, 2, -1};
    // bool is_server = "true";
    //  send_game_data(indexX, indexO, is_server);

    wifi_init();
    // wifi_provision();
    draw_board();
    wifi_connect();

    i2c_init();
    double temp;
    double hum;

    // send_SOS();
    adc_init();
    SPI_init();
    SPI_write(0x20, 0b01110111);
    SPI_write(0x23, 0b00110000);

    for (;;) {
        // send_sensor_data(25, 23, 0.5, -0.3, 0.1);
        printf("Hello world\n");
        // int Xjoy = adc_read_x();
        // int Yjoy = adc_read_y();
        // printf("x:%d y:%d\n", Xjoy, Yjoy);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/

void app_init(void)
{
    ui_init();
    xTaskCreatePinnedToCore(_app_task, "app", 4096 * 2, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(_button_task, "btn", 4096 * 2, NULL, 0, NULL, 0);
}

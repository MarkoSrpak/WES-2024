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

// lvgl
#include "lvgl.h"
#include "lvgl_helpers.h"

// squareline
#include "ui.h"

// wifi
#include "mqtt_driver.h"
#include "wifi.h"

#include "led_pwm.h"

#include "coms.h"

#include "accelerometer.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
static void _app_task(void *p_parameter);
/*--------------------------- VARIABLES --------------------------------------*/
/*--------------------------- STATIC FUNCTIONS -------------------------------*/

static void _app_task(void *p_parameter)
{
    (void)p_parameter;

    send_sensor_data(25, 23, 0.5, -0.3, 0.1);

    int indexX[] = {0, 4, -1};
    int indexO[] = {1, 2, -1};
    bool is_server = "true";
    send_game_data(indexX, indexO, is_server);

    wifi_init();
    wifi_provision();
    for (;;) {
        uint8_t X_L = SPI_read(0x28);
        uint8_t X_H = SPI_read(0x29);

        int16_t X = (X_H << 8) | (X_L);
        vTaskDelay(100 / portTICK_PERIOD_MS);

        uint8_t Y_L = SPI_read(0x2A);
        uint8_t Y_H = SPI_read(0x2B);
        int16_t Y = (Y_H << 8) | (Y_L);
        vTaskDelay(100 / portTICK_PERIOD_MS);

        uint8_t Z_L = SPI_read(0x2C);
        uint8_t Z_H = SPI_read(0x2D);
        int16_t Z = (Z_H << 8) | (Z_L);
        vTaskDelay(100 / portTICK_PERIOD_MS);

        printf("x: %d y: %d z: %d\n", X, Y, Z);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/

void app_init(void)
{
    ui_init();
    xTaskCreatePinnedToCore(_app_task, "app", 4096 * 2, NULL, 0, NULL, 0);
}

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
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
static void _app_task(void *p_parameter);

/*--------------------------- VARIABLES --------------------------------------*/
/*--------------------------- STATIC FUNCTIONS -------------------------------*/

static void _app_task(void *p_parameter)
{
    (void)p_parameter;

    int indexX[] = {0, 4, -1};
    int indexO[] = {1, 2, -1};
    bool is_server = "true";
    // send_game_data(indexX, indexO, is_server);

    wifi_init();
    wifi_provision();

    for (;;) {
        send_sensor_data(25, 23, 0.5, -0.3, 0.1);
        printf("Hello world\n");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/

void app_init(void)
{
    ui_init();
    xTaskCreatePinnedToCore(_app_task, "app", 4096 * 2, NULL, 0, NULL, 0);
}

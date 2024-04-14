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
#include "btnqueue.h"

#include "button.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <stdio.h>

// lvgl
#include "lvgl.h"
#include "lvgl_helpers.h"
// squareline
#include "../app.h"
#include "led_pwm.h"
#include "mqtt_driver.h"
#include "tictac.h"
#include "ui.h"
#include "wifi.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
QueueHandle_t btn_queue;
int btn_up = BTN_UP;
int btn_right = BTN_RIGHT;
int btn_down = BTN_DOWN;
int btn_left = BTN_LEFT;
int btn_start = BTN_START;
int btn_prov = BTN_PROV;
int btn_conn = BTN_CONN;
int btn_play = BTN_PLAY;
int btn_back = BTN_BACK;
int btn_p0 = BTN_P0;
int btn_p1 = BTN_P1;
int btn_p2 = BTN_P2;
int btn_p3 = BTN_P3;
int btn_p4 = BTN_P4;
int btn_p5 = BTN_P5;
int btn_p6 = BTN_P6;
int btn_p7 = BTN_P7;
int btn_p8 = BTN_P8;
int btn_player = BTN_PLAYER;
int btn_reset = BTN_RESET;

int game_state = SCREEN1;
int curr_type = 1;
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
void _button_task(void *p_parameter)
{
    (void)p_parameter;
    btn_queue = xQueueCreate(1, sizeof(int));
    int pressed_button;
    for (;;) {
        xQueueReceive(btn_queue, &pressed_button, portMAX_DELAY);
        switch (game_state) {
        case SCREEN1 :
            switch (pressed_button) {
            case BTN_RIGHT :
            case BTN_CONN :
                wifi_connect();
                mqtt_init();
                mqtt_subscribe_topic("WES/Jupiter/game");
                lv_obj_set_style_bg_grad_color(ui_StartButton,
                                               lv_color_hex(0xE63451),
                                               LV_PART_MAIN | LV_STATE_DEFAULT);
                game_state = SCREEN1_CONNECTED;
                // _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_MOVE_LEFT,
                // 500, 0, &ui_Screen3_screen_init);

                // _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_MOVE_RIGHT,
                // 500, 0, &ui_Screen2_screen_init);
                break;
            case BTN_LEFT :
            case BTN_PROV :
                wifi_provision();
                mqtt_init();
                mqtt_subscribe_topic("WES/Jupiter/game");
                lv_obj_set_style_bg_grad_color(ui_StartButton,
                                               lv_color_hex(0xE63451),
                                               LV_PART_MAIN | LV_STATE_DEFAULT);
                game_state = SCREEN1_CONNECTED;
                break;
            default :
                break;
            }
            break;
        case SCREEN1_CONNECTED :
            switch (pressed_button) {
            case BTN_UP :
            case BTN_START :
                // switch ekran
                xTaskCreatePinnedToCore(_app_task, "app", 4096 * 2, NULL, 0,
                                        NULL, 0);
                mqtt_subscribe_topic("WES/Jupiter/game");
                _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500,
                                  0, &ui_Screen2_screen_init);
                game_state = SCREEN2;
                break;

            default :
                break;
            }
            break;
        case SCREEN2 :
            switch (pressed_button) {
            case BTN_RIGHT :
            case BTN_PLAY :
                _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500,
                                  0, &ui_Screen3_screen_init);
                game_state = SCREEN3;
                break;
            case BTN_DOWN :
                led_on_pwm_pattern(1, 50, 250, 250);
                break;
            default :
                break;
            }
            break;
        case SCREEN3 :
            switch (pressed_button) {
            case BTN_LEFT :
            case BTN_BACK :
                _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500,
                                  0, &ui_Screen2_screen_init);
                game_state = SCREEN2;
                break;
            case BTN_RIGHT :
            case BTN_RESET :
                reset_board();
                curr_type = 1;
                break;
            case BTN_P0 :
                add_piece(0, curr_type);
                break;
            case BTN_P1 :
                add_piece(1, curr_type);
                break;
            case BTN_P2 :
                add_piece(2, curr_type);
                break;
            case BTN_P3 :
                add_piece(3, curr_type);
                break;
            case BTN_P4 :
                add_piece(4, curr_type);
                break;
            case BTN_P5 :
                add_piece(5, curr_type);
                break;
            case BTN_P6 :
                add_piece(6, curr_type);
                break;
            case BTN_P7 :
                add_piece(7, curr_type);
                break;
            case BTN_P8 :
                add_piece(8, curr_type);
                break;
            case BTN_PLAYER :
                curr_type = 2;
                send_empty();
                break;
            default :
                break;
            }
            break;
        default :
            break;
        }
    }
}

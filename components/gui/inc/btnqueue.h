/**
 *    ____    __    ____  _______     _______.      ___     ___    ___    _  _
 *    \   \  /  \  /   / |   ____|   /       |     |__ \   / _ \  |__ \  | || |
 *     \   \/    \/   /  |  |__     |   (----` ______ ) | | | | |    ) | | || |_
 *      \            /   |   __|     \   \    |______/ /  | | | |   / /  |__ _|
 *       \    /\    /    |  |____.----)   |         / /_  | |_| |  / /_     | |
 *        \__/  \__/     |_______|_______/         |____|  \___/  |____|    |_|
 *
 */

#ifndef INC_BTNQUEUE_H
#define INC_BTNQUEUE_H

/*--------------------------- INCLUDES ---------------------------------------*/
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
enum all_btns {
    BTN_UP,
    BTN_RIGHT,
    BTN_DOWN,
    BTN_LEFT,
    BTN_START,
    BTN_PROV,
    BTN_CONN,
    BTN_PLAY,
    BTN_BACK,
    BTN_P0,
    BTN_P1,
    BTN_P2,
    BTN_P3,
    BTN_P4,
    BTN_P5,
    BTN_P6,
    BTN_P7,
    BTN_P8,
    BTN_PLAYER,
    BTN_RESET,
};
enum game_states {
    SCREEN1,
    SCREEN2,
    SCREEN3,
    SCREEN1_CONNECTED,
};
/*--------------------------- EXTERN -----------------------------------------*/
extern QueueHandle_t btn_queue;
extern int btn_up;
extern int btn_down;
extern int btn_left;
extern int btn_right;
extern int btn_start;
extern int btn_prov;
extern int btn_conn;
extern int btn_play;
extern int btn_back;
extern int btn_p0;
extern int btn_p1;
extern int btn_p2;
extern int btn_p3;
extern int btn_p4;
extern int btn_p5;
extern int btn_p6;
extern int btn_p7;
extern int btn_p8;
extern int btn_player;
extern int btn_reset;

extern int game_state;
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/
void _button_task(void *p_parameter);

#endif /*INC_BTNQUEUE_H*/

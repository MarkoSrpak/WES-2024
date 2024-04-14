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
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <stdbool.h>
#include <stdio.h>
// lvgl
#include "lvgl.h"
#include "lvgl_helpers.h"
// squareline
#include "coms.h"
#include "ui.h"
#include "wifi.h"

#include "tictac.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
uint8_t board[9] = {1, 1, 1, 1, 2, 2, 1, 2, 1};
// 0 prazno, 1 je X, 2 je O
int indexX_moves[10] = {
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1}; // Initialize with -1 to ensure termination
int indexO_moves[10] = {
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1}; // Initialize with -1 to ensure termination
lv_obj_t *img_objects[9];
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
// Function to check if a player has won
bool check_win(int player)
{
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i * 3] == player && board[i * 3 + 1] == player
            && board[i * 3 + 2] == player) {
            return true;
        }
    }

    // Check columns
    for (int i = 0; i < 3; i++) {
        if (board[i] == player && board[i + 3] == player
            && board[i + 6] == player) {
            return true;
        }
    }

    // Check diagonals
    if (board[0] == player && board[4] == player && board[8] == player) {
        return true;
    }
    if (board[2] == player && board[4] == player && board[6] == player) {
        return true;
    }

    return false;
}

// Function to check if the game is a draw
bool check_draw()
{
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) {
            return false; // There's still an empty space, game is not draw
        }
    }
    return true; // All spaces are filled, game is draw
}

// Function to determine the winner of the game
int determine_winner()
{
    if (check_win(1)) {
        return 1; // Player X wins
    } else if (check_win(2)) {
        return 2; // Player O wins
    } else if (check_draw()) {
        return -1; // It's a draw
    } else {
        return 0; // Game is still ongoing
    }
}
int send_empty()
{
    int indexX_moves[10] = {-1}; // Initialize with -1 to ensure termination
    int indexO_moves[10] = {-1}; // Initialize with -1 to ensure termination
    int indexX = 0, indexO = 0;

    send_game_data(indexX_moves, indexO_moves, 1);
    return 0;
}
int add_piece(int position, int type)
{
    if (board[position] == 0) {
        board[position] = type;
    }
    draw_board();
    memset(indexX_moves, -1, sizeof(indexX_moves));
    memset(indexO_moves, -1, sizeof(indexO_moves));
    int indexXa = 0, indexOa = 0;

    for (int i = 0; i < 9; i++) {
        if (board[i] == 1) {
            indexX_moves[indexXa++] = i;
        } else if (board[i] == 2) {
            indexO_moves[indexOa++] = i;
        }
    }
    send_game_data(indexX_moves, indexO_moves, 1);
    return 0;
}
int reset_board()
{
    memset(board, 0, sizeof(board));
    draw_board();
    return 0;
}
int draw_board()
{
    for (int i = 0; i < 9; i++) {
        draw_field(i, board[i]);
    }
    return 0;
}
int draw_field(int field_num, int type)
{
    lv_obj_t *button = NULL;
    lv_img_dsc_t *image_descriptor = NULL;

    // Determine which button to use based on field_num
    switch (field_num) {
    case 0 :
        button = ui_Button0;
        break;
    case 1 :
        button = ui_Button1;
        break;
    case 2 :
        button = ui_Button2;
        break;
    case 3 :
        button = ui_Button3;
        break;
    case 4 :
        button = ui_Button4;
        break;
    case 5 :
        button = ui_Button5;
        break;
    case 6 :
        button = ui_Button6;
        break;
    case 7 :
        button = ui_Button7;
        break;
    case 8 :
        button = ui_Button8;
        break;
    default :
        // Invalid field_num
        return -1;
    }

    // Determine which image to use based on type
    switch (type) {
    case 0 :
        // Empty field, no image
        image_descriptor = NULL;
        break;
    case 1 :
        // X image
        image_descriptor = &ui_img_x_icon2_png;
        break;
    case 2 :
        // O image
        image_descriptor = &ui_img_o_icon3_png;
        break;
    default :
        // Invalid type
        return -2;
    }

    // Remove any existing image from the button
    remove_image_from_button(button);

    // If type is not 0 (empty), add the image to the button
    if (type != 0) {
        add_image_to_button(button, image_descriptor, field_num);
    }

    return 0; // Success
}

void add_image_to_button(lv_obj_t *button, lv_img_dsc_t *image_descriptor,
                         int field_num)
{
    img_objects[field_num] = lv_img_create(button);
    lv_img_set_src(img_objects[field_num], image_descriptor);
    lv_obj_set_width(img_objects[field_num], LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(img_objects[field_num], LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(img_objects[field_num], LV_ALIGN_CENTER);
    lv_obj_add_flag(img_objects[field_num], LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
    lv_obj_clear_flag(img_objects[field_num], LV_OBJ_FLAG_SCROLLABLE); /// Flags
}

void remove_image_from_button(lv_obj_t *button)
{
    // Get the child count of the button
    uint32_t child_cnt = lv_obj_get_child_cnt(button);
    // Iterate through the children
    for (uint32_t i = 0; i < child_cnt; i++) {
        // Get the child object
        lv_obj_t *child = lv_obj_get_child(button, i);
        if (NULL != child) {
            lv_obj_del(child);
            break;
        }
    }
}

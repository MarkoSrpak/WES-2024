/**
 *    ____    __    ____  _______     _______.      ___     ___    ___    _  _
 *    \   \  /  \  /   / |   ____|   /       |     |__ \   / _ \  |__ \  | || |
 *     \   \/    \/   /  |  |__     |   (----` ______ ) | | | | |    ) | | || |_
 *      \            /   |   __|     \   \    |______/ /  | | | |   / /  |__ _|
 *       \    /\    /    |  |____.----)   |         / /_  | |_| |  / /_     | |
 *        \__/  \__/     |_______|_______/         |____|  \___/  |____|    |_|
 *
 */

#ifndef INC_TICTAC_H
#define INC_TICTAC_H

/*--------------------------- INCLUDES ---------------------------------------*/
#include "ui.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- EXTERN -----------------------------------------*/
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/
int reset_board();
int draw_board();
int draw_field(int field_num, int type);
void add_image_to_button(lv_obj_t *button, lv_img_dsc_t *image_descriptor,
                         int field_num);
void remove_image_from_button(lv_obj_t *button);
int send_empty();
int add_piece(int position, int type);
bool check_win(int player);
bool check_draw();
int determine_winner();

#endif /*INC_TICTAC_H*/

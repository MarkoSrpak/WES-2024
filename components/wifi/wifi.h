/**
 *    ____    __    ____  _______     _______.      ___     ___    ___    _  _
 *    \   \  /  \  /   / |   ____|   /       |     |__ \   / _ \  |__ \  | || |
 *     \   \/    \/   /  |  |__     |   (----` ______ ) | | | | |    ) | | || |_
 *      \            /   |   __|     \   \    |______/ /  | | | |   / /  |__ _|
 *       \    /\    /    |  |____.----)   |         / /_  | |_| |  / /_     | |
 *        \__/  \__/     |_______|_______/         |____|  \___/  |____|    |_|
 *
 */

#ifndef WIFI_WIFI_H
#define WIFI_WIFI_H

/*--------------------------- INCLUDES ---------------------------------------*/
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- EXTERN -----------------------------------------*/
extern time_t now;
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/

int wifi_init();
int wifi_connect();
int wifi_provision();
void show_current_time(char *strftime_buf);
void get_current_time_from_rtc(struct tm *timeinfo);

#endif /*WIFI_WIFI_H*/

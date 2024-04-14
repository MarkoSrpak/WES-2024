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
#include <cJSON.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "mqtt_driver.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
int send_sensor_data(int temp, int hum, float x, float y, float z)
{
    cJSON *root, *acc;
    root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "temp", temp);
    cJSON_AddNumberToObject(root, "hum", hum);

    acc = cJSON_CreateObject();

    /* cJSON_AddNumberToObject(acc, "x", x);
     cJSON_AddNumberToObject(acc, "y", y);
     cJSON_AddNumberToObject(acc, "z", z);
    */

    // Limit to one decimal
    char x_str[20], y_str[20], z_str[20];
    snprintf(x_str, 20, "%.1f", x);
    snprintf(y_str, 20, "%.1f", y);
    snprintf(z_str, 20, "%.1f", z);

    cJSON_AddStringToObject(acc, "x", x_str);
    cJSON_AddStringToObject(acc, "y", y_str);
    cJSON_AddStringToObject(acc, "z", z_str);

    cJSON_AddItemToObject(root, "acc", acc);

    char *json_data = cJSON_Print(root);

    cJSON_Delete(root);

    // Print JSON data for testing
    printf("JSON Data: %s\n", json_data);

    // Do something with json_data
    mqtt_publish("WES/Jupiter/sensors", json_data);

    free(json_data);

    return 0;
}

int send_game_data(int *indexX_moves, int *indexO_moves, bool is_server)
{
    cJSON *root, *indexX, *indexO;

    root = cJSON_CreateObject();
    indexX = cJSON_CreateArray();
    indexO = cJSON_CreateArray();

    for (int i = 0; indexX_moves[i] != -1; ++i) {
        cJSON_AddItemToArray(indexX, cJSON_CreateNumber(indexX_moves[i]));
    }
    for (int i = 0; indexO_moves[i] != -1; ++i) {
        cJSON_AddItemToArray(indexO, cJSON_CreateNumber(indexO_moves[i]));
    }

    cJSON_AddItemToObject(root, "indexX", indexX);
    cJSON_AddItemToObject(root, "indexO", indexO);

    cJSON_AddStringToObject(root, "turn", is_server ? "server" : "device");

    char *json_data = cJSON_Print(root);

    // Print JSON data for testing
    printf("JSON Data: %s\n", json_data);

    mqtt_publish("WES/Jupiter/game", json_data);

    cJSON_Delete(root);
    free(json_data);

    return 0;
}

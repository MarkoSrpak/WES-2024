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

    cJSON_AddNumberToObject(acc, "x", x);
    cJSON_AddNumberToObject(acc, "y", y);
    cJSON_AddNumberToObject(acc, "z", z);

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

    /*   for (int i = 0; i < 9; ++i) {
           cJSON_AddItemToArray(indexX, cJSON_CreateNumber(indexX_moves[i]));
       }
       for (int i = 0; i < 9; ++i) {
           cJSON_AddItemToArray(indexO, cJSON_CreateNumber(indexO_moves[i]));
       }
    */
    cJSON_AddItemToObject(root, "indexX", indexX);
    cJSON_AddItemToObject(root, "indexO", indexO);

    if (is_server) {
        char *turn = "server";
        cJSON_AddStringToObject(root, "turn", turn);
    }

    char *json_data = cJSON_Print(root);

    // Print JSON data for testing
    printf("JSON Data: %s\n", json_data);

    cJSON_Delete(root);
    free(json_data);

    return 0;
}

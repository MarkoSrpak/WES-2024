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
#include <string.h>

#include "mqtt_driver.h"
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
void parse_game_data(const char *json_data, int **indexX_moves,
                     int **indexO_moves, bool *is_server)
{
    cJSON *root = cJSON_Parse(json_data);
    if (root != NULL) {
        cJSON *indexX = cJSON_GetObjectItemCaseSensitive(root, "indexX");
        cJSON *indexO = cJSON_GetObjectItemCaseSensitive(root, "indexO");
        cJSON *turn = cJSON_GetObjectItemCaseSensitive(root, "turn");

        if (indexX != NULL && cJSON_IsArray(indexX) && indexO != NULL
            && cJSON_IsArray(indexO) && turn != NULL && cJSON_IsString(turn)) {
            int x_count = cJSON_GetArraySize(indexX);
            int o_count = cJSON_GetArraySize(indexO);

            // Allocate memory for indexX_moves and indexO_moves arrays
            *indexX_moves = (int *)malloc(
                (x_count + 1)
                * sizeof(int)); // Add extra space for terminating -1
            *indexO_moves = (int *)malloc(
                (o_count + 1)
                * sizeof(int)); // Add extra space for terminating -1

            // Parse indexX_moves array
            for (int i = 0; i < x_count; ++i) {
                cJSON *item = cJSON_GetArrayItem(indexX, i);
                if (cJSON_IsNumber(item)) {
                    (*indexX_moves)[i] = item->valueint;
                }
            }
            (*indexX_moves)[x_count] = -1; // Terminate with -1

            // Parse indexO_moves array
            for (int i = 0; i < o_count; ++i) {
                cJSON *item = cJSON_GetArrayItem(indexO, i);
                if (cJSON_IsNumber(item)) {
                    (*indexO_moves)[i] = item->valueint;
                }
            }
            (*indexO_moves)[o_count] = -1; // Terminate with -1

            // Parse turn
            if (strcmp(turn->valuestring, "server") == 0) {
                *is_server = true;
            } else if (strcmp(turn->valuestring, "device") == 0) {
                *is_server = false;
            }

        } else {
            // Invalid JSON structure
            printf("Invalid JSON structure!\n");
        }

        cJSON_Delete(root);
    } else {
        // JSON parsing error
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("JSON parsing error before: %s\n", error_ptr);
        } else {
            printf("Unknown JSON parsing error\n");
        }
    }
}

void on_receive_cb(const char *p_topic, const char *p_data)
{
    // Parse the received JSON data
    int *indexX_moves = NULL;
    int *indexO_moves = NULL;
    bool is_server = false;
    parse_game_data(p_data, &indexX_moves, &indexO_moves, &is_server);

    // Use the parsed data as needed
    if (indexX_moves != NULL && indexO_moves != NULL) {
        printf("Received indexX_moves: ");
        for (int i = 0; indexX_moves[i] != -1; ++i) {
            printf("%d ", indexX_moves[i]);
        }
        printf("\n");

        printf("Received indexO_moves: ");
        for (int i = 0; indexO_moves[i] != -1; ++i) {
            printf("%d ", indexO_moves[i]);
        }
        printf("\n");

        printf("Received turn: %s\n", is_server ? "server" : "device");

        // Free the allocated memory for indexX_moves and indexO_moves
        free(indexX_moves);
        free(indexO_moves);
    }
}

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

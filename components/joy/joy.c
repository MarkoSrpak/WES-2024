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
#include "joy.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*--------------------------- MACROS AND DEFINES -----------------------------*/
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
static int adc_raw[2][10];
adc_oneshot_unit_handle_t adc1_handle;
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
int adc_init(void)
{
    //-------------ADC1 Init---------------//
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = EXAMPLE_ADC_ATTEN,
    };
    adc_oneshot_config_channel(adc1_handle, ADC1_CHAN6, &config);
    adc_oneshot_config_channel(adc1_handle, ADC1_CHAN7, &config);
    return 0;
}

int adc_read_x()
{
    adc_oneshot_read(adc1_handle, ADC1_CHAN6, &adc_raw[0][0]);
    return ((adc_raw[0][0] * 100) / 4095);
}

int adc_read_y()
{
    adc_oneshot_read(adc1_handle, ADC1_CHAN7, &adc_raw[0][1]);
    return ((adc_raw[0][1] * 100) / 4095);
}

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
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include <stdio.h>
/*--------------------------- MACROS AND DEFINES -----------------------------*/
#define I2C_MASTER_SCL_IO         21U /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO         22U /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM            0
#define I2C_MASTER_FREQ_HZ        400000 /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0      /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0      /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS     1000

#define SENSOR_ADDR 0x44

/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
/*--------------------------- STATIC FUNCTIONS -------------------------------*/
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/

int i2c_start_measurement()
{
    // starta measurement temperature
    uint8_t write_buf[2] = {0x2C, 0x10};
    i2c_master_write_to_device(I2C_MASTER_NUM, SENSOR_ADDR, write_buf,
                               sizeof(write_buf),
                               I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    return 0;
}

int read_temp(uint16_t *temp, uint16_t *humidity)
{
    /*  uint8_t write_buf[2] = {0xE0, 0x00};
      i2c_master_write_to_device(I2C_MASTER_NUM, SENSOR_ADDR, write_buf,
                                 sizeof(write_buf),
                               I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
*/
    uint8_t read_buffer[6];
    // citamo 6 bytova
    i2c_master_read_from_device(I2C_MASTER_NUM, SENSOR_ADDR, read_buffer, 6,
                                I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    printf("%x:%x:%x::%x:%x:%x\n", read_buffer[0], read_buffer[1],
           read_buffer[2], read_buffer[3], read_buffer[4], read_buffer[5]);
    uint16_t temp_r = (uint16_t)(read_buffer[0] << 8) | (read_buffer[1]);
    uint16_t humi_r = (uint16_t)(read_buffer[3] << 8) | (read_buffer[4]);

    *temp = (175 * temp_r / ((2 ^ 16) - 1) - 45);
    *humidity = (100 * (humi_r / ((2 ^ 16) - 1)));

    return 0;
}

esp_err_t i2c_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode,
                              I2C_MASTER_RX_BUF_DISABLE,
                              I2C_MASTER_TX_BUF_DISABLE, 0);
}

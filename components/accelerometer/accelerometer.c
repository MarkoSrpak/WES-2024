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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>

#include "esp_log.h"

#include "driver/gpio.h"

#include "accelerometer.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "hal/spi_types.h"

/*--------------------------- MACROS AND DEFINES -----------------------------*/
#define PIN_NUM_MOSI 23
#define PIN_NUM_MISO 19
#define PIN_NUM_SCK  18
#define PIN_NUM_CS   13

#define CLK_SPEED_SPI 1000 * 100
/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
/*--------------------------- STATIC FUNCTION PROTOTYPES ---------------------*/
/*--------------------------- VARIABLES --------------------------------------*/
/*--------------------------- STATIC FUNCTIONS -------------------------------*/

static spi_device_interface_config_t spi_device_interface_config = {
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode = 0,
    .clock_speed_hz = CLK_SPEED_SPI, // 10kHz
    .spics_io_num = PIN_NUM_CS,
    .queue_size = 1, // nebitno :)

};

static spi_device_handle_t spi_device_handle;
// Local function declarations
/*--------------------------- GLOBAL FUNCTIONS -------------------------------*/
void SPI_init()
{
    esp_err_t ret;

    ret = spi_bus_add_device(SPI3_HOST, &spi_device_interface_config,
                             &spi_device_handle);
    ESP_ERROR_CHECK(ret);
    gpio_set_level(PIN_NUM_CS, 1);
}

uint8_t SPI_transaction(uint8_t address, uint8_t rwb, uint8_t data)
{
    uint8_t transmit_buffer[2];
    memset(&transmit_buffer, 0, sizeof(transmit_buffer));
    transmit_buffer[0] = ((rwb & 0x01) << 7) | (0 << 6) | (address & 0x3F);
    transmit_buffer[1] = data & 0xFF;
    // printf("trans:%d:%d\n", transmit_buffer[0], transmit_buffer[1]);
    uint8_t recieve_buffer[2];
    memset(&recieve_buffer, 0, sizeof(recieve_buffer));
    spi_transaction_t spi_transaction = {//   .flags = SPI_TRANS_USE_RXDATA,
                                         .length = 16,
                                         .rxlength = 16,
                                         .tx_buffer = &transmit_buffer,
                                         .rx_buffer = &recieve_buffer};
    esp_err_t ret;
    ret = spi_device_polling_transmit(spi_device_handle, &spi_transaction);
    ESP_ERROR_CHECK(ret);
    // printf("recv:%d:%d\n", recieve_buffer[0], recieve_buffer[1]);
    uint8_t recieved_data = 0;
    recieved_data = recieve_buffer[1];

    return recieved_data;
}

void SPI_write(uint8_t address, uint8_t data)
{
    SPI_transaction(address, 0, data);
}

uint8_t SPI_read(uint8_t address)
{
    return SPI_transaction(address, 1, 0x00);
}

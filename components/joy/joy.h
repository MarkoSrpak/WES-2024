/**
 *    ____    __    ____  _______     _______.      ___     ___    ___    _  _
 *    \   \  /  \  /   / |   ____|   /       |     |__ \   / _ \  |__ \  | || |
 *     \   \/    \/   /  |  |__     |   (----` ______ ) | | | | |    ) | | || |_
 *      \            /   |   __|     \   \    |______/ /  | | | |   / /  |__ _|
 *       \    /\    /    |  |____.----)   |         / /_  | |_| |  / /_     | |
 *        \__/  \__/     |_______|_______/         |____|  \___/  |____|    |_|
 *
 */

#ifndef joy_H
#define joy_H

/*--------------------------- INCLUDES ---------------------------------------*/
/*--------------------------- MACROS AND DEFINES -----------------------------*/
#define JOYSTICK_X (34u)
#define JOYSTICK_Y (35u)

#define ADC1_CHAN6 ADC_CHANNEL_6 // channel for X
#define ADC1_CHAN7 ADC_CHANNEL_7 // channel for Y

#define EXAMPLE_ADC_ATTEN ADC_ATTEN_DB_11

/*--------------------------- TYPEDEFS AND STRUCTS ---------------------------*/
int adc_init(void);
int adc_read_x();
int adc_read_y();
/*--------------------------- EXTERN -----------------------------------------*/
/*--------------------------- GLOBAL FUNCTION PROTOTYPES ---------------------*/

#endif /*joy_H*/

/**
 * Keil project example for GPS parsing
 *
 * @note      Check defines.h file for configuration settings!
 *
 * Before you start, select your target, on the right of the "Load" button
 *
 * @author    Tilen Majerle
 * @email     tilen@majerle.eu
 * @website   http://esp8266at.com
 * @ide       Keil uVision 5
 * @conf      PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 * @packs     STM32F4xx/STM32F7xx Keil packs are requred with HAL driver support
 * @stdperiph STM32F4xx/STM32F7xx HAL drivers required
 *
 * This examples shows how you can read data from GPS device and parse it using GPS library.
 * It uses custom GPS NMEA sentence to parse user specific data
 */
/* Include core modules */
#include "stm32fxxx_hal.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32_disco.h"
#include "tm_stm32_delay.h"
#include "tm_stm32_usart.h"
#include "gps.h"

#define DEBUG_USART         USART2
#define DEBUG_USART_PP      TM_USART_PinsPack_1

#define GPS_USART           USART1
#define GPS_USART_PP        TM_USART_PinsPack_1

/* GPS Structure */
GPS_t GPS;
GPS_Result_t gpsRes;
GPS_Custom_t Custom;

int main(void) {
    TM_RCC_InitSystem();                                    /* Init system */
    HAL_Init();                                             /* Init HAL layer */
    TM_DISCO_LedInit();                                     /* Init leds */
    TM_DISCO_ButtonInit();                                  /* Init button */
    TM_DELAY_Init();                                        /* Init delay */
    TM_USART_Init(DEBUG_USART, DEBUG_USART_PP, 921600);     /* Init USART for debug, PA2 is TX output */
    
    printf("GPS NMEA parser; Compiled date and time: %s %s\r\n", __DATE__, __TIME__);
 
    /* GPS initialization, RX on STM is PA10, connect TX from GPS to this pin */
    TM_USART_Init(GPS_USART, GPS_USART_PP, 115200);         /* Init USART for GPS receive data */
    GPS_Init(&GPS);                                         /* Init GPS stack */
    
    /* Add custom statement parser, GPRMC sentence, parse as latitude or longitude, data number 3 in sentence */
    GPS_Custom_Add(&GPS, &Custom, "$GPRMC", 3, GPS_CustomType_LatLong);
    
    /* While loop */
    while (1) {
        gpsRes = GPS_Update(&GPS);                          /* Process GPS data */
        
        if (gpsRes == gpsNEWDATA) {                         /* Check for new pack of data */
            if (GPS.Valid) {                                /* Check valid GPS */
                printf("GPS is valid! Latitude: %f, Longitude: %f, Altitude: %f\r\n", 
                    GPS.Latitude, GPS.Longitude, GPS.Altitude);
                
                /* Print custom value, latitude/longitude print as latitude/longitude (float number) */
                printf("Custom $GPRMC.3 value: %f\r\n", Custom.Value.L);
            }
        }
    }
}

/* Custom USART1 IRQ handler for GPS receive data */
void TM_USART1_ReceiveHandler(uint8_t ch) {
    GPS_DataReceived(&ch, 1);                               /* Send character to GPS stack */
}

/* printf handler */
int fputc(int ch, FILE* fil) {
	TM_USART_Putc(DEBUG_USART, ch);                         /* Send over debug USART */
	return ch;                                              /* Return OK */
}

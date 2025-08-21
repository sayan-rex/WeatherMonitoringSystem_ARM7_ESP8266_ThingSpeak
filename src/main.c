#include <lpc214x.h>
#include <string.h>
#include <stdio.h>
#include "uart0.h"
#include "dht11.h"
#include "delay.h"

// Replace with your WiFi & ThingSpeak API details
#define WIFI_SSID     "YourSSID"
#define WIFI_PASS     "YourPassword"
#define API_KEY       "YOUR_API_KEY_HERE"

void esp8266_init(void);
void send_to_thingspeak(DHT11_Data *data);

int main(void) {
    DHT11_Data dht;

    // Initialize peripherals
    UART0_Init();        // UART0 for ESP8266
    delay_init();        // Timer for delays
    dht11_init();        // DHT11 sensor init

    // Initialize ESP8266 and connect to Wi-Fi
    esp8266_init();

    while (1) {
        if (dht11_read(&dht) == 0) {
            send_to_thingspeak(&dht);
        } else {
            UART0_SendString("DHT11 Read Error\r\n");
        }
        delay_ms(15000); // ThingSpeak: 15s update interval
    }
}

/**
 * @brief Initialize ESP8266 and connect to WiFi
 */
void esp8266_init(void) {
    UART0_SendString("AT\r\n");               // Test AT
    delay_ms(1000);

    UART0_SendString("AT+CWMODE=1\r\n");      // Set mode: Station
    delay_ms(1000);

    char wifi_cmd[100];
    sprintf(wifi_cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASS);
    UART0_SendString(wifi_cmd);
    delay_ms(6000); // Wait for WiFi connection
}

/**
 * @brief Send DHT11 data to ThingSpeak
 */
void send_to_thingspeak(DHT11_Data *data) {
    char http_cmd[200];
    char at_cmd[40];

    // Start TCP connection
    UART0_SendString("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    delay_ms(3000);

    // Format GET request with integer + decimal parts
    sprintf(http_cmd,
            "GET /update?api_key=%s&field1=%d.%d&field2=%d.%d\r\n",
            API_KEY,
            data->temp_int, data->temp_dec,
            data->hum_int, data->hum_dec);

    // Tell ESP8266 how many bytes we will send
    sprintf(at_cmd, "AT+CIPSEND=%d\r\n", strlen(http_cmd));
    UART0_SendString(at_cmd);
    delay_ms(1000);

    // Send actual GET request
    UART0_SendString(http_cmd);
    delay_ms(3000);
}

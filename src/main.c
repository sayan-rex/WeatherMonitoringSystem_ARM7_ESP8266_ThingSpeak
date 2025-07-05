#include <lpc214x.h>
#include <string.h>
#include <stdio.h>
#include "uart0.h"
#include "dht11.h"
#include "delay.h"

// Replace with your ThingSpeak API key
#define API_KEY "YOUR_API_KEY_HERE"

void esp8266_init(void);
void send_to_thingspeak(int temp, int hum);

int main(void) {
    int temperature = 0, humidity = 0;

    // Initialize peripherals
    UART0_Init();        // UART for ESP8266
    delay_init();        // Initialize timer for delay
    dht11_init();        // Initialize DHT11 sensor

    // Initialize ESP8266 and connect to Wi-Fi
    esp8266_init();

    while (1) {
        if (dht11_read(&temperature, &humidity) == 0) {
            send_to_thingspeak(temperature, humidity);
        }
        delay_ms(15000); // Delay to meet ThingSpeak update limit
    }
}

void esp8266_init(void) {
    UART0_SendString("AT\r\n");                     // Test AT
    delay_ms(1000);

    UART0_SendString("AT+CWMODE=1\r\n");            // Set mode to Station
    delay_ms(1000);

    UART0_SendString("AT+CWJAP=\"YourSSID\",\"YourPassword\"\r\n"); // Connect to Wi-Fi
    delay_ms(6000); // Longer delay for connection
}

void send_to_thingspeak(int temp, int hum) {
    char http_cmd[150];
    char at_cmd[30];

    // Start TCP connection
    UART0_SendString("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    delay_ms(3000);

    // Format GET request
    sprintf(http_cmd, "GET /update?api_key=%s&field1=%d&field2=%d\r\n", API_KEY, temp, hum);
    sprintf(at_cmd, "AT+CIPSEND=%d\r\n", strlen(http_cmd));

    // Send length of request
    UART0_SendString(at_cmd);
    delay_ms(1000);

    // Send actual GET request
    UART0_SendString(http_cmd);
    delay_ms(3000);
}

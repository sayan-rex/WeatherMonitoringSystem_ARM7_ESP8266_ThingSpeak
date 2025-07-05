#include <lpc214x.h>
#include "uart0.h"
#include "dht11.h"
#include "delay.h"

#define API_KEY "YOUR_API_KEY_HERE"

void send_to_thingspeak(int temp, int hum) {
    UART0_SendString("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    delay_ms(2000);

    char http_cmd[150];
    sprintf(http_cmd, "GET /update?api_key=%s&field1=%d&field2=%d\r\n", API_KEY, temp, hum);

    char cmd[30];
    sprintf(cmd, "AT+CIPSEND=%d\r\n", strlen(http_cmd));
    UART0_SendString(cmd);
    delay_ms(1000);
    UART0_SendString(http_cmd);
    delay_ms(3000);
}

int main() {
    int temperature = 0, humidity = 0;

    UART0_Init();
    dht11_init();
    delay_init();

    UART0_SendString("AT\r\n");
    delay_ms(1000);
    UART0_SendString("AT+CWMODE=1\r\n");
    delay_ms(1000);
    UART0_SendString("AT+CWJAP=\"YourSSID\",\"YourPassword\"\r\n");
    delay_ms(5000);

    while (1) {
        if (dht11_read(&temperature, &humidity) == 0) {
            send_to_thingspeak(temperature, humidity);
        }
        delay_ms(15000); // ThingSpeak limit
    }
}

#include <lpc214x.h>
#include "dht11.h"
#include "delay.h"

#define DHT_PIN (1 << 20)

void dht11_init() {
    IODIR0 |= DHT_PIN;
    IOSET0 |= DHT_PIN;
}

static void dht11_start_signal() {
    IODIR0 |= DHT_PIN;
    IOCLR0 = DHT_PIN;
    delay_ms(20);
    IOSET0 = DHT_PIN;
    delay_us(30);
    IODIR0 &= ~DHT_PIN;
}

static int dht11_check_response() {
    int retry = 100;
    while ((IOPIN0 & DHT_PIN) && retry--) delay_us(1);
    if (retry <= 0) return 1;
    retry = 100;
    while (!(IOPIN0 & DHT_PIN) && retry--) delay_us(1);
    if (retry <= 0) return 1;
    return 0;
}

static unsigned char dht11_read_byte() {
    unsigned char i, byte = 0;
    for (i = 0; i < 8; i++) {
        while (!(IOPIN0 & DHT_PIN));
        delay_us(30);
        if (IOPIN0 & DHT_PIN)
            byte = (byte << 1) | 1;
        else
            byte = (byte << 1);
        while (IOPIN0 & DHT_PIN);
    }
    return byte;
}

int dht11_read(int *temp, int *hum) {
    unsigned char rh_byte1, rh_byte2, temp_byte1, temp_byte2, checksum;

    dht11_start_signal();
    if (dht11_check_response() != 0) return 1;

    rh_byte1 = dht11_read_byte();
    rh_byte2 = dht11_read_byte();
    temp_byte1 = dht11_read_byte();
    temp_byte2 = dht11_read_byte();
    checksum = dht11_read_byte();

    if (checksum == (rh_byte1 + rh_byte2 + temp_byte1 + temp_byte2)) {
        *hum = rh_byte1;
        *temp = temp_byte1;
        return 0;
    } else {
        return 2;
    }
}

#include <lpc214x.h>
#include "dht11.h"
#include "delay.h"

#define DHT_PIN (1 << 20)

/**
 * @brief Initialize DHT11 pin (set as output, pull high)
 */
void dht11_init() {
    IODIR0 |= DHT_PIN;
    IOSET0 |= DHT_PIN;
}

/**
 * @brief Send start signal to DHT11 sensor
 */
static void dht11_start_signal() {
    IODIR0 |= DHT_PIN;        // Set pin as output
    IOCLR0 = DHT_PIN;         // Pull pin low
    delay_ms(20);             // Wait for at least 18 ms
    IOSET0 = DHT_PIN;         // Pull pin high
    delay_us(30);             // Wait for 20-40 us
    IODIR0 &= ~DHT_PIN;       // Set pin as input
}

/**
 * @brief Check for response from DHT11 sensor
 * @return 0 if response OK, 1 if timeout/error
 */
static int dht11_check_response() {
    int retry = 100;
    // Wait for DHT11 response: line low
    while ((IOPIN0 & DHT_PIN) && retry--) delay_us(1);
    if (retry <= 0) {
        // Error: No response (line did not go low)
        return 1;
    }
    retry = 100;
    // Wait for DHT11 response: line high
    while (!(IOPIN0 & DHT_PIN) && retry--) delay_us(1);
    if (retry <= 0) {
        // Error: No response (line did not go high)
        return 1;
    }
    return 0;
}

/**
 * @brief Read a byte from DHT11 sensor
 * @return 8-bit value
 */
static unsigned char dht11_read_byte() {
    unsigned char i, byte = 0;
    for (i = 0; i < 8; i++) {
        // Wait for pin to go high (start of bit)
        while (!(IOPIN0 & DHT_PIN));
        delay_us(30); // Wait to sample bit value (~30us)
        if (IOPIN0 & DHT_PIN)
            byte = (byte << 1) | 1; // Bit value is '1'
        else
            byte = (byte << 1);     // Bit value is '0'
        // Wait for pin to go low (end of bit)
        while (IOPIN0 & DHT_PIN);
    }
    return byte;
}

/**
 * @brief Read DHT11 sensor values
 * @param[out] temp_int Integer part of temperature
 * @param[out] temp_dec Decimal part of temperature
 * @param[out] hum_int Integer part of humidity
 * @param[out] hum_dec Decimal part of humidity
 * @return 0 if read OK, 1 if response error, 2 if checksum error
 */
int dht11_read(int *temp_int, int *temp_dec, int *hum_int, int *hum_dec) {
    unsigned char rh_byte1, rh_byte2, temp_byte1, temp_byte2, checksum;

    dht11_start_signal();
    if (dht11_check_response() != 0) {
        // Response error
#ifdef DHT11_DEBUG
        printf("DHT11: No response from sensor.\n");
#endif
        return 1;
    }

    rh_byte1     = dht11_read_byte(); // Humidity integer part
    rh_byte2     = dht11_read_byte(); // Humidity decimal part
    temp_byte1   = dht11_read_byte(); // Temperature integer part
    temp_byte2   = dht11_read_byte(); // Temperature decimal part
    checksum     = dht11_read_byte(); // Checksum

    if (checksum == (rh_byte1 + rh_byte2 + temp_byte1 + temp_byte2)) {
        *hum_int  = rh_byte1;
        *hum_dec  = rh_byte2;
        *temp_int = temp_byte1;
        *temp_dec = temp_byte2;
        return 0;
    } else {
        // Checksum error
#ifdef DHT11_DEBUG
        printf("DHT11: Checksum error. Data=[%d,%d,%d,%d] Checksum=%d\n", rh_byte1, rh_byte2, temp_byte1, temp_byte2, checksum);
#endif
        return 2;
    }
}
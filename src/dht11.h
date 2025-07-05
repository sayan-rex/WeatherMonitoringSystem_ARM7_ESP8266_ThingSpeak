#ifndef __DHT11_H
#define __DHT11_H

void dht11_init(void);
int dht11_read(int *temperature, int *humidity);

#endif

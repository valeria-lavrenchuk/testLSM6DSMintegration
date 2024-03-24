#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include "lsm6dsm_reg.h" // Assuming this is the header file provided by the LSM6DSM driver

#define I2C_BUS "/dev/i2c-1"
#define BOOT_TIME 20 // ms

int32_t platform_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len);
int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len);
void platform_delay(uint32_t ms);
void platform_init(void);

#endif /* PLATFORM_H */

#include "platform.h"
#include <stdio.h> 
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <sys/ioctl.h>

// use linux i2c bus. code for st i2s in i2cforst.c as it need a lot of dependencies
int32_t platform_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len)
{
    int file;
    (void)handle;
    uint8_t buffer[len + 1]; // Include space for register address

    // Open I2C device
    if ((file = open(I2C_BUS, O_RDWR)) < 0)
    {
        perror("Failed to open I2C device");
        return -1;
    }

    // Set slave address
    if (ioctl(file, I2C_SLAVE, LSM6DSM_I2C_ADD_L) < 0)
    {
        perror("Failed to set I2C slave address");
        close(file);
        return -1;
    }

    // Copy register address and data to buffer
    buffer[0] = Reg;
    for (int i = 0; i < len; i++)
    {
        buffer[i + 1] = Bufp[i];
    }

    // Write data to I2C bus
    if (write(file, buffer, len + 1) != len + 1)
    {
        perror("Failed to write to I2C bus");
        close(file);
        return -1;
    }

    close(file);
    return 0; // Return 0 for success
}

int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len)
{
    int file;
    (void)handle;
    // Open I2C device
    if ((file = open(I2C_BUS, O_RDWR)) < 0)
    {
        perror("Failed to open I2C device");
        return -1;
    }

    // Set slave address
    if (ioctl(file, I2C_SLAVE, LSM6DSM_I2C_ADD_L) < 0)
    {
        perror("Failed to set I2C slave address");
        close(file);
        return -1;
    }

    // Write register address to read from
    if (write(file, &Reg, 1) != 1)
    {
        perror("Failed to write register address");
        close(file);
        return -1;
    }

    // Read data from I2C bus
    if (read(file, Bufp, len) != len)
    {
        perror("Failed to read from I2C bus");
        close(file);
        return -1;
    }

    close(file);
    return 0; // Return 0 for success
}

// Platform-specific initialization function can be added to work with a real system
/*
 * @brief  platform specific delay (platform dependent)
 *
 * @param  ms        delay in ms
 *
 */
void platform_delay(uint32_t ms) {
    usleep(ms * 1000);  // Delay in microseconds, so multiply by 1000 to convert milliseconds to microseconds
}

/*
 * @brief  platform specific initialization (platform dependent)
 */
void platform_init(void)
{
    //TODO add a platform related itit
    // #if defined(STEVAL_MKI109V3)
    //   TIM3->CCR1 = PWM_3V3;
    //   TIM3->CCR2 = PWM_3V3;
    //   HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    //   HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    //   platform_delay(1000);
    // #endif
}

//  #include "stm32f4xx_hal.h" TODO use a real driver
// I2C_HandleTypeDef hi2c1; // I2C handler structure

// // example for ST i2c driver.

// void platform_i2c_init() {
//     // Initialize I2C peripheral
//     hi2c1.Instance = I2C1; // Example: Use I2C1 peripheral
//     hi2c1.Init.ClockSpeed = 400000; // Set I2C clock speed to 400 kHz
//     hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2; // Set I2C duty cycle to 2
//     hi2c1.Init.OwnAddress1 = 0; // Not used in master mode
//     hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT; // Use 7-bit addressing mode
//     hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // Disable dual addressing mode
//     hi2c1.Init.OwnAddress2 = 0; // Not used in master mode
//     hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; // Disable general call mode
//     hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE; // Enable clock stretching
//     if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
//         // Initialization error handling
//         Error_Handler();
//     }
// }

// int32_t platform_i2c_write(void *handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len) {
//     // Implement platform-specific I2C write function using HAL library
//     // Example:
//     // Transmit data to I2C slave device
//     if (HAL_I2C_Master_Transmit(&hi2c1, LSM6DSM_I2C_ADDRESS, &Reg, 1, HAL_MAX_DELAY) != HAL_OK) {
//         // Error handling
//         return -1;
//     }
//     if (HAL_I2C_Master_Transmit(&hi2c1, LSM6DSM_I2C_ADDRESS, (uint8_t *)Bufp, len, HAL_MAX_DELAY) != HAL_OK) {
//         // Error handling
//         return -1;
//     }
//     return 0; // Return 0 for success
// }

// int32_t platform_i2c_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len) {
//     // Implement platform-specific I2C read function using HAL library
//     // Example:
//     // Send register address to the I2C slave device
//     if (HAL_I2C_Master_Transmit(&hi2c1, LSM6DSM_I2C_ADDRESS, &Reg, 1, HAL_MAX_DELAY) != HAL_OK) {
//         // Error handling
//         return -1;
//     }
//     // Receive data from the I2C slave device
//     if (HAL_I2C_Master_Receive(&hi2c1, LSM6DSM_I2C_ADDRESS, Bufp, len, HAL_MAX_DELAY) != HAL_OK) {
//         // Error handling
//         return -1;
//     }
//     return 0; // Return 0 for success
// }

#include <stdio.h>       // Include standard I/O library for printf function
#include <string.h>
#include <stdint.h>
#include "platform.h"

#include "inclinometer.h"

/**
 * @brief Initializes the sensor driver (LSM6DSM).
 * 
 * @return Returns 0 on success, or a non-zero error code on failure.
 */
int init_sensor_driver()
{
    // Declare and initialize device context structure
    stmdev_ctx_t dev_ctx;
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;
    platform_init();
    // Initialize handle parameter if needed by the platform read and write functions
    // dev_ctx.handle = &platform_handle; // Uncomment and initialize if needed

    // Perform any platform-specific initialization here, if required

    // Initialize the LSM6DSM sensor using the device context
    // This initialization may involve writing to specific registers or configuring the sensor
    // Example:
    // Write configuration settings to the sensor
    uint8_t config_data[] = {0x00, 0x01, 0x02}; // Example configuration data
    platform_write(&dev_ctx, LSM6DSM_I2C_ADD_L, config_data, sizeof(config_data));

    // Check sensor identification register to verify initialization
    uint8_t sensor_id = 0;
    // platform_read(&dev_ctx, LSM6DSM_WHO_AM_I_REG, &sensor_id, 1);
    if (sensor_id == LSM6DSM_ID) {
        printf("LSM6DSM sensor initialized successfully\n");
        return 0; // Return success
    } else {
        printf("Error: Failed to initialize LSM6DSM sensor\n");
        return -1; // Return failure
    }
}

/**
 * @brief Reads sensor data.
 * 
 * @param acceleration Pointer to an array to store acceleration values (X, Y, Z axes).
 * @param angular_rate Pointer to an array to store angular velocity values (X, Y, Z axes).
 */
void read_sensor_data(float *acceleration, float *angular_rate)
{
    stmdev_ctx_t dev_ctx;
    uint8_t whoamI, rst;
    uint8_t tx_buffer[1000]; // Adjust buffer size as needed
    int16_t data_raw_acceleration[3];
    int16_t data_raw_angular_rate[3];
    float acceleration_mg[3];
    float angular_rate_mdps[3];

    // Initialize the device context structure
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;
    dev_ctx.mdelay = platform_delay;
    dev_ctx.handle = &I2C_BUS; // Adjust according to your platform

    // Initialize the test platform
    platform_init();
    platform_delay(BOOT_TIME);

    // Check device ID
    lsm6dsm_device_id_get(&dev_ctx, &whoamI);
    if (whoamI != LSM6DSM_ID)
    {
        printf("Error: LSM6DSM not found.\n");
        return;
    }

    // Restore default configuration
    lsm6dsm_reset_set(&dev_ctx, PROPERTY_ENABLE);
    do
    {
        lsm6dsm_reset_get(&dev_ctx, &rst);
    } while (rst);

    // Enable Block Data Update
    lsm6dsm_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

    // Set Output Data Rate for Acc and Gyro
    lsm6dsm_xl_data_rate_set(&dev_ctx, LSM6DSM_XL_ODR_12Hz5);
    lsm6dsm_gy_data_rate_set(&dev_ctx, LSM6DSM_GY_ODR_12Hz5);

    // Set full scale
    lsm6dsm_xl_full_scale_set(&dev_ctx, LSM6DSM_2g);
    lsm6dsm_gy_full_scale_set(&dev_ctx, LSM6DSM_2000dps);

    // Read samples in polling mode
    while (1)
    {
        lsm6dsm_reg_t reg;

        // Read output only if new value is available
        lsm6dsm_status_reg_get(&dev_ctx, &reg.status_reg);

        if (reg.status_reg.xlda)
        {
            // Read acceleration field data
            memset(data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
            lsm6dsm_acceleration_raw_get(&dev_ctx, data_raw_acceleration);
            acceleration_mg[0] = lsm6dsm_from_fs2g_to_mg(data_raw_acceleration[0]);
            acceleration_mg[1] = lsm6dsm_from_fs2g_to_mg(data_raw_acceleration[1]);
            acceleration_mg[2] = lsm6dsm_from_fs2g_to_mg(data_raw_acceleration[2]);
            sprintf((char *)tx_buffer, "Acceleration [mg]: %4.2f\t%4.2f\t%4.2f\r\n",
                    acceleration_mg[0], acceleration_mg[1], acceleration_mg[2]);
            printf("%s", tx_buffer);
            // Assign values to acceleration pointer
            acceleration[0] = acceleration_mg[0];
            acceleration[1] = acceleration_mg[1];
            acceleration[2] = acceleration_mg[2];
        }

        if (reg.status_reg.gda)
        {
            // Read angular rate field data
            memset(data_raw_angular_rate, 0x00, 3 * sizeof(int16_t));
            lsm6dsm_angular_rate_raw_get(&dev_ctx, data_raw_angular_rate);
            angular_rate_mdps[0] = lsm6dsm_from_fs2000dps_to_mdps(data_raw_angular_rate[0]);
            angular_rate_mdps[1] = lsm6dsm_from_fs2000dps_to_mdps(data_raw_angular_rate[1]);
            angular_rate_mdps[2] = lsm6dsm_from_fs2000dps_to_mdps(data_raw_angular_rate[2]);
            sprintf((char *)tx_buffer, "Angular rate [mdps]: %4.2f\t%4.2f\t%4.2f\r\n",
                    angular_rate_mdps[0], angular_rate_mdps[1], angular_rate_mdps[2]);
            printf("%s", tx_buffer);
            // Assign values to angular_rate pointer
            angular_rate[0] = angular_rate_mdps[0];
            angular_rate[1] = angular_rate_mdps[1];
            angular_rate[2] = angular_rate_mdps[2];
        }
    }
}
/**
 * @brief Initializes an inclinometer instance with the specified alpha value.
 * 
 * @param inclinometer Pointer to the inclinometer instance to initialize.
 * @param alpha The alpha parameter for the complementary filter.
 */
void init_inclinometer(Inclinometer *inclinometer, float alpha) {
    // Set initial values for acceleration, angular velocity, and filter parameters
    for (int i = 0; i < 3; i++) {
        inclinometer->acceleration[i] = 0.0;
        inclinometer->angular_velocity[i] = 0.0;
    }
    inclinometer->previous_angle = 0.0;
    inclinometer->alpha = alpha;
    inclinometer->beta = 1.0 - alpha;
}

/**
 * @brief Applies the complementary filter algorithm to calculate the angle.
 * 
 * @param inclinometer Pointer to the inclinometer instance.
 * @param filter_alpha The alpha parameter for the complementary filter.
 * @param sample_rate The sample rate of the sensor data.
 */
void complementary_filter(Inclinometer *inclinometer, float filter_alpha, int sample_rate) {
    // Calculate new angle using Complementary Filter
    inclinometer->previous_angle = filter_alpha * (inclinometer->previous_angle + inclinometer->angular_velocity[2] * (1.0f / sample_rate)) +
                                    (1.0f - filter_alpha) * atan2f(inclinometer->acceleration[1], inclinometer->acceleration[0]) * 180.0f / M_PI;
    
    // Ensure the angle is within the range -180 to +180 degrees
    while (inclinometer->previous_angle > 180.0f) {
        inclinometer->previous_angle -= 360.0f;
    }
    while (inclinometer->previous_angle < -180.0f) {
        inclinometer->previous_angle += 360.0f;
    }

    // Print the resulting angle
    printf("Filtered Angle: %.2f degrees\n", inclinometer->previous_angle);
}
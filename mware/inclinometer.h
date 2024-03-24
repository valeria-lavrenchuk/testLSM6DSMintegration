#ifndef INCLINOMETER_H
#define INCLINOMETER_H

// Structure to represent an inclinometer
typedef struct {
    float acceleration[3];       // Array to store acceleration values in X, Y, Z axes
    float angular_velocity[3];   // Array to store angular velocity values in X, Y, Z axes
    float previous_angle;        // Previous angle value
    float alpha;                 // Alpha parameter for the complementary filter
    float beta;                  // Beta parameter for the complementary filter
} Inclinometer;

// Function prototypes

/**
 * @brief Initializes the sensor driver (LSM6DSM).
 * 
 * @return Returns 0 on success, or a non-zero error code on failure.
 */
int init_sensor_driver();

/**
 * @brief Reads sensor data.
 * 
 * @param acceleration Pointer to an array to store acceleration values (X, Y, Z axes).
 * @param angular_rate Pointer to an array to store angular velocity values (X, Y, Z axes).
 */
void read_sensor_data(float *acceleration, float *angular_rate);

/**
 * @brief Initializes an inclinometer instance with the specified alpha value.
 * 
 * @param inclinometer Pointer to the inclinometer instance to initialize.
 * @param alpha The alpha parameter for the complementary filter.
 */
void init_inclinometer(Inclinometer *inclinometer, float alpha);

/**
 * @brief Applies the complementary filter algorithm to calculate the angle.
 * 
 * @param inclinometer Pointer to the inclinometer instance.
 * @param filter_alpha The alpha parameter for the complementary filter.
 * @param sample_rate The sample rate of the sensor data.
 */
void complementary_filter(Inclinometer *inclinometer, float filter_alpha, int sample_rate);

#endif /* INCLINOMETER_H */

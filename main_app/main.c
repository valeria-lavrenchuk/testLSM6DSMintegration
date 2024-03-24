#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>
#include "inclinometer.h"

#define FILTER_TIME_CONSTANT 0.1 // Adjust as needed
#define FILTER_ALPHA (FILTER_TIME_CONSTANT / (FILTER_TIME_CONSTANT + (1.0 / SAMPLE_RATE)))
#define SAMPLE_RATE 100 // Adjust as needed (in Hz)
#define SENSOR_READ_TASK_PRIORITY 2
#define COMPLEMENTARY_FILTER_TASK_PRIORITY 1
#define SEM_INIT_VALUE 0 // Named constant for semaphore initialization

sem_t sensorDataSemaphore;
Inclinometer inclinometer;

void *sensor_read_thread(void *args)
{
    (void)args; // Suppress unused parameter warning
    while (1)
    {
        // Read sensor data
        read_sensor_data(inclinometer.acceleration, inclinometer.angular_velocity);
        sem_post(&sensorDataSemaphore);

        // Delay to control sampling rate
        usleep(1000000); // Sleep for 1 second (1000000 microseconds)
    }
    return NULL;
}

void *angle_calculation_thread(void *arg)
{
    (void)arg; // Suppress unused parameter warning
    while (1)
    {
        // Acquire mutex before accessing shared sensor data
        sem_wait(&sensorDataSemaphore);

        // Apply Complementary Filter algorithm
        complementary_filter(&inclinometer, FILTER_ALPHA, SAMPLE_RATE);
    }
    return NULL;
}

void cleanup()
{
    // Destroy semaphore
    sem_destroy(&sensorDataSemaphore);
}

int initialize_semaphore()
{
    int sem_value;
    if (sem_init(&sensorDataSemaphore, 0, SEM_INIT_VALUE) != 0)
    {
        fprintf(stderr, "Failed to initialize semaphore\n");
        cleanup();
        return 1;
    }
    if (sem_getvalue(&sensorDataSemaphore, &sem_value) != 0 || sem_value != SEM_INIT_VALUE)
    {
        fprintf(stderr, "Semaphore initialization failed\n");
        cleanup();
        return 1;
    }
    return 0;
}

int main()
{
    // Initialize sensor driver
    if (init_sensor_driver() != 0)
    {
        fprintf(stderr, "Failed to initialize sensor driver\n");
        return 1; // Return error code indicating failure
    }

    // Initialize inclinometer

    init_inclinometer(&inclinometer, FILTER_ALPHA);

    // Initialize semaphore
    if (initialize_semaphore() != 0)
    {
        return 1;
    }

    // Create sensor read thread
    pthread_t sensor_read_tid;
    if (pthread_create(&sensor_read_tid, NULL, sensor_read_thread, NULL) != 0)
    {
        fprintf(stderr, "Failed to create sensor read thread\n");
        // Clean up any resources that were already initialized
        cleanup();
        return 1; // Return error code indicating failure
    }

    // Create angle calculation thread
    pthread_t angle_calc_tid;
    if (pthread_create(&angle_calc_tid, NULL, angle_calculation_thread, NULL) != 0)
    {
        fprintf(stderr, "Failed to create angle calculation thread\n");
        // Clean up any resources that were already initialized
        cleanup();
        return 1; // Return error code indicating failure
    }

    // Join threads
    if (pthread_join(sensor_read_tid, NULL) != 0)
    {
        fprintf(stderr, "Failed to join sensor read thread\n");
        // Clean up any resources that were already initialized
        cleanup();
        return 1; // Return error code indicating failure
    }

    if (pthread_join(angle_calc_tid, NULL) != 0)
    {
        fprintf(stderr, "Failed to join angle calculation thread\n");
        // Clean up any resources that were already initialized
        cleanup();
        return 1; // Return error code indicating failure
    }

    // Clean up resources
    cleanup();

    return 0; // Return success code
}

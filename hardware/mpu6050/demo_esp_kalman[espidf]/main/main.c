#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "MPU6050.h"
#include "KalmanFilter.h"

void mpu_task(void *args)
{
    mpu_Init();
    float dt = 0.005;
    float pitch_raw, roll_raw;
    float pitch, roll;
    static Kalman_t kalman_pitch = {
        .Q_angle = 0.1,
        .Q_bias = 0.003,
        .R_measure = 0.03};
    static Kalman_t kalman_roll = {
        .Q_angle = 0.1,
        .Q_bias = 0.003,
        .R_measure = 0.03};
    while (true)
    {
        pitch_raw = mpu_getPitch();
        roll_raw = mpu_getRoll();
        pitch = Kalman_getAngle(&kalman_pitch, pitch_raw, mpu_getGyroX(), dt);
        roll = Kalman_getAngle(&kalman_roll, roll_raw, mpu_getGyroY(), dt);
        printf("Pitch:%f    ", pitch);
        printf("Pitch_Raw:%f    ", pitch_raw);
        printf("Roll:%f    ", roll);
        printf("Roll_Raw:%f    \n", roll_raw);
        vTaskDelay(30 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

void app_main()
{
    xTaskCreatePinnedToCore(mpu_task, "mpu_task", 2048, NULL, 5, NULL, 0);
}

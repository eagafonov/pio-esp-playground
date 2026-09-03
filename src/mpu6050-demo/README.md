# mpu6050-demo

MPU6050 IMU demo: prints acceleration, rotation, and temperature over serial. Tolerates MPU6050 clones with different device IDs (configured via `MPU6050_DEVICE_ID`).

```bash
pio run -e mpu6050-demo -t upload
```

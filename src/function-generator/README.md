# function-generator

Serial-controlled signal generator on GPIO 4:

- **c** — cosine (ESP32 DAC, GPIO 25)
- **s** — square wave (LEDC)
- **w** — PWM, duty adjustable 1–99% in 1% steps (**j**/**k**)
- **p** — pulse train (software-driven)
- **i** — single impulse
- **0–7** — frequency 1 Hz to 10 MHz

Cosine needs the ESP32 hardware DAC; everything else runs on any board, including ESP32-S3.

```bash
pio run -e function-generator -t upload
```

# load-cell

HX711 load cell scale on ESP32-S3. Streams weight readings over serial, with tare, calibration (against a known weight), and average/median sampling modes. FreeRTOS task does the measuring so serial commands stay responsive.

```bash
pio run -e load-cell -t upload
```

#include <Arduino.h>

#include "HX711.h"

HX711 loadcell;

// 1. HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2; // D1
const int LOADCELL_SCK_PIN = 3;  // D2

volatile int numSamples = 1;

void measureTask(void *arg) {
    loadcell.wait_ready();

    for (;;) {
        if (loadcell.is_ready()) {
            Serial.printf("Weight:%9.2f\r\n", loadcell.get_units(numSamples));
        } else {
            vTaskDelay(1);
        }
    }
}

void printHelp() {
    Serial.println("Commands:");
    Serial.println("\th - Help");
    Serial.println("\tc - calibrate");
    Serial.println("\tt - tare");
    Serial.println("\ta - average mode");
    Serial.println("\tm - median mode");
    Serial.println("\tM - median/average mode");
    Serial.println();
    Serial.println("Current settings:");
    Serial.printf("mode: %d\r\n", loadcell.get_mode());
    Serial.printf("scale/offset: %f/%ld\r\n", loadcell.get_scale(), (long)loadcell.get_offset());
    Serial.printf("gain: %d\r\n", loadcell.get_gain());
    Serial.printf("tare: %f (set:%d)\r\n", loadcell.get_tare(), loadcell.tare_set());
}

TaskHandle_t xMeasureHandle = NULL;
TaskHandle_t xReadSerialHandle = NULL;

void suspendMeasurement() {
    vTaskSuspend(xMeasureHandle);
}

void resumeMeasurement() {
    vTaskResume(xMeasureHandle);
}

void waitForEnter(const char* prompt) {
    Serial.println(prompt);

    for (;;) {
        if (Serial.available()) {
            if (Serial.read() == '\r') {
                return;
            }
        } else {
            vTaskDelay(1);
        }
    }
}

void calibrate()  {
    Serial.println("Wait for scale is ready");

    while (!loadcell.is_ready()) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    Serial.println("Preparing for calibration");
    loadcell.tare(40);

    waitForEnter("Place 100g weight and press enter to calibrate");

    Serial.println("Calibrating");
    loadcell.calibrate_scale(100, 40);
    Serial.printf("Calibration is complete (scale:%f)\r\n", loadcell.get_scale());
}

void readSerial(void *arg) {
    for(;;) {
        if (Serial.available()) {
            auto c = Serial.read();

            switch (c) {
                case '\r':
                case 'h':
                    suspendMeasurement();
                    vTaskDelay(500 / portTICK_PERIOD_MS);
                    printHelp();
                    waitForEnter("Press enter to continue");
                    resumeMeasurement();
                    break;

                case 'c':
                    suspendMeasurement();
                    calibrate();
                    resumeMeasurement();
                    break;

                case 't':
                    suspendMeasurement();
                    loadcell.tare();
                    resumeMeasurement();
                    break;

                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    numSamples = c - '0';
                    break;

                case '0':
                    numSamples = 10;
                    break;

                case 'r':
                    loadcell.set_raw_mode();
                    break;

                case 'a':
                    loadcell.set_runavg_mode();
                    break;

                case 'm':
                    loadcell.set_median_mode();
                    break;

                case 'M':
                    loadcell.set_medavg_mode();
                    break;

                default:
                    Serial.printf("serial: %d\r\n", c);
            }
        } else {
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
}

// Captured by calibration procedure
static constexpr float default_scale = 438.821747;

void setup() {
    Serial.begin(115200);

#ifdef NATIVE_USB
    while(!Serial) {
        delay(50);
    }
#endif

    Serial.println();
    Serial.println("Load cell demo");

    loadcell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    loadcell.set_gain(HX711_CHANNEL_A_GAIN_128, true);

    loadcell.set_scale(default_scale);
    loadcell.tare();

    xTaskCreate(measureTask, "measure", 10000, NULL, tskIDLE_PRIORITY, &xMeasureHandle);
    xTaskCreate(readSerial, "readSerial", 10000, NULL, tskIDLE_PRIORITY, &xReadSerialHandle);
}

void loop() {
}

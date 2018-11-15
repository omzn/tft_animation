#include "character.h"
#include "util.h"
#include <SPI.h>
#include <SparkFunMPU9250-DMP.h>

#define TFT_CS 10 // Chip select line for TFT display
#define TFT_RST 9 // Reset line for TFT (or see below...)
#define TFT_DC 8  // Data/command line for TFT

#define FPS 20

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Character aquatan = Character(&tft);

MPU9250_DMP imu;

uint32_t stepCount = 0;
uint32_t stepTime = 0;
uint32_t lastStepCount = 0;

void setup(void) {
  Serial.begin(115200);

  tft.initR(INITR_MINI160x80);
  tft.setRotation(3);
  tft.fillScreen(ST7735_BLACK);

  tft.setCursor(0, 0);
  tft.setTextColor(fixColor(ST7735_WHITE));
  tft.println("Aquatan pedometer");

  delay(1000);

  if (imu.begin() != INV_SUCCESS) {
    tft.println("Unable to communicate with MPU-9250");
    tft.println("Check connections, and try again.");
    while (1) {
      delay(5000);
    }
  }

  imu.dmpBegin(DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_PEDOMETER, 10);

  imu.dmpSetPedometerSteps(stepCount);
  imu.dmpSetPedometerTime(stepTime);

  delay(1000);
  aquatan.start(64, 32, ORIENT_FRONT);
}

void loop() {
  stepCount = imu.dmpGetPedometerSteps();
  stepTime = imu.dmpGetPedometerTime();

  if (stepCount != lastStepCount) {
    lastStepCount = stepCount;
    tft.setCursor(0,64);
    tft.print("Walked " + String(stepCount) + " steps");
    tft.println(" (" + String((float)stepTime / 1000.0) + " s)");
  }

  if (imu.fifoAvailable() > 0) {
    // Use dmpUpdateFifo to update the ax, gx, qx, etc. values
    if (imu.dmpUpdateFifo() == INV_SUCCESS) {
      tft.println("ax:" + String(imu.ax) + " ay:" + String(imu.ay) +
                     " az:" + String(imu.az));
      // The following variables will have data from the top of the FIFO:
      // imu.ax, imu.ay, imu.az, -- Accelerometer
      // imu.gx, imu.gy, imu.gz -- calibrated gyroscope
      // and imu.qw, imu.qx, imu.qy, and imu.qz -- quaternions
    }
  }

  if (Serial.available() > 0) {
    // 受信したデータの1バイトを読み取る
    byte op = Serial.read();
    if (op == 'f') {
      aquatan.setOrient(ORIENT_FRONT);
    } else if (op == 'l') {
      aquatan.setOrient(ORIENT_LEFT);
    } else if (op == 'r') {
      aquatan.setOrient(ORIENT_RIGHT);
    } else if (op == 'b') {
      aquatan.setOrient(ORIENT_BACK);
    } else if (op == 'j') {
      aquatan.setOrient(ORIENT_JUMP);
    } else if (op == 's') {
      aquatan.setOrient(ORIENT_SLEEP);
    } else if (op == '-') {
      aquatan.incSpeed();
    } else if (op == '+') {
      aquatan.decSpeed();
    } else if (op == 'U') {
      aquatan.move(MOVE_UP, 16);
    } else if (op == 'D') {
      aquatan.move(MOVE_DOWN, 16);
    } else if (op == 'L') {
      aquatan.move(MOVE_LEFT, 32);
    } else if (op == 'R') {
      aquatan.move(MOVE_RIGHT, 32);
    } else if (op == 'M') {
      aquatan.move(MOVE_LEFTBACK, 32);
    } else if (op == 'N') {
      aquatan.move(MOVE_RIGHTBACK, 32);
    }
  }
  delay(aquatan.update());
}

#include <M5StickC.h>
#include "BLEDevice.h"

uint8_t seq;
#define MyManufacturerId 0xd502  // Omron Manufacturer ID

BLEScan* pBLEScan;

void setup() {
  M5.begin();

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(false);

  M5.Axp.ScreenBreath(9);
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 1);
  M5.Lcd.printf("Searching Omron Env Device...\r\n");
}

void loop() {
  
  BLEScanResults foundDevices = pBLEScan->start(3);
  int numOfDevices = foundDevices.getCount();

  for (int i = 0; i < numOfDevices; i++) {
    BLEAdvertisedDevice d = foundDevices.getDevice(i);
    if (d.haveManufacturerData()) {
      std::string data = d.getManufacturerData();
      int manu = data[0] << 8 | data[1];

      if (manu == MyManufacturerId && seq != data[2]){
        seq = data[2];

        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 0, 1);
        M5.Lcd.printf("Seq          : %d\r\n", data[2]);
        M5.Lcd.printf("Temperature  : %2.1f degC\r\n", (float)(data[4] << 8 | data[3]) / 100);
        M5.Lcd.printf("Humidity     : %2.1f %%\r\n", (float)(data[6] << 8 | data[5]) / 100);
        M5.Lcd.printf("Pressure     : %4.1f hPa\r\n", (float)(data[12] << 8 | data[11]) / 10);
        M5.Lcd.printf("AmbientLight : %4.0f lx\r\n", (float)(data[8] << 8 | data[7]));
        M5.Lcd.printf("UvIndex      : %1.2f\r\n", (data[10] << 8 | data[9]) / 100);
        M5.Lcd.printf("SoundNoise   : %2.1f dB\r\n", (float)(data[14] << 8 | data[13]) / 100);
        M5.Lcd.printf("BatteryVolt  : %d\r\n", (data[21] + 100) * 10);

      }
      
    }
  }
  delay(3000);

}

// arduinoy用testコード
/**********
 * ---使用品---
 * マイコンボード:Wio-LTE
 * センサ:Adafruit LPS33HW
 * ---ピン配置---
 * LPS33HW
 *  VIN :
 *  3Vo:
 *  GND :
 *  SCK :
 *  SDO :
 *  SDI :
 *  CS :
 *  INT :
**********/
#include <WioLTEforArduino.h>
#include <Adafruit_LPS35HW.h>

Adafruit_LPS35HW lps33hw = Adafruit_LPS35HW();

#define LPS35HW_I2CADDR_ANOTHER 0x5C ///< LPS33HW another i2c address(connect from SDO to GND)

/*// For SPI mode, we need a CS pin
#define LPS_CS  10
// For software-SPI mode we need SCK/MOSI/MISO pins
#define LPS_SCK 13
#define LPS_MISO 12
#define LPS_MOSI 11
*/

WioLTE wiolte = WioLTE();
void debugLED(short repeat = 5,int interval=200, byte red=255, byte green=255, byte blue=255);
/*!
*    @brief  use to debug with color LED (WioLTEforArduino.h)
*    @param  repeat
*            点滅回数(default = 5)
*    @param  interval
*            点滅間隔(default = 200 [ms])
*    @param  RedGreenBlue
*            RGBの度合い(0~255)(default = 255,255,255)
*/
void debugLED(short repeat,int interval, byte red, byte green, byte blue){
  wiolte.PowerSupplyLed(true);
  for(int index=0; index < repeat; index++){
    wiolte.LedSetRGB(red, green, blue);
    delay(interval);
    wiolte.LedSetRGB(0, 0, 0);
    delay(interval);
  }
  wiolte.PowerSupplyLed(false);
}


Adafruit_LPS35HW lps33hw_air = Adafruit_LPS35HW();
Adafruit_LPS35HW lps33hw_water = Adafruit_LPS35HW();
float pressure_air;
float pressure_water;
// setup LPS33HW
boolean setupLPS33HW(){
  if(!lps33hw_air.begin_I2C(LPS35HW_I2CADDR_DEFAULT)){
  //if (!lps33hw_air.begin_SPI(LPS_CS)) {
  //if (!lps33hw_air.begin_SPI(LPS_CS, LPS_SCK, LPS_MISO, LPS_MOSI)) {
    return false;
  }
  if(!lps33hw_water.begin_I2C(LPS35HW_I2CADDR_ANOTHER)){
  //if (!lps33hw_water.begin_SPI(LPS_CS)) {
  //if (!lps33hw_water.begin_SPI(LPS_CS, LPS_SCK, LPS_MISO, LPS_MOSI)) {
    return false;
  }
  return true;
}
// get pressure from two LPS33HW
void readLPS33HW(){
  pressure_air = lps33hw_air.readPressure();
  pressure_water = lps33hw_water.readPressure();
  return;
}
// serial print of two pressure values(LPS33HW)
void printLPS33HW(){
  SerialUSB.print("1:");
  SerialUSB.println(pressure_air);
  SerialUSB.print("2:");
  SerialUSB.println(pressure_water);
  return;
}

void setup() {
  debugLED(5,200,255,255,255);
  SerialUSB.begin(9600);
  SerialUSB.println("serial start");
  setupLPS33HW();

  /*if (!lps33hw.begin_I2C()) {
  //if (!lps33hw.begin_SPI(LPS_CS)) {
  //if (!lps33hw.begin_SPI(LPS_CS, LPS_SCK, LPS_MISO, LPS_MOSI)) {
    //SerialUSB.println("Couldn't find LPS33HW chip");
    while (1){
      debugLED(5, 200, 255, 0, 0);
    }
  }*/

}

void loop() {
  debugLED(5,200,0,255,0);
  readLPS33HW();
  printLPS33HW();

  //SerialUSB.println(lps33hw.readPressure());
  
  delay(1000);
}
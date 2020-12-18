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

#define sen0193_A4 (WIOLTE_A4)
#define sen0193_A5 (WIOLTE_A5)
#define sen0193_A6 (WIOLTE_A6)
#define sen0193_A7 (WIOLTE_A7)

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


int val_array_soil[4];
void readSoilSensor(){
  val_array_soil[0] = analogRead(WIOLTE_A4);
  val_array_soil[1] = analogRead(WIOLTE_A5);
  val_array_soil[2] = analogRead(WIOLTE_A6);
  val_array_soil[3] = analogRead(WIOLTE_A7);
}
void printSoilSensor(){
  for(int index=0; index<4; index++){
    SerialUSB.print(index);
    SerialUSB.print(":");
    SerialUSB.println(val_array_soil[index]);
  }
}

void setup() {
  debugLED(5,200,255,255,255);
  SerialUSB.begin(9600);
  debugLED(5,200,255,0,0);
  SerialUSB.println("SerialUSB start");
  debugLED(5,200,0,255,0);

}

void loop() {
	SerialUSB.print("a");
  debugLED(5,200,0,0,255);

  readSoilSensor();
  printSoilSensor();
  
  delay(1000);
}
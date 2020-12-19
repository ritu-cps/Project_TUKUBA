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
#include <WioLTEClient.h>
#include <PubSubClient.h> // https://github.com/SeeedJP/pubsubclient
#include <stdio.h>
#include <ArduinoJson.h>

#if defined ARDUINO_ARCH_STM32F4
#elif defined ARDUINO_ARCH_STM32
#endif

#define APN "soracom.io"
#define USERNAME "sora"
#define PASSWORD "sora"

#define MQTT_SERVER_HOST "beam.soracom.io"
#define MQTT_SERVER_PORT (1883)

#define ID "AWS IoT"
#define OUT_TOPIC "tukuba/Iot"

#define LPS35HW_I2CADDR_ANOTHER 0x5C ///< LPS33HW another i2c address(connect from SDO to GND)

WioLTE wiolte = WioLTE();
/*--------lte & mqtt declare--------*/
WioLTEClient WioClient(&wiolte);
PubSubClient MqttClient;
// send_dataBox
char pubMessage[512];
// 通信許可
<<<<<<< HEAD
bool isSend_data=true;
/*--------LPS33HW declare--------*/
=======
bool isInternet = true;
//--------LPS33HW declare--------
>>>>>>> origin/dev_WioLTE
Adafruit_LPS35HW lps33hw_air = Adafruit_LPS35HW();
Adafruit_LPS35HW lps33hw_water = Adafruit_LPS35HW();
float pressure_air; // pressure value obtained from LPS33HW 
float pressure_water;
<<<<<<< HEAD
// result of water level(height)[cm]
float result_water_level;
/*--------Soil sensor declare--------*/
int values_soil[4]; // raw data of soil moisture sensor obtained by analogRead
// results of soil moisture[%]
float results_soil_moisture[4];
=======
float result_water_level; // result of water level(height)[cm]
//--------Soil sensor declare--------
int values_soil[4];
float results_soil_moisture[4]; // results of soil moisture[%]
>>>>>>> origin/dev_WioLTE

void debugLED(short repeat = 5, int interval = 200, byte red = 255, byte green = 255, byte blue = 255);
/*!
*    @brief  use to debug with color LED (WioLTEforArduino.h)
*    @param  repeat
*            点滅回数(default = 5)
*    @param  interval
*            点滅間隔(default = 200 [ms])
*    @param  RedGreenBlue
*            RGBの度合い(0~255)(default = 255,255,255)
*/
void debugLED(short repeat, int interval, byte red, byte green, byte blue)
{
  wiolte.PowerSupplyLed(true);
  for (int index = 0; index < repeat; index++)
  {
    wiolte.LedSetRGB(red, green, blue);
    delay(interval);
    wiolte.LedSetRGB(0, 0, 0);
    delay(interval);
  }
  wiolte.PowerSupplyLed(false);
}

/*--------wio setup function--------*/
void wio_setUP()
{
  delay(200);

  SerialUSB.println("### I/O Initialize.");
  wiolte.Init();

  Wire.begin();

  SerialUSB.println("### Power supply ON.");
  wiolte.PowerSupplyGrove(true);
  delay(500);
}
/*--------end wio setup function--------*/

/*--------send LTE function--------*/
//　Wio_setUpしてください
void setup_Internet()
{
  wiolte.PowerSupplyLTE(true);
  SerialUSB.println("### Turn on or reset.");
  if (!wiolte.TurnOnOrReset())
  {
    SerialUSB.println("### ERROR! ###");
    return;
  }

  SerialUSB.println("### Connecting to \"" APN "\".");
  if (!wiolte.Activate(APN, USERNAME, PASSWORD))
  {
    SerialUSB.println("### ERROR! ###");
    return;
  }
}
//　通信切断
void wio_down()
{
  delay(200);

  SerialUSB.println("### Power supply OFF.");
  wiolte.PowerSupplyLTE(false);
  wiolte.PowerSupplyGrove(false);
}
/*--------end send LTE function--------*/

/*--------mqtt function--------*/
void callback(char *topic, byte *payload, unsigned int length)
{
  SerialUSB.print("Subscribe:");
  for (int i = 0; i < length; i++)
    SerialUSB.print((char)payload[i]);
  SerialUSB.println("");
}
// Mqtt create connection
void connectMqtt()
{
  SerialUSB.println("### Connecting to MQTT server \"" MQTT_SERVER_HOST "\"");
  MqttClient.setServer(MQTT_SERVER_HOST, MQTT_SERVER_PORT);
  MqttClient.setCallback(callback);
  MqttClient.setClient(WioClient);
  if (!MqttClient.connect(ID))
  {
    SerialUSB.println("### ERROR! ###");
    return;
  }
  delay(1000);
}
/*--------end mqtt function--------*/

/*--------json function--------*/
String buildJson()
{
  // データ作成
  String json = "";
  const int capacity = JSON_OBJECT_SIZE(20);
  StaticJsonDocument<capacity> doc;
  DynamicJsonDocument logs(64);
  doc["water_level"] = result_water_level;
  doc["soil_moistureA"] = results_soil_moisture[0];
  doc["soil_moistureB"] = results_soil_moisture[1];
  //doc["soil_moistureC"] = results_soil_moisture[2];
  //doc["soil_moistureD"] = results_soil_moisture[3];
  doc["lps33hw_air"] = pressure_air;
  doc["lps33hw_water"] = pressure_water;
  doc["soil_valueA"] = values_soil[0];
  doc["soil_valueB"] = values_soil[1];
  //doc["soil_valueC"] = values_soil[2];
  //doc["soil_valueD"] = values_soil[3];

  //  Writ_sd(val1, val2, val3, val4);
  serializeJson(doc, json);

  return json;
}
/*--------end json function--------*/

/*--------LPS35HW function--------*/
// setup LPS33HW
boolean setupLPS33HW()
{
  if (!lps33hw_air.begin_I2C(LPS35HW_I2CADDR_DEFAULT))
  {
    //if (!lps33hw_air.begin_SPI(LPS_CS)) {
    //if (!lps33hw_air.begin_SPI(LPS_CS, LPS_SCK, LPS_MISO, LPS_MOSI)) {
    //return false;
  }
  if (!lps33hw_water.begin_I2C(LPS35HW_I2CADDR_ANOTHER))
  {
    //if (!lps33hw_water.begin_SPI(LPS_CS)) {
    //if (!lps33hw_water.begin_SPI(LPS_CS, LPS_SCK, LPS_MISO, LPS_MOSI)) {
    //return false;
  }
  return true;
}
// get pressure from two LPS33HW
void readLPS33HW()
{
  pressure_air = lps33hw_air.readPressure();
  pressure_water = lps33hw_water.readPressure();
  return;
}
// serial print of two pressure values(LPS33HW)
void printLPS33HW()
{
  SerialUSB.print("1:");
  SerialUSB.println(pressure_air);
  SerialUSB.print("2:");
  SerialUSB.println(pressure_water);
  return;
}
/*--------end LPS35HW function--------*/
/*--------Soil sensor function--------*/
// get soil moisture from four soil moisture sensors
void readSoilSensor()
{
  values_soil[0] = analogRead(WIOLTE_A4);
  values_soil[1] = analogRead(WIOLTE_A5);
}
// serial print of four soil moisture values
void printSoilSensor()
{
  for (int index = 0; index < 4; index++)
  {
    SerialUSB.print(index);
    SerialUSB.print(":");
    SerialUSB.println(values_soil[index]);
  }
}
/*--------end Soil sensor function--------*/

// calculates sensor values and get the measurement result
void calculateSensors()
{
  static const float rho = 999.97;                            // water density[kg/m^3]
  static const float g0 = 9.80665;                            // standard gravitational acceleration[m/s^2]
  float pressure = pressure_water - pressure_air;             // water pressure[hPa](p=ρgh)
  result_water_level = ((pressure * 100) / (rho * g0)) * 100; // water level(hight)[cm]

  static const int value_air = 700; // soil-moisture sensor value in the air(analogRead)
  static const int value_water = 0; // soil-moisture sensor value in the water(analogRead)
  for (int index = 0; index < 4; index++)
  {
    results_soil_moisture[index] = (values_soil[index] - value_water) / (value_air - value_water);
  }
}

void setup()
{
  SerialUSB.begin(9600);
  //SerialUSB.println("serial start");

  wio_setUP();
  ///test code///
  setup_Internet();
  connectMqtt();
  ///////////////
  setupLPS33HW();
}

void loop()
{
  readLPS33HW();
  readSoilSensor();
  calculateSensors();
  //printLPS33HW();
  //printSoilSensor();

  //通信許可ならば
  if (isInternet)
  {
    if (!MqttClient.connected())
    {
      connectMqtt();
    }
    String data = buildJson();
    //SerialUSB.println(data);
    data.toCharArray(pubMessage, data.length() + 1);
    MqttClient.publish(OUT_TOPIC, pubMessage);

    MqttClient.loop();
    // センサー値が異常でない場合が続いたら false にする。
  }
  else
  {
    //センサー値で異常を検知したら isSend_data を true する
    //通信許可
    if (values_soil[0] < 600)
    {
      isInternet = true;
      setup_Internet();
      connectMqtt();
    }
  }

  delay(3000);
}

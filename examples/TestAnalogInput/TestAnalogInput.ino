/*
  Modbus-Arduino Example - Test Holding Register (Modbus IP ESP8266)
  Read Analog sensor on Pin ADC (ADC input between 0 ... 1V)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/

#include <ESP8266WiFi.h>
#include <Modbus.h>
#include <ModbusESP.h>

//Modbus Registers Offsets (0-9999)
const int SENSOR_IREG = 100;

//ModbusIP object
ModbusESP mb;

struct ConfigStruct
{
  //-----MODBUS SETUP-----
  uint8_t statusModbus;
  char ModbusIP_ssid[20];
  char ModbusIP_password[20];
  uint16_t ModbusIP_Port;
  IPAddress ModbusIP_IP;
  IPAddress ModbusIP_Gateway;
  IPAddress ModbusIP_Subnet;
} wifiConfig;

long ts;

void setup() {
  Serial.begin(115200);

  //Config Modbus IP
  String ModbusIP_ssid = "BRISTOM";
  String ModbusIP_password = "SOLUCOES";
  //    String ModbusIP_ssid = "PD240x";
  //    String ModbusIP_password = "12345678";
  ModbusIP_ssid.toCharArray(wifiConfig.ModbusIP_ssid, 20);
  ModbusIP_password.toCharArray(wifiConfig.ModbusIP_password, 20);
  wifiConfig.ModbusIP_Port = 502;
  wifiConfig.ModbusIP_IP = IPAddress(192, 168, 0, 71);
  wifiConfig.ModbusIP_Gateway = IPAddress(192, 168, 0, 1);
  wifiConfig.ModbusIP_Subnet = IPAddress(255, 255, 255, 0);

  mb.config(wifiConfig.ModbusIP_ssid, wifiConfig.ModbusIP_password, wifiConfig.ModbusIP_IP, wifiConfig.ModbusIP_Gateway, wifiConfig.ModbusIP_Subnet, wifiConfig.ModbusIP_Port);
  //mb.config("BRISTOM", "SOLUCOES", IPAddress(192, 168, 0, 71), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0), 502);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Add SENSOR_IREG register - Use addIreg() for analog Inputs
  mb.addIreg(SENSOR_IREG);

  ts = millis();
}

void loop() {
  //Call once inside loop() - all magic here
  mb.task();

  //Read each two seconds
  if (millis() > ts + 2000) {
    ts = millis();
    //Setting raw value (0-1024)
    mb.Ireg(SENSOR_IREG, analogRead(A0));
  }
}

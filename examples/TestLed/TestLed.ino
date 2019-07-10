#include <ESP8266WiFi.h>
#include <Modbus.h>
#include <ModbusESP.h>

//Modbus Registers Offsets (0-9999)
const int LED_COIL = 100;
//Used Pins
const int ledPin = 0; //GPIO0

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

void setup() {
  Serial.begin(115200);

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

  pinMode(ledPin, OUTPUT);
  mb.addCoil(LED_COIL);
}

void loop() {
  //Call once inside loop() - all magic here
  mb.task();

  //Attach ledPin to LED_COIL register
  digitalWrite(ledPin, mb.Coil(LED_COIL));
}

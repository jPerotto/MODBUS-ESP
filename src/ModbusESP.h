/*
    ModbusIP_ESP8266.h - Header for Modbus IP ESP8266 Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include "Modbus.h"
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#else

#ifndef MODBUSIP_ESP8266_H
#define MODBUSIP_ESP8266_H

#define MODBUSIP_PORT     502
#define MODBUSIP_MAXFRAME 200
#define MODBUSIP_TIMEOUT   10

class ModbusESP : public Modbus {
  private:
    byte _MBAP[7];
  public:
    ModbusESP();
    void close();
	void config(const char* ssid, const char* password, IPAddress ModbusIP_IP, IPAddress ModbusIP_Gateway, IPAddress ModbusIP_Subnet, uint16_t port = MODBUSIP_PORT);
    void task();
};

#endif //MODBUSIP_ESP8266_H


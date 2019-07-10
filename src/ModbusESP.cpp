/*
    ModbusIP_ESP8266.cpp - Source for Modbus IP ESP8266 Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include "ModbusESP.h"

WiFiServer WiFiModbus(MODBUSIP_PORT);

ModbusESP::ModbusESP()
{
	
}

void ModbusESP::config(const char* ssid, const char* password, IPAddress ModbusIP_IP, IPAddress ModbusIP_Gateway, IPAddress ModbusIP_Subnet, uint16_t port)
{
  WiFi.begin(ssid, password);
  WiFi.config(ModbusIP_IP, ModbusIP_Gateway, ModbusIP_Subnet);
  WiFiModbus.begin(port);
}

void ModbusESP::close()
{
  WiFiModbus.close();
}

void ModbusESP::task() {
  WiFiClient client = WiFiModbus.available();

  int raw_len = 0;

  if (client) {
    if (client.connected()) {
      for (int x = 0; x < 300; x++) { // Time to have data available
        if (client.available()) {
          while (client.available() > raw_len) {  //Computes data length
            raw_len = client.available();
            delay(1);
          }
          break;
        }
        delay(10);
      }
    }

    if (raw_len > 7) {
      for (int i = 0; i < 7; i++) _MBAP[i] = client.read(); //Get MBAP

      _len = _MBAP[4] << 8 | _MBAP[5];
      _len--; // Do not count with last byte from MBAP
      if (_MBAP[2] != 0 || _MBAP[3] != 0) return; //Not a MODBUSIP packet
      if (_len > MODBUSIP_MAXFRAME) return;      //Length is over MODBUSIP_MAXFRAME
      _frame = (byte*) malloc(_len);

      raw_len = raw_len - 7;
      for (int i = 0; i < raw_len; i++)  _frame[i] = client.read(); //Get Modbus PDU

      this->receivePDU(_frame);
      client.flush();

      if (_reply != MB_REPLY_OFF) {
        //MBAP
        _MBAP[4] = (_len + 1) >> 8;   //_len+1 for last byte from MBAP
        _MBAP[5] = (_len + 1) & 0x00FF;

        size_t send_len = (unsigned int)_len + 7;
        uint8_t sbuf[send_len];

        for (int i = 0; i < 7; i++)     sbuf[i] = _MBAP[i];
        for (int i = 0; i < _len; i++)  sbuf[i + 7] = _frame[i];

        client.write(sbuf, send_len);
      }

      client.stop();
      free(_frame);
      _len = 0;
    }
  }
}

#include "ApJSY333.h"

/*
  ApJSY333.h - ApJSY333 Implementation file
  For instructions, go to https://github.com/AntonioPrevitali/ApJSY333
  Created by Antonio Previtali, 29/07/2025.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the Gnu general public license version 3
*/

ApJSY333::ApJSY333(void)
{
     VoltageA = 0;
     VoltageB = 0;
     VoltageC = 0;
     CurrentA = 0;
     CurrentB = 0;
     CurrentC = 0;
     ActivePowerA = 0;
     ActivePowerB = 0;
     ActivePowerC = 0;
     TotalActivePower = 0;
     ReactivePowerA = 0;
     ReactivePowerB = 0;
     ReactivePowerC = 0;
     TotalReactivePower = 0;
     ApparentPowerA = 0;
     ApparentPowerB = 0;
     ApparentPowerC = 0;
     TotalApparentPower = 0;
     Frequency = 0;
     PowerFactorA = 0;
     PowerFactorB = 0;
     PowerFactorC = 0;
     TotalPowerFactor = 0;
     ActiveEnergyA = 0;
     ActiveEnergyB = 0;
     ActiveEnergyC = 0;
     TotalActiveEnergy = 0;
     ReactiveEnergyA = 0;
     ReactiveEnergyB = 0;
     ReactiveEnergyC = 0;
     TotalReactiveEnergy = 0;
     ApparentEnergyA = 0;
     ApparentEnergyB = 0;
     ApparentEnergyC = 0;
     TotalApparentEnergy = 0;

     ForwardActiveEnergyA = 0;
     ForwardActiveEnergyB = 0;
     ForwardActiveEnergyC = 0;
     ForwardTotalActiveEnergy = 0;
     OppositeActiveEnergyA = 0;
     OppositeActiveEnergyB = 0;
     OppositeActiveEnergyC = 0;
     OppositeTotalActiveEnergy = 0;
     ForwardReactiveEnergyA = 0;
     ForwardReactiveEnergyB = 0;
     ForwardReactiveEnergyC = 0;
     ForwardTotalReactiveEnergy = 0;
     OppositeReactiveEnergyA = 0;
     OppositeReactiveEnergyB = 0;
     OppositeReactiveEnergyC = 0;
     OppositeTotalReactiveEnergy = 0;
}


void ApJSY333::UseSerialAndEPin(Stream *xserial, uint8_t xpinDE, uint8_t xpinRE )
{
   _serial = xserial;
   _pinDE = xpinDE;
   _pinRE = xpinRE;
   digitalWrite(_pinDE,LOW);  // DE low  cioè max485 non trasmette nulla (alta impedenza sul bus)
   digitalWrite(_pinRE,HIGH); // RE HIGH cioè max485 non riceve nulla tutto in alta impedenza...
}


uint16_t ApJSY333::calculateCrc(byte* data, size_t size)
{
    uint16_t _crc;
    _crc = 0xFFFF;
    for (size_t i = 0; i < size; i++) {
        _crc ^= data[i];
        for (byte j = 0; j < 8; j++) {
            if (_crc & 0x0001)
                _crc = (_crc >> 1) ^ 0xA001; //< fixed polynomial
            else
                _crc = _crc >> 1;
        }
    }
    return _crc >> 8 | _crc << 8;
}


uint16_t ApJSY333::bytesToUInt16(byte* regData)
{
    uint16_t result = 0;
    for (int8_t i = 1; i >= 0; i--) {
        ((uint8_t*)&result)[i] = regData[1 - i];
    }
    return result;
}


uint32_t ApJSY333::bytesToUInt32(byte* regData)
{
    uint32_t result = 0;
    for (int8_t i = 3; i >= 0; i--) {
        ((uint8_t*)&result)[i] = regData[3 - i];
    }
    return result;
}


boolean ApJSY333::okTranza(void)
{

  uint16_t ilcrc;
  uint8_t  xbyte;
  
  uint8_t xreq[] = {0x01, 0x03, 0x01, 0x00, 0x00, 0x54, 0x0, 0x0 };  // legge tutti i 84 registri

  xreq[0] = _Addre;
  ilcrc = calculateCrc(xreq, 6);
  xreq[6] =  highByte(ilcrc);
  xreq[7] = lowByte(ilcrc);
  // ok xreq pronto.
  while (_serial->available()) _buffer[0] = _serial->read(); // pulisce eventuali skifezze nel buffer rx.

  _serial->write(xreq, 8);
  _serial->flush();
  // metto ora il max 485 in RX
  digitalWrite(4,LOW);
  digitalWrite(5,LOW); // in RX
  delayMicroseconds(104 * 2);  // un po di tempo di assestamento bus.
  if (!waitResponse()) return false;   // la risposta va tutta nel buffer e sono ben 21 caratteri
                                       // conviene fare cosi, fare letture di registri mirate (esempio power e direzione)
                                       // alla fine si impiega piu tempo...  
  return true;
}



boolean ApJSY333::okReadMeter(uint8_t xAddress)
{
  byte*   pDat;   // un pointer sul buffer.
  uint16_t xdir;
  uint16_t xalar;  // non usato ma c'e'
  uint8_t  xdirLow;

  _Addre = xAddress;

  // attiva max 485 in TX
  digitalWrite(_pinDE,HIGH);  // in TX
  digitalWrite(_pinRE,HIGH);
  delayMicroseconds(104 * 2);  // un po di tempo di assestamento bus.

  if (!okTranza()) {
      digitalWrite(_pinDE,LOW);  // DE low alta impedenza sul bus
      digitalWrite(_pinRE,HIGH); // RE HIGH alta impedenza.
      return false;
  }

  digitalWrite(_pinDE,LOW);  // DE low alta impedenza sul bus
  digitalWrite(_pinRE,HIGH); // RE HIGH alta impedenza.


  // conversione in float dei valori
  pDat = _buffer+3;
  VoltageA = bytesToUInt16(pDat);
  VoltageA = VoltageA / 100;
  pDat += 2;
  VoltageB = bytesToUInt16(pDat);
  VoltageB = VoltageB / 100;
  pDat += 2;
  VoltageC = bytesToUInt16(pDat);
  VoltageC = VoltageC / 100;
  pDat += 2;
  CurrentA = bytesToUInt16(pDat);
  CurrentA = CurrentA / 100;
  pDat += 2;
  CurrentB = bytesToUInt16(pDat);
  CurrentB = CurrentB / 100;
  pDat += 2;
  CurrentC = bytesToUInt16(pDat);
  CurrentC = CurrentC / 100;
  pDat += 2;
  ActivePowerA = bytesToUInt16(pDat);
  pDat += 2;
  ActivePowerB = bytesToUInt16(pDat);
  pDat += 2;
  ActivePowerC = bytesToUInt16(pDat);
  pDat += 2;
  TotalActivePower = bytesToUInt32(pDat);
  pDat += 4;
  ReactivePowerA = bytesToUInt16(pDat);
  pDat += 2;
  ReactivePowerB = bytesToUInt16(pDat);
  pDat += 2;
  ReactivePowerC = bytesToUInt16(pDat);
  pDat += 2;
  TotalReactivePower = bytesToUInt32(pDat);
  pDat += 4;
  ApparentPowerA = bytesToUInt16(pDat);
  pDat += 2;
  ApparentPowerB = bytesToUInt16(pDat);
  pDat += 2;
  ApparentPowerC = bytesToUInt16(pDat);
  pDat += 2;
  TotalApparentPower = bytesToUInt32(pDat);  // reg  0113H  0114H
  pDat += 4;
  Frequency = bytesToUInt16(pDat);
  Frequency = Frequency / 100;
  pDat += 2;
  PowerFactorA = bytesToUInt16(pDat);
  PowerFactorA = PowerFactorA / 1000;
  pDat += 2;
  PowerFactorB = bytesToUInt16(pDat);
  PowerFactorB = PowerFactorB / 1000;
  pDat += 2;
  PowerFactorC = bytesToUInt16(pDat);
  PowerFactorC = PowerFactorC / 1000;
  pDat += 2;
  TotalPowerFactor = bytesToUInt16(pDat);
  TotalPowerFactor = TotalPowerFactor / 1000;
  pDat += 2;
  ActiveEnergyA = bytesToUInt32(pDat);
  ActiveEnergyA /= 100;
  pDat += 4;
  ActiveEnergyB = bytesToUInt32(pDat);
  ActiveEnergyB /= 100;
  pDat += 4;
  ActiveEnergyC = bytesToUInt32(pDat);
  ActiveEnergyC /= 100;
  pDat += 4;
  TotalActiveEnergy  = bytesToUInt32(pDat);
  TotalActiveEnergy /= 100;
  pDat += 4;
  ReactiveEnergyA = bytesToUInt32(pDat);
  ReactiveEnergyA /= 100;
  pDat += 4;
  ReactiveEnergyB = bytesToUInt32(pDat);
  ReactiveEnergyB /= 100;
  pDat += 4;
  ReactiveEnergyC = bytesToUInt32(pDat);
  ReactiveEnergyC /= 100;
  pDat += 4;
  TotalReactiveEnergy = bytesToUInt32(pDat);
  TotalReactiveEnergy /= 100;
  pDat += 4;
  ApparentEnergyA = bytesToUInt32(pDat);
  ApparentEnergyA /= 100;
  pDat += 4;
  ApparentEnergyB = bytesToUInt32(pDat);
  ApparentEnergyB /= 100;
  pDat += 4;
  ApparentEnergyC = bytesToUInt32(pDat);
  ApparentEnergyC /= 100;
  pDat += 4;
  TotalApparentEnergy = bytesToUInt32(pDat);  // reg 0x0130, 0x0131
  TotalApparentEnergy /= 100;
  pDat += 4;
  xdir = bytesToUInt16(pDat);
  pDat += 2; 
  xalar = bytesToUInt16(pDat);
  pDat += 2; 

  xdirLow = lowByte(xdir);   // Highbyte is not used
                             // lowbyte   bit7~bit0 are
                             //  total reactive, C phase reactive, B phase reactive, A phase reactive,
                             //  total active, C  phasea ctive, B phase active, A phase active
                             //    (0 is forward, 1 is reverse)

  // quindi qui in base ai bit inverto i segni.
  if (xdirLow & 128) TotalReactivePower = -TotalReactivePower;
  if (xdirLow & 64) ReactivePowerC = -ReactivePowerC;
  if (xdirLow & 32) ReactivePowerB = -ReactivePowerB;
  if (xdirLow & 16) ReactivePowerA = -ReactivePowerA;
  if (xdirLow & 8)  TotalActivePower = -TotalActivePower;
  if (xdirLow & 4)  ActivePowerC = -ActivePowerC;
  if (xdirLow & 2)  ActivePowerB = -ActivePowerB;
  if (xdirLow & 1)  ActivePowerA = -ActivePowerA;

  // ed adesso altri campi
  // direi che ActiveEnergyA = Abs(ForwardActiveEnergyA - OppositeActiveEnergyA)
  // quindi ActiveEnergyA sopra va interpretata, sarei tentato di toglierla e lasciare queste ma ok.

  ForwardActiveEnergyA = bytesToUInt32(pDat);
  ForwardActiveEnergyA /= 100;
  pDat += 4;
  ForwardActiveEnergyB = bytesToUInt32(pDat);
  ForwardActiveEnergyB /= 100;
  pDat += 4;
  ForwardActiveEnergyC = bytesToUInt32(pDat);
  ForwardActiveEnergyC /= 100;
  pDat += 4;
  ForwardTotalActiveEnergy = bytesToUInt32(pDat);
  ForwardTotalActiveEnergy /= 100;
  pDat += 4;
  OppositeActiveEnergyA = bytesToUInt32(pDat);
  OppositeActiveEnergyA /= 100;
  pDat += 4;
  OppositeActiveEnergyB = bytesToUInt32(pDat);
  OppositeActiveEnergyB /= 100;
  pDat += 4;
  OppositeActiveEnergyC = bytesToUInt32(pDat);
  OppositeActiveEnergyC /= 100;
  pDat += 4;
  OppositeTotalActiveEnergy = bytesToUInt32(pDat);
  OppositeTotalActiveEnergy /= 100;
  pDat += 4;
  ForwardReactiveEnergyA = bytesToUInt32(pDat);
  ForwardReactiveEnergyA /= 100;
  pDat += 4;
  ForwardReactiveEnergyB = bytesToUInt32(pDat);
  ForwardReactiveEnergyB /= 100;
  pDat += 4;
  ForwardReactiveEnergyC = bytesToUInt32(pDat);
  ForwardReactiveEnergyC /= 100;
  pDat += 4;
  ForwardTotalReactiveEnergy = bytesToUInt32(pDat);
  ForwardTotalReactiveEnergy /= 100;
  pDat += 4;
  OppositeReactiveEnergyA = bytesToUInt32(pDat);
  OppositeReactiveEnergyA /= 100;
  pDat += 4;
  OppositeReactiveEnergyB = bytesToUInt32(pDat);
  OppositeReactiveEnergyB /= 100;
  pDat += 4;
  OppositeReactiveEnergyC = bytesToUInt32(pDat);
  OppositeReactiveEnergyC /= 100;
  pDat += 4;
  OppositeTotalReactiveEnergy = bytesToUInt32(pDat);
  OppositeTotalReactiveEnergy /= 100;
  pDat += 4;
  
  // mia verifica  (pDat - (_buffer+3))  // Ok 168 is correct !

  return true;
}




boolean ApJSY333::waitResponse(void)
{

  unsigned long startTimeMs = millis();
  unsigned long startTimeus;
  uint16_t ilcrc;
  uint8_t numBytes = 0;
  while (!_serial->available()) {
    if (millis() - startTimeMs >= 400) {   // qui di solito risponde velocemente (meno di 50ms) ma a volte (se non lo si
                                           // consulta velocemente (ogni 1 sec circa) il meter va forse in sleep e 
                                           // impiega tempo prima di rispondere, misurato anche 300 ms !
      return false;
    }
  }

  startTimeMs = millis();  // cosi tolgo il tempo variabile forse di sleep di cui sopra.

  do {
    do {
      if (_serial->available()) {
        startTimeus = micros();
        _buffer[numBytes] = _serial->read();
        numBytes++;
      }
    } while (micros() - startTimeus <= 2870 && numBytes < 173);   // 2870 tempo massimo tra un carattere e l'altro i caratteri devono arrivare uno di seguito all'altro con un certo ritmo imposto dal modbus
    if (numBytes < 173) // cioè uscito dal do per fuori ritmo..
    {
      numBytes = 0;   // caratteri arrivati prima sono rumore o skifezze li butta via !
    }
  } while (millis() - startTimeMs <= 300 && numBytes < 173);    // di solito per trasferire a 9600 tutta la enorme risposta
                                                                // servono 185 ms metto 300 anche qui.

  if (numBytes < 173)
  {
    // non sono arrivati tutti i caratteri della risposta quindi non va bene ritorna false.
    return false;
  }
  // ok vediamo se il crc è giusto
  ilcrc = calculateCrc(_buffer,171);   // Qui servono ben 1180 micros su un NANO ATMega328P per calcolare questo crc

  if (_buffer[171] != highByte(ilcrc) || _buffer[172] != lowByte(ilcrc) )
  {
     return false;  // crc errato !
  }
  // vediamo anche se questi sono giusti !
  if (_buffer[0] != _Addre || _buffer[1] != 3 || _buffer[2] != 0xA8) return false;
  return true; // ok è giusto !
}


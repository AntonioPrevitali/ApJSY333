#ifndef ApJSY333_h
#define ApJSY333_h

/*
  ApJSY333.h - ApJSY333 include file
  For instructions, go to https://github.com/AntonioPrevitali/ApJSY333
  Created by Antonio Previtali, 29/07/2025.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the Gnu general public license version 3
*/

#include "Arduino.h"


class ApJSY333 {
  public:
    ApJSY333(void);

    void UseSerialAndEPin(Stream *xserial, uint8_t xpinDE, uint8_t xpinRE );

    boolean okReadMeter(uint8_t xAddress);

    float VoltageA;
    float VoltageB;
    float VoltageC;
    float CurrentA;
    float CurrentB;
    float CurrentC;
    float ActivePowerA;
    float ActivePowerB;
    float ActivePowerC;
    float TotalActivePower;
    float ReactivePowerA;
    float ReactivePowerB;
    float ReactivePowerC;
    float TotalReactivePower;
    float ApparentPowerA;
    float ApparentPowerB;
    float ApparentPowerC;
    float TotalApparentPower;
    float Frequency;
    float PowerFactorA;
    float PowerFactorB;
    float PowerFactorC;
    float TotalPowerFactor;
    float ActiveEnergyA;           // vedi avanti ForwardActiveEnergyA e OppositeActiveEnergyA
    float ActiveEnergyB;
    float ActiveEnergyC;
    float TotalActiveEnergy;
    float ReactiveEnergyA;
    float ReactiveEnergyB;
    float ReactiveEnergyC;
    float TotalReactiveEnergy;
    float ApparentEnergyA;
    float ApparentEnergyB;
    float ApparentEnergyC;
    float TotalApparentEnergy;

    float ForwardActiveEnergyA;        // direi che ActiveEnergyA = Abs(ForwardActiveEnergyA - OppositeActiveEnergyA)
    float ForwardActiveEnergyB;
    float ForwardActiveEnergyC;
    float ForwardTotalActiveEnergy;
    float OppositeActiveEnergyA;
    float OppositeActiveEnergyB;
    float OppositeActiveEnergyC;
    float OppositeTotalActiveEnergy;
    float ForwardReactiveEnergyA;
    float ForwardReactiveEnergyB;
    float ForwardReactiveEnergyC;
    float ForwardTotalReactiveEnergy;
    float OppositeReactiveEnergyA;
    float OppositeReactiveEnergyB;
    float OppositeReactiveEnergyC;
    float OppositeTotalReactiveEnergy; 


  private:
    boolean  waitResponse(void);
    uint16_t calculateCrc(byte* data, size_t size);
    uint16_t bytesToUInt16(byte* regData);
    uint32_t bytesToUInt32(byte* regData);

    boolean  okTranza(void);

    Stream *_serial;

    uint8_t _pinDE;
    uint8_t _pinRE;

    uint8_t _Addre;

    uint8_t _buffer[173];

};


#endif

// Sample program for usage ApJSY333 library whith Arduino Nano
// For instructions, go to https://github.com/AntonioPrevitali/ApJSY333
// Created by Antonio Previtali 29/07/2025

// Meter configuration is default ( address 1 9600bps 8,N,1 )

// Aduino Nano -->  5 Volt TTL to RS485 converter (max485 DE RE pin) ---> JSY-MK-333 three-phase meter

// The JSY-MK-333 also has a serial output (3.3V, not directly connectable to the 5V Arduinos), so I preferred
// to communicate via the RS485 bus.
// Obviously, however, you also need the 5V to RS485 TTL converter for Arduino.
 
//   use arduino Serial as console.
//   use AltSoftSerial which goes on pins 8 and 9, on 8 Arduino receives data and on 9 Arduino transmits data.

//   Pin for the 5v TTL to RS485 converter:
//     DI connected to PIN 9 arduino which is where arduino transmits data with AltSoftSerial.
//     DE is the MAX485 PIN to enable the transmission of data it receives on the DI. HIGH to enable. I USED PIN 4
//     RO connected to Arduino PIN 8 which is where Arduino receives data with AltSoftSerial.
//     RE is the PIN of the MAX485 to enable reception which sends out on its RO pin LOW to enable I USED PIN 5
//

#include <Arduino.h>

#include <AltSoftSerial.h>   // library for an extra serial port for arduino nano.
                             // seee AltSoftSerial docu.

#include <ApJSY333.h>      // library for meter JSY-MK-333

ApJSY333 MyJSY333;     // declare object MyJSY333

AltSoftSerial altSerial; // object for AltSoftSerial (pin 8 and 9 seee AltSoftSerial docu)


void setup() {

    Serial.begin(9600);    // serial monitor console at 9600 bps
    while(!Serial){};
    delay(200); 

    pinMode(4, OUTPUT);         // DE max 485
    pinMode(5, OUTPUT);         // RE max 485
    altSerial.begin(9600);                        // default speed of meter.
    MyJSY333.UseSerialAndEPin(&altSerial,4,5);    // pass stream and pin to use at library.
                                                  // Please note that PinMode is not executed by the library, so you must execute PinMode, see the code.

    delay(200);   // a bit of time to start altSerial (NOT REMOVE, TEST)
    
}


void loop() {
 
    // 1 is the meter address. By default, the meter is set to address 1.
    // Use the JSY software to set the meter to a different address.
    // This is necessary if you want to use more than one meter. 
    // Simple is usage default address 1.
 
    // I display all available fields that make sense in a true three-phase system.
    // If you use this meter in a single-phase system with three loads, see Sample2.ino
    
    if ( MyJSY333.okReadMeter(1) )   
    {
      Serial.println(F("----- JSY-MK-333 -----"));
      Serial.print(F("VoltageA = "));
      Serial.println(MyJSY333.VoltageA);
      Serial.print(F("VoltageB = "));
      Serial.println(MyJSY333.VoltageB);
      Serial.print(F("VoltageC = "));
      Serial.println(MyJSY333.VoltageC);
      Serial.print(F("CurrentA = "));
      Serial.println(MyJSY333.CurrentA);
      Serial.print(F("CurrentB = "));
      Serial.println(MyJSY333.CurrentB);
      Serial.print(F("CurrentC = "));
      Serial.println(MyJSY333.CurrentC);
      Serial.print(F("ActivePowerA = "));
      Serial.println(MyJSY333.ActivePowerA);
      Serial.print(F("ActivePowerB = "));
      Serial.println(MyJSY333.ActivePowerB);
      Serial.print(F("ActivePowerC = "));
      Serial.println(MyJSY333.ActivePowerC);      
      Serial.print(F("TotalActivePower (W) = "));
      Serial.println(MyJSY333.TotalActivePower); 
      Serial.print(F("ReactivePowerA (var) = "));
      Serial.println(MyJSY333.ReactivePowerA);
      Serial.print(F("ReactivePowerB (var) = "));
      Serial.println(MyJSY333.ReactivePowerB);
      Serial.print(F("ReactivePowerC (var) = "));
      Serial.println(MyJSY333.ReactivePowerC);
      Serial.print(F("TotalReactivePower (var) = "));
      Serial.println(MyJSY333.TotalReactivePower);
      Serial.print(F("ApparentPowerA (VA) = "));
      Serial.println(MyJSY333.ApparentPowerA);
      Serial.print(F("ApparentPowerB (VA) = "));
      Serial.println(MyJSY333.ApparentPowerB);
      Serial.print(F("ApparentPowerC (VA) = "));
      Serial.println(MyJSY333.ApparentPowerC);
      Serial.print(F("TotalApparentPower (VA) = "));
      Serial.println(MyJSY333.TotalApparentPower);
      Serial.print(F("Frequency (Hz) = "));
      Serial.println(MyJSY333.Frequency);
      Serial.print(F("PowerFactorA = "));
      Serial.println(MyJSY333.PowerFactorA);
      Serial.print(F("PowerFactorB = "));
      Serial.println(MyJSY333.PowerFactorB);
      Serial.print(F("PowerFactorC = "));
      Serial.println(MyJSY333.PowerFactorC);
      Serial.print(F("TotalPowerFactor = "));
      Serial.println(MyJSY333.TotalPowerFactor);
      Serial.print(F("ActiveEnergyA (KWh) = "));
      Serial.println(MyJSY333.ActiveEnergyA);        // see ForwardActiveEnergyA e OppositeActiveEnergyA
                                                     // ActiveEnergyA = Abs(ForwardActiveEnergyA - OppositeActiveEnergyA)
      Serial.print(F("ActiveEnergyB (KWh) = "));        
      Serial.println(MyJSY333.ActiveEnergyB);
      Serial.print(F("ActiveEnergyC (KWh) = "));
      Serial.println(MyJSY333.ActiveEnergyC);
      Serial.print(F("TotalActiveEnergy (KWh) = "));
      Serial.println(MyJSY333.TotalActiveEnergy);
      Serial.print(F("ReactiveEnergyA (kvarh) = "));
      Serial.println(MyJSY333.ReactiveEnergyA);
      Serial.print(F("ReactiveEnergyB (kvarh) = "));
      Serial.println(MyJSY333.ReactiveEnergyB);
      Serial.print(F("ReactiveEnergyC (kvarh) = "));
      Serial.println(MyJSY333.ReactiveEnergyC);      
      Serial.print(F("TotalReactiveEnergy (kvarh) = "));
      Serial.println(MyJSY333.TotalReactiveEnergy);
      Serial.print(F("ApparentEnergyA (kVAh) = "));
      Serial.println(MyJSY333.ApparentEnergyA);
      Serial.print(F("ApparentEnergyB (kVAh) = "));
      Serial.println(MyJSY333.ApparentEnergyB);      
      Serial.print(F("ApparentEnergyC (kVAh) = "));
      Serial.println(MyJSY333.ApparentEnergyC);
      Serial.print(F("TotalApparentEnergy (kVAh) = "));
      Serial.println(MyJSY333.TotalApparentEnergy);

      Serial.print(F("ForwardActiveEnergyA (kWh) = "));
      Serial.println(MyJSY333.ForwardActiveEnergyA);
      Serial.print(F("ForwardActiveEnergyB (kWh) = "));
      Serial.println(MyJSY333.ForwardActiveEnergyB);
      Serial.print(F("ForwardActiveEnergyC (kWh) = "));
      Serial.println(MyJSY333.ForwardActiveEnergyC);
      Serial.print(F("ForwardTotalActiveEnergy (kWh) = "));
      Serial.println(MyJSY333.ForwardTotalActiveEnergy);

      Serial.print(F("OppositeActiveEnergyA (kWh) = "));
      Serial.println(MyJSY333.OppositeActiveEnergyA);
      Serial.print(F("OppositeActiveEnergyB (kWh) = "));
      Serial.println(MyJSY333.OppositeActiveEnergyB);
      Serial.print(F("OppositeActiveEnergyC (kWh) = "));
      Serial.println(MyJSY333.OppositeActiveEnergyC);
      Serial.print(F("ForwardTotalActiveEnergy (kWh) = "));
      Serial.println(MyJSY333.ForwardTotalActiveEnergy);

      Serial.print(F("ForwardReactiveEnergyA (kvarh) = "));
      Serial.println(MyJSY333.ForwardReactiveEnergyA);
      Serial.print(F("ForwardReactiveEnergyB (kvarh) = "));
      Serial.println(MyJSY333.ForwardReactiveEnergyB);
      Serial.print(F("ForwardReactiveEnergyC (kvarh) = "));
      Serial.println(MyJSY333.ForwardReactiveEnergyC);
      Serial.print(F("ForwardTotalReactiveEnergy (kvarh) = "));
      Serial.println(MyJSY333.ForwardTotalReactiveEnergy);

      Serial.print(F("OppositeReactiveEnergyA (kvarh) = "));
      Serial.println(MyJSY333.OppositeReactiveEnergyA);
      Serial.print(F("OppositeReactiveEnergyB (kvarh) = "));
      Serial.println(MyJSY333.OppositeReactiveEnergyB);
      Serial.print(F("OppositeReactiveEnergyC (kvarh) = "));
      Serial.println(MyJSY333.OppositeReactiveEnergyC);
      Serial.print(F("OppositeTotalReactiveEnergy (kvarh) = "));
      Serial.println(MyJSY333.OppositeTotalReactiveEnergy);

    }
    else  Serial.println(F("Error reading meter"));

    delay(1000);  // time for human reading
}

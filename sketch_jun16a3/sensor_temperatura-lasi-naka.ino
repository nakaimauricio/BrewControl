#include "timer.h"

void sensor_temperatura (void);

byte Estado_sensor_temperatura = 1;
byte addr[8];
byte present = 0;
byte timer_sensor_temperatura;
byte data[12];
int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;

void sensor_temperatura (void){
    byte i;
//  Serial.print("Estado_sensor_temperatura "); 
//  Serial.println(Estado_sensor_temperatura); 
 
  switch(Estado_sensor_temperatura){
    case 1:
//      Serial.print(Estado_sensor_temperatura);
      timer_sensor_temperatura = get_timer();
//      Serial.print(Estado_sensor_temperatura);
      Estado_sensor_temperatura++;
      break;
    case 2:
      ds.reset_search();
      if ( ds.search(addr)) {
        Serial.print("No more addresses.\n");
        Estado_sensor_temperatura++;
      }
      else{
        Serial.print("Procurando\n");
      }
      break;
    case 3:
      Serial.print("R=");
      for( i = 0; i < 8; i++) {
        Serial.print(addr[i], HEX);
        Serial.print(" ");
      }
      if ( addr[0] == 0x10) {
        Serial.print("Device is a DS18S20 family device.\n");
      }
      else if ( addr[0] == 0x28) {
        Serial.print("Device is a DS18B20 family device.\n");
      }
      else {
        Serial.print("Device family is not recognized: 0x");
        Serial.println(addr[0],HEX);
        return;
      }
      Estado_sensor_temperatura++;
      break;
    case 4:
      ds.reset();
      ds.select(addr);
      ds.write(0x44,1);         // start conversion, with parasite power on at the end
      init_timer(timer_sensor_temperatura,1000);
      Estado_sensor_temperatura++;
      break;
    case 5:
      if (check_timer(timer_sensor_temperatura)==0 ){
        Estado_sensor_temperatura++;
        // we might do a ds.depower() here, but the reset will take care of it.
      }
      break;
    case 6:
      present = ds.reset();
      ds.select(addr);    
      ds.write(0xBE);         // Read Scratchpad
//  Serial.print("P=");
//  Serial.print(present,HEX);
//  Serial.print(" ");
      for ( i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = ds.read();
        Serial.print(data[i], HEX);
        Serial.print(" ");
      }
//  Serial.print(" CRC=");
//  Serial.print( OneWire::crc8( data, 8), HEX);
//  Serial.println();
//}
      Estado_sensor_temperatura++;
      break;
    case 7:
      LowByte = data[0];
      HighByte = data[1];
      TReading = (HighByte << 8) + LowByte;
      SignBit = TReading & 0x8000;  // test most sig bit
      if (SignBit) // negative
      {
        TReading = (TReading ^ 0xffff) + 1; // 2's comp
      }
      Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25
    
      Whole = Tc_100 / 100;  // separate off the whole and fractional portions
      Fract = Tc_100 % 100;
    
      if (SignBit) // If its negative
      {
         Serial.print("-");
      }
//      lcd.clear();
      lcd.setCursor(2,1);
      lcd.print(Whole);
      lcd.print(".");

      Serial.print(Whole);
      Serial.print(".");
      if (Fract < 10)
      {
        Serial.print("0");
        lcd.print("0");
      }
      Serial.print(Fract);
      lcd.print(Fract);
      lcd.print("C");

      Serial.print("\n");
      Estado_sensor_temperatura=4;
      break;
  }
}

int get_temperatura(int inteiro, int decimos){
  if (Whole == 0 && Fract == 0){
    return 0;
  }else if (Whole<inteiro) {
    return 1;
  }else if(Whole>inteiro) {
    return 0;
  }else if(Whole==inteiro) {
    if (Fract<decimos){
      return 1;
    }else {
      return 0;      
    }
  }
}
//
//
//
//LowByte = data[0];
//  HighByte = data[1];
//  TReading = (HighByte << 8) + LowByte;
//  SignBit = TReading & 0x8000;  // test most sig bit
//  if (SignBit) // negative
//  {
//    TReading = (TReading ^ 0xffff) + 1; // 2's comp
//  }
//  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25
//
//  Whole = Tc_100 / 100;  // separate off the whole and fractional portions
//  Fract = Tc_100 % 100;
//
//
//  if (SignBit) // If its negative
//  {
//     Serial.print("-");
//  }
//  Serial.print(Whole);
//  Serial.print(".");
//  if (Fract < 10)
//  {
//     Serial.print("0");
//  }
//  Serial.print(Fract);
//
//  Serial.print("\n");

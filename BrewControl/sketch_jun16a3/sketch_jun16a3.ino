
/*
 * Projeto para Arduino UNO
 * Controle de temperatura para fermentação da cerveja
 * Criação: 23/07/2020
 * Atualização: 23/07/2020
 * Autor: Mauricio Eiji Nakai
 */

#include <LiquidCrystal.h>
#include <OneWire.h>
#include "sensor_temperatura.h"
#include "timer.h"

#define saida_aquecedor 12

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
#define ONE_WIRE_BUS 10
OneWire ds(ONE_WIRE_BUS);
 
void setup() {
  pinMode(saida_aquecedor, OUTPUT);
  pinMode(ONE_WIRE_BUS, INPUT_PULLUP);
  
  
  lcd.begin(16,2);
  lcd.noCursor();
  lcd.noBlink();
  lcd.display();
  lcd.noAutoscroll();
  lcd.leftToRight();
  lcd.print("D.");
  lcd.setCursor(0,2);
  lcd.print("A.");
  Serial.begin(9600);
  timer_pool_init();

  // put your setup code here, to run once:
}
//byte contador;

void loop() {
//  Serial.print("Estado_sensor_temperatura "); 
//  Serial.println(contador++); 
    decrement_timers();
    sensor_temperatura();
    interface();
    LCD_buttons();
}

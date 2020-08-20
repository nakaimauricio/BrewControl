#include <LiquidCrystal.h>

byte Estado_interface = 0;
byte timer_interface = 0;
byte P_inteiro = 28;
byte P_fracao = 0;
byte botao_temp = btnNONE;
byte tempo_aquecedor = 1;
byte timer_delaydisplay = 0;
unsigned long calc_temp = 0;

void interface(void) {
//  Serial.print("timer_interface ");
//  Serial.println(timer_interface);
  switch (Estado_interface) {
    case 0:
      timer_interface = get_timer();
      init_timer(timer_interface, 1000);
      timer_delaydisplay = get_timer();
      init_timer(timer_delaydisplay, 200);
      Estado_interface++;
    case 1:
      lcd.setCursor(2,0);
      lcd.print(P_inteiro);
      lcd.print(".");
      if (P_fracao < 10){
        lcd.print("0");
      }
      lcd.print(P_fracao);
      lcd.print("C");
      Estado_interface++;
      break;
    case 2:
      botao_temp = btnNONE;
      botao_temp = read_LCD_buttons();
      if (botao_temp == btnUP){
        if (P_inteiro < 30){
          if (P_fracao == 50){
            P_fracao = 0;
            P_inteiro++;
          }else {
            P_fracao = 50;
          }
        }
      }
      Estado_interface++;
      break;
    case 3:
      if (botao_temp == btnDOWN){
        if (P_inteiro > 23){
          if (P_fracao == 50){
            P_fracao = 0;
          }else {
            P_fracao = 50;
            P_inteiro--;
          }
        }
      }
      Estado_interface++;
      break;    
    case 4:
      if (botao_temp == btnRIGHT){
        if (tempo_aquecedor < 99){
          tempo_aquecedor++;
        }
      }
      if (botao_temp == btnLEFT){
        if (tempo_aquecedor >1){
          tempo_aquecedor--;
        }
      }
      if (check_timer(timer_delaydisplay)==0 ){
        init_timer(timer_delaydisplay, 200);
        lcd.setCursor(10,1);
        lcd.print("       ");
        lcd.setCursor(10,1);
        lcd.print(tempo_aquecedor);
        lcd.print(" min");
        lcd.setCursor(13,0);
        lcd.print("    ");
        lcd.setCursor(13,0);
        lcd.print(check_timer(timer_interface)/1000);
      }
      Estado_interface++;
      break;
    case 5:
      Serial.println(check_timer(timer_interface));
      if (check_timer(timer_interface)==0 ){
        if (get_temperatura(P_inteiro,P_fracao)){
          lcd.setCursor(9,0);
          lcd.print("lig.");
          digitalWrite(saida_aquecedor,LOW);
        }else{
          lcd.setCursor(9,0);
          lcd.print("    ");
          digitalWrite(saida_aquecedor,HIGH);
        }
//        calc_temp = tempo_aquecedor*60000;
//        Serial.println(calc_temp);
        init_timer(timer_interface,tempo_aquecedor*60000);
//        Serial.println(check_timer(timer_interface));
      }
      Estado_interface=1;
      break;
    case 6:
//      if (check_timer(timer_interface)==0 ){
////        saida_aquecedor:
//      }
      break;
    case 7:
      break;
  }
}

//  Serial.print("sensoraquario2 ");
//  Serial.println(lcd_key);
//  lcd_key = read_LCD_buttons();   // read the buttons


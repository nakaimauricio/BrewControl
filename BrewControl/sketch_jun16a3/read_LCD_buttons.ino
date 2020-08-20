/* 
 *  Filtro de entrada de botões
 *  Autor: Maurici Eiji Nakai
 *  Criado em 11/07/2020
 *  Atualizado em 23/07/2020
*/



int flag = btnNONE;
int flag_temp = btnNONE;
int adc_key_in  = 0;
int Estado_LCD_buttons = 0;
int timer_debounce;
int flag_btnRIGHT,flag_btnUP,flag_btnDOWN,flag_btnLEFT,flag_btnSELECT;

#define tempo_debounce 1
#define limit_debounce 10
#define inicial 0
#define principal 1
#define limit_ad 20

int read_LCD_buttons(int botao) {              // read the buttons
    if (botao==flag){
        flag = btnNONE;
        return true;
    }else{
        return false;
    }
}

int LCD_buttons(void){
    switch (Estado_LCD_buttons) {
        case inicial:
            timer_debounce = get_timer();
            init_timer(timer_debounce, tempo_debounce);
            Estado_LCD_buttons = principal;
            break;
        case principal:
//            Serial.println(check_timer(timer_debounce));
            if (check_timer(timer_debounce)==0 ){
                init_timer(timer_debounce, tempo_debounce);
                adc_key_in = analogRead(0);       // read the value from the sensor
                if (adc_key_in < 50 && flag_btnRIGHT<=limit_debounce) {
                    flag_btnRIGHT+=2;
                }else if (adc_key_in < 144+limit_ad && adc_key_in > 144-limit_ad && flag_btnUP<=limit_debounce) {
                    flag_btnUP+=2;
                }else if (adc_key_in < 340+limit_ad && adc_key_in > 340-limit_ad && flag_btnDOWN<=limit_debounce) {
                    flag_btnDOWN+=2;
                }else if (adc_key_in < 510+limit_ad && adc_key_in > 510-limit_ad && flag_btnLEFT<=limit_debounce) {
                    flag_btnLEFT+=2;
                }else if (adc_key_in < 732+limit_ad && adc_key_in > 732-limit_ad && flag_btnSELECT<=limit_debounce) {
                    flag_btnSELECT+=2;
                }
                if (flag_btnRIGHT>0 && flag_btnRIGHT<limit_debounce)      flag_btnRIGHT--;
                if (flag_btnUP>0 && flag_btnUP<limit_debounce)         flag_btnUP--;
                if (flag_btnDOWN>0 && flag_btnDOWN<limit_debounce)       flag_btnDOWN--;
                if (flag_btnLEFT>0 && flag_btnLEFT<limit_debounce)       flag_btnLEFT--;
                if (flag_btnSELECT>0 && flag_btnSELECT<limit_debounce)     flag_btnSELECT--;     
                if (adc_key_in > 1000) {
                    if (flag_btnRIGHT>=limit_debounce){
                        flag_btnRIGHT=0;
                        flag = btnRIGHT;
                    }else if (flag_btnUP>=limit_debounce) {
                        flag_btnUP=0;
                        flag = btnUP;
                    }else if (flag_btnDOWN>=limit_debounce) {
                        flag_btnDOWN=0;
                        flag = btnDOWN;
                    }else if (flag_btnLEFT>=limit_debounce) {
                        flag_btnLEFT=0;
                        flag = btnLEFT;
                    }else if (flag_btnSELECT>=limit_debounce) {
                        flag_btnSELECT=0;
                        flag = btnSELECT;
                    }
                }           
//                Serial.print(adc_key_in);
//                Serial.print(" ");
//                Serial.println(flag);
//                Serial.print(" ");
            }
    }
}

  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  // We make this the 1st option for speed reasons since it will be the most likely result




    // For V1.1 us this threshold
   

  // For V1.0 comment the other threshold and use the one below:
  /*
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 380)  return btnDOWN;
    if (adc_key_in < 555)  return btnLEFT;
    if (adc_key_in < 790)  return btnSELECT;
  */

//  return btnNONE;                // when all others fail, return this.




int flag = btnNONE;
int flag_temp = btnNONE;
int adc_key_in  = 0;

int read_LCD_buttons() {              // read the buttons
  adc_key_in = analogRead(0);       // read the value from the sensor

  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  // We make this the 1st option for speed reasons since it will be the most likely result

  if (adc_key_in > 1000){
    flag_temp = flag;
    flag = btnNONE;
    return flag_temp;
  }

  // For V1.1 us this threshold
  if (adc_key_in < 50) {
    flag = btnRIGHT;
        Serial.println("right");
  }else if (adc_key_in < 250) {
    flag = btnUP;
        Serial.println("up");
  }else if (adc_key_in < 450) {
    flag = btnDOWN;
        Serial.println("down");
  }else if (adc_key_in < 650) {
    flag = btnLEFT;
         Serial.println("left");
 }else if (adc_key_in < 850) {
    flag = btnSELECT;
             Serial.println("select");

 }

  // For V1.0 comment the other threshold and use the one below:
  /*
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 380)  return btnDOWN;
    if (adc_key_in < 555)  return btnLEFT;
    if (adc_key_in < 790)  return btnSELECT;
  */

//  return btnNONE;                // when all others fail, return this.
}


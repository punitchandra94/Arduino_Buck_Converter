#include <LiquidCrystal.h>
//LCD pin to Arduino
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);
int PWM = 3, voltage = 100;
//Button
int U_Button = 1;
int D_Button = 2;
int L_Button = 3;
int R_Button = 4;
int S_Button = 5;

int sel_screen = 0, cursor = 0;

int button_sel() {
  int x;
  x = analogRead(0);
  delay(200);
  lcd.clear();
  if (x < 60) {
    return R_Button;
  } else if (x < 200) {
    return U_Button;
  } else if (x < 400) {
    return D_Button;
  } else if (x < 600) {
    return L_Button;
  } else if (x < 800) {
    return S_Button;
  }
  return 0;
}

void set_pwm(int sel_btn_state) {
  if (sel_btn_state == U_Button)
    voltage = voltage + 5;
  if (sel_btn_state == D_Button)
    voltage = voltage - 5;

  if (voltage > 1024)
    voltage = 1024;
  if (voltage < 0)
    voltage = 0;

  lcd.setCursor(2, 0);
  lcd.print("PWM: ");
  lcd.print(voltage);

  int VALUE = map(voltage, 0, 1024, 0, 254);
  analogWrite(PWM, VALUE);

  lcd.setCursor(2, 1);
  lcd.print("VAL: ");
  lcd.print(VALUE);
}

//sub_process
void sub_proc(int cursor, int sel_screen) {
  int btn_state = 0;

  if (cursor == 0 && sel_screen == 1) {
    while (1) {
      btn_state = button_sel();
      set_pwm(0);
      lcd.setCursor(0, 0);
      lcd.print("<");

      if (btn_state == U_Button) {
        set_pwm(U_Button);
      }
      if (btn_state == D_Button) {
        set_pwm(D_Button);
      }
      if (btn_state == L_Button)
        break;
    }
  }
  if (cursor == 1 && sel_screen == 1) {
    while (1) {
      lcd.setCursor(0, 0);
      lcd.print("<");
      lcd.setCursor(3, 0);
      lcd.print("Set Curr: NIY");
      if (button_sel() == L_Button)
        break;
    }
  }
}

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("BUCK Converter..");
  lcd.setCursor(0, 1);
  lcd.print("VERSION: 1.0");
  delay(1000);
  pinMode(PWM, OUTPUT);
  TCCR2B = TCCR2B & B11111000 | B00000001;  // pin 3 and 11 PWM frequency of 31372.55 Hz
}

void loop() {

  int left_right_btn_state = 0;
  int sel_btn_state = button_sel();
  // set_pwm(sel_btn_state);
  if (sel_btn_state == U_Button) {
    cursor -= 1;
  }
  if (sel_btn_state == D_Button) {
    cursor += 1;
  }
  if (sel_btn_state == S_Button) {
    sel_screen += 1;
  }
  if (sel_btn_state == L_Button || sel_btn_state == R_Button) {
    left_right_btn_state = sel_btn_state;
  }

  cursor = constrain(cursor, 0, 1);
  if (sel_screen > 1)
    sel_screen = 0;
  sel_screen = constrain(sel_screen, 0, 1);

  switch (sel_screen) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Vin:    ");
      lcd.print("Vout:   ");
      lcd.setCursor(0, 1);
      lcd.print("Iin:    ");
      lcd.print("Iout:    ");
      break;

    case 1:
      if (left_right_btn_state == R_Button) {
        lcd.clear();
        sub_proc(cursor, sel_screen);
      } else {
        lcd.setCursor(0, cursor);
        lcd.print(">");
        lcd.setCursor(1, 0);
        lcd.print(" SET_BATT_VOLT");
        lcd.setCursor(1, 1);
        lcd.print(" SET_BATT_CURR");
      }
      break;
    case 2:
      // SET_Battery_T
      // Set_Load_VOLT
      // Set_CUTO_VOLT


    break;
  }
}

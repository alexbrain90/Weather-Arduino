#include <SFE_BMP180.h>
#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);
DHT temp(2, DHT22);
SFE_BMP180 pressure;

long timer = 0;
unsigned long timer_prev = 0;
float T0=-100,T1=-100,T2=-100,T3=-100,T4=-100,T5=-100,T6=-100,T7=-100;
float H0=-100,H1=-100,H2=-100,H3=-100,H4=-100,H5=-100,H6=-100,H7=-100;
float P0=-100,P1=-100,P2=-100,P3=-100,P4=-100,P5=-100,P6=-100,P7=-100;
float G0=-100,G1=-100,G2=-100,G3=-100,G4=-100,G5=-100,G6=-100,G7=-100;

void setup() {
  lcd.init();
  lcd.backlight();
  temp.begin();
  pressure.begin();
  Serial.begin(9600);
}

void loop() {

  tick();
  
  getTemp();
  getHum();
  getPres();
  getGas();

  Serial.println(timer);
  if (timer <= 0) {
    timer = 3600000;
    T7 = T6; T6 = T5; T5 = T4; T4 = T3; T3 = T2; T2 = T1; T1 = T0;
    H7 = H6; H6 = H5; H5 = H4; H4 = H3; H3 = H2; H2 = H1; H1 = H0;
    P7 = P6; P6 = P5; P5 = P4; P4 = P3; P3 = P2; P2 = P1; P1 = P0;
    G7 = G6; G6 = G5; G5 = G4; G4 = G3; G3 = G2; G2 = G1; G1 = G0;
  }

  showData();
  delay(5000);
}

void tick() {
  unsigned long cur = millis();
  int r = 0;
  if (cur < timer_prev)
    r = 5000;
  else
    r = (int)(cur - timer_prev);

    timer_prev = cur;
    timer -= r;
}

void showData() {

  lcd.setCursor(0,0); lcd.print("                    ");
  lcd.setCursor(0,0);
  lcd.print("T: ");
  if (T0 < 10)
    lcd.print(" ");
  if (T0 >= 0)
    lcd.print("+");
  lcd.print(T0,1);
  lcd.print(" ");
  if (T2 != -100) {
    if ((T0-T2) < 10)
      lcd.print(" ");
    if ((T0-T2) >= 0)
      lcd.print("+");
    lcd.print((T0-T2),1);
    lcd.print(" ");
    if (T7 != -100) {
      if ((T0-T7) < 10)
        lcd.print(" ");
      if ((T0-T7) >= 0)
        lcd.print("+");
      lcd.print((T0-T7),1);
    }
  }

  lcd.setCursor(0,1); lcd.print("                    ");
  lcd.setCursor(0,1);
  lcd.print("H:   ");
  if (H0 < 100)
    lcd.print(" ");
  if (H0 < 10)
    lcd.print(" ");
  lcd.print(H0,0);
  lcd.print("  ");
  if (H2 != -100) {
    if ((H0-H2) < 100)
      lcd.print(" ");
    if ((H0-H2) < 10)
      lcd.print(" ");
    if ((H0-H2) >= 0)
      lcd.print("+");
    lcd.print((H0-H2),0);
    lcd.print("  ");
    if (H7 != -100) {
      if ((H0-H7) < 100)
        lcd.print(" ");
      if ((H0-H7) < 10)
        lcd.print(" ");
      if ((H0-H7) >= 0)
        lcd.print("+");
      lcd.print((H0-H7),0);
      }
  }

  lcd.setCursor(0,2); lcd.print("                    ");
  lcd.setCursor(0,2);
  lcd.print("P: ");
  lcd.print(P0,1);
  lcd.print(" ");
  if (P2 != -100) {
    if ((P0-P2) < 10)
      lcd.print(" ");
    if ((P0-P2) >= 0)
      lcd.print("+");
    lcd.print((P0-P2),1);
    lcd.print(" ");
    if (P7 != -100) {
      if ((P0-P7) < 10)
        lcd.print(" ");
      if ((P0-P7) >= 0)
        lcd.print("+");
      lcd.print((P0-P7),1);
    }
  }

  lcd.setCursor(0,3); lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("G: ");
  if (G0 < 100)
    lcd.print(" ");
  if (G0 < 10)
    lcd.print(" ");
  lcd.print(G0,1);
  if (G2 != -100) {
    if ((G0-G2) < 100)
      lcd.print(" ");
    if ((G0-G2) < 10)
      lcd.print(" ");
    if ((G0-G2) >= 0)
      lcd.print("+");
    lcd.print((G0-G2),1);
    if (G7 != -100) {
      if ((G0-G7) < 100)
        lcd.print(" ");
      if ((G0-G7) < 10)
        lcd.print(" ");
      if ((G0-G7) >= 0)
        lcd.print("+");
      lcd.print((G0-G7),1);
      }
  }
}

void getTemp() {
  T0 = temp.readTemperature();
}
void getHum() {
  H0 = temp.readHumidity();
}
void getPres() {
  char status;
  double T,P,p0,a;
  
  status = pressure.startTemperature();
  if (status != 0){
    delay(status);
    status = pressure.getTemperature(T);
     if (status != 0){
      status = pressure.startPressure(3);
      if (status != 0){
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0){
          P0 = P*0.750062-2.3;
        }
      }
    }
  }
}
void getGas() {
  float temp = (float)(analogRead(0));
  temp /= 1023; temp *= 100;
  G0 = temp;
}

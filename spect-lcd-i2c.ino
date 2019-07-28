#define AUTO_GAIN 1       // автонастройка по громкости (экспериментальная функция)
#define VOL_THR 35        // порог тишины (ниже него отображения на матрице не будет)
#define LOW_PASS 30        // нижний порог чувствительности шумов (нет скачков при отсутствии звука)
#define DEF_GAIN 80       // максимальный порог по умолчанию (при GAIN_CONTROL игнорируется)
#define FHT_N 256          // ширина спектра х2
// вручную забитый массив тонов, сначала плавно, потом круче
byte posOffset[16] = {2, 3, 4, 6, 8, 10, 12, 14, 16, 20, 25, 30, 35, 60, 80, 100};

#define LOG_OUT 1
#include <FHT.h>   
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>       
//#include <LiquidCrystal.h>
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

byte v1[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11011};
byte v2[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11011, 0b11011};
byte v3[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11011, 0b11011, 0b11011};
byte v4[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b11011, 0b11011, 0b11011, 0b11011};
byte v5[8] = {0b00000, 0b00000, 0b00000, 0b11011, 0b11011, 0b11011, 0b11011, 0b11011};
byte v6[8] = {0b00000, 0b00000, 0b11011, 0b11011, 0b11011, 0b11011, 0b11011, 0b11011};
byte v7[8] = {0b00000, 0b11011, 0b11011, 0b11011, 0b11011, 0b11011, 0b11011, 0b11011};
byte v8[8] = {0b11011, 0b11011, 0b11011, 0b11011, 0b11011, 0b11011, 0b11011, 0b11011};

byte gain = DEF_GAIN;   
unsigned long gainTimer;
byte maxValue, maxValue_f;
float k = 0.1;

void setup() { 
//  lcd.begin(16, 2);// LCD 16X2
lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);
  analogReference(INTERNAL);
  lcd.createChar(0, v1);
  lcd.createChar(1, v2);
  lcd.createChar(2, v3);
  lcd.createChar(3, v4);
  lcd.createChar(4, v5);
  lcd.createChar(5, v6);
  lcd.createChar(6, v7);
  lcd.createChar(7, v8);
}

void loop() {
  analyzeAudio();   // функция FHT, забивает массив fht_log_out[] величинами по спектру

  for (int pos = 0; pos < 16; pos++) {   // для окошек дисплея с 0 по 15
    // найти максимум из пачки тонов
    if (fht_log_out[posOffset[pos]] > maxValue) maxValue = fht_log_out[posOffset[pos]];

    lcd.setCursor(pos, 0);

    // преобразовать значение величины спектра в диапазон 0..15 с учётом настроек
    int posLevel = map(fht_log_out[posOffset[pos]], LOW_PASS, gain, 0, 15);
    posLevel = constrain(posLevel, 0, 15);

    if (posLevel > 7) {               // если значение больше 7 (значит нижний квадратик будет полный)
      lcd.write((uint8_t)posLevel - 8);    // верхний квадратик залить тем что осталось
      lcd.setCursor(pos, 1);          // перейти на нижний квадратик
      lcd.write((uint8_t)7);        // залить его полностью
  } 
    else {                          // если значение меньше 8
      lcd.print(" ");                 // верхний квадратик пустой
      lcd.setCursor(pos, 1);          // нижний квадратик
      lcd.write((uint8_t)posLevel);        // залить полосками
   }
  }  

  if (AUTO_GAIN) {
    maxValue_f = maxValue * k + maxValue_f * (1 - k);
    if (millis() - gainTimer > 1500) {      // каждые 1500 мс
      // если максимальное значение больше порога, взять его как максимум для отображения
      if (maxValue_f > VOL_THR) gain = maxValue_f;

      // если нет, то взять порог побольше, чтобы шумы вообще не проходили
      else gain = 100;
      gainTimer = millis();
    }
  }
}

void analyzeAudio() {
  for (int i = 0 ; i < FHT_N ; i++) {
    int sample = analogRead(A3);
    fht_input[i] = sample; // put real data into bins
  }
  fht_window();  // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run();     // process the data in the fht
  fht_mag_log(); // take the output of the fht
}

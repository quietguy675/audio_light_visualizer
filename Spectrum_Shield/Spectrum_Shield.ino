#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define STROBE_PIN 4
#define RESET_PIN 5
#define NEOPIXEL_PIN 6
#define NUM_NEOPIXELS 70
#define NUM_FREQ 7
int LEFT_DATA_PIN=14;
int RIGHT_DATA_PIN=15;
int frequencies_left[NUM_FREQ];
int frequencies_right[NUM_FREQ];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_NEOPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(STROBE_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  pinMode(NEOPIXEL_PIN, OUTPUT);
  pinMode(LEFT_DATA_PIN, INPUT);
  pinMode(RIGHT_DATA_PIN, INPUT);
  digitalWrite(STROBE_PIN, HIGH);
  digitalWrite(RESET_PIN, HIGH);
  Serial.begin(9600);
  Initialize_Spectrum_Analyzers();
  strip.begin();
  strip.show();
}

void loop() {
  /*
  for (int i = 0; i < NUM_FREQ; i++){
    Serial.print("Freq ");
    Serial.print(i);
    Serial.print("\t");
    Serial.print(frequencies_left[i]);
    Serial.print("\t");
    Serial.print(frequencies_right[i]);
    Serial.println();
  }*/
  update_neopixels(50);
}


void update_neopixels(int wait){
  int rescaled_value = 0;
  for(int k=0; k<256; k++) {
    Read_Frequencies(frequencies_left, frequencies_right);
    for (int i = 0; i < NUM_FREQ; i++){
      rescaled_value = map(frequencies_left[i], 0, 1023, 0, 9);
      //Due to the way this is wired every odd row is upside-down.
      if (i%2 == 1){
        for (int j = 9; j >= 0; j--){
          if (j > (9 - rescaled_value)) {
            strip.setPixelColor((i*10)+j, Wheel(k & 255));
          } else {
            strip.setPixelColor((i*10)+j, strip.Color(0,0,0));
          }
        }
      // Normal from bottom - to - top specifying.
      } else {
        for (int j = 0; j <= 9; j++){
          if (j < rescaled_value ){
            strip.setPixelColor((i*10)+j, Wheel(k & 255));
          } else {
            strip.setPixelColor((i*10)+j, strip.Color(0,0,0));
          } 
        }
      }
    }
    strip.show();
    delay(wait);
  }
}

void display_neopixels(int freqs[NUM_FREQ], int wait){
  int rescaled_value = 0;
  uint16_t i, j;

  for(int k=0; k<256; k++) {
    for (int i = 0; i < NUM_FREQ-2; i++){
      rescaled_value = map(freqs[i]*1.25, 0, 1023, 0, 9);
      for (int j = 0; j <= rescaled_value; j++){
        strip.setPixelColor((i*10)+j, Wheel(k & 255));
      }
      for (int j = rescaled_value; j < 10; j++){
        strip.setPixelColor((i*10)+j, strip.Color(0,0,0));
      }
    }
    strip.show();
    delay(wait);
  }
  
}


void Read_Frequencies(int freq_left[7], int freq_right[7]){
  for(int i = 0; i < NUM_FREQ; i++){
    freq_left[i] = analogRead(LEFT_DATA_PIN);
    freq_right[i] = analogRead(RIGHT_DATA_PIN);
    digitalWrite(STROBE_PIN, HIGH);
    digitalWrite(STROBE_PIN, LOW);
  }
}

void Initialize_Spectrum_Analyzers(){
  digitalWrite(STROBE_PIN, LOW);
  delay(1);
  digitalWrite(RESET_PIN, HIGH);
  delay(1);
  digitalWrite(STROBE_PIN, HIGH);
  delay(1);
  digitalWrite(STROBE_PIN, LOW);
  delay(1);
  digitalWrite(RESET_PIN, LOW);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

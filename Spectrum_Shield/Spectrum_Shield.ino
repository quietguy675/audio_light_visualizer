#define STROBE_PIN 4
#define RESET_PIN 5
#define LEFT_DATA_PIN A0
#define RIGHT_DATA_PIN A1
#define NEOPIXEL_PIN 3
#define NUM_FREQ 7
int frequencies_left[NUM_FREQ];
int frequencies_right[NUM_FREQ];

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
}

void loop() {
  Read_Frequencies();
  delay(50);
}

void Read_Frequencies(){
  for(int i = 0; i < NUM_FREQ; i++){
    frequencies_left[i] = analogRead(LEFT_DATA_PIN);
    frequencies_right[i] = analogRead(RIGHT_DATA_PIN);
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



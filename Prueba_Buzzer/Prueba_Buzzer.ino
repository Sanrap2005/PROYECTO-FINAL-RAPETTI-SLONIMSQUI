#define BUZZER_PIN 13     // Pin del buzzer
#define BUZZER_CHANNEL 0 // Canal PWM del buzzer


void setup() {
  
  Serial.begin(115200);
  ledcSetup(BUZZER_CHANNEL, 2000, 8); // Configurar el canal PWM
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL); // Asociar el pin al canal PWM
  ledcWrite(BUZZER_CHANNEL, 0);

}

void loop() {

  Serial.print(digitalRead(13));
  ledcWrite(BUZZER_CHANNEL, 3000);

  delay(2000);

  ledcWrite(BUZZER_CHANNEL, 0);

  delay(2000);

}

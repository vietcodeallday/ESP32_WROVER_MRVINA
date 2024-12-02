#define LED_BUILTIN 15
void setup() {
  // put your setup code here, to run once:
pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(21, HIGH);

  delay(1000);
  
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(21, LOW);
  delay(1000);
}

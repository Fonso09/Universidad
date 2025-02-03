
const int Pin_ADC = 15;
int ADC_value = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  ADC_value =analogRead(Pin_ADC);
  Serial.println(ADC_value);
  delay(10);

}

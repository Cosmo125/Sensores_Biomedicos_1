#define LM35_PIN A0

void setup() {
  Serial.begin(9600);
}

void loop() {
  int lectura = analogRead(LM35_PIN);

  // Convertir lectura ADC (0-1023) a voltaje (0-5V), 
  // y voltaje a temperatura (LM35: 10mV por cada °C)
  float voltaje = lectura * (5.0 / 1023.0);
  float temperaturaC = voltaje * 100.0;

  Serial.print("Temperatura: ");
  Serial.print(temperaturaC);
  Serial.println(" C");

  delay(1000);
}

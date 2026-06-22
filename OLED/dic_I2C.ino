#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Escaneando bus I2C...");
  Serial.println("------------------------------");

  byte nDispositivos = 0;

  for (byte dir = 0x08; dir <= 0x77; dir++) {
    Wire.beginTransmission(dir);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo encontrado en 0x");
      if (dir < 16) Serial.print("0");
      Serial.println(dir, HEX);
      nDispositivos++;
    } else if (error == 4) {
      Serial.print("Error desconocido en 0x");
      if (dir < 16) Serial.print("0");
      Serial.println(dir, HEX);
    }
  }

  Serial.println("------------------------------");
  if (nDispositivos == 0)
    Serial.println("Ningún dispositivo encontrado.");
  else {
    Serial.print(nDispositivos);
    Serial.println(" dispositivo(s) encontrado(s).");
  }
}

void loop() {
  // El escaneo ocurre una sola vez en setup()
}

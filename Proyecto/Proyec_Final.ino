#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PulseSensorPlayground.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PULSE_PIN 0
#define LM35_PIN  A1     // <-- el LM35 va en A1, A0 ya esta ocupado por el pulso
#define THRESHOLD 515

PulseSensorPlayground pulseSensor;

int x = 0;
int lastx = 0;
int lasty = 0;
int BPM = 0;
float temperaturaC = 0;

#define GRAPH_TOP    9
#define GRAPH_BOTTOM (SCREEN_HEIGHT - 1)

int dynMin = 600;
int dynMax = 600;
#define MIN_RANGE 10
#define DECAY     1

// --- Temperatura: leer cada cierto tiempo, no en cada loop ---
unsigned long lastTempRead = 0;
#define TEMP_INTERVAL 1000  // ms

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  pulseSensor.analogInput(PULSE_PIN);
  pulseSensor.setThreshold(THRESHOLD);
  pulseSensor.begin();
}

void loop()
{
  if (x > 127)
  {
    display.clearDisplay();
    x = 0;
    lastx = x;
  }

  int value = pulseSensor.getLatestSample();

  if (value > dynMax) dynMax = value;
  else dynMax -= DECAY;

  if (value < dynMin) dynMin = value;
  else dynMin += DECAY;

  if (dynMax - dynMin < MIN_RANGE) {
    dynMax = dynMin + MIN_RANGE;
  }

  int y = map(value, dynMin, dynMax, GRAPH_BOTTOM, GRAPH_TOP);
  y = constrain(y, GRAPH_TOP, GRAPH_BOTTOM);

  display.writeLine(lastx, lasty, x, y, WHITE);
  lasty = y;
  lastx = x;

  if (pulseSensor.sawStartOfBeat()) {
    BPM = pulseSensor.getBeatsPerMinute();
  }

  // --- Leer LM35 una vez por segundo ---
  if (millis() - lastTempRead >= TEMP_INTERVAL) {
    int lecturaLM35 = analogRead(LM35_PIN);
    float voltaje = lecturaLM35 * (2.0 / 1023.0);
    temperaturaC = voltaje * 100.0;
    lastTempRead = millis();
  }

  // --- Mostrar BPM y Temperatura en la misma linea ---
  display.fillRect(0, 0, SCREEN_WIDTH, 8, BLACK);
  display.setCursor(0, 0);
  display.print(BPM);
  display.print(" BPM  ");
  display.print(temperaturaC, 1);
  display.print(" C");
  display.display();

  x++;
}

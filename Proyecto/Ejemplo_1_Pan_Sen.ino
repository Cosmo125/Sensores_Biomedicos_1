#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PulseSensorPlayground.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SENSOR_PIN 0
#define THRESHOLD  515

PulseSensorPlayground pulseSensor;

int x = 0;
int lastx = 0;
int lasty = 0;
int BPM = 0;

// Area del grafico (deja 9px arriba para el texto "BPM")
#define GRAPH_TOP    9
#define GRAPH_BOTTOM (SCREEN_HEIGHT - 1)

// --- Auto-escala ---
int dynMin = 600;
int dynMax = 600;
#define MIN_RANGE 10   // ancho minimo del rango, evita division entre casi 0
#define DECAY     1    // que tan rapido se "olvida" el min/max viejo

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  pulseSensor.analogInput(SENSOR_PIN);
  pulseSensor.setThreshold(THRESHOLD);
  pulseSensor.begin();   // <-- ESTO FALTABA: activa el muestreo de la libreria
}

void loop()
{
  if (x > 127)
  {
    display.clearDisplay();
    x = 0;
    lastx = x;
  }

  int value = pulseSensor.getLatestSample();  // mismo dato que usa la libreria

  // --- Actualizar rango dinamico ---
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

  // --- Calculo de BPM ---
  if (pulseSensor.sawStartOfBeat()) {
    BPM = pulseSensor.getBeatsPerMinute();
  }

  // --- Mostrar BPM ---
  display.fillRect(0, 0, SCREEN_WIDTH, 8, BLACK);
  display.setCursor(0, 0);
  display.print(BPM);
  display.print(" BPM");
  display.display();

  x++;
}

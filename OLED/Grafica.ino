#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Parametros de la senoidal ---
#define FREQ_HZ     60.0      // frecuencia en Hz
#define AMPLITUDE_V 120.0     // amplitud pico en V

// --- Tiempo simulado ---
float t = 0;
const float dt = 0.0005;      // paso de tiempo por pixel (0.5 ms) -> controla "zoom" en el tiempo

// --- Area del grafico (deja 9px arriba para el texto) ---
#define GRAPH_TOP    9
#define GRAPH_BOTTOM (SCREEN_HEIGHT - 1)
#define GRAPH_CENTER ((GRAPH_TOP + GRAPH_BOTTOM) / 2.0)
#define GRAPH_HALF   ((GRAPH_BOTTOM - GRAPH_TOP) / 2.0)

int x = 0;
int lastx = 0;
int lasty = (int)GRAPH_CENTER;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  if (x > 127) {
    display.clearDisplay();
    x = 0;
    lastx = 0;
  }

  // v(t) = A * sin(2*pi*f*t)
  float v = AMPLITUDE_V * sin(2.0 * PI * FREQ_HZ * t);

  // Escalar el voltaje (-120V a +120V) al area del grafico
  int y = (int)(GRAPH_CENTER - (v / AMPLITUDE_V) * GRAPH_HALF);
  y = constrain(y, GRAPH_TOP, GRAPH_BOTTOM);

  display.writeLine(lastx, lasty, x, y, WHITE);
  lasty = y;
  lastx = x;

  // --- Etiqueta superior ---
  display.fillRect(0, 0, SCREEN_WIDTH, 8, BLACK);
  display.setCursor(0, 0);
  display.print("60Hz  120Vp");

  display.display();

  t += dt;
  x++;
}

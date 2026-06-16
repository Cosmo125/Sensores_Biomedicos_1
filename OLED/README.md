# Pantalla OLED 128×32 (SSD1306)

Este proyecto utiliza una pantalla **OLED de 128×32 píxeles** con controlador **SSD1306** para visualizar en tiempo real los datos del sensor cardíaco HW-827: la señal de pulso, el valor de BPM y el estado del ritmo detectado.

## 1. ¿Qué es una pantalla OLED?

**OLED** significa *Organic Light Emitting Diode* (Diodo Orgánico Emisor de Luz). A diferencia de una pantalla LCD, **cada píxel genera su propia luz**: no necesita retroiluminación. Esto tiene consecuencias prácticas importantes:

- Los píxeles apagados consumen **cero energía**, lo que hace que el consumo total sea muy bajo.
- El **contraste es altísimo**: los negros son completamente negros y los blancos son muy brillantes.
- La imagen es **visible incluso a plena luz solar**.
- El módulo físico es **extremadamente compacto** y liviano, ideal para dispositivos portátiles o wearables.

## 2. Especificaciones del módulo 128×32

| Parámetro | Valor |
|---|---|
| Controlador | SSD1306 |
| Resolución | 128 × 32 píxeles |
| Tamaño de pantalla | 0.91" diagonal |
| Dimensiones del PCB | 38 mm × 12 mm |
| Voltaje de operación | 3.3 V – 5.5 V DC |
| Corriente de consumo | ~30 mA (máx.) |
| Consumo total | ~0.097 W |
| Interfaz de comunicación | I2C (IIC) |
| Dirección I2C por defecto | `0x3C` |
| Número de pines | 4 (GND, VCC, SCL, SDA) |
| Temperatura de trabajo | -40 °C a 85 °C |
| Color de píxeles | Blanco / Azul (según variante) |

### Descripción de pines

| Pin | Función |
|---|---|
| **GND** | Tierra (masa) |
| **VCC** | Alimentación (3.3 V o 5 V) |
| **SCL** | Línea de reloj I2C |
| **SDA** | Línea de datos I2C |

> Solo necesita **2 pines de datos** (SDA y SCL) para comunicarse, lo que simplifica mucho el cableado con Arduino o ESP32.

## 3. ¿Cómo funciona?

### Tecnología de pantalla

Cada uno de los **4,096 píxeles** (128 columnas × 32 filas) de esta pantalla es un pequeño diodo orgánico que se enciende o apaga de forma independiente. El controlador **SSD1306** se encarga de gestionar todos esos píxeles a partir de los datos que recibe por I2C.

### Memoria de video (buffer)

El microcontrolador no dibuja directamente en la pantalla píxel por píxel en tiempo real. En cambio, mantiene en su RAM un **buffer de imagen** del mismo tamaño que la pantalla (128 × 32 bits = 512 bytes). Cuando se quiere actualizar lo que se muestra, se envía ese buffer completo al SSD1306 a través del bus I2C, y el controlador lo renderiza en pantalla de una sola vez.

El flujo típico en el código es:

```
1. display.clearDisplay()     → limpiar el buffer en RAM
2. display.print() / display.drawBitmap() / etc. → escribir en el buffer
3. display.display()          → enviar el buffer al SSD1306 y mostrarlo
```

### Comunicación I2C

El protocolo **I2C** (Inter-Integrated Circuit) permite conectar varios dispositivos usando solo dos líneas:

- **SCL** (*Serial Clock Line*): el microcontrolador genera una señal de reloj que sincroniza la transmisión.
- **SDA** (*Serial Data Line*): por aquí viajan los datos en serie, bit a bit.

Cada dispositivo en el bus tiene una **dirección única** (la pantalla usa `0x3C` por defecto). Esto permite tener varios módulos I2C conectados al mismo par de pines del microcontrolador.

## 4. ¿Para qué sirve una pantalla de 128×32?

La resolución 128×32 es pequeña pero suficiente para una gran variedad de aplicaciones embebidas:

- Mostrar **valores numéricos** (BPM, temperatura, voltaje, RPM)
- Desplegar **texto de estado** ("Normal", "Arritmia detectada", etc.)
- Graficar **señales simples en tiempo real** (como la curva de pulso del HW-827)
- Dibujar **iconos y logotipos** en proyectos portátiles o wearables
- Mostrar **menús de configuración** sencillos

En este proyecto se usa para mostrar en tiempo real:
- La **señal analógica del pulso** graficada como una curva
- El **valor de BPM** calculado
- El **tipo de ritmo** detectado (normal, rápido, lento, irregular)

## 5. Mostrar imágenes y gráficos: ¿por qué se necesitan arrays de bits?

La pantalla SSD1306 es **monocromática**: cada píxel solo puede estar encendido (1) o apagado (0). Por eso, para mostrar una imagen o un ícono, no se envía un archivo `.png` o `.jpg`. En cambio, hay que convertir esa imagen en un **array de bytes** donde cada bit representa un píxel.

Por ejemplo, un ícono de 16×16 píxeles se convierte en un array de 32 bytes (16×16 = 256 bits = 32 bytes), y esos bytes se guardan directamente en el código del microcontrolador.

### ¿Cómo se usa ese array en el código?

```cpp
// Array de ejemplo para un ícono de corazón 16x16
const uint8_t corazon [] PROGMEM = {
  0x00, 0x00, 0x38, 0x1C, 0x7C, 0x3E, 0xFE, 0x7F,
  0xFE, 0x7F, 0xFE, 0x7F, 0xFC, 0x3F, 0xF8, 0x1F,
  0xF0, 0x0F, 0xE0, 0x07, 0xC0, 0x03, 0x80, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Dibujarlo en la pantalla:
display.drawBitmap(x, y, corazon, 16, 16, WHITE);
```

La palabra clave `PROGMEM` indica que el array se guarda en la **memoria de programa (Flash)** del microcontrolador en lugar de en la RAM, lo que es esencial cuando se tienen varios íconos.

## 6. Herramienta: convertir imágenes a arrays de bits con image2cpp

Para no tener que hacer la conversión de imagen a bytes manualmente, existe una herramienta web gratuita y de código abierto:

### 🔗 [https://javl.github.io/image2cpp/](https://javl.github.io/image2cpp/)

**image2cpp** es una utilidad que convierte imágenes (PNG, JPG, BMP, etc.) en arrays de bytes listos para pegar en un proyecto de Arduino, ESP32 o cualquier microcontrolador. Todo el procesamiento ocurre **localmente en el navegador**; las imágenes nunca se suben a ningún servidor.

### ¿Cómo se usa?

1. **Subir la imagen** (o pegar un array existente para convertirlo de regreso a imagen).
2. **Configurar el canvas**: definir el tamaño de salida (por ejemplo, 128×32 para que ocupe toda la pantalla, o 16×16 para un ícono).
3. **Ajustar los parámetros de imagen**:
   - *Background color*: elegir si el fondo debe ser blanco o negro.
   - *Invert colors*: invertir la imagen si los colores quedan al revés en la pantalla.
   - *Scaling*: escalar la imagen al tamaño del canvas manteniendo proporciones, estirando, etc.
   - *Brightness threshold* (0–255): define qué tan brillante debe ser un píxel para considerarse "blanco" (encendido). Si el brillo está por encima del umbral → píxel ON; si está por debajo → píxel OFF.
   - *Dithering*: algoritmos (Binary, Bayer, Floyd-Steinberg, Atkinson) para simular tonos de gris en una pantalla monocromática usando patrones de píxeles encendidos/apagados.
   - *Rotate / Flip*: rotar o voltear la imagen si es necesario.
4. **Elegir el formato de salida**:
   - *Arduino code*: genera el array listo con la declaración `const uint8_t nombre[] PROGMEM = { ... };`
   - *Plain bytes*: solo los bytes, sin código adicional.
   - *Draw mode*: `Horizontal - 1 bit per pixel` es el modo correcto para la mayoría de pantallas OLED con SSD1306.
5. **Copiar el array** generado y pegarlo directamente en el sketch de Arduino/ESP32.

### ¿Por qué es tan útil en este proyecto?

En el monitor cardíaco se puede usar image2cpp para:

- Convertir un **ícono de corazón** para mostrarlo en la OLED junto al BPM.
- Crear una **pantalla de inicio** con el nombre del proyecto o un logo.
- Preparar **íconos de estado** (advertencia de arritmia, pulso rápido, pulso lento) que se muestren dinámicamente según la lectura del sensor.

> Sin una herramienta como image2cpp, habría que calcular manualmente qué bytes corresponden a cada fila de píxeles de la imagen, lo cual es completamente impráctico. Con image2cpp, el proceso toma menos de un minuto.

## 7. Librerías recomendadas (Arduino / ESP32)

```cpp
#include <Wire.h>               // Comunicación I2C
#include <Adafruit_GFX.h>       // Funciones gráficas base
#include <Adafruit_SSD1306.h>   // Driver específico para SSD1306
```

Inicialización básica para la pantalla 128×32:

```cpp
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1        // Sin pin de reset (comparte con microcontrolador)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();
}
```

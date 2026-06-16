# Taller de Sensores Biomédicos Aplicados y Desarrollo de Wearables

Repositorio oficial del taller práctico organizado por la **Rama Estudiantil IEEE UTP** y el capítulo técnico **IEEE EMBS**, donde los participantes construyen, paso a paso, un prototipo funcional tipo **smartwatch** capaz de medir frecuencia cardíaca y temperatura, y mostrar los resultados en una pantalla OLED.

## Sobre el taller

El uso de sensores biomédicos es clave en el desarrollo de tecnologías de monitoreo de salud, especialmente en dispositivos *wearables*. Este taller busca cubrir la brecha entre la teoría y la práctica, ofreciendo una experiencia extendida de aprendizaje en la que cada participante no solo conoce el funcionamiento de los sensores, sino que **desarrolla un proyecto completo a lo largo de varios días**, adquiriendo habilidades reales en electrónica, programación y adquisición de datos biomédicos.

### Objetivo general

Desarrollar competencias prácticas en sensores biomédicos mediante la construcción progresiva de un prototipo tipo smartwatch.

### Objetivos específicos

- Capacitar en el uso y funcionamiento de sensores biomédicos.
- Fomentar el aprendizaje progresivo mediante sesiones prácticas estructuradas.
- Aplicar programación y electrónica en sistemas embebidos.
- Desarrollar habilidades de integración de sistemas.
- Incentivar la innovación en dispositivos de monitoreo de salud.

## Estructura del taller

El taller se desarrolla en 5 sesiones, avanzando de los fundamentos hasta un prototipo funcional integrado:

**Día 1 — Fundamentos de Arduino**
Introducción al entorno Arduino, uso básico de microcontroladores, entradas/salidas digitales y los primeros programas de lectura y control simple.

**Día 2 — Sensores biomédicos y adquisición de señales**
Introducción a los sensores biomédicos, fundamentos de adquisición de señales, conexión del sensor de pulso cardíaco y del sensor de temperatura, y primeras pruebas de lectura de datos.

**Día 3 — Programación e integración del sistema**
Programación del microcontrolador, visualización de datos en tiempo real, integración de múltiples sensores y arranque del mini proyecto (smartwatch).

**Día 4 — Desarrollo del prototipo**
Construcción del sistema completo, integración de hardware y software, ajustes, mejoras y pruebas funcionales.

**Día 5 — Cierre y presentación**
Optimización del sistema, validación de resultados, presentación de proyectos y retroalimentación final.

## Componentes del proyecto

El prototipo integra tres elementos principales de hardware, cada uno documentado en detalle dentro de la carpeta [`docs/`](docs):

| Componente | Función en el proyecto | Documentación |
|---|---|---|
| **Sensor de pulso cardíaco HW-827** | Mide la frecuencia cardíaca mediante fotopletismografía (PPG) y permite clasificar el ritmo (normal, rápido, lento, irregular) | [`docs/sensor-hw827.md`](docs/sensor-hw827.md) |
| **Sensor de temperatura LM35** | Mide la temperatura corporal/ambiental con salida analógica lineal (10 mV/°C) | [`docs/lm35-temperatura.md`](docs/lm35-temperatura.md) |
| **Pantalla OLED 128×32 (SSD1306)** | Muestra en tiempo real el BPM, la curva de pulso y el estado del ritmo cardíaco detectado | [`docs/oled-display.md`](docs/oled-display.md) |

Todo el sistema se controla con una **placa de desarrollo Arduino**, que lee ambos sensores de forma analógica y envía la información procesada a la pantalla OLED por I2C.

## Diagrama general del sistema

```
   HW-827 (pulso)  ---\
                        \
                         >---  Arduino  --->  Pantalla OLED 128x32
                        /        (I2C)
   LM35 (temperatura) -/
```

1. El **HW-827** y el **LM35** entregan señales analógicas que el Arduino lee mediante sus entradas ADC.
2. El Arduino procesa esas señales: calcula el BPM a partir de los picos de pulso y convierte el voltaje del LM35 a grados Celsius.
3. Los resultados se envían por **I2C** a la pantalla **OLED**, que muestra el valor de BPM, la temperatura y el estado del ritmo cardíaco.

## Estructura del repositorio

```
.
├── README.md                  ← este archivo (introducción al proyecto)
├── docs/
│   ├── sensor-hw827.md        ← sensor de pulso cardíaco
│   ├── lm35-temperatura.md    ← sensor de temperatura
│   └── oled-display.md        ← pantalla OLED + conversión de imágenes con image2cpp
├── images/
│   └── ciclo_cardiaco.png     ← referencia gráfica del ciclo cardíaco (ECG)
└── src/
    └── ...                    ← código fuente del proyecto (Arduino/ESP32)
```

## Responsables de la actividad

- **IEEE SB UTP** — Logística, gestión de espacios, convocatoria y promoción del evento.
- **IEEE EMBS** — Organización general, desarrollo del contenido técnico del taller y facilitación de las sesiones prácticas.

## Cómo empezar

1. Revisa la documentación de cada componente en la carpeta [`docs/`](docs) para entender cómo funciona cada sensor.
2. Sigue la estructura de 5 días para avanzar de los fundamentos de Arduino hasta el prototipo integrado.
3. Usa la herramienta [image2cpp](https://javl.github.io/image2cpp/) (explicada en `docs/oled-display.md`) si quieres personalizar íconos o gráficos para la pantalla OLED.
4. Sube tu propio código y avances dentro de la carpeta `src/` siguiendo el flujo de trabajo del taller.

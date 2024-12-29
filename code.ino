#include <LiquidCrystal.h> 
#include <SimpleDHT.h>

// Define los pines para los sensores y otros dispositivos
#define SENSOR 38             // Pin del sensor de temperatura y humedad
#define PIN1 42               // Pines de control para la pantalla LCD
#define PIN2 44
#define PIN3 46
#define PIN4 48
#define PIN5 50
#define PIN6 52

LiquidCrystal lcd(PIN1, PIN2, PIN3, PIN4, PIN5, PIN6); // Inicializa la pantalla LCD

SimpleDHT11 sensor;           // Inicializa el sensor DHT11

int value, normalizedValue;

// Define las notas en frecuencias para la melodía
#define NOTE_E7 2637
#define NOTE_C7 2093
#define NOTE_G7 3136
#define NOTE_G6 1568

// Define los pines de entrada y salida
#define fotoresistor A0         // Pin del fotoresistor
#define temperaturaHumedad A1    // Pin de temperatura y humedad
#define timbre 10               // Pin del timbre
#define luces 5                 // Pin para controlar las luces
#define speaker 4               // Pin para el altavoz
#define sensor_movimiento 3     // Pin del sensor de movimiento

// Notas de alarma
#define NOTE_A5 880
#define NOTE_A4 440

// Define la melodía (tema de Mario)
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0, 0,
  NOTE_G6, 0, 0, 0
};

// Duración de cada nota en la melodía
int noteDurations[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12
};

// Configuración inicial
void setup() {
  pinMode(luces, OUTPUT);               // Configura el pin de luces como salida
  pinMode(timbre, INPUT);               // Configura el pin del timbre como entrada
  pinMode(sensor_movimiento, INPUT);    // Configura el pin del sensor de movimiento como entrada
  pinMode(speaker, OUTPUT);             // Configura el pin del altavoz como salida
  pinMode(SENSOR, INPUT);               // Configura el pin del sensor de temperatura como entrada

  // Configura los pines de la pantalla LCD
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT);
  pinMode(PIN5, OUTPUT);
  pinMode(PIN6, OUTPUT);

  // Inicializa la pantalla LCD
  lcd.begin(16, 2);
  lcd.print("SENSOR INICIADO");
  delay(2000); // Muestra el mensaje inicial durante 2 segundos
  lcd.clear();
}

void loop() {
  int valor_fotoresistor = analogRead(fotoresistor);       // Lee el valor del fotoresistor
  int estado_timbre = digitalRead(timbre);                 // Lee el estado del timbre
  int estado_movimiento = digitalRead(sensor_movimiento);  // Lee el estado del sensor de movimiento

  // Control de las luces según el valor del fotoresistor
  if (valor_fotoresistor > 512) {
    digitalWrite(luces, LOW);         // Apaga las luces si el valor es alto
  } else {
    digitalWrite(luces, HIGH);        // Enciende las luces si el valor es bajo
  }

  // Reproduce la melodía si se activa el timbre
  if (!estado_timbre) {
    lcd.clear();
    lcd.setCursor(0, 0);                // Coloca el cursor en la primera línea
    lcd.print("Leyendo sensor...");     // Muestra el mensaje de lectura del sensor

    for (int thisNote = 0; thisNote < 16; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];   // Calcula la duración de cada nota
      tone(speaker, melody[thisNote], noteDuration);       // Emite la nota en el altavoz
      int pauseBetweenNotes = noteDuration * 1.3;          // Pausa entre notas
      delay(pauseBetweenNotes);
      noTone(speaker);                                     // Detiene el tono actual
    }
    delay(500);  // Pausa antes de repetir la melodía

    byte temperatura = 0;
    byte humedad = 0;
    byte datos[40] = {};
    sensor.setPin(38);             // Configura el pin del sensor de temperatura y humedad
    if (sensor.read(SENSOR, &temperatura, &humedad, datos)) {
      lcd.clear();
      lcd.print("Error de lectura!");    // Muestra un mensaje de error si falla la lectura
      delay(1000);                       // Espera un segundo
      lcd.clear();
    }

    // Muestra temperatura y humedad en la pantalla LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print((int)temperatura);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Humedad: ");
    lcd.print((int)humedad);
    lcd.print(" %");
  } else {
    digitalWrite(speaker, LOW);    // Apaga el altavoz si el timbre no está activado
  }

  // Activa el altavoz si hay movimiento
  if (estado_movimiento) {
    // Parpadea las luces y activa el sonido de alarma
    for (int i = 0; i < 5; i++) {
      digitalWrite(luces, HIGH);     // Enciende las luces
      tone(speaker, NOTE_A5, 200);   // Tono alto de alarma
      delay(200);

      digitalWrite(luces, LOW);      // Apaga las luces
      tone(speaker, NOTE_A4, 200);   // Tono bajo de alarma
      delay(200);
    }
  } else {
    digitalWrite(luces, LOW);        // Apaga las luces si no hay movimiento
    noTone(speaker);                 // Detiene el sonido
  }
}

/* ------------------------------------------------------------------------------------------------------ /

  Código Unificado

  Autor: Ariel Slonimsqui
  Materia: Taller de Programación
  Trabajo: Proyecto Final 4to Año Mecatrónica.

  Descripción:
  Utilizando dos sensores ultrasónicos, se calcula y mide la distancia en centímetros, y en base a estas mediciones se producen algunas animaciones en la pantalla.

  /* ----------------------------------------------------------------------------------------------------- */

#include <TFT_eSPI.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>


#define TFT_LIGHTGRAY 0x8C30
#define TFT_GRAY 0x4A69
#define TFT_DARKRED 0x7041
#define TFT_GREEN2 0x1ECA
#define TFT_YELLOW2 0xEF63
#define TFT_RED2 0xE042
#define TFT_BLACK 0x0000
#define TFT_ORANGE 0xFC60

#define TEXTO_INICIALIZACION_1 0
#define TEXTO_INICIALIZACION_2 1
#define TEXTO_INICIALIZACION_3 2
#define TEXTO_INICIALIZACION_4 3
#define TEXTO_INICIALIZACION_5 4
#define TEXTO_INICIALIZACION_6 5
#define PRINTEO_OJO 6
#define PRINTEO_AUTO 7
#define COMPARACIONES_DATOS_UMBRAL 8

#define UMBRAL_DE_DETECCION_PRIMERO 60 //Límite de distancia en centímetros
#define UMBRAL_DE_DETECCION_SEGUNDO 40
#define UMBRAL_DE_DETECCION_TERCERO 20

#define TIEMPO_DE_ANIMACION 200 //Tiempo de duración de los dos estados del titileo, tanto para el indicador IZQ como el DER
#define BAUD_RATE 115200

#define TRIG_PIN_DER 25 // Pin 25 conectado al pin TRIG del sensor ultrasónico diestro.
#define ECHO_PIN_DER 26 // Pin 26 conectado al pin ECHO del sensor ultrasónico diestro.

#define TRIG_PIN_IZQ 23 // Pin 23 conectado al pin TRIG del sensor ultrasónico zurdo.
#define ECHO_PIN_IZQ 22 // Pin 22 conectado al pin ECHO del sensor ultrasónico zurdo.

#define TRIGEO_ULTRASONICO_ON 0
#define MEDICION_Y_ENVIO 1

#define INTERVALO_SENSOR 20000 // Frecuencia en unidad de microsegundos que va a trigear el sensor. En este caso, una vez cada 1o micros.

#define BUZZER_PIN 13     // Pin del buzzer
#define BUZZER_CHANNEL 0 // Canal PWM del buzzer

TFT_eSPI tft = TFT_eSPI ();

Adafruit_NeoPixel tiraDer = Adafruit_NeoPixel (8, 21, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel tiraIzq = Adafruit_NeoPixel (8, 19, NEO_GRB + NEO_KHZ800);

int duracionUsDer;
int distanciaCmDer;

int distanciaCmIzq;
int duracionUsIzq;

unsigned long microsActualesDer;
unsigned long microsPreviosDer;
unsigned long milisActualesDer;
unsigned long milisPreviosDer;

unsigned long microsActualesIzq;
unsigned long microsPreviosIzq;
unsigned long milisActualesIzq;
unsigned long milisPreviosIzq;

unsigned long milisActualesGeneral;
unsigned long milisPreviosGeneral;

unsigned long milisActualesBuzzerPrimerUmbral;
unsigned long milisPreviosBuzzerPrimerUmbral;

unsigned long milisActualesBuzzerSegundoUmbral;
unsigned long milisPreviosBuzzerSegundoUmbral;

unsigned long milisActualesBuzzerTercerUmbral;
unsigned long milisPreviosBuzzerTercerUmbral;

int estadoMaquinaGeneral;
int estadoMaquinaDeteccionDer;
int estadoMaquinaDeteccionIzq;

int estadoMaquinaBuzzerPrimerUmbral;
int estadoMaquinaBuzzerSegundoUmbral;
int estadoMaquinaBuzzerTercerUmbral;


int flagAmarilloIzq;
int flagNaranjaIzq;
int flagRojoIzq;
int flagNegroIzq;

int flagAmarilloDer;
int flagNaranjaDer;
int flagRojoDer;
int flagNegroDer;

int prioridad;

void setup() {

  Serial.begin (BAUD_RATE);

  pinMode(TRIG_PIN_DER, OUTPUT);
  pinMode(ECHO_PIN_DER, INPUT);

  pinMode(TRIG_PIN_IZQ, OUTPUT);
  pinMode(ECHO_PIN_IZQ, INPUT);

  tft.init(); //Funcion de inicializacion de la pantalla
  tft.setRotation(2); //Seteo de la orientación vertical de la pantalla
  tft.fillScreen(TFT_BLACK); //Seteo color del fondo

  tiraDer.begin();
  tiraIzq.begin();



  tiraDer.setBrightness(100);
  tiraIzq.setBrightness(100);

  ledcSetup(BUZZER_CHANNEL, 2000, 8); // Configurar el canal PWM
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL); // Asociar el pin al canal PWM
  ledcWrite(BUZZER_CHANNEL, 0);

}

void loop() {

  // Serial.print("DERECHA: ");
  // Serial.println(distanciaCmDer);

  // Serial.print("IZQUIERDA: ");
  //Serial.println(distanciaCmIzq);

  maquinaDeEstadosGeneral ();

  maquinaDeEstadosDeteccionDerecha();

  maquinaDeEstadosDeteccionIzquierda();


}


void maquinaDeEstadosDeteccionDerecha () {


  switch (estadoMaquinaDeteccionDer) { //Maquina de estados que manipula el ultrasónico de la derecha

    case TRIGEO_ULTRASONICO_ON:

      digitalWrite(TRIG_PIN_DER, HIGH); //Envio de un solo pulso

      estadoMaquinaDeteccionDer = 1;

      break;

    case MEDICION_Y_ENVIO:

      microsActualesDer = micros(); // Guarda el tiempo en microsegundos desde que se inicio el programa

      if ((microsActualesDer - microsPreviosDer) >= INTERVALO_SENSOR) { // La resta representa el reinicio del contador

        digitalWrite(TRIG_PIN_DER, LOW);

        microsPreviosDer = microsActualesDer;

        duracionUsDer = pulseIn(ECHO_PIN_DER, HIGH);// Función para medir la duración del pulso desde el pin ECHO

        distanciaCmDer = 0.017 * duracionUsDer; //Cálculo de la distancia, en base a la duración

        estadoMaquinaDeteccionDer = 0;



      }

      break;

  }

}

void maquinaDeEstadosDeteccionIzquierda() {


  switch (estadoMaquinaDeteccionIzq) { //Máquina de estados que manipula el ultrasónico de la izquierda, y funciona de la misma manera que la máquina del sensor de la derecha

    case TRIGEO_ULTRASONICO_ON:

      digitalWrite(TRIG_PIN_IZQ, HIGH);

      estadoMaquinaDeteccionIzq = 1;

      break;

    case MEDICION_Y_ENVIO:

      microsActualesIzq = micros();

      if ((microsActualesIzq - microsPreviosIzq) >= INTERVALO_SENSOR) {

        digitalWrite(TRIG_PIN_IZQ, LOW);

        microsPreviosIzq = microsActualesIzq;

        duracionUsIzq = pulseIn(ECHO_PIN_IZQ, HIGH);

        distanciaCmIzq = 0.017 * duracionUsIzq;

        estadoMaquinaDeteccionIzq = 0;

      }

      break;

  }

}

void maquinaDeEstadosGeneral () {

  milisActualesGeneral = millis(); // Guarda el tiempo en milisegundos desde que se inicio el programa


  switch (estadoMaquinaGeneral) { //Máquina de estados general, que maneja la iniciación de la pantalla

    case TEXTO_INICIALIZACION_1:

      if ((milisActualesGeneral - milisPreviosGeneral) >= 1000) {

        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(1);
        tft.drawString("Bienvenido Usuario", 10, 30, 4);

        milisPreviosGeneral = milisActualesGeneral;
        estadoMaquinaGeneral = 1;

      }

      break;

    case TEXTO_INICIALIZACION_2:

      if ((milisActualesGeneral - milisPreviosGeneral) >= 2000) {

        tft.setTextSize(2);
        tft.drawString("Iniciando Sistema", 20, 80 , 2);

        milisPreviosGeneral = milisActualesGeneral;
        estadoMaquinaGeneral = 2;

      }

      break;

    case TEXTO_INICIALIZACION_3:

      if ((milisActualesGeneral - milisPreviosGeneral) >= 800) {

        tft.drawString("de Deteccion", 40, 118 , 2);

        milisPreviosGeneral = milisActualesGeneral;
        estadoMaquinaGeneral = 3;

      }

      break;

    case TEXTO_INICIALIZACION_4:

      if ((milisActualesGeneral - milisPreviosGeneral) >= 800) {

        tft.drawString("de", 115, 156 , 2);

        milisPreviosGeneral = milisActualesGeneral;
        estadoMaquinaGeneral = 4;

      }

      break;

    case TEXTO_INICIALIZACION_5:

      if ((milisActualesGeneral - milisPreviosGeneral) >= 800) {

        tft.setTextSize(3);
        tft.drawString("Puntos", 8, 200 , 1);

        milisPreviosGeneral = milisActualesGeneral;
        estadoMaquinaGeneral = 5;

      }

      break;

    case TEXTO_INICIALIZACION_6:

      if ((milisActualesGeneral - milisPreviosGeneral) >= 800) {

        tft.setTextSize(3);
        tft.drawString("Ciegos", 125, 200 , 1);

        milisPreviosGeneral = milisActualesGeneral;
        estadoMaquinaGeneral = 6;

      }

      break;

    case PRINTEO_OJO:

      if ((milisActualesGeneral - milisPreviosGeneral) >= 800) {

        printeoOjo();
        milisPreviosGeneral = milisActualesGeneral;
        estadoMaquinaGeneral = 7;

      }

      break;

    case PRINTEO_AUTO:

      if ((milisActualesGeneral - milisPreviosGeneral) >= 1000) {

        printeoAuto();
        animacionLineaIzq();
        animacionLineaDer();

        estadoMaquinaGeneral = 8;

      }

      break;

    case COMPARACIONES_DATOS_UMBRAL: //Condiciones para que printee en la pantalla las animaciones de detección

      establecimientoPrioridades();

      if (distanciaCmIzq > UMBRAL_DE_DETECCION_PRIMERO && flagNegroIzq == 0) {

        estadoNegroIzq();
        animacionLineaIzq();

        pintarPixelNegroIzq();

        flagNegroIzq = 1;

      }

      if (distanciaCmIzq < UMBRAL_DE_DETECCION_PRIMERO) {

        flagNegroIzq = 0;

      }


      if (distanciaCmIzq < UMBRAL_DE_DETECCION_PRIMERO && distanciaCmIzq > UMBRAL_DE_DETECCION_SEGUNDO && flagAmarilloIzq == 0) {//41 y 60

        estadoAmarilloIzq();
        animacionLineaIzq();

        pintarPixelAmarilloIzq();
        flagAmarilloIzq = 1;
      }

      if (distanciaCmIzq > UMBRAL_DE_DETECCION_PRIMERO || distanciaCmIzq < UMBRAL_DE_DETECCION_SEGUNDO) {

        flagAmarilloIzq = 0;

      }



      if (distanciaCmIzq < UMBRAL_DE_DETECCION_SEGUNDO && distanciaCmIzq > UMBRAL_DE_DETECCION_TERCERO && flagNaranjaIzq == 0) { //21 y 40

        estadoNaranjaIzq();
        animacionLineaIzq();

        pintarPixelNaranjaIzq();


        flagNaranjaIzq = 1;

      }

      if (distanciaCmIzq > UMBRAL_DE_DETECCION_SEGUNDO || distanciaCmIzq < UMBRAL_DE_DETECCION_TERCERO) {

        flagNaranjaIzq = 0;

      }


      if (distanciaCmIzq < UMBRAL_DE_DETECCION_TERCERO && flagRojoIzq == 0) { //21 y 40

        estadoRojoIzq();
        animacionLineaIzq();

        pintarPixelRojoIzq();

        flagRojoIzq = 1;

      }

      if (distanciaCmIzq > UMBRAL_DE_DETECCION_TERCERO) {

        flagRojoIzq = 0;

      }


      if (distanciaCmDer > UMBRAL_DE_DETECCION_PRIMERO && flagNegroDer == 0) {

        estadoNegroDer();
        animacionLineaDer();

        pintarPixelNegroDer();

        flagNegroDer = 1;

      }

      if (distanciaCmDer < UMBRAL_DE_DETECCION_PRIMERO) {

        flagNegroDer = 0;

      }



      if (distanciaCmDer < UMBRAL_DE_DETECCION_PRIMERO && distanciaCmDer > UMBRAL_DE_DETECCION_SEGUNDO && flagAmarilloDer == 0) {//41 y 60

        estadoAmarilloDer();
        animacionLineaDer();

        pintarPixelAmarilloDer();

        flagAmarilloDer = 1;
      }

      if (distanciaCmDer > UMBRAL_DE_DETECCION_PRIMERO || distanciaCmDer < UMBRAL_DE_DETECCION_SEGUNDO) {

        flagAmarilloDer = 0;

      }



      if (distanciaCmDer < UMBRAL_DE_DETECCION_SEGUNDO && distanciaCmDer > UMBRAL_DE_DETECCION_TERCERO && flagNaranjaDer == 0) { //21 y 40

        estadoNaranjaDer();
        animacionLineaDer();

        pintarPixelNaranjaDer();

        flagNaranjaDer = 1;

      }

      if (distanciaCmDer > UMBRAL_DE_DETECCION_SEGUNDO || distanciaCmDer < UMBRAL_DE_DETECCION_TERCERO) {

        flagNaranjaDer = 0;

      }


      if (distanciaCmDer < UMBRAL_DE_DETECCION_TERCERO && flagRojoDer == 0) { //21 y 40

        estadoRojoDer();
        animacionLineaDer();

        pintarPixelRojoDer();

        flagRojoDer = 1;

      }

      if (distanciaCmDer > UMBRAL_DE_DETECCION_TERCERO) {

        flagRojoDer = 0;

      }

      if (prioridad == 0) {
        if (distanciaCmIzq < UMBRAL_DE_DETECCION_PRIMERO && distanciaCmIzq > UMBRAL_DE_DETECCION_SEGUNDO) {
          buzzerPrimerUmbral();
        }

        if (distanciaCmIzq < UMBRAL_DE_DETECCION_SEGUNDO && distanciaCmIzq > UMBRAL_DE_DETECCION_TERCERO) {
          buzzerSegundoUmbral();
        }
        if (distanciaCmIzq < UMBRAL_DE_DETECCION_TERCERO) {
          ledcWrite(BUZZER_CHANNEL, 128);
        }

        if (distanciaCmDer < UMBRAL_DE_DETECCION_PRIMERO && distanciaCmDer > UMBRAL_DE_DETECCION_SEGUNDO) {
          buzzerPrimerUmbral();
        }

        if (distanciaCmDer < UMBRAL_DE_DETECCION_SEGUNDO && distanciaCmDer > UMBRAL_DE_DETECCION_TERCERO) {
          buzzerSegundoUmbral();
        }
        if (distanciaCmDer < UMBRAL_DE_DETECCION_TERCERO) {
          ledcWrite(BUZZER_CHANNEL, 128);
        }
      }

      if (prioridad == 1) {
        ledcWrite(BUZZER_CHANNEL, 128);
      }

      if (prioridad == 2) {
        buzzerSegundoUmbral();
      }

      if (prioridad == 3) {
        buzzerPrimerUmbral();
      }

      if (prioridad == 4) {
        ledcWrite(BUZZER_CHANNEL, 0);
      }


      break;

  }

}




void printeoOjo() {

  tft.fillEllipse(120, 270, 100, 35, TFT_WHITE); // Dibujo ojo
  tft.drawEllipse(120, 270, 100, 35, TFT_BLACK);
  tft.fillCircle(120, 270, 35, TFT_WHITE);
  tft.drawCircle(120, 270, 35, TFT_BLACK);
  tft.fillCircle(120, 270, 15, TFT_BLACK);
  tft.fillCircle(128, 275, 4, TFT_WHITE);

}

void printeoAuto() {

  tft.fillScreen(TFT_BLACK);

  tft.fillRoundRect(60, 20, 120, 210, 30, TFT_SILVER); //Silueta del auto
  tft.drawRoundRect(60, 20, 120, 210, 30, TFT_BLACK);

  tft.drawLine(80, 40, 160, 40, TFT_BLACK); //Línea horizontal superior del CAPÓ
  tft.drawLine(70, 70, 170, 70, TFT_BLACK); //Línea horizontal inferior
  tft.drawLine(80, 40, 70, 70, TFT_BLACK); //Diagonal izquierda
  tft.drawLine(160, 40, 170, 70, TFT_BLACK); //Diagonal derecha

  tft.fillTriangle(75, 80, 165, 80, 80, 105, TFT_SKYBLUE);
  tft.fillTriangle(165, 80, 80, 105, 160, 105, TFT_SKYBLUE);

  tft.drawLine(75, 80, 165, 80, TFT_BLACK); //Línea horizontal superior del VIDRIO DELANTERO
  tft.drawLine(80, 105, 160, 105, TFT_BLACK); //Línea horizontal inferior
  tft.drawLine(75, 80, 80, 105, TFT_BLACK); //Diagonal izquierda
  tft.drawLine(165, 80, 160, 105, TFT_BLACK); //Diagonal derecha

  tft.fillTriangle(80, 210, 85, 190, 160, 210, TFT_SKYBLUE);
  tft.fillTriangle(85, 190, 160, 210, 155, 190, TFT_SKYBLUE);

  tft.drawLine(80, 210, 160, 210, TFT_BLACK); //Línea horizontal inferior del VIDRIO TRASERO
  tft.drawLine(85, 190, 155, 190, TFT_BLACK); //Línea horizontal superior
  tft.drawLine(80, 210, 85, 190, TFT_BLACK); //Diagonal izquierda
  tft.drawLine(160, 210, 155, 190, TFT_BLACK); //Diagonal derecha

  tft.fillTriangle(172, 105, 160, 175, 172, 190, TFT_SKYBLUE);
  tft.fillTriangle(172, 105, 160, 175, 160, 120, TFT_SKYBLUE);

  tft.drawLine(172, 105, 172, 190, TFT_BLACK); //Línea vertical externa VIDRIO DERECHO
  tft.drawLine(160, 120, 160, 175, TFT_BLACK); //Linea vertical interna
  tft.drawLine(172, 105, 160, 120, TFT_BLACK); //Diagonal superior
  tft.drawLine(172, 190, 160, 175, TFT_BLACK); //Diagonal inferior

  tft.fillTriangle(68, 105, 80, 120, 68, 190, TFT_SKYBLUE);
  tft.fillTriangle(80, 120, 68, 190, 80, 175, TFT_SKYBLUE);

  tft.drawLine(68, 105, 68, 190, TFT_BLACK); //Linea vertical externa VIDRIO IZQUIERDO
  tft.drawLine(80, 120, 80, 175, TFT_BLACK); //Linea vertical interna
  tft.drawLine(68, 105, 80, 120, TFT_BLACK); //Diagonal superior
  tft.drawLine(68, 190, 80, 175, TFT_BLACK); //Diagonal inferior

  tft.fillTriangle(87, 120, 92, 188, 97, 190, TFT_BLACK); //PORTAEQUIPAJE IZQUIERDA
  tft.fillTriangle(153, 120, 148, 188, 143, 190, TFT_BLACK); //PORTAEQUIPAJE DERECHA

  tft.fillEllipse(88, 223, 15, 5, TFT_RED2); //LUCES TRASERAS IZQUIERDA
  tft.fillTriangle(85, 229, 103, 229, 103, 223, TFT_RED2);
  tft.drawPixel(81, 228, TFT_RED2);

  tft.fillEllipse(151, 223, 15, 5, TFT_RED2); //LUCES TRASERAS DERECHA
  tft.fillTriangle(154, 229, 136, 229, 136, 223, TFT_RED2);
  tft.drawPixel(158, 228, TFT_RED2);

  tft.fillEllipse(55, 85, 14, 5, TFT_SILVER); //ESPEJO RETROVISOR IZQUIERDA
  tft.fillTriangle(40, 83, 40, 85, 70, 85, TFT_SILVER);

  tft.fillEllipse(185, 85, 14, 5, TFT_SILVER); //ESPEJO RETROVISOR DERECHA
  tft.fillTriangle(200, 80, 197, 85, 170, 85, TFT_SILVER);

  tft.fillRect(57, 55, 3, 23, TFT_LIGHTGRAY); //RUEDA delantera izquierda
  tft.fillRect(57, 180, 3, 25, TFT_LIGHTGRAY); //Rueda trasera izquierda
  tft.fillRect(180, 55, 3, 23, TFT_LIGHTGRAY); //Rueda delantera derecha
  tft.fillRect(180, 180, 3, 25, TFT_LIGHTGRAY); //Rueda trasera derecha


}


void pintarPixelAmarilloDer() {

  tiraDer.setPixelColor(0, 239, 239, 25);
  tiraDer.setPixelColor(1, 239, 239, 25);
  tiraDer.setPixelColor(2, 239, 239, 25);
  tiraDer.setPixelColor(3, 239, 239, 25);
  tiraDer.setPixelColor(4, 239, 239, 25);
  tiraDer.setPixelColor(5, 239, 239, 25);
  tiraDer.setPixelColor(6, 239, 239, 25);
  tiraDer.setPixelColor(7, 239, 239, 25);

  tiraDer.show();

}

void pintarPixelNaranjaDer() {

  tiraDer.setPixelColor(0, 255, 140, 0);
  tiraDer.setPixelColor(1, 255, 140, 0);
  tiraDer.setPixelColor(2, 255, 140, 0);
  tiraDer.setPixelColor(3, 255, 140, 0);
  tiraDer.setPixelColor(4, 255, 140, 0);
  tiraDer.setPixelColor(5, 255, 140, 0);
  tiraDer.setPixelColor(6, 255, 140, 0);
  tiraDer.setPixelColor(7, 255, 140, 0);

  tiraDer.show();

}

void pintarPixelRojoDer() {

  tiraDer.setPixelColor(0, 230, 8, 16);
  tiraDer.setPixelColor(1, 230, 8, 16);
  tiraDer.setPixelColor(2, 230, 8, 16);
  tiraDer.setPixelColor(3, 230, 8, 16);
  tiraDer.setPixelColor(4, 230, 8, 16);
  tiraDer.setPixelColor(5, 230, 8, 16);
  tiraDer.setPixelColor(6, 230, 8, 16);
  tiraDer.setPixelColor(7, 230, 8, 16);

  tiraDer.show();

}

void pintarPixelNegroDer() {

  tiraDer.setPixelColor(0, 0, 0, 0);
  tiraDer.setPixelColor(1, 0, 0, 0);
  tiraDer.setPixelColor(2, 0, 0, 0);
  tiraDer.setPixelColor(3, 0, 0, 0);
  tiraDer.setPixelColor(4, 0, 0, 0);
  tiraDer.setPixelColor(5, 0, 0, 0);
  tiraDer.setPixelColor(6, 0, 0, 0);
  tiraDer.setPixelColor(7, 0, 0, 0);

  tiraDer.show();

}

void pintarPixelAmarilloIzq() {

  tiraIzq.setPixelColor(0, 239, 239, 25);
  tiraIzq.setPixelColor(1, 239, 239, 25);
  tiraIzq.setPixelColor(2, 239, 239, 25);
  tiraIzq.setPixelColor(3, 239, 239, 25);
  tiraIzq.setPixelColor(4, 239, 239, 25);
  tiraIzq.setPixelColor(5, 239, 239, 25);
  tiraIzq.setPixelColor(6, 239, 239, 25);
  tiraIzq.setPixelColor(7, 239, 239, 25);

  tiraIzq.show();

}

void pintarPixelNaranjaIzq() {

  tiraIzq.setPixelColor(0, 255, 140, 0);
  tiraIzq.setPixelColor(1, 255, 140, 0);
  tiraIzq.setPixelColor(2, 255, 140, 0);
  tiraIzq.setPixelColor(3, 255, 140, 0);
  tiraIzq.setPixelColor(4, 255, 140, 0);
  tiraIzq.setPixelColor(5, 255, 140, 0);
  tiraIzq.setPixelColor(6, 255, 140, 0);
  tiraIzq.setPixelColor(7, 255, 140, 0);

  tiraIzq.show();

}

void pintarPixelRojoIzq() {

  tiraIzq.setPixelColor(0, 230, 8, 16);
  tiraIzq.setPixelColor(1, 230, 8, 16);
  tiraIzq.setPixelColor(2, 230, 8, 16);
  tiraIzq.setPixelColor(3, 230, 8, 16);
  tiraIzq.setPixelColor(4, 230, 8, 16);
  tiraIzq.setPixelColor(5, 230, 8, 16);
  tiraIzq.setPixelColor(6, 230, 8, 16);
  tiraIzq.setPixelColor(7, 230, 8, 16);

  tiraIzq.show();

}

void pintarPixelNegroIzq() {

  tiraIzq.setPixelColor(0, 0, 0, 0);
  tiraIzq.setPixelColor(1, 0, 0, 0);
  tiraIzq.setPixelColor(2, 0, 0, 0);
  tiraIzq.setPixelColor(3, 0, 0, 0);
  tiraIzq.setPixelColor(4, 0, 0, 0);
  tiraIzq.setPixelColor(5, 0, 0, 0);
  tiraIzq.setPixelColor(6, 0, 0, 0);
  tiraIzq.setPixelColor(7, 0, 0, 0);

  tiraIzq.show();

}


void animacionLineaIzq() {

  tft.drawLine(60, 207, 75, 230, TFT_WHITE); //linea superior cono izquierda
  tft.drawLine(60, 207, 0, 244, TFT_WHITE); //diagonal izquierda
  tft.drawLine(75, 230, 60, 320, TFT_WHITE); //diagonal derecha

  tft.drawLine(33, 224, 72, 250, TFT_WHITE); //divisor rojo naranja
  tft.drawLine(0, 244, 65, 290, TFT_WHITE); //divisor rojo naranja

}


void estadoAmarilloIzq() {

  tft.fillTriangle(0, 244, 0, 320, 65, 290, TFT_YELLOW2);
  tft.fillTriangle(60, 320, 0, 320, 65, 290, TFT_YELLOW2);

  tft.fillTriangle(60, 207, 75, 230, 33, 224, TFT_BLACK);//Rojo der
  tft.fillTriangle(72, 250, 75, 230, 33, 224, TFT_BLACK);

  tft.fillTriangle(0, 244, 72, 250, 33, 224, TFT_BLACK);
  tft.fillTriangle(0, 244, 72, 250, 65, 290, TFT_BLACK);


}

void estadoNaranjaIzq() {

  tft.fillTriangle(0, 244, 72, 250, 33, 224, TFT_ORANGE);
  tft.fillTriangle(0, 244, 72, 250, 65, 290, TFT_ORANGE);

  tft.fillTriangle(0, 244, 0, 320, 65, 290, TFT_ORANGE);
  tft.fillTriangle(60, 320, 0, 320, 65, 290, TFT_ORANGE);

  tft.fillTriangle(60, 207, 75, 230, 33, 224, TFT_BLACK);//Rojo der
  tft.fillTriangle(72, 250, 75, 230, 33, 224, TFT_BLACK);



}

void estadoRojoIzq() {

  tft.fillTriangle(60, 207, 75, 230, 33, 224, TFT_RED2);//Rojo der
  tft.fillTriangle(72, 250, 75, 230, 33, 224, TFT_RED2);

  tft.fillTriangle(0, 244, 72, 250, 33, 224, TFT_RED2);
  tft.fillTriangle(0, 244, 72, 250, 65, 290, TFT_RED2);

  tft.fillTriangle(0, 244, 0, 320, 65, 290, TFT_RED2);
  tft.fillTriangle(60, 320, 0, 320, 65, 290, TFT_RED2);

}

void estadoNegroIzq() {

  tft.fillTriangle(60, 207, 75, 230, 33, 224, TFT_BLACK);//Rojo der
  tft.fillTriangle(72, 250, 75, 230, 33, 224, TFT_BLACK);

  tft.fillTriangle(0, 244, 72, 250, 33, 224, TFT_BLACK);
  tft.fillTriangle(0, 244, 72, 250, 65, 290, TFT_BLACK);

  tft.fillTriangle(0, 244, 0, 320, 65, 290, TFT_BLACK);
  tft.fillTriangle(60, 320, 0, 320, 65, 290, TFT_BLACK);

}


void animacionLineaDer() {

  tft.drawLine(180, 207, 165, 230, TFT_WHITE); //linea superior cono izquierda
  tft.drawLine(180, 207, 240, 244, TFT_WHITE); //diagonal izquierda
  tft.drawLine(165, 230, 180, 320, TFT_WHITE); //diagonal derecha

  tft.drawLine(207, 224, 168, 250, TFT_WHITE); //divisor rojo naranja
  tft.drawLine(240, 244, 175, 290, TFT_WHITE); //divisor rojo naranja

}




void estadoAmarilloDer() {

  tft.fillTriangle(240, 244, 240, 320, 175, 290, TFT_YELLOW2);
  tft.fillTriangle(180, 320, 240, 320, 175, 290, TFT_YELLOW2);

  tft.fillTriangle(180, 207, 165, 230, 207, 224, TFT_BLACK);//Rojo der
  tft.fillTriangle(168, 250, 165, 230, 207, 224, TFT_BLACK);

  tft.fillTriangle(240, 244, 168, 250, 207, 224, TFT_BLACK);
  tft.fillTriangle(240, 244, 168, 250, 175, 290, TFT_BLACK);

}

void estadoNaranjaDer() {

  tft.fillTriangle(240, 244, 240, 320, 175, 290, TFT_ORANGE);
  tft.fillTriangle(180, 320, 240, 320, 175, 290, TFT_ORANGE);

  tft.fillTriangle(240, 244, 168, 250, 207, 224, TFT_ORANGE);
  tft.fillTriangle(240, 244, 168, 250, 175, 290, TFT_ORANGE);

  tft.fillTriangle(180, 207, 165, 230, 207, 224, TFT_BLACK);//Rojo der
  tft.fillTriangle(168, 250, 165, 230, 207, 224, TFT_BLACK);

}

void estadoRojoDer() {

  tft.fillTriangle(240, 244, 240, 320, 175, 290, TFT_RED2);
  tft.fillTriangle(180, 320, 240, 320, 175, 290, TFT_RED2);

  tft.fillTriangle(180, 207, 165, 230, 207, 224, TFT_RED2);//Rojo der
  tft.fillTriangle(168, 250, 165, 230, 207, 224, TFT_RED2);

  tft.fillTriangle(240, 244, 168, 250, 207, 224, TFT_RED2);
  tft.fillTriangle(240, 244, 168, 250, 175, 290, TFT_RED2);


}

void estadoNegroDer() {

  tft.fillTriangle(240, 244, 240, 320, 175, 290, TFT_BLACK);
  tft.fillTriangle(180, 320, 240, 320, 175, 290, TFT_BLACK);

  tft.fillTriangle(180, 207, 165, 230, 207, 224, TFT_BLACK);//Rojo der
  tft.fillTriangle(168, 250, 165, 230, 207, 224, TFT_BLACK);

  tft.fillTriangle(240, 244, 168, 250, 207, 224, TFT_BLACK);
  tft.fillTriangle(240, 244, 168, 250, 175, 290, TFT_BLACK);

}

void buzzerPrimerUmbral() {

  milisActualesBuzzerPrimerUmbral = millis();

  switch (estadoMaquinaBuzzerPrimerUmbral) {

    case 0:

      ledcWrite(BUZZER_CHANNEL, 128);

      if ((milisActualesBuzzerPrimerUmbral - milisPreviosBuzzerPrimerUmbral) > 300) {

        milisPreviosBuzzerPrimerUmbral = milisActualesBuzzerPrimerUmbral;
        estadoMaquinaBuzzerPrimerUmbral = 1;

      }

      break;

    case 1:

      ledcWrite(BUZZER_CHANNEL, 0);

      if ((milisActualesBuzzerPrimerUmbral - milisPreviosBuzzerPrimerUmbral) > 300) {

        milisPreviosBuzzerPrimerUmbral = milisActualesBuzzerPrimerUmbral;
        estadoMaquinaBuzzerPrimerUmbral = 0;

      }

      break;

  }

}

void buzzerSegundoUmbral() {

  milisActualesBuzzerSegundoUmbral = millis();

  switch (estadoMaquinaBuzzerSegundoUmbral) {

    case 0:

      ledcWrite(BUZZER_CHANNEL, 128);

      if ((milisActualesBuzzerSegundoUmbral - milisPreviosBuzzerSegundoUmbral) > 150) {

        milisPreviosBuzzerSegundoUmbral = milisActualesBuzzerSegundoUmbral;
        estadoMaquinaBuzzerSegundoUmbral = 1;

      }

      break;

    case 1:

      ledcWrite(BUZZER_CHANNEL, 0);

      if ((milisActualesBuzzerSegundoUmbral - milisPreviosBuzzerSegundoUmbral) > 150) {

        milisPreviosBuzzerSegundoUmbral = milisActualesBuzzerSegundoUmbral;
        estadoMaquinaBuzzerSegundoUmbral = 0;

      }

      break;

  }

}

void buzzerTercerUmbral() {

  milisActualesBuzzerTercerUmbral = millis();

  switch (estadoMaquinaBuzzerTercerUmbral) {

    case 0:

      ledcWrite(BUZZER_CHANNEL, 128);

      if ((milisActualesBuzzerTercerUmbral - milisPreviosBuzzerTercerUmbral) > 150) {

        milisPreviosBuzzerTercerUmbral = milisActualesBuzzerTercerUmbral;
        estadoMaquinaBuzzerTercerUmbral = 1;

      }

      break;

    case 1:

      ledcWrite(BUZZER_CHANNEL, 0);

      if ((milisActualesBuzzerTercerUmbral - milisPreviosBuzzerTercerUmbral) > 150) {

        milisPreviosBuzzerTercerUmbral = milisActualesBuzzerTercerUmbral;
        estadoMaquinaBuzzerTercerUmbral = 0;

      }

      break;

  }

}

void establecimientoPrioridades() {
  

  if (distanciaCmIzq < UMBRAL_DE_DETECCION_TERCERO && distanciaCmDer < UMBRAL_DE_DETECCION_TERCERO) {
    prioridad = 1;
  }

  if (distanciaCmIzq < UMBRAL_DE_DETECCION_TERCERO && distanciaCmDer < UMBRAL_DE_DETECCION_PRIMERO && distanciaCmDer > UMBRAL_DE_DETECCION_SEGUNDO) {
    prioridad = 1;
  }

  if (distanciaCmIzq < UMBRAL_DE_DETECCION_TERCERO && distanciaCmDer < UMBRAL_DE_DETECCION_SEGUNDO && distanciaCmDer > UMBRAL_DE_DETECCION_TERCERO) {
    prioridad = 1;
  }

  if (distanciaCmDer < UMBRAL_DE_DETECCION_TERCERO && distanciaCmIzq < UMBRAL_DE_DETECCION_PRIMERO && distanciaCmIzq > UMBRAL_DE_DETECCION_SEGUNDO) {
    prioridad = 1;
  }

  if (distanciaCmDer < UMBRAL_DE_DETECCION_TERCERO && distanciaCmIzq < UMBRAL_DE_DETECCION_SEGUNDO && distanciaCmIzq > UMBRAL_DE_DETECCION_TERCERO) {
    prioridad = 1;
  }


  if (distanciaCmIzq < UMBRAL_DE_DETECCION_SEGUNDO && distanciaCmIzq > UMBRAL_DE_DETECCION_TERCERO && distanciaCmDer < UMBRAL_DE_DETECCION_SEGUNDO && distanciaCmDer > UMBRAL_DE_DETECCION_TERCERO) {
    prioridad = 2;
  }

  if (distanciaCmIzq < UMBRAL_DE_DETECCION_SEGUNDO && distanciaCmIzq > UMBRAL_DE_DETECCION_TERCERO && distanciaCmDer < UMBRAL_DE_DETECCION_PRIMERO && distanciaCmDer > UMBRAL_DE_DETECCION_SEGUNDO) {
    prioridad = 2;
  }

  if (distanciaCmDer < UMBRAL_DE_DETECCION_SEGUNDO && distanciaCmDer > UMBRAL_DE_DETECCION_TERCERO && distanciaCmIzq < UMBRAL_DE_DETECCION_PRIMERO && distanciaCmIzq > UMBRAL_DE_DETECCION_SEGUNDO) {
    prioridad = 2;
  }
  

  if (distanciaCmIzq < UMBRAL_DE_DETECCION_PRIMERO && distanciaCmIzq > UMBRAL_DE_DETECCION_SEGUNDO && distanciaCmDer < UMBRAL_DE_DETECCION_PRIMERO && distanciaCmDer > UMBRAL_DE_DETECCION_SEGUNDO) {
    prioridad = 3;
  }
  

  if (distanciaCmIzq < UMBRAL_DE_DETECCION_PRIMERO && distanciaCmDer > UMBRAL_DE_DETECCION_PRIMERO) {
    prioridad = 0;
  }

  if (distanciaCmDer < UMBRAL_DE_DETECCION_PRIMERO && distanciaCmIzq > UMBRAL_DE_DETECCION_PRIMERO) {
    prioridad = 0;
  }
  

  if (distanciaCmIzq > UMBRAL_DE_DETECCION_PRIMERO && distanciaCmDer > UMBRAL_DE_DETECCION_PRIMERO) {
    prioridad = 4;
  }
  
}

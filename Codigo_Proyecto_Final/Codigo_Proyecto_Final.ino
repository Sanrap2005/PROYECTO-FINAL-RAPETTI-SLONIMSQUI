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

#define CAMBIO_ROJO_DER_1 0
#define CAMBIO_AMARILLO_DER_1 1
#define CAMBIO_ROJO_DER_2 2
#define CAMBIO_AMARILLO_DER_2 3
#define CAMBIO_ROJO_DER_3 4
#define CAMBIO_AMARILLO_DER_3 5

#define CAMBIO_ROJO_IZQ_1 0
#define CAMBIO_AMARILLO_IZQ_1 1
#define CAMBIO_ROJO_IZQ_2 2
#define CAMBIO_AMARILLO_IZQ_2 3
#define CAMBIO_ROJO_IZQ_3 4
#define CAMBIO_AMARILLO_IZQ_3 5

#define TEXTO_INICIALIZACION_1 0
#define TEXTO_INICIALIZACION_2 1
#define TEXTO_INICIALIZACION_3 2
#define TEXTO_INICIALIZACION_4 3
#define TEXTO_INICIALIZACION_5 4
#define TEXTO_INICIALIZACION_6 5
#define PRINTEO_OJO 6
#define PRINTEO_AUTO 7
#define COMPARACIONES_DATOS_UMBRAL 8

#define UMBRAL_DE_DETECCION 20 //Límite de distancia en centímetros
#define INTERVALO_DE_ANIMACION 200 //Tiempo de duración de los dos estados del titileo, tanto para el indicador IZQ como el DER
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

uint32_t naranjaDer = tiraDer.Color(255, 140, 0);
uint32_t vacioDer = tiraDer.Color(0, 0, 0);

uint32_t naranjaIzq = tiraIzq.Color(255, 140, 0);
uint32_t vacioIzq = tiraIzq.Color(0, 0, 0);

int duracionUsDer;
int duracionUsIzq;

int distanciaCmDer;
int distanciaCmIzq;;

unsigned long microsPreviosDer;
unsigned long microsPreviosIzq;

unsigned long milisActualesGeneral;
unsigned long milisActualesIzq;
unsigned long milisActualesDer;

unsigned long milisPreviosGeneral;
unsigned long milisPreviosIzq;
unsigned long milisPreviosDer;

unsigned long microsActualesDer;
unsigned long microsActualesIzq;

int estadoMaquinaGeneral;
int estadoMaquinaAnimacionIzq;
int estadoMaquinaAnimacionDer;

int estadoMaquinaDeteccionDer;
int estadoMaquinaDeteccionIzq;

int flagIzq;
int flagDer;

int estadoDibujoDeteccionIzq;
int estadoDibujoDeteccionDer;


void setup() {

  Serial.begin (BAUD_RATE);

  pinMode(TRIG_PIN_DER, OUTPUT);
  pinMode(ECHO_PIN_DER, INPUT);

  pinMode(TRIG_PIN_IZQ, OUTPUT);
  pinMode(ECHO_PIN_IZQ, INPUT);

  microsPreviosDer = 0;
  microsPreviosIzq = 0;

  milisPreviosGeneral = 0;
  milisPreviosIzq = 0;
  milisPreviosDer = 0;

  tft.init(); //Funcion de inicializacion de la pantalla
  tft.setRotation(0); //Seteo de la orientación vertical de la pantalla
  tft.fillScreen(TFT_BLACK); //Seteo color del fondo

  tiraDer.begin();
  tiraIzq.begin();

  tiraDer.fill(vacioDer, 0, 8);
  tiraIzq.fill(vacioIzq, 0, 8);

  ledcSetup(BUZZER_CHANNEL, 2000, 8); // Configurar el canal PWM
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL); // Asociar el pin al canal PWM
  ledcWrite(BUZZER_CHANNEL, 0);

}

void loop() {

  // Serial.print("DERECHA: ");
  // Serial.println(distanciaCmDer);

  // Serial.print("IZQUIERDA: ");
  // Serial.println(distanciaCmIzq);

  milisActualesGeneral = millis(); // Guarda el tiempo en milisegundos desde que se inicio el programa

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

        estadoMaquinaGeneral = 8;

      }

      break;

    case COMPARACIONES_DATOS_UMBRAL: //Condiciones para que printee en la pantalla las animaciones de detección

      ledcWrite(BUZZER_CHANNEL, 0);


      if (distanciaCmIzq < UMBRAL_DE_DETECCION && flagIzq == 0) {

        maquinaDeEstadosAnimacionIzquierda ();

      }

      if (flagIzq == 1) {

        maquinaDeEstadosAnimacionIzquierda ();

      }

      if (distanciaCmDer < UMBRAL_DE_DETECCION && flagDer == 0) {

        maquinaDeEstadosAnimacionDerecha ();

      }

      if (flagDer == 1) {

        maquinaDeEstadosAnimacionDerecha ();
      }


      break;

  }

}



void maquinaDeEstadosAnimacionIzquierda() { //Función que contiene las dos animaciones del ultrasonido y mitad izquierda: El titileo y el símbolo de detección

  milisActualesIzq = millis();


  if (flagIzq == 0) {

    flagIzq = 1;
    milisActualesIzq = 0;

  }

  switch (estadoMaquinaAnimacionIzq) {

    case CAMBIO_ROJO_IZQ_1:

      ledcWrite(BUZZER_CHANNEL, 128);
      
      Serial.println("NARANJA");

      tiraIzq.show();
      tiraIzq.setBrightness(100); // brillo global para toda la tira
      tiraIzq.fill(naranjaIzq, 0, 8);

      if ((milisActualesIzq - milisPreviosIzq) >= INTERVALO_DE_ANIMACION) {

        estadoRojoTitileoIzq ();

        if (estadoDibujoDeteccionIzq == 0) {

          tft.fillCircle(38, 135, 15, TFT_GREEN );
          tft.fillCircle(36, 139, 17, TFT_BLACK );

          estadoDibujoDeteccionIzq = 1;

        }

        milisPreviosIzq = milisActualesIzq;

        estadoMaquinaAnimacionIzq = 1;

      }

      break;


    case CAMBIO_AMARILLO_IZQ_1:

      if ((milisActualesIzq - milisPreviosIzq) >= INTERVALO_DE_ANIMACION) {

        estadoAmarilloTitileoIzq ();

        milisPreviosIzq = milisActualesIzq;
        estadoMaquinaAnimacionIzq = 2;

      }

      break;

    case CAMBIO_ROJO_IZQ_2:

      if ((milisActualesIzq - milisPreviosIzq) >= INTERVALO_DE_ANIMACION) {

        estadoRojoTitileoIzq ();

        if (estadoDibujoDeteccionIzq == 1) {

          tft.fillCircle(28, 157, 25, TFT_GREEN );
          tft.fillCircle(25, 163, 29, TFT_BLACK );

          estadoDibujoDeteccionIzq = 2;

        }

        milisPreviosIzq = milisActualesIzq;
        estadoMaquinaAnimacionIzq = 3;

      }

      break;

    case CAMBIO_AMARILLO_IZQ_2:

      if ((milisActualesIzq - milisPreviosIzq) >= INTERVALO_DE_ANIMACION) {

        estadoAmarilloTitileoIzq ();

        milisPreviosIzq = milisActualesIzq;
        estadoMaquinaAnimacionIzq = 4;

      }

      break;

    case CAMBIO_ROJO_IZQ_3:

      if ((milisActualesIzq - milisPreviosIzq) >= INTERVALO_DE_ANIMACION) {

        estadoRojoTitileoIzq ();

        if (estadoDibujoDeteccionIzq == 2) {

          tft.fillCircle(21, 177, 28, TFT_GREEN );
          tft.fillCircle(18, 181, 30, TFT_BLACK );

          estadoDibujoDeteccionIzq = 0;

        }

        milisPreviosIzq = milisActualesIzq;
        estadoMaquinaAnimacionIzq = 5;

      }

      break;

    case CAMBIO_AMARILLO_IZQ_3:

      if ((milisActualesIzq - milisPreviosIzq) >= INTERVALO_DE_ANIMACION) {

        estadoAmarilloTitileoIzq ();

        tft.fillCircle(38, 135, 15, TFT_BLACK );
        tft.fillCircle(28, 157, 25, TFT_BLACK );
        tft.fillCircle(21, 177, 28, TFT_BLACK );

        tft.fillRect(20, 270, 80, 40, TFT_BLACK); //Tapar cuadrante IZQ


        flagIzq = 0;
        Serial.println("VACIO");
        tiraIzq.clear();
        milisPreviosIzq = milisActualesIzq;
        estadoMaquinaAnimacionIzq = 0;

      }

      break;

  }

}

void maquinaDeEstadosAnimacionDerecha () { //Funcion que contiene las dos animaciones del ultrasonido y mitad derecha: El titileo y el símbolo de detección

  milisActualesDer = millis();

  ledcWrite(BUZZER_CHANNEL, 128);

  tiraDer.show();
  tiraDer.setBrightness(100); // brillo global para toda la tira
  tiraDer.fill(naranjaDer, 0, 8);

  if (flagDer == 0) {

    flagDer = 1;
    milisActualesDer = 0;

  }

  switch (estadoMaquinaAnimacionDer) {

    case CAMBIO_ROJO_DER_1:

      if ((milisActualesDer - milisPreviosDer) >= INTERVALO_DE_ANIMACION) {

        estadoRojoTitileoDer ();

        if (estadoDibujoDeteccionDer == 0) {

          tft.fillCircle(202, 135, 15, TFT_GREEN );
          tft.fillCircle(204, 139, 17, TFT_BLACK );

          estadoDibujoDeteccionDer = 1;

        }

        milisPreviosDer = milisActualesDer;
        estadoMaquinaAnimacionDer = 1;

      }

      break;


    case CAMBIO_AMARILLO_DER_1:

      if ((milisActualesDer - milisPreviosDer) >= INTERVALO_DE_ANIMACION) {

        estadoAmarilloTitileoDer ();

        milisPreviosDer = milisActualesDer;
        estadoMaquinaAnimacionDer = 2;

      }

      break;

    case CAMBIO_ROJO_DER_2:

      if ((milisActualesDer - milisPreviosDer) >= INTERVALO_DE_ANIMACION) {

        estadoRojoTitileoDer ();

        if (estadoDibujoDeteccionDer == 1) {

          tft.fillCircle(212, 157, 25, TFT_GREEN );
          tft.fillCircle(215, 163, 29, TFT_BLACK );

          estadoDibujoDeteccionDer = 2;

        }

        milisPreviosDer = milisActualesDer;
        estadoMaquinaAnimacionDer = 3;

      }

      break;

    case CAMBIO_AMARILLO_DER_2:

      if ((milisActualesDer - milisPreviosDer) >= INTERVALO_DE_ANIMACION) {

        estadoAmarilloTitileoDer ();

        tft.fillCircle(212, 157, 25, TFT_GREEN );
        tft.fillCircle(215, 163, 29, TFT_BLACK );

        milisPreviosDer = milisActualesDer;
        estadoMaquinaAnimacionDer = 4;

      }

      break;

    case CAMBIO_ROJO_DER_3:

      if ((milisActualesDer - milisPreviosDer) >= INTERVALO_DE_ANIMACION) {

        estadoRojoTitileoDer ();

        if (estadoDibujoDeteccionDer == 2) {

          tft.fillCircle(219, 177, 28, TFT_GREEN );
          tft.fillCircle(222, 181, 30, TFT_BLACK );

          estadoDibujoDeteccionDer = 0;

        }

        milisPreviosDer = milisActualesDer;
        estadoMaquinaAnimacionDer = 5;

      }

      break;

    case CAMBIO_AMARILLO_DER_3:

      if ((milisActualesDer - milisPreviosDer) >= INTERVALO_DE_ANIMACION) {

        estadoAmarilloTitileoDer ();

        tft.fillCircle(202, 135, 15, TFT_BLACK);
        tft.fillCircle(212, 157, 25, TFT_BLACK);
        tft.fillCircle(219, 177, 28, TFT_BLACK);

        tft.fillRect(140, 270, 80, 40, TFT_BLACK); //Tapar cuadrante DER


        flagDer = 0;
        tiraDer.fill(0, 8);
        milisPreviosDer = milisActualesDer;
        estadoMaquinaAnimacionDer = 0;

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

  tft.fillRoundRect(60, 40, 120, 210, 30, TFT_SILVER); //Silueta del auto
  tft.drawRoundRect(60, 40, 120, 210, 30, TFT_BLACK);

  tft.drawLine(80, 60, 160, 60, TFT_BLACK); //Línea horizontal superior del CAPÓ
  tft.drawLine(70, 90, 170, 90, TFT_BLACK); //Línea horizontal inferior
  tft.drawLine(80, 60, 70, 90, TFT_BLACK); //Diagonal izquierda
  tft.drawLine(160, 60, 170, 90, TFT_BLACK); //Diagonal derecha

  tft.fillTriangle(75, 100, 165, 100, 80, 125, TFT_SKYBLUE);
  tft.fillTriangle(165, 100, 80, 125, 160, 125, TFT_SKYBLUE);

  tft.drawLine(75, 100, 165, 100, TFT_BLACK); //Línea horizontal superior del VIDRIO DELANTERO
  tft.drawLine(80, 125, 160, 125, TFT_BLACK); //Línea horizontal inferior
  tft.drawLine(75, 100, 80, 125, TFT_BLACK); //Diagonal izquierda
  tft.drawLine(165, 100, 160, 125, TFT_BLACK); //Diagonal derecha

  tft.fillTriangle(80, 230, 85, 210, 160, 230, TFT_SKYBLUE);
  tft.fillTriangle(85, 210, 160, 230, 155, 210, TFT_SKYBLUE);

  tft.drawLine(80, 230, 160, 230, TFT_BLACK); //Línea horizontal inferior del VIDRIO TRASERO
  tft.drawLine(85, 210, 155, 210, TFT_BLACK); //Línea horizontal superior
  tft.drawLine(80, 230, 85, 210, TFT_BLACK); //Diagonal izquierda
  tft.drawLine(160, 230, 155, 210, TFT_BLACK); //Diagonal derecha

  tft.fillTriangle(172, 125, 160, 195, 172, 210, TFT_SKYBLUE);
  tft.fillTriangle(172, 125, 160, 195, 160, 140, TFT_SKYBLUE);

  tft.drawLine(172, 125, 172, 210, TFT_BLACK); //Línea vertical externa VIDRIO DERECHO
  tft.drawLine(160, 140, 160, 195, TFT_BLACK); //Linea vertical interna
  tft.drawLine(172, 125, 160, 140, TFT_BLACK); //Diagonal superior
  tft.drawLine(172, 210, 160, 195, TFT_BLACK); //Diagonal inferior

  tft.fillTriangle(68, 125, 80, 140, 68, 210, TFT_SKYBLUE);
  tft.fillTriangle(80, 140, 68, 210, 80, 195, TFT_SKYBLUE);

  tft.drawLine(68, 125, 68, 210, TFT_BLACK); //Linea vertical externa VIDRIO IZQUIERDO
  tft.drawLine(80, 140, 80, 195, TFT_BLACK); //Linea vertical interna
  tft.drawLine(68, 125, 80, 140, TFT_BLACK); //Diagonal superior
  tft.drawLine(68, 210, 80, 195, TFT_BLACK); //Diagonal inferior

  tft.fillTriangle(87, 140, 92, 208, 97, 210, TFT_BLACK); //PORTAEQUIPAJE IZQUIERDA
  tft.fillTriangle(153, 140, 148, 208, 143, 210, TFT_BLACK); //PORTAEQUIPAJE DERECHA

  tft.fillEllipse(88, 243, 15, 5, TFT_RED2); //LUCES TRASERAS IZQUIERDA
  tft.fillTriangle(85, 249, 103, 249, 103, 243, TFT_RED2);
  tft.drawPixel(81, 248, TFT_RED2);

  tft.fillEllipse(151, 243, 15, 5, TFT_RED2); //LUCES TRASERAS DERECHA
  tft.fillTriangle(154, 249, 136, 249, 136, 243, TFT_RED2);
  tft.drawPixel(158, 248, TFT_RED2);

  tft.fillEllipse(55, 105, 14, 5, TFT_SILVER); //ESPEJO RETROVISOR IZQUIERDA
  tft.fillTriangle(40, 103, 40, 105, 70, 105, TFT_SILVER);

  tft.fillEllipse(185, 105, 14, 5, TFT_SILVER); //ESPEJO RETROVISOR DERECHA
  tft.fillTriangle(200, 100, 197, 105, 170, 105, TFT_SILVER);

  tft.fillRect(57, 75, 3, 23, TFT_LIGHTGRAY); //RUEDA delantera izquierda
  tft.fillRect(57, 200, 3, 25, TFT_LIGHTGRAY); //Rueda trasera izquierda
  tft.fillRect(180, 75, 3, 23, TFT_LIGHTGRAY); //Rueda delantera derecha
  tft.fillRect(180, 200, 3, 25, TFT_LIGHTGRAY); //Rueda trasera derecha

  tft.setTextColor(TFT_GREEN2); //Power ON
  tft.setTextSize(3);
  tft.drawString("Power", 50, 5, 1);
  tft.drawString("ON", 150, 5, 1);

}

void estadoAmarilloTitileoIzq () { //Funcion para cambiar el color a amarillo del indicador IZQ

  tft.setTextColor(TFT_BLACK);
  tft.fillRect(20, 270, 80, 40, TFT_YELLOW2);
  tft.setTextSize(3);
  tft.drawString("IZQ", 33, 280, 1);

}

void estadoRojoTitileoIzq () { //Función para cambiar el color a rojo del indicador IZQ

  tft.setTextColor(TFT_BLACK);
  tft.fillRect(20, 270, 80, 40, TFT_RED);
  tft.setTextSize(3);
  tft.drawString("IZQ", 33, 280, 1);

}
void estadoAmarilloTitileoDer () { //Funcion para cambiar el color a amarillo del indicador DER

  tft.setTextColor(TFT_BLACK);
  tft.fillRect(140, 270, 80, 40, TFT_YELLOW2);
  tft.setTextSize(3);
  tft.drawString("DER", 153, 280, 1);

}

void estadoRojoTitileoDer () { //Funcion para cambiar el color a rojo del indicador DER

  tft.setTextColor(TFT_BLACK);
  tft.fillRect(140, 270, 80, 40, TFT_RED);
  tft.setTextSize(3);
  tft.drawString("DER", 153, 280, 1);

}

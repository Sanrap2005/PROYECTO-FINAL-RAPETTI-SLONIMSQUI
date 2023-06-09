#include <TFT_eSPI.h>
#include <SPI.h>

#define TFT_LIGHTGRAY 0x8C30
#define TFT_GRAY 0x4A69
#define TFT_DARKRED 0x7041
#define TFT_GREEN2 0x1ECA
#define TFT_YELLOW2 0xEF63
#define TFT_RED2 0xE042

TFT_eSPI tft = TFT_eSPI ();


void setup() {

  tft.init(); //Funcion de inicializacion de la pantalla
  tft.setRotation(0); //Seteo de la orientación vertical de la pantalla
  tft.fillScreen(TFT_BLACK); //Seteo color del fondo

  printeoAuto();
  estadoRojoTitileoIzq ();
  estadoRojoTitileoDer ();
  animacion();
}

void loop() {


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

void estadoRojoTitileoIzq () { //Función para cambiar el color a rojo del indicador IZQ

  tft.setTextColor(TFT_BLACK);
  tft.fillRect(20, 270, 80, 40, TFT_RED);
  tft.setTextSize(3);
  tft.drawString("IZQ", 33, 280, 1);

}

void estadoRojoTitileoDer () { //Funcion para cambiar el color a rojo del indicador DER

  tft.setTextColor(TFT_BLACK);
  tft.fillRect(140, 270, 80, 40, TFT_RED);
  tft.setTextSize(3);
  tft.drawString("DER", 153, 280, 1);

}

void animacion() {

  tft.fillCircle(35, 145, 15, TFT_GREEN );
  tft.fillCircle(33, 150, 17, TFT_BLACK );

  tft.fillCircle(23, 177, 25, TFT_GREEN );
  tft.fillCircle(20, 184, 29, TFT_BLACK );

  tft.fillCircle(16, 207, 28, TFT_GREEN );
  tft.fillCircle(13, 212, 30, TFT_BLACK );

}

#include <TFT_eSPI.h>
#include <SPI.h>

#define TFT_LIGHTGRAY 0x8C30
#define TFT_GRAY 0x4A69
#define TFT_DARKRED 0x7041
#define TFT_GREEN2 0x1ECA
#define TFT_YELLOW2 0xEF63
#define TFT_RED2 0xE042
#define TFT_WHITE 0xFFFF


TFT_eSPI tft = TFT_eSPI ();


void setup() {

  tft.init(); //Funcion de inicializacion de la pantalla
  tft.setRotation(2); //Seteo de la orientación vertical de la pantalla
  tft.fillScreen(TFT_BLACK); //Seteo color del fondo

  printeoAuto();
  // estadoRojoTitileoIzq ();
  //estadoRojoTitileoDer ();
  animacionLinea();
}

void loop() {


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



  //tft.fillTriangle(80, 230, 85, 210, 160, 230, TFT_SKYBLUE);
  //tft.fillTriangle(85, 210, 160, 230, 155, 210, TFT_SKYBLUE);






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

  //tft.setTextColor(TFT_GREEN2); //Power ON
  //tft.setTextSize(3);
  //tft.drawString("Power", 50, 5, 1);
  //tft.drawString("ON", 150, 5, 1);

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

  tft.fillTriangle(75, 230, 0, 320, 60, 320, TFT_WHITE);
  tft.fillTriangle(75, 230, 0, 320, 0, 245, TFT_WHITE);
  tft.fillTriangle(60, 207, 75, 230, 0, 245, TFT_WHITE);

  tft.fillTriangle(60, 207, 75, 230, 33, 224, TFT_RED2);//Rojo der
  tft.fillTriangle(72, 250, 75, 230, 33, 224, TFT_RED2);

  tft.fillTriangle(0, 244, 72, 250, 33, 224, TFT_ORANGE);
  tft.fillTriangle(0, 244, 72, 250, 65, 290, TFT_ORANGE);

  tft.fillTriangle(0, 244, 0, 320, 65, 290, TFT_YELLOW2);
  tft.fillTriangle(60, 320, 0, 320, 65, 290, TFT_YELLOW2);


}

void animacionLinea() {

  tft.drawLine(60, 207, 75, 230, TFT_WHITE); //linea superior cono izquierda
  tft.drawLine(60, 207, 0, 244, TFT_WHITE); //diagonal izquierda
  tft.drawLine(75, 230, 60, 320, TFT_WHITE); //diagonal derecha
  
  tft.drawLine(33, 224, 72, 250, TFT_WHITE); //divisor rojo naranja
  
  tft.drawLine(0, 244, 65, 290, TFT_WHITE); //divisor rojo naranja


}

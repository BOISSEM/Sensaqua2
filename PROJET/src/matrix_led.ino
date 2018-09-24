/********************************************************************* 
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>

/*=========================================================================
    APPLICATION SETTINGS
                              
    MATRIX DECLARATION        Parameter 1 = width of NeoPixel matrix
                              Parameter 2 = height of matrix
                              Parameter 3 = pin number (most are valid)
                              Parameter 4 = matrix layout flags, add together as needed:
    NEO_MATRIX_TOP,
    NEO_MATRIX_BOTTOM,
    NEO_MATRIX_LEFT,
    NEO_MATRIX_RIGHT          Position of the FIRST LED in the matrix; pick two, e.g.
                              NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
                              NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
                              rows or in vertical columns, respectively; pick one or the other.
    NEO_MATRIX_PROGRESSIVE,
    NEO_MATRIX_ZIGZAG         all rows/columns proceed in the same order, 
                              or alternate lines reverse direction; pick one.

                              See example below for these values in action.
    
    Parameter 5 = pixel type flags, add together as needed:
                              
    NEO_KHZ800                800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
    NEO_KHZ400                400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
    NEO_GRB                   Pixels are wired for GRB bitstream (most NeoPixel products)
    NEO_RGB                   Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE     1

    #define PIN                     6   // Which pin on the Arduino is connected to the NeoPixels
    #define width                   2
    #define height                  15

// Example for NeoPixel 8x8 Matrix.  In this application we'd like to use it 
// with the back text positioned along the bottom edge.
// When held that way, the first pixel is at the top left, and
// lines are arranged in columns, zigzag order.  The 8x8 matrix uses
// 800 KHz (v2) pixels that expect GRB color data.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(width, height, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT+
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);
/*=========================================================================*/
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX, TX

String data ; 
int type ; 

/*=========================================================================*/

void setup(void)
{
  pinMode(PIN, OUTPUT);      // sets the digital pin 6 as output
  matrix.begin();
  matrix.setBrightness(50);
  matrix.fillScreen(0);
  matrix.show(); // This sends the updated pixel colors to the hardware.

  Serial.begin(115200);
  Serial.println("Arduino ready");
  mySerial.begin(115200);
  
}

/*=========================================================================*/

void loop(void)
{
   if (mySerial.available() > 0){ 
     data = mySerial.readString(); // Lecture des données arrivant sur le port serie
     
      if(data[0] == 'A'){
        Serial.println("allumage");
        allumage_progressif();
      }

     else if(data[0] == 'E'){
        Serial.println("extinction");
        extinction_progressif();
      }

      else if(data[0] == 'M'){
         Serial.println("\nmodulation");
         
         // Parse of String data
         type = data[2] -'0';
                    
         Serial.print("type = ");
         Serial.print(type);
         modulation_luminosite(type);
        }
   }
  
}

/*=========================================================================*/
/*=========================================================================*/
void allumage_progressif(void){
  /******Lumière rouge *******/
  matrix.drawPixel(0, 2, matrix.Color(255,0,0)); // x, y, color
  matrix.drawPixel(0,12, matrix.Color(255,0,0)); // x, y, color
  
  matrix.drawPixel(1, 7, matrix.Color(255,0,0)); // x, y, color
  matrix.show(); // This sends the updated pixel colors to the hardware.

  // Attente de 3 min 
  //delay(18000);
  delay(2000); // 2 s

   /******Lumière Bleue *******/
  matrix.drawPixel(0, 1, matrix.Color(0,0,255)); // x, y, color
  matrix.drawPixel(0, 3, matrix.Color(0,0,255)); // x, y, color
  matrix.drawPixel(0, 7, matrix.Color(0,0,255)); // x, y, color
  matrix.drawPixel(0, 11, matrix.Color(0,0,255)); // x, y, color
  matrix.drawPixel(0, 13, matrix.Color(0,0,255)); // x, y, color

  matrix.drawPixel(1, 12, matrix.Color(0,0,255)); // x, y, color
  matrix.drawPixel(1, 8, matrix.Color(0,0,255)); // x, y, color
  matrix.drawPixel(1, 6, matrix.Color(0,0,255)); // x, y, color
  matrix.drawPixel(1, 2, matrix.Color(0,0,255)); // x, y, color
  matrix.show(); // This sends the updated pixel colors to the hardware.
  
  // Attente de 10 min 
  //delay(60000);
  delay(4000); // 4 s

   /******Lumière Blanche *******/
  matrix.drawPixel(0, 0, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(0, 4, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(0, 5, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(0, 6, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(0, 8, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(0, 9, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(0, 10, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(0, 14, matrix.Color(255,255,255)); // x, y, color

  matrix.drawPixel(1, 14, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(1, 13, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(1, 11, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(1, 10, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(1, 9, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(1, 5, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(1, 4, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(1, 3, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(1, 1, matrix.Color(255,255,255)); // x, y, color
  matrix.drawPixel(1, 0, matrix.Color(255,255,255)); // x, y, color
  matrix.show(); // This sends the updated pixel colors to the hardware.

}
/*=========================================================================*/
/*=========================================================================*/

void extinction_progressif(void){

  /******Lumière Blanche *******/
  matrix.drawPixel(0, 0, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(0, 4, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(0, 5, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(0, 6, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(0, 8, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(0, 9, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(0, 10, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(0, 14, matrix.Color(0,0,0)); // x, y, color

  matrix.drawPixel(1, 14, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(1, 13, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(1, 11, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(1, 10, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(1, 9, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(1, 5, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(1, 4, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(1, 3, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(1, 1, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(1, 0, matrix.Color(0,0,0)); // x, y, color
  matrix.show(); // This sends the updated pixel colors to the hardware.

   // Attente de 10 min 
  //delay(60000);
  delay(4000); //  s

  
  /******Lumière Bleue *******/
  matrix.drawPixel(0, 1, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(0, 3, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(0, 7, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(0, 11, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(0, 13, matrix.Color(0,0,0)); // x, y, color

  matrix.drawPixel(1, 12, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(1, 8, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(1, 6, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(1, 2, matrix.Color(0,0,0)); // x, y, color
  matrix.show(); // This sends the updated pixel colors to the hardware.
  
  // Attente de 3 min 
  //delay(18000);
  delay(2000); // 1 s
  
  
  /******Lumière rouge *******/
  matrix.drawPixel(0, 2, matrix.Color(0,0,0)); // x, y, color
  matrix.drawPixel(0,12, matrix.Color(0,0,0)); // x, y, color
  
  matrix.drawPixel(1, 7, matrix.Color(0,0,0)); // x, y, color
  matrix.show(); // This sends the updated pixel colors to the hardware.

 
}
/*=========================================================================*/
// Type    : augmentation = 1 , diminution = 0
/*=========================================================================*/
void modulation_luminosite(int type){

   int pourcentage = 90 ; // pourcentage de changement
   int ancien_val,new_val,delta;
  
  ancien_val= matrix.getBrightness(); 
  delta = (int)((ancien_val * pourcentage) / 100 ); 
  
  switch(type){
    case 0 : 
        new_val = ancien_val - delta ; 
        matrix.setBrightness(new_val);
        break ; 
    case 1:
        new_val = ancien_val + delta ; 
        matrix.setBrightness(new_val);
        break;
     default :
        Serial.print("Action impossible\n\r");
  }

}


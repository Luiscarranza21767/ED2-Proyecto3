//PROGRAMA: PROYECTO DIGITAL 2, ESP32 Y TIVA C

#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define BAUDRATE 115200
#include <WiFi.h>
#include <WebServer.h>
 
#define  OLED_RESET -1   
#define SCREEN_WIDTH 128 //  OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display heightint  setButton = 9;
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
HardwareSerial SerialPort2(2); // use UART2
HardwareSerial SerialPort1(1); // use UART2


// SSID & Password
const char* ssid = "Pablo";  // Enter your SSID here
const char* password = "mkst1801";  //Enter your Password here
WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

//***********************VARIABLES*********************** 
int P1 = 0;
int P2 = 0;
int P3 = 0;
int P4 = 0;
int P5 = 0;
int P6 = 0;
int P7 = 0;
int P8 = 0;
int barra = 0;
//***********************************************************


//SETUP
void setup() {
  Serial.begin(115200);
  SerialPort2.begin(115200, SERIAL_8N1, 16, 17);
  SerialPort1.begin(115200, SERIAL_8N1, 4, 2);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  Serial.println("Try Connecting to ");
  Serial.println(ssid);

    // Connect to your wi-fi modem
  WiFi.begin(ssid, password);
  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial
  
  server.on("/", handle_OnConnect);
  server.on("/P1oc", handle_P1ocupado);
  server.on("/P2oc", handle_P2ocupado);
  server.on("/P3oc", handle_P3ocupado);
  server.on("/P4oc", handle_P4ocupado);
  server.on("/P5oc", handle_P5ocupado);
  server.on("/P6oc", handle_P6ocupado);
  server.on("/P7oc", handle_P7ocupado);
  server.on("/P8oc", handle_P8ocupado);
  server.on("/P1lb", handle_P1libre);
  server.on("/P2lb", handle_P2libre);
  server.on("/P3lb", handle_P3libre);
  server.on("/P4lb", handle_P4libre);
  server.on("/P5lb", handle_P5libre);
  server.on("/P6lb", handle_P6libre);
  server.on("/P7lb", handle_P7libre);
  server.on("/P8lb", handle_P8libre);
  server.on("/barra", handle_barra);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}

void loop() {
  server.handleClient();
  barra = P1+P2+P3+P4+P5+P6+P7+P8;
  if (SerialPort2.available()){
    uint32_t inByte = SerialPort2.read();
    //PARQUEO 1
    if(inByte == '1'){
      P1 = 1; //libre
    }
    else if(inByte == '2'){
      P1 = 0;
    }
    else if(inByte == '3'){
      P2 = 1;
    }
    else if(inByte == '4'){
      P2 = 0;
    }
    else if(inByte == '5'){
      P3 = 1;
    }
    else if(inByte == '6'){
      P3 = 0;
    }
    else if(inByte == '7'){
      P4 = 1;
    }
    else if(inByte == '8'){
      P4 = 0;
    }
  }

  if (SerialPort1.available()){
    uint32_t inByte = SerialPort1.read();
    //PARQUEO 1
    if(inByte == '1'){
      P5 = 1; //libre
    }
    else if(inByte == '2'){
      P5 = 0;
    }
    else if(inByte == '3'){
      P6 = 1;
    }
    else if(inByte == '4'){
      P6 = 0;
    }
    else if(inByte == '5'){
      P7 = 1;
    }
    else if(inByte == '6'){
      P7 = 0;
    }
    else if(inByte == '7'){
      P8 = 1;
    }
    else if(inByte == '8'){
      P8 = 0;
    }
  }

  char libres[10];
  sprintf(libres, "%d", barra);

    // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F(" CONTROL DE PARQUEO"));
  display.setTextSize(2);             // Draw 2X-scale text
  display.println(F("HOT WHEELS"));
  display.setTextSize(1);             // Draw 2X-scale text
  display.println(F(" "));
  display.println(F("Parqueos libres:"));
  display.setTextSize(3);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.print(F("   "));
  display.println(libres);
  display.display();

}


void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(P1, P2, P3, P4, P5, P6, P7, P8, barra)); 
}

//HANDLES DE PARQUEOS OCUPADOS
void handle_P1ocupado() {
  server.send(200, "text/html", SendHTML(0, P2, P3, P4, P5, P6, P7, P8, barra)); 
}

void handle_P2ocupado() {
  server.send(200, "text/html", SendHTML(P1, 0, P3, P4, P5, P6, P7, P8, barra)); 
}

void handle_P3ocupado() {
  server.send(200, "text/html", SendHTML(P1, P2, 0, P4, P5, P6, P7, P8, barra)); 
}

void handle_P4ocupado() {
  server.send(200, "text/html", SendHTML(P1, P2, P3, 0, P5, P6, P7, P8, barra)); 
}

void handle_P5ocupado() {
  server.send(200, "text/html", SendHTML(P1, P2, P3, P4, 0, P6, P7, P8, barra)); 
}

void handle_P6ocupado() {
  server.send(200, "text/html", SendHTML(P1, P2, P3, P4, P5, 0, P7, P8, barra)); 
}

void handle_P7ocupado() {
  server.send(200, "text/html", SendHTML(P1, P2, P3, P4, P5, P6, 0, P8, barra)); 
}

void handle_P8ocupado() {
  server.send(200, "text/html", SendHTML(P1, P2, P3, P4, P5, P6, P7, 0, barra)); 
}
//Handles de parqueos libres
void handle_P1libre() {
  server.send(200, "text/html", SendHTML(1, P2, P3, P4, P5, P6, P7, P8, barra)); 
}
void handle_P2libre() {
  server.send(200, "text/html", SendHTML(P1, 1, P3, P4, P5, P6, P7, P8, barra)); 
}
void handle_P3libre() {
  server.send(200, "text/html", SendHTML(P1, P2, 1, P4, P5, P6, P7, P8, barra)); 
}
void handle_P4libre() {
  server.send(200, "text/html", SendHTML(P1, P2, P3, 1, P5, P6, P7, P8, barra)); 
}
void handle_P5libre() {
  server.send(200, "text/html", SendHTML(P1, P2, P3, P4, 1, P6, P7, P8, barra)); 
}
void handle_P6libre() {
  server.send(200, "text/html", SendHTML(P1, P2, P3, P4, P5, 1, P7, P8, barra)); 
}
void handle_P7libre() {
  server.send(200, "text/html", SendHTML(P1, P2, P3, P4, P5, P6, 1, P8, barra)); 
}
void handle_P8libre() {
  server.send(200, "text/html", SendHTML(P1, P2, P3, P4, P5, P6, P7, 1, barra)); 
}
//HANDLER DE LA BARRA DE PROGRESO
void handle_barra(){
  server.send(200, "text/html", SendHTML(P1, P2, P3, P4, P5, P6, P7, P8, barra));
}


//HANDLE NOT FOUND
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(int PQ1, int PQ2, int PQ3, int PQ4, int PQ5, int PQ6, int PQ7, int PQ8, int valorbarra){
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html lang=\"en\">\n";
  ptr += "<head>\n";
  ptr += "  <title>Control de parqueos</title>\n";
  ptr += "<script>";
  ptr += "function autoRefresh(){";
  ptr += "window.location = window.location.href;";
  ptr += "}";
  ptr += "setInterval('autoRefresh()', 1000)";
  ptr += "</script>";
  ptr += "  <meta charset=\"utf-8\">\n";
  ptr += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
  ptr += "  <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.2/dist/css/bootstrap.min.css\">\n";
  ptr += "  <script src=\"https://cdn.jsdelivr.net/npm/jquery@3.6.4/dist/jquery.slim.min.js\"></script>\n";
  ptr += "  <script src=\"https://cdn.jsdelivr.net/npm/popper.js@1.16.1/dist/umd/popper.min.js\"></script>\n";
  ptr += "  <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@4.6.2/dist/js/bootstrap.bundle.min.js\"></script>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "\n";
  ptr += "<div class=\"container p-3 my-3 bg-dark text-white\">\n";
  ptr += "  <h1>Control de parqueos Hot Wheels</h1>\n";
  ptr += "  <p>Listado de parqueos online:</p>\n";
  ptr += "</div> <br>\n";
  ptr += "\n";
  ptr += "<div class=\"container text-center\">\n";
  ptr += "\n";
  //BARRA DE PROGRESO
  if (valorbarra == 8){
    ptr += "  <h2>Parqueos ocupados</h2>\n";
    ptr += "  <div class=\"progress text-center\">\n";
    ptr += "    <div class=\"progress-bar\" style=\"width:100%\">100%</div>\n";
    ptr += "  </div> <br> <br>\n";
    ptr += "\n";
  }

  if (valorbarra == 7){
    ptr += "  <h2>Parqueos ocupados</h2>\n";
    ptr += "  <div class=\"progress text-center\">\n";
    ptr += "    <div class=\"progress-bar\" style=\"width:88%\">88%</div>\n";
    ptr += "  </div> <br> <br>\n";
    ptr += "\n";
  }

  if (valorbarra == 6){
    ptr += "  <h2>Parqueos ocupados</h2>\n";
    ptr += "  <div class=\"progress text-center\">\n";
    ptr += "    <div class=\"progress-bar\" style=\"width:75%\">75%</div>\n";
    ptr += "  </div> <br> <br>\n";
    ptr += "\n";
  }

  if (valorbarra == 5){
    ptr += "  <h2>Parqueos ocupados</h2>\n";
    ptr += "  <div class=\"progress text-center\">\n";
    ptr += "    <div class=\"progress-bar\" style=\"width:63%\">63%</div>\n";
    ptr += "  </div> <br> <br>\n";
    ptr += "\n";
  }

  if (valorbarra == 4){
    ptr += "  <h2>Parqueos ocupados</h2>\n";
    ptr += "  <div class=\"progress text-center\">\n";
    ptr += "    <div class=\"progress-bar\" style=\"width:50%\">50%</div>\n";
    ptr += "  </div> <br> <br>\n";
    ptr += "\n";
  }

  if (valorbarra == 3){
    ptr += "  <h2>Parqueos ocupados</h2>\n";
    ptr += "  <div class=\"progress text-center\">\n";
    ptr += "    <div class=\"progress-bar\" style=\"width:38%\">38%</div>\n";
    ptr += "  </div> <br> <br>\n";
    ptr += "\n";
  }

  if (valorbarra == 2){
    ptr += "  <h2>Parqueos ocupados</h2>\n";
    ptr += "  <div class=\"progress text-center\">\n";
    ptr += "    <div class=\"progress-bar\" style=\"width:25%\">25%</div>\n";
    ptr += "  </div> <br> <br>\n";
    ptr += "\n";
  }

  if (valorbarra == 1){
    ptr += "  <h2>Parqueos ocupados</h2>\n";
    ptr += "  <div class=\"progress text-center\">\n";
    ptr += "    <div class=\"progress-bar\" style=\"width:13%\">13%</div>\n";
    ptr += "  </div> <br> <br>\n";
    ptr += "\n";
  }

  if (valorbarra == 0){
    ptr += "  <h2>Parqueos ocupados</h2>\n";
    ptr += "  <div class=\"progress text-center\">\n";
    ptr += "    <div class=\"progress-bar\" style=\"width:0%\">0%</div>\n";
    ptr += "  </div> <br> <br>\n";
    ptr += "\n";
  }

//EMPIEZA TABLA DE PARQUEOS
  ptr += "  <table class=\"table table-bordered\">\n";
  ptr += "    <thead>\n";
  ptr += "      <tr class=\"thead-dark \">\n";
  ptr += "        <th>No. Parqueo</th>\n";
  ptr += "        <th>Estado</th>\n";
  ptr += "      </tr>\n";
  ptr += "    </thead>\n";
  ptr += "    <tbody>\n";
  ptr += "\n";
  //TABLA DE PARQUEOS
  if(PQ1 == 0){
    ptr += "      <tr class=\"table-danger\">\n";
    ptr += "        <td>1</td>\n";
    ptr += "        <td>Ocupado</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";

  }
  else{
    ptr += "      <tr class=\"table-success\">\n";
    ptr += "        <td>1</td>\n";
    ptr += "        <td>Libre</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";
  }

  if(PQ2 == 0){
    ptr += "      <tr class=\"table-danger\">\n";
    ptr += "        <td>2</td>\n";
    ptr += "        <td>Ocupado</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";

  }
  else{
    ptr += "      <tr class=\"table-success\">\n";
    ptr += "        <td>2</td>\n";
    ptr += "        <td>Libre</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";
  }

  if(PQ3 == 0){
    ptr += "      <tr class=\"table-danger\">\n";
    ptr += "        <td>3</td>\n";
    ptr += "        <td>Ocupado</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";

  }
  else{
    ptr += "      <tr class=\"table-success\">\n";
    ptr += "        <td>3</td>\n";
    ptr += "        <td>Libre</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";
  }

  if(PQ4 == 0){
    ptr += "      <tr class=\"table-danger\">\n";
    ptr += "        <td>4</td>\n";
    ptr += "        <td>Ocupado</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";

  }
  else{
    ptr += "      <tr class=\"table-success\">\n";
    ptr += "        <td>4</td>\n";
    ptr += "        <td>Libre</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";
  }

  if(PQ5 == 0){
    ptr += "      <tr class=\"table-danger\">\n";
    ptr += "        <td>5</td>\n";
    ptr += "        <td>Ocupado</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";

  }
  else{
    ptr += "      <tr class=\"table-success\">\n";
    ptr += "        <td>5</td>\n";
    ptr += "        <td>Libre</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";
  }

  if(PQ6 == 0){
    ptr += "      <tr class=\"table-danger\">\n";
    ptr += "        <td>6</td>\n";
    ptr += "        <td>Ocupado</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";

  }
  else{
    ptr += "      <tr class=\"table-success\">\n";
    ptr += "        <td>6</td>\n";
    ptr += "        <td>Libre</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";
  }

  if(PQ7 == 0){
    ptr += "      <tr class=\"table-danger\">\n";
    ptr += "        <td>7</td>\n";
    ptr += "        <td>Ocupado</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";

  }
  else{
    ptr += "      <tr class=\"table-success\">\n";
    ptr += "        <td>7</td>\n";
    ptr += "        <td>Libre</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";
  }

  if(PQ8 == 0){
    ptr += "      <tr class=\"table-danger\">\n";
    ptr += "        <td>8</td>\n";
    ptr += "        <td>Ocupado</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";

  }
  else{
    ptr += "      <tr class=\"table-success\">\n";
    ptr += "        <td>8</td>\n";
    ptr += "        <td>Libre</td>\n";
    ptr += "      </tr>\n";
    ptr += "      \n";
  }

  ptr += "    </tbody>\n";
  ptr += "  </table>\n";
  ptr += "</div>\n";
  ptr += "\n";
  ptr += "</body>\n";
  ptr += "</html>\n";


  // ptr +="</body>\n";
  // ptr +="</html>\n";
  return ptr;
}

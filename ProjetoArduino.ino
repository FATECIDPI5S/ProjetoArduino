#include <SPI.h>
#include <Ethernet.h>
String readString;
int numeroPedidos = 0;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 97, 242);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 97, 1);

int ledVerde = 7;
int ledVermelho = 6;

EthernetServer server(80);

void setup() {
  Serial.begin(9600);  
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  while (!Serial) {
    ;
  }
  
  Serial.println("Ethernet WebServer Example");  
  Ethernet.begin(mac, ip);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1);
    }
  }
  
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string
          readString += c;
          Serial.print(c); //print what server receives to serial monitor
        }
        //Serial.write(c);
        
        if (c == '\n' && currentLineIsBlank) {
          Serial.println(readString);
          //Serial.println(numeroPedidos);
          //numeroPedidos = readString.indexOf("?pedid");


          Serial.println(readString.indexOf("?pedidoNovo"));
          if (readString.indexOf("?pedidoNovo") > 0){
              digitalWrite(ledVermelho, HIGH);
              delay(1000);
              digitalWrite(ledVermelho, LOW);
          }

          if (readString.indexOf("?pedidoPronto") > 0) {
              digitalWrite(ledVerde, HIGH);
              delay(1000);
              digitalWrite(ledVerde, LOW);
          }

          readString = "";
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          //client.println("Refresh: 5");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("  <title>iComidaCozinha</title>");
          client.println("  <meta charset='utf-8'>");
          client.println("  <meta name='viewport' content='width=device-width, initial-scale=1'>");
          client.println("  <link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.1.3/css/bootstrap.min.css'>");
          client.println("  <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script>");
          client.println("  <script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.3/umd/popper.min.js'></script>");
          client.println("  <script src='https://maxcdn.bootstrapcdn.com/bootstrap/4.1.3/js/bootstrap.min.js'></script>");
          client.println("</head>");
          client.println("<body>");
          client.println("<div class='jumbotron text-center bg-danger text-white'>");
          client.println("  <h1><strong>iComida</strong> Cozinha</h1>");
          client.println("  <p>Gerencie o status dos pedidos aqui!</p>");
          client.println("</div>");
          client.println("<button class='btn btn-warning btn-atualizar' style='margin: 0 0 15px 15px'>Atualizar</button>");
          client.println("<div id='pedidos'></div>");
          client.println("<script src='http://187.255.165.251:8080/iComidaREST/rest/icomida/downloadScriptArchive/'></script>");  
          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    delay(5);
    client.stop();
    Serial.println("client disconnected");
  }
}

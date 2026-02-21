#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
const int mq135Pin = 34;

const char* ssid = "Unnati";
const char* password = "MICKEY0808";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int air = analogRead(mq135Pin);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html; charset=UTF-8");
    client.println();

    client.println("<!DOCTYPE html><html><head>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");

    client.println("<style>");
    client.println("@import url('https://fonts.googleapis.com/css2?family=Poppins:wght@300;600&display=swap');");

    client.println("body{font-family:Poppins;background:#020617;color:white;text-align:center;margin:0;}");

    client.println(".title{font-size:32px;font-weight:600;margin-top:20px;}");

    client.println(".card{background:#0f172a;border:2px solid #38bdf8;border-radius:25px;padding:25px;margin:20px auto;width:340px;box-shadow:0 0 25px rgba(56,189,248,0.6);}");    

    client.println(".value{font-size:40px;font-weight:600;margin-top:10px;color:#38bdf8;}");

    client.println(".label{font-size:20px;color:#cbd5e1;}");

    client.println(".gauge{width:260px;height:130px;background:#1e293b;border-radius:260px 260px 0 0;overflow:hidden;margin:20px auto;position:relative;border:2px solid #38bdf8;}");

    client.println(".needle{width:4px;height:125px;background:#ef4444;position:absolute;bottom:0;left:50%;transform-origin:bottom center;}");

    client.println("button{padding:14px 28px;font-size:18px;border:none;border-radius:15px;background:#38bdf8;color:black;margin:25px;font-weight:600;box-shadow:0 0 15px #38bdf8;}");

    client.println("</style>");

    client.println("<script>");
    client.println("setInterval(()=>location.reload(),3000);");
    client.println("</script>");

    client.println("</head><body>");

    client.println("<div class='title'>AIR QUALITY MONITOR</div>");

    client.println("<div class='card'>");
    client.println("<div class='label'>Temperature</div>");
    client.print("<div class='value'>");
    client.print(t);
    client.println(" C</div></div>");

    client.println("<div class='card'>");
    client.println("<div class='label'>Humidity</div>");
    client.print("<div class='value'>");
    client.print(h);
    client.println(" %</div></div>");

    client.println("<div class='card'>");
    client.println("<div class='label'>Air Quality</div>");

    client.println("<div class='gauge'>");
    client.print("<div class='needle' style='transform: rotate(");
    client.print(air / 12);
    client.println("deg);'></div>");
    client.println("</div>");

    client.print("<div class='value'>");
    client.print(air);
    client.println("</div></div>");

    client.println("<button onclick='location.reload()'>Refresh Data</button>");

    client.println("</body></html>");
    client.println();

    client.stop();
  }
}
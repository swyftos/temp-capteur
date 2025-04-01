#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <SD.h>
#include <RTClib.h>
#include <DHT.h>

// Déclaration des modules
#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

RTC_DS1307 RTC;

// Calibration de l'écran tactile
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

void bouton()
{
  tft.fillScreen(ILI9341_RED);
  const char* labels[] = {"DATE/HEURE", "TEMPERATURE", "HUMIDITE", "PRESSION", "AUTRE..."};
  
  for (int i = 0; i < 5; i++) {
    tft.fillRect(40, 20 + (i * 60), 160, 40, ILI9341_WHITE);
    tft.setCursor(60, 33 + (i * 60));
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setTextWrap(0);
    tft.print(labels[i]);
  }
}

// Affichage de la température en °C (centré)
void afficher_temperature()
{
  float temperature = dht.readTemperature();
  tft.fillScreen(ILI9341_BLACK); // Fond noir

  tft.setTextColor(ILI9341_RED, ILI9341_BLACK); // Texte rouge sur fond noir
  tft.setTextSize(4);

  if (isnan(temperature)) {
    tft.setCursor(50, 100); // Position centrée
    tft.print("Erreur");
  } else {
    // Calcul de la position X pour centrer le texte
    int textWidth = 6 * 4 * String((int)temperature).length(); // Largeur du texte (6 pixels par caractère, taille 4)
    int posX = (240 - textWidth) / 2; // Centrage horizontal

    tft.setCursor(posX, 90);
    tft.print(temperature);

    tft.setTextSize(3); // Taille plus petite pour "°C"
    tft.setCursor(110, 140); // Position bien centrée sous la température
    tft.print("°C");
  }
}

// Affichage de l'humidité en % (centré)
void afficher_humidite()
{
  float humidite = dht.readHumidity();
  tft.fillScreen(ILI9341_BLUE); // Fond bleu

  tft.setTextColor(ILI9341_WHITE, ILI9341_BLUE); // Texte blanc sur fond bleu
  tft.setTextSize(4);

  if (isnan(humidite)) {
    tft.setCursor(50, 100); // Position centrée
    tft.print("Erreur");
  } else {
    // Calcul de la position X pour centrer le texte
    int textWidth = 6 * 4 * String((int)humidite).length();
    int posX = (240 - textWidth) / 2;

    tft.setCursor(posX, 90);
    tft.print(humidite);

    tft.setTextSize(3);
    tft.setCursor(110, 140); // Position bien centrée sous l'humidité
    tft.print("%");
  }
}

void heure_date()
{
  DateTime now = RTC.now();
  tft.fillScreen(ILI9341_RED);
  tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
  tft.setTextSize(3);

  tft.setCursor(50, 100);
  if (now.day() < 10) tft.print("0");
  tft.print(now.day());
  tft.print("/");
  if (now.month() < 10) tft.print("0");
  tft.print(now.month());
  tft.print("/");
  tft.print(now.year());

  tft.setCursor(60, 150);
  if (now.hour() < 10) tft.print("0");
  tft.print(now.hour());
  tft.print(":");
  if (now.minute() < 10) tft.print("0");
  tft.print(now.minute());
  tft.print(":");
  if (now.second() < 10) tft.print("0");
  tft.print(now.second());
}

// Placeholders pour pression et autre...
void afficher_pression() {
  tft.fillScreen(ILI9341_RED);
  tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
  tft.setTextSize(2);
  tft.setCursor(50, 100);
  tft.print("Pression: Donnees non dispo.");
}

void afficher_autre() {
  tft.fillScreen(ILI9341_RED);
  tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
  tft.setTextSize(2);
  tft.setCursor(50, 100);
  tft.print("Autre: Fonction a definir.");
}

void setup()
{
  Serial.begin(9600);
  tft.begin();
  RTC.begin();
  dht.begin();

  if (!ts.begin()) {
    Serial.println("Unable to start touchscreen.");
  } else {
    Serial.println("Touchscreen started.");
  }

  RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); // Mise à jour automatique de l'horloge
  bouton();
}

void loop()
{
  if (!ts.bufferEmpty())
  {
    TS_Point p = ts.getPoint();
    int x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    int y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    if ((x > 40) && (x < 200))
    {
      if ((y > 20) && (y < 60)) heure_date();
      else if ((y > 80) && (y < 120)) afficher_temperature();
      else if ((y > 140) && (y < 180)) afficher_humidite();
      else if ((y > 200) && (y < 240)) afficher_pression();
      else if ((y > 260) && (y < 300)) afficher_autre();

      delay(2000);
      bouton();
    }
  }
}

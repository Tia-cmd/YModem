#include <Arduino.h>
#include "YModem.h"

#define BAUD_RATE     115200
#define BUFFER_SIZE   65536

YModem ymodem(Serial, 5000);
uint8_t* dataBuffer = nullptr;

struct EPROMType {
  const char* name;
  uint32_t size;
  uint8_t id;
};

EPROMType epromTypes[] = {
  {"1 - 2716  (2K x 8)",   2048, 1},
  {"2 - 2732  (4K x 8)",   4096, 2},
  {"3 - 2764  (8K x 8)",   8192, 3},
  {"4 - 27128 (16K x 8)", 16384, 4},
  {"5 - 27256 (32K x 8)", 32768, 5},
  {"6 - 27512 (64K x 8)", 65536, 6},
  {"7 - EPROM Simulator", 65536, 7},
  {nullptr, 0, 0}
};

int currentTypeIndex = 5;     // alapból 27512
bool fastWriteMode = false;

// ====================== PIN DEFINÍCIÓK (Hardver teszt menühöz) ======================
const uint8_t PIN_CE  = 4;    // /CE
const uint8_t PIN_OE  = 5;    // /OE
const uint8_t PIN_WE  = 6;    // /WE vagy PGM

// Vcc és Vpp vezérléshez (később MOSFET-eken keresztül)
const uint8_t PIN_VCC_EN = 7;
const uint8_t PIN_VPP_EN = 8;

// ====================== MENÜK (ugyanaz mint eddig) ======================
void showMainMenu() {
  Serial.println("\n=== ESP32 EPROM Programmer v0.4 ===");
  Serial.printf("Aktuális EPROM: %s\n", epromTypes[currentTypeIndex].name);
  Serial.printf("Írás mód: %s\n\n", fastWriteMode ? "GYORS" : "HAGYOMÁNYOS");

  Serial.println("1 - EPROM típus beállítása");
  Serial.println("2 - Írás mód beállítása");
  Serial.println("3 - EPROM műveletek");
  Serial.println("4 - Hardver teszt / Égető teszt menü");
  Serial.println("0 - Kilépés / Újraindítás");
  Serial.print("\nVálassz: ");
}

void showTestMenu() {
  Serial.println("\n=== HARDVER / ÉGETŐ TESZT MENÜ ===");
  Serial.println("a - Write to address bus");
  Serial.println("d - Write to data bus");
  Serial.println("c - /CE = H (pin 20)");
  Serial.println("o - /OE = H (pin 22)");
  Serial.println("v - Vcc ON");
  Serial.println("e - Vcc -> A13 (pin 26)");
  Serial.println("0 - Vcc and Vpp OFF");
  Serial.println("1 - Vcc = 5V");
  Serial.println("2 - Vcc = 6V");
  Serial.println("3 - Vcc = 6.5V");
  Serial.println("p - Vpp ON");
  Serial.println("4 - Vpp = 12.5V");
  Serial.println("5 - Vpp = 12.75V");
  Serial.println("6 - Vpp = 13V");
  Serial.println("7 - Vpp = 21V");
  Serial.println("8 - Vpp = 25V");
  Serial.println("9 - Vpp -> A11 (pin 23)");
  Serial.println("r - Vpp -> A15 (pin 1)");
  Serial.println("s - Vpp -> /OE (pin 22)");
  Serial.println("l - LED 5,6,7,8 ON");
  Serial.println("q - Vissza a főmenübe");
  Serial.print("\nVálassz: ");
}

// ====================== HARDVER TESZT PARANCSKEZELŐ ======================
void handleTestCommand(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;

  char c = tolower(cmd.charAt(0));

  switch (c) {
    case 'a': Serial.println("→ Address bus write teszt (később implementálva)"); break;
    case 'd': Serial.println("→ Data bus write teszt"); break;
    case 'c': 
      digitalWrite(PIN_CE, HIGH);
      Serial.println("→ /CE = HIGH");
      break;
    case 'o': 
      digitalWrite(PIN_OE, HIGH);
      Serial.println("→ /OE = HIGH");
      break;
    case 'v': 
      digitalWrite(PIN_VCC_EN, HIGH);
      Serial.println("→ Vcc bekapcsolva");
      break;
    case '0':
      digitalWrite(PIN_VCC_EN, LOW);
      digitalWrite(PIN_VPP_EN, LOW);
      Serial.println("→ Vcc és Vpp kikapcsolva");
      break;
    case 'p':
      digitalWrite(PIN_VPP_EN, HIGH);
      Serial.println("→ Vpp bekapcsolva");
      break;
    case 'l':
      Serial.println("→ LED-ek bekapcsolva (placeholder)");
      break;
    case 'q':
      Serial.println("Vissza a főmenübe...");
      return;
    default:
      Serial.println("Ismeretlen teszt parancs.");
  }
}

// ====================== FŐ PARANCSKEZELŐ ======================
void handleInput() {
  if (!Serial.available()) return;

  String input = Serial.readStringUntil('\n');
  input.trim();
  if (input.length() == 0) return;

  int choice = input.toInt();

  if (choice == 1) {
    // EPROM típus választás (maradjon ugyanaz)
    Serial.println("Típus választás menü később finomítva...");
  }
  else if (choice == 2) {
    Serial.println("Írás mód választás...");
    fastWriteMode = !fastWriteMode;   // egyszerű toggle most
    Serial.printf("Írás mód: %s\n", fastWriteMode ? "GYORS" : "HAGYOMÁNYOS");
  }
  else if (choice == 3) {
    Serial.println("EPROM műveletek menü később...");
  }
  else if (choice == 4) {
    showTestMenu();
    while (true) {
      while (!Serial.available()) delay(10);
      String testInput = Serial.readStringUntil('\n');
      if (testInput.length() > 0) {
        handleTestCommand(testInput);
        if (tolower(testInput.charAt(0)) == 'q') break;
      }
    }
  }
  else if (choice == 0) {
    ESP.restart();
  }

  delay(300);
  showMainMenu();
}

void setup() {
  Serial.begin(BAUD_RATE);
  delay(1000);

  // PinMode beállítás
  pinMode(PIN_CE, OUTPUT);
  pinMode(PIN_OE, OUTPUT);
  pinMode(PIN_WE, OUTPUT);
  pinMode(PIN_VCC_EN, OUTPUT);
  pinMode(PIN_VPP_EN, OUTPUT);

  digitalWrite(PIN_CE, HIGH);
  digitalWrite(PIN_OE, HIGH);
  digitalWrite(PIN_WE, HIGH);
  digitalWrite(PIN_VCC_EN, LOW);
  digitalWrite(PIN_VPP_EN, LOW);

  dataBuffer = (uint8_t*)malloc(BUFFER_SIZE);
  if (!dataBuffer) Serial.println("Memória hiba!");

  Serial.println("\nESP32 EPROM Programmer v0.4 indul...");
  showMainMenu();
}

void loop() {
  handleInput();
}

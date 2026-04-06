# YModem
# ESP32 EPROM Programmer

----------------------------------------------------HUN---------------------------------------------------------

Univerzális EPROM programozó ESP32 alapokon, **teljesen terminál-alapú** kezelőfelülettel.  
Nem igényel saját PC-s szoftvert — csak egy egyszerű terminálprogramot (minicom, PuTTY, Tera Term stb.).

## Főbb jellemzők

- Támogatott EPROM típusok: 2716, 2732, 2764, 27128, 27256, 27512 + EPROM Simulator
- Menü alapú kezelőfelület (számokkal és betűkkel)
- YModem protokoll fájlátvitelhez (send & receive)
- Hardver teszt menü (Vcc, Vpp, CE, OE, address/data bus tesztelés)
- Gyors / Hagyományos írás mód támogatása
- Teljesen nyílt forráskódú, saját YModem implementációval

## Hardver

- ESP32 DevKit
- Level shifter (5V ↔ 3.3V)
- ZIF socket (EPROM-okhoz)
- Vcc és Vpp vezérlés (MOSFET-ekkel)

## Használat

1. Csatlakoztasd az ESP32-t USB-n keresztül
2. Nyiss egy terminálprogramot **115200 baud** sebességgel
3. Válaszd a menüpontokat a számok és betűk segítségével

**Főmenü parancsok:**
- `1` → EPROM típus beállítása
- `2` → Írás mód (Gyors/Hagyományos)
- `3` → EPROM műveletek (Dump, Write, Verify...)
- `4` → Hardver teszt / Égető teszt menü
- `0` → Újraindítás

## Fejlesztés alatt

- Teljes EPROM olvasás és írás implementáció
- Valós address és data bus vezérlés
- Több EPROM típus támogatása
- Vpp feszültség pontos szabályozása

## Licenc

MIT License

---

Készítette: [Tia]
---------------------------------------------------ENG-----------------------------------------------------
# ESP32 EPROM Programmer

Universal EPROM programmer based on ESP32 with a **fully terminal-based** user interface.  
No custom PC software required — just a standard terminal program (minicom, PuTTY, Tera Term, etc.).

## Features

- Supported EPROM types: 2716, 2732, 2764, 27128, 27256, 27512 + EPROM Simulator
- Menu-driven interface (numeric and letter commands)
- YModem protocol for reliable file transfer (both send and receive)
- Hardware test menu for low-level debugging (Vcc, Vpp, CE, OE, address/data bus)
- Fast / Normal write mode selection
- Custom lightweight YModem implementation
- Open source and easy to extend

## Hardware Requirements

- ESP32 DevKit (ESP32-WROOM-32)
- Bidirectional level shifter (3.3V ↔ 5V)
- ZIF socket for EPROMs (28/32/40 pin with adapters)
- MOSFETs or drivers for Vcc and Vpp voltage control

## Usage

1. Connect the ESP32 via USB
2. Open a terminal program with **115200 baud**
3. Use the menu to control the programmer

**Main Menu Commands:**
- `1` → Select EPROM type
- `2` → Set write mode (Fast / Normal)
- `3` → EPROM operations (Dump, Write, Verify, Blank Check...)
- `4` → Hardware Test / Burner Test Menu
- `0` → Restart

## Hardware Test Menu (Option 4)

Contains low-level commands:
- `a` - Write to address bus  
- `d` - Write to data bus  
- `c` - /CE = HIGH  
- `o` - /OE = HIGH  
- `v` - Vcc ON  
- `p` - Vpp ON  
- `0` - Vcc & Vpp OFF  
- Voltage selection (5V, 6V, 12.5V, 21V, 25V etc.)

## Status

**Work in progress** — Currently implementing:
- Full EPROM read/write routines
- Proper address and data bus handling
- Vpp voltage switching and control


by Tia
---



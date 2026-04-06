# YModem
# ESP32 EPROM Programmer

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
# YModem

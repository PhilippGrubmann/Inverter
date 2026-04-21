# Projektkontext — Inverter Firmware

Dieses Dokument wird am Anfang jedes neuen Chats eingefügt,
damit der Assistent sofort den vollen Kontext hat.

---

## Projekt

Diplomarbeit: Entwicklung eines Traktionsumrichters (Inverter) für ein Elektro-Kart.
Zwei Personen: Philipp Grubmann (Hardware + Software), Florian Kiesl.
Die Hardware (LV-Board, Gate-Treiber-Board, Strommessplatine, Leistungsmodul) ist fertig.
Jetzt wird die Firmware entwickelt.

## Hardware-Übersicht

| Komponente          | Typ / Wert                                          |
|---------------------|------------------------------------------------------|
| Motor               | Motenergy ME1304, PMSM, 14,5 kW, p = 4              |
| R_ph (Phase-Phase)  | 6,3 mΩ → Phase-Neutral: 3,15 mΩ                    |
| L_ph (Phase-Phase)  | 66 µH → Phase-Neutral: 33 µH                       |
| kM                  | 0,12 Nm/A                                           |
| Bemessungsspannung   | 96 V                                                |
| Bemessungsstrom      | 180 A, Maximalstrom 420 A (30s)                     |
| Leistungsmodul       | Infineon FS05MR12A6MA1B, SiC, 1200 V / 200 A       |
| Zwischenkreis-C      | Panasonic Folie, 660 µF, 450 V                      |
| MCU                  | STM32F446ZET6, LQFP144, 180 MHz, FPU               |
| Quarz (HSE)          | 16 MHz                                              |
| Debugger             | STLINK-V3MODS (onboard, SWD, Micro-USB)             |
| Stromsensoren        | 3× LEM HAIS 150-P (Open-Loop Hall, 5V, 2,5V@0A)   |
| Gate-Treiber         | Isoliert, ±15V / -5V umschaltbar, galv. getrennt   |
| CAN-Transceiver      | TCAN1042V, Split-Termination                        |
| IMU                  | Bosch BMI088, SPI                                    |
| Pedale               | Potentiometer, 15V versorgt, Spannungsteiler → 3,3V |
| LED                  | PD15                                                 |
| Kühlung              | Flüssigkeitskühlung (Erwin Quarder Gehäuse)          |

## Schutzkonzept (Hardware)

- Fensterkomparator (AD8564ARZ) für Überstrom pro Phase (pos + neg)
- Einfache Komparatoren für U_DC-Unterspannung, Modultemp, Motortemp
- Fehler → D-Flip-Flop (SN74LVC2G74) speichert latched
- Alle Fehler über ODER-Gatter zusammengefasst → FLT_SET
- FLT_SET + DSP_GDR_EN → NOR-Gatter (SN74AHC1G02) → GDR_NRST/EN
- Quittierung über FLT_NRST (CLR-Pin am Flip-Flop)
- Gemessene Abschaltzeit: < 200 ns
- Funktioniert komplett softwareunabhängig

## IDE / Toolchain

- STM32CubeIDE
- HAL-Bibliothek (kein LL, kein Bare-Metal)
- Sprache: C

## Mein Erfahrungslevel

- Hardware: Fortgeschritten (habe die gesamte Hardware selbst entwickelt)
- Theorie: Grundlagen verstanden (Clarke, Park, dq-Transformation, SVM-Prinzip)
- Programmierung: Anfänger — ich brauche konkreten, lauffähigen Code
- Ich verstehe Schaltpläne, Datenblätter, Oszilloskop-Messungen

## Wie du mir helfen sollst

1. **Schritt für Schritt.** Immer nur EINEN Schritt, dann testen.
   Nie mehrere Dinge gleichzeitig implementieren.

2. **Immer mit Test.** Jeder Schritt muss eine klare Testanweisung haben:
   Was messe ich? Womit? Was muss rauskommen?
   Erst wenn der Test bestanden ist → nächster Schritt.

3. **Code** Zuerst erklären ich überleg dann und wenn ich nicht mit dem Code weiterweiß hilf mir. 
	Nicht zu viel Zeit verblödeln.

4. **Schonungsloser Mentor.** Wenn meine Idee Mist ist, sag es direkt und warum.
   Prüf alles auf Herz und Nieren. Mach die Lösung wasserdicht.

5. **Fehlerhilfe.** Wenn etwas nicht funktioniert, hilf mir systematisch debuggen.
   Frag gezielt nach: Was zeigt das Terminal? Was zeigt das Oszi?
   Keine Ratespiele — eingrenzen, messen, fixen.

6. **Kein Overengineering.** Erstmal das Minimum das funktioniert.
   CAN, IMU, Telemetrie kommen SPÄTER. Erst muss der Motor drehen.

## Aktueller Fortschritt

(Hier nach jedem abgeschlossenen Schritt updaten!)

```
[x] = erledigt und getestet
[ ] = offen
[~] = in Arbeit

PHASE 1 — Peripherie (ohne Motor)
[x] 1.1 — LED blinken, Clock verifiziert (1 Hz am Oszi)
[x] 1.2 — UART funktioniert (Terminal zeigt Nachrichten)
[ ] 1.3 — ADC Ströme (3× ~2048 bei 0 A)
[ ] 1.4 — ADC U_DC + Pedale (linear, plausibel)
[ ] 1.5 — Encoder (sauberer Winkel beim Handdrehen)
[ ] 1.6 — PWM (20 kHz, Deadtime 200 ns, komplementär)
[ ] 1.7 — Hardware-Schutz (Fehler auslösen + quittieren)
[ ] 1.8 — Offset-Kalibrierung (~0 A bei stromlos)

PHASE 2 — Open Loop (Motor dreht ohne Regelung)
[ ] 2.1 — Open-Loop-Rotation (Motor dreht gleichmäßig)
[ ] 2.2 — Encoder-Offset bestimmt und eingetragen

PHASE 3 — Closed-Loop FOC
[ ] 3.1 — Clarke + Park (i_d, i_q sind Gleichwerte im Open Loop)
[ ] 3.2 — PI i_d (i_d stabil auf 0 A)
[ ] 3.3 — Volle FOC (i_d ≈ 0, i_q = Sollwert)
[ ] 3.4 — Pedal anbinden (Motor reagiert auf Gas)

PHASE 4 — Schutz und Robustheit
[ ] 4.1 — Zustandsmaschine (alle Übergänge korrekt)
[ ] 4.2 — Plausibilitätsprüfung (Sensorfehler → FAULT)
[ ] 4.3 — ISR-basierte FOC (< 30 µs, stabil)
```

## Pin-Zuordnungen (soweit bekannt)

| Funktion       | Pin    | Typ          |
|----------------|--------|--------------|
| LED_STATUS     | PD15   | GPIO_Output  |
| DSP_GDR_EN     | ???    | GPIO_Output  |
| FLT_NRST       | ???    | GPIO_Output  |
| FLT_SET_IU     | ???    | GPIO_Input   |
| FLT_SET_IV     | ???    | GPIO_Input   |
| FLT_SET_IW     | ???    | GPIO_Input   |
| GDR_EN_STATUS  | ???    | GPIO_Input   |
| FILT_I_U (ADC) | PA1    | ADC1_CHx     |
| FILT_I_V (ADC) | PA2    | ADC1_CHx     |
| FILT_I_W (ADC) | PC2    | ADC1_CHx     |
| U_DC (ADC)     | PC3    | ADC2_CHx     |
| GAS_MCU (ADC)  | PC0    | ADC2_CHx     |
| BRAKE_MCU (ADC)| PB1    | ADC2_CHx     |
| Sinus Encoder  | PF6    | TIMx_CH1     |
| Cosinus Encoder| PF7    | TIMx_CH2     |
| TIM1_CH1 (U)   | PE9    | PWM          |
| TIM1_CH1N (U)  | PE8    | PWM_N        |
| TIM1_CH2 (V)   | PE11   | PWM          |
| TIM1_CH2N (V)  | PE10   | PWM_N        |
| TIM1_CH3 (W)   | PE13   | PWM          |
| TIM1_CH3N (W)  | PE12   | PWM_N        |
| UART TX        | PD8    | USART3       |
| UART RX        | PD9    | USART3       |

(Die ??? müssen aus dem Schaltplan eingetragen werden!)

## Wichtige Dateien im Projekt

- `inverter_software_architektur.md` — Gesamtarchitektur, Dateistruktur, ISR-Hierarchie
- `inverter_implementierung_schritt_fuer_schritt.md` — dieses Step-by-Step Dokument
- `motor_params.h` — Motorparameter (R, L, p, ψ_PM)
- `inverter_config.h` — PWM-Frequenz, Deadtime, Schutz-Schwellwerte, PI-Parameter

## Bekannte offene Punkte / Notizen

(Hier Notizen eintragen die im nächsten Chat relevant sind)

- Encoder-Typ und Auflösung (CPR) noch eintragen
- SCALE_FACTOR für Strommessung muss am realen Aufbau kalibriert werden
- Pin-Zuordnung aus Schaltplan vervollständigen
- UART: Prüfen ob STLINK-V3MODS Virtual COM Port unterstützt

---

## So startest du einen neuen Chat

1. Dieses Dokument als erstes einfügen (oder als Datei hochladen)
2. Sagen wo du gerade stehst: "Ich bin bei Schritt X.Y"
3. Beschreiben was passiert: "Die LED blinkt aber mit 0,5 Hz statt 1 Hz"
4. Bei Fehlern: Terminal-Ausgabe und/oder Oszi-Messwerte mitgeben

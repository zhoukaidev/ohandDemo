# Open & Close Fingers One by One

## Materials

1. OHand
2. OHand serial cable
3. Dupont cable(female socket)
4. Battery & cable
5. Base
6. STM32 Blue Pill & pins
7. 3mm LED + 1K resistor + cable

## Prerequisites

1. Remove LED form STM32 Blue Pill
2. Solder the pins of STM32 Blue Pill
3. Burn STM32duino bootloader, see "Burn Bootloader" in [../../doc/STM32duino/README_STM32.md](../../doc/STM32duino/README_STM32.md) as reference

## Pin Connections

| Pin              | STM32 Blue Pill |
| :--------------- | :-------------- |
| LED+             | PB1             |
| LED-             | GND             |
| OHand 3.3V(red)  | 3.3V            |
| OHand GND(black) | GND             |
| OHand TX(yellow) | PB11(RX)        |
| OHand RX(green)  | PB10(TX)        |

## Burn Sketch

See "Arduino Setup" in [../../doc/STM32duino/README_STM32.md](../../doc/STM32duino/README_STM32.md) as reference

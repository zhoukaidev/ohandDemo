# 手指重复打开关闭

## 材料

1. OHand
2. OHand串行电缆
3. 杜邦线（母头）
4. 电池及延长线
5. 底座（含3D打印底座，铝合金面板，铝合金圆管）
6. STM32小板（STM32 Blue Pill）
7. 3mm LED + 1K电阻 + 引线

## 准备工作

1. 移除STM32小板LDO
2. 焊接STM32排针
3. 烧写STM32duino bootloader， 参考[../../doc/STM32duino/README_STM32.md](../../doc/STM32duino/README_STM32.md)的“Burn Bootloader”部分

## 连接

|引脚          |STM32小板引脚|
|:-            |:-          |
|LED+          |PB1         |
|LED-          |GND         |
|OHand 3.3V(红)|3.3V        |
|OHand GND(黑) |GND         |
|OHand TX(黄)  |PB11(RX)    |
|OHand RX(绿)  |PB10(TX)    |

连接完成后，用热熔胶固定STM32小板和电池。

## 烧写sketch

参考[../../doc/STM32duino/README_STM32.md](../../doc/STM32duino/README_STM32.md)的“Arduino Setup”部分。

# ARDUINO UNO R4 - Play with Led Matrix!

## CONNECTION

Connect the Arduino to the computer, one each.

## CONFIGURATION

The board comes with the firmware already flashed, PlayerONE create an Access Point and PlayerTWO connect to it automatically. There is a visual way to debug possible error state:

1. LED_BUILTIN flashing 5 times (1 per second): board is starting
2. LED_BUILTIN flashing 2 times rapidly - PlayerONE: PlayerTWO is not connected
3. LED_BUILTIN flashing 2 times rapidly - PlayerTWO: Trying to connect to PlayerONE
4. LED_TX: flashing 1 times every 10 sec: Game on!

## LIVE DEMO

If the 2 boards are connected with the LED_TX flashing 1 times every 10 sec you can open the Serial monitor and follow the instructions.

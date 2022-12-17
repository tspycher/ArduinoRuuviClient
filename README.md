# ArduinoRuuviClient POC
[ArduinoBLE][1] based Client to read RuuviTag Advertised Data.

Important sidenode; the Arduino Documentation misses some importatant Methods which are crucial to get the communication with the RuuviTag working. Check out the Header files directly to get methods like `int BLEDevice::advertisementData(uint8_t value[], int length) const`

The whole Ruuvi Protocol is described [here][2]

[1]:	https://www.arduino.cc/reference/en/libraries/arduinoble/
[2]:  https://github.com/ruuvi/ruuvi-sensor-protocols/blob/master/dataformat_05.md

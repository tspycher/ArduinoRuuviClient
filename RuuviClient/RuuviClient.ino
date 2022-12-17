/*
  This is an example script in how to interact with a RuuviTag Environmental Sensor
  to read BLE Advertisements.

  Board used: MKR WiFi 1010
*/

#include <ArduinoBLE.h>

float temperature, humidity, pressure;
bool new_data;

void blePeripheralDiscoveredHandler(BLEDevice central) {
  Serial.print("Discovered event from: ");
  Serial.println(central.address());

  if (central.hasAdvertisementData()) {
    uint8_t value[64];
    int l = central.advertisementDataLength();
    central.advertisementData(value, 64);

    int l_payload = (int)value[3];
    int format = (int)value[7];
    int payload_start = 8;


    if (format == 5) { // Data Format 5 Protocol Specification (RAWv2)
      // https://github.com/ruuvi/ruuvi-sensor-protocols/blob/master/dataformat_05.md
      
      temperature = (int)(((byte)value[payload_start] << 8) | ((byte)value[payload_start + 1])) * 0.005;
      humidity = (int)(((byte)value[payload_start+2] << 8) | ((byte)value[payload_start + 3])) * 0.0025;
      pressure = ((int)(((byte)value[payload_start+4] << 8) | ((byte)value[payload_start + 5])) + 50000) / 100;
      new_data = true;
    }
  }
}

void print_data() {
  if(!new_data)
    return;

  new_data = false;
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("C, ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, ");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println("hPa");
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  new_data = false;
  Serial.println("Ruuvi Prototype");
  BLE.setEventHandler(BLEDiscovered, blePeripheralDiscoveredHandler);
    //scanForName("Ruuvi 69E0", true);
  BLE.scanForAddress("f2:40:f1:bc:69:e0");
}

void loop() {
  BLE.poll();
  
  print_data();
}

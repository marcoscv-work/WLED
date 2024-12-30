#include <Arduino.h>
/*
 * WLED Arduino IDE compatibility file.
 * (this is the former wled00.ino)
 * 
 * Where has everything gone?
 * 
 * In April 2020, the project's structure underwent a major change.
 * We now use the platformIO build system, and building WLED in Arduino IDE is not supported any more.
 * Global variables are now found in file "wled.h"
 * Global function declarations are found in "fcn_declare.h"
 * 
 * Usermod compatibility: Existing wled06_usermod.ino mods should continue to work. Delete usermod.cpp.
 * New usermods should use usermod.cpp instead.
 */
#include "wled.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLECharacteristic.h>
#include <BLE2902.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"  // Reemplaza por tu UUID de servicio
#define CHARACTERISTIC_UUID "abcd1234-5678-1234-5678-abcdef012345" // Reemplaza por tu UUID de característica

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pCharacteristic = NULL;

void setup() {
  WLED::instance().setup();

  // Inicializa el dispositivo BLE
  BLEDevice::init("lampara");

  // Crea el servidor BLE
  pServer = BLEDevice::createServer();

  // Crea un servicio BLE
  pService = pServer->createService(SERVICE_UUID);

  // Crea una característica dentro del servicio
  pCharacteristic = pService->createCharacteristic(
                               CHARACTERISTIC_UUID,
                               BLECharacteristic::PROPERTY_READ |
                               BLECharacteristic::PROPERTY_WRITE
                             );

  // Configura la característica con opciones adicionales (si lo deseas)
  pCharacteristic->addDescriptor(new BLE2902());

  // Inicia el servicio
  pService->start();

  // Comienza a hacer publicidad del servicio
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  WLED::instance().loop();
}

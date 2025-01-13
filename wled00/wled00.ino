#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
/*
 * WLED Arduino IDE compatibility file.
 * 
 * Where has everything gone?
 * 
 * In April 2020, the project's structure underwent a major change. 
 * Global variables are now found in file "wled.h"
 * Global function declarations are found in "fcn_declare.h"
 * 
 * Usermod compatibility: Existing wled06_usermod.ino mods should continue to work. Delete usermod.cpp.
 * New usermods should use usermod.cpp instead.
 */
#include "wled.h"

// Nombre del servidor BLE
const char* bleServerName = "lampara";

// Objeto para el servidor BLE
BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    deviceConnected = true;
    Serial.println("Dispositivo conectado");
  }

  void onDisconnect(BLEServer* pServer) override {
    deviceConnected = false;
    Serial.println("Dispositivo desconectado");
    // Reinicia la publicidad para aceptar nuevas conexiones
    pServer->startAdvertising();
  }
};

void setup() {
  WLED::instance().setup();

  // Inicializa la comunicación serie
  Serial.begin(9600);
  Serial.println("Iniciando...");

  // Inicializa el dispositivo BLE
  BLEDevice::init(bleServerName);

  // Crea el servidor BLE
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Crea un servicio BLE (puedes definir tu propio UUID)
  BLEService* pService = pServer->createService(BLEUUID((uint16_t)0x181A));

  // Crea una característica BLE (puedes definir tu propio UUID)
  pCharacteristic = pService->createCharacteristic(
                      BLEUUID((uint16_t)0x2A6E),
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  // Inicia el servicio
  pService->start();

  // TESTEAR: ***** Habilita el uso de Bluetooth 5 y establece el modo de largo alcance (LE Coded PHY)
  // Obtén el objeto de publicidad BLE
  BLEAdvertising* pAdvertising = pServer->getAdvertising();
  // Configura el PHY para usar LE Coded PHY (Bluetooth 5, largo alcance)
  pAdvertising->setAdvertisementType(ADV_TYPE_IND);  // Publicidad de tipo indicado
  pAdvertising->setScanResponse(false);  // No respuesta de escaneo

  // Inicia la publicidad del servidor BLE
  pAdvertising->start();
  Serial.print("Servidor BLE '");
  Serial.print(bleServerName);
  Serial.println("' iniciado y en espera de conexiones.");
}

void loop() {
  WLED::instance().loop();
}

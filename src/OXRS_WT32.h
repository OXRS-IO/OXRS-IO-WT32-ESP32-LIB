/*
 * OXRS_WT32.h
 */

#ifndef OXRS_WT32_H
#define OXRS_WT32_H

#include <OXRS_MQTT.h>    // For MQTT pub/sub
#include <OXRS_API.h>     // For REST API

// Ethernet
#define DHCP_TIMEOUT_MS           15000
#define DHCP_RESPONSE_TIMEOUT_MS  4000

// REST API
#define REST_API_PORT             80

// Screen dimensions
#define WT32_SCREEN_WIDTH         320
#define WT32_SCREEN_HEIGHT        480

// Enum for the different connection states
enum connectionState_t { CONNECTED_NONE, CONNECTED_IP, CONNECTED_MQTT };

class OXRS_WT32 : public Print
{
public:
  // These are only needed if performing manual configuration in your sketch, otherwise
  // config is provisioned via the API and bootstrap page
  void setMqttBroker(const char *broker, uint16_t port);
  void setMqttClientId(const char *clientId);
  void setMqttAuth(const char *username, const char *password);
  void setMqttTopicPrefix(const char *prefix);
  void setMqttTopicSuffix(const char *suffix);

  void begin(jsonCallback config, jsonCallback command);
  void loop(void);

  // Firmware can define the config/commands it supports - for device discovery and adoption
  void setConfigSchema(JsonVariant json);
  void setCommandSchema(JsonVariant json);

  // Helpers for registering custom REST API endpoints
  void apiGet(const char * path, Router::Middleware * middleware);
  void apiPost(const char * path, Router::Middleware * middleware);

  // Helpers for publishing to stat/ and tele/ topics
  boolean publishStatus(JsonVariant json);
  boolean publishTelemetry(JsonVariant json);

  // Helpers for retrieving the connection status and properties
  connectionState_t getConnectionState(void);
  void getIPAddressTxt(char *buffer);
  void getMACAddressTxt(char *buffer);
  void getMQTTTopicTxt(char *buffer);

  // Implement Print.h wrapper
  virtual size_t write(uint8_t);
  using Print::write;

private:
  void _initialiseNetwork(byte *mac);
  void _initialiseMqtt(byte *mac);
  void _initialiseRestApi(void);

  boolean _isNetworkConnected(void);
};

#endif

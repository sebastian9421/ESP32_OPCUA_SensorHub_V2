#include <WiFi.h>
#include <WebServer.h>
#include <Arduino_OPC_UA.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Preferences.h>

#define TMP36_PIN 34  // TMP36 analog pin

// WiFi configuration
Preferences preferences;
String ssid = "";
String password = "";

// WebServer instance
WebServer server(80);

// MPU6050 sensor instance
Adafruit_MPU6050 mpu;

// OPC UA server instance
UA_Server* serverInstance;

// HTML page for WiFi configuration
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head><title>WiFi Configuration</title></head>
<body>
  <h1>WiFi Configuration</h1>
  <form action="/connect" method="POST">
    <label for="ssid">SSID:</label><br>
    <input type="text" id="ssid" name="ssid"><br><br>
    <label for="password">Password:</label><br>
    <input type="password" id="password" name="password"><br><br>
    <input type="submit" value="Connect">
  </form>
</body>
</html>
)rawliteral";

// Read TMP36 temperature sensor
float readTemperature() {
    int analogValue = analogRead(TMP36_PIN);
    float voltage = analogValue * (3.3 / 4095.0);  // Scale ADC value to voltage
    return (voltage - 0.5) * 100.0;  // TMP36 conversion formula
}

// Handle WiFi root page
void handleRoot() {
    server.send(200, "text/html", htmlPage);
}

// Handle WiFi connect request
void handleConnect() {
    ssid = server.arg("ssid");
    password = server.arg("password");
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    server.send(200, "text/html", "<h1>WiFi configured. Restarting...</h1>");
    delay(2000);
    ESP.restart();
}

// WiFi connection logic
void connectToWiFi() {
    ssid = preferences.getString("ssid", "");
    password = preferences.getString("password", "");

    if (ssid == "" || password == "") {
        WiFi.softAP("ESP32_Config");
        Serial.println("Started Access Point: ESP32_Config");
        Serial.print("AP IP Address: ");
        Serial.println(WiFi.softAPIP());

        server.on("/", handleRoot);
        server.on("/connect", HTTP_POST, handleConnect);
        server.begin();

        while (true) {
            server.handleClient();
        }
    } else {
        WiFi.begin(ssid.c_str(), password.c_str());
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Connecting to WiFi...");
        }
        Serial.println("WiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }
}

void setup() {
    Serial.begin(115200);

    preferences.begin("wifi-config", false);
    connectToWiFi();

    if (!mpu.begin()) {
        Serial.println("MPU6050 not found! Check your wiring.");
        while (1);
    }
    Serial.println("MPU6050 initialized.");

    serverInstance = UA_Server_new();
    UA_ServerConfig* config = UA_Server_getConfig(serverInstance);
    UA_ServerConfig_setMinimal(config, 4840, NULL);

    // Temperature Node
    UA_VariableAttributes tempAttr = UA_VariableAttributes_default;
    float tempValue = 0.0f;
    tempAttr.value.type = &UA_TYPES[UA_TYPES_FLOAT];
    tempAttr.value.storageType = UA_VARIANT_DATA_NODELETE;
    tempAttr.value.data = &tempValue;
    tempAttr.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"Temperature");
    tempAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"Temperature");
    UA_NodeId tempNodeId = UA_NODEID_STRING(1, (char*)"Temperature");
    UA_NodeId tempTypeDefinition = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);
    UA_Server_addVariableNode(serverInstance, tempNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, (char*)"Temperature"),
                              tempTypeDefinition, tempAttr, NULL, NULL);

    // Acceleration Nodes (Separate X, Y, Z)
    float accelX = 0.0f, accelY = 0.0f, accelZ = 0.0f;

    UA_VariableAttributes accelXAttr = UA_VariableAttributes_default;
    accelXAttr.value.type = &UA_TYPES[UA_TYPES_FLOAT];
    accelXAttr.value.storageType = UA_VARIANT_DATA_NODELETE;
    accelXAttr.value.data = &accelX;
    accelXAttr.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"Acceleration X");
    accelXAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"Acceleration X");
    UA_NodeId accelXNodeId = UA_NODEID_STRING(1, (char*)"AccelerationX");
    UA_Server_addVariableNode(serverInstance, accelXNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, (char*)"AccelerationX"),
                              tempTypeDefinition, accelXAttr, NULL, NULL);

    UA_VariableAttributes accelYAttr = UA_VariableAttributes_default;
    accelYAttr.value.type = &UA_TYPES[UA_TYPES_FLOAT];
    accelYAttr.value.storageType = UA_VARIANT_DATA_NODELETE;
    accelYAttr.value.data = &accelY;
    accelYAttr.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"Acceleration Y");
    accelYAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"Acceleration Y");
    UA_NodeId accelYNodeId = UA_NODEID_STRING(1, (char*)"AccelerationY");
    UA_Server_addVariableNode(serverInstance, accelYNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, (char*)"AccelerationY"),
                              tempTypeDefinition, accelYAttr, NULL, NULL);

    UA_VariableAttributes accelZAttr = UA_VariableAttributes_default;
    accelZAttr.value.type = &UA_TYPES[UA_TYPES_FLOAT];
    accelZAttr.value.storageType = UA_VARIANT_DATA_NODELETE;
    accelZAttr.value.data = &accelZ;
    accelZAttr.description = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"Acceleration Z");
    accelZAttr.displayName = UA_LOCALIZEDTEXT((char*)"en-US", (char*)"Acceleration Z");
    UA_NodeId accelZNodeId = UA_NODEID_STRING(1, (char*)"AccelerationZ");
    UA_Server_addVariableNode(serverInstance, accelZNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_QUALIFIEDNAME(1, (char*)"AccelerationZ"),
                              tempTypeDefinition, accelZAttr, NULL, NULL);

    Serial.println("OPC UA Server initialized.");
}

void loop() {
    float temperature = readTemperature();

    // Create event structures for MPU6050
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    float accelX = accel.acceleration.x;
    float accelY = accel.acceleration.y;
    float accelZ = accel.acceleration.z;

    // Update Temperature
    UA_Variant tempValue;
    UA_Variant_setScalar(&tempValue, &temperature, &UA_TYPES[UA_TYPES_FLOAT]);
    UA_Server_writeValue(serverInstance, UA_NODEID_STRING(1, "Temperature"), tempValue);

    // Update Acceleration X
    UA_Variant accelXValue;
    UA_Variant_setScalar(&accelXValue, &accelX, &UA_TYPES[UA_TYPES_FLOAT]);
    UA_Server_writeValue(serverInstance, UA_NODEID_STRING(1, "AccelerationX"), accelXValue);

    // Update Acceleration Y
    UA_Variant accelYValue;
    UA_Variant_setScalar(&accelYValue, &accelY, &UA_TYPES[UA_TYPES_FLOAT]);
    UA_Server_writeValue(serverInstance, UA_NODEID_STRING(1, "AccelerationY"), accelYValue);

    // Update Acceleration Z
    UA_Variant accelZValue;
    UA_Variant_setScalar(&accelZValue, &accelZ, &UA_TYPES[UA_TYPES_FLOAT]);
    UA_Server_writeValue(serverInstance, UA_NODEID_STRING(1, "AccelerationZ"), accelZValue);

    UA_Server_run_iterate(serverInstance, true);

    if (WiFi.getMode() == WIFI_AP) {
        server.handleClient();
    }

    Serial.printf("Temperature: %.2f°C, AccelerationX: %.2f, AccelerationY: %.2f, AccelerationZ: %.2f\n",
                  temperature, accelX, accelY, accelZ);

    delay(1000);
}

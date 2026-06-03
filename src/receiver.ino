#include <WiFi.h>
#include <WiFiUdp.h>

// --- Network Configuration ---
const char* ssid = "YOUR_SSID_HERE";
const char* password = "YOUR_PASSWORD_HERE";

// --- Hardware Configuration ---
WiFiUDP udp;
char packetBuffer[255];
const int dacPin = 25; // GPIO 25 is DAC1 on ESP32
const int udpPort = 4210;

// Variables for Telemetry
int currentX = 0; 

void setup() {
  Serial.begin(115200);
  
  // Initialize Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\n[CONNECTED]");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  
  // Start UDP listener
  udp.begin(udpPort);
}

void loop() {
  // Check for incoming UDP packets from the Python Bridge
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
    
    String msg = String(packetBuffer);
    
    // Parse the X-vector data (Format: "X:value")
    if (msg.startsWith("X:")) {
      currentX = msg.substring(2).toInt(); // Expected range: -127 to 127
    }
  }

  // --- Op-Amp Control Logic ---
  // Map joystick (-127 to 127) to DAC (0 to 255)
  // Neutral (0) maps to 128 (~1.65V) to establish the Virtual Ground bias
  int dacValue = map(currentX, -127, 127, 0, 255);
  dacWrite(dacPin, dacValue);

  // --- Telemetry Math ---
  float currentVoltage = (dacValue / 255.0) * 3.3;
  
  // Current (mA) calculation based on 500-ohm feedback resistor
  // Formula: I = (V_out - V_ref) / R
  float current_mA = ((currentVoltage - 1.65) / 500.0) * 1000.0;

  // Real-time Serial Output for Debugging/Documentation
  Serial.print("Vector: "); Serial.print(currentX);
  Serial.print(" | DAC: "); Serial.print(dacValue);
  Serial.print(" | Voltage: "); Serial.print(currentVoltage, 2);
  Serial.print("V | Current: "); Serial.print(current_mA, 2);
  Serial.println(" mA");

  delay(10); // Maintain stability for the Serial buffer
}

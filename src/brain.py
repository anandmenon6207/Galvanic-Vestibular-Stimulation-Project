from flask import Flask, request
from flask_cors import CORS
import socket

app = Flask(__name__)
CORS(app) # Enable Cross-Origin Resource Sharing for web controller access

# --- Network Configuration ---
ESP32_IP = "192.168.X.X" # Replace with your ESP32's Local IP
UDP_PORT = 4210

# Initialize UDP Socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print("🚀")
print(f"Streaming telemetry to target: {ESP32_IP}:{UDP_PORT}")

@app.route('/move', methods=['POST'])
def move():
    try:
        # Accept JSON payload regardless of browser Content-Type headers
        data = request.get_json(force=True)
        x_vector = data.get('x')

        if x_vector is not None:
            # Format and transmit the UDP packet to the ESP32
            message = f"X:{x_vector}".encode()
            sock.sendto(message, (ESP32_IP, UDP_PORT))
            
            # Local logging for data verification
            print(f"[TRANSMITTING] Vector: {x_vector}")

        return "Packet Sent", 200
    except Exception as e:
        print(f"[ERROR] {e}")
        return str(e), 500

if __name__ == '__main__':
    # Run on 0.0.0.0 to allow connections from other devices on the network
    app.run(host='0.0.0.0', port=5000)

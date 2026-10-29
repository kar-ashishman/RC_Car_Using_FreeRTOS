import socket
import time
# ESP32 default IP and Application selected PORT
DESTINATION_IP = "172.16.42.123"
DESTINATION_PORT = 5000
MESSAGE = b"\x69"

# Sending 10 UDP messages at 100ms gap

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as udp_socket:
	for _ in range(10):
		print("sending")
		udp_socket.sendto(MESSAGE, (DESTINATION_IP, DESTINATION_PORT))
		time.sleep(0.3)

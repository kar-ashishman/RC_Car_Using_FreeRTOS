import socket
import time
# ESP32 default IP and Application selected PORT
DESTINATION_IP = "192.168.1.10"
DESTINATION_PORT = 5000
MESSAGE = b"\x10"

# Sending 10 UDP messages at 100ms gap

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as udp_socket:
	udp_socket.bind(("192.168.1.7", DESTINATION_PORT))
	for _ in range(1):
		print("sending")
		udp_socket.sendto(MESSAGE, (DESTINATION_IP, DESTINATION_PORT))
		data, addr = udp_socket.recvfrom(1024)
		print(data)
		time.sleep(0.3)


"""
UDP_IP = "127.0.0.1" 
UDP_PORT = 5005

# 1. Create a UDP socket (AF_INET = IPv4, SOCK_DGRAM = UDP)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# 2. Bind the socket to the target IP and port
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening for UDP messages on {UDP_IP}:{UDP_PORT}...")

# 3. Receive the packet (1024 is the buffer size in bytes)
data, addr = sock.recvfrom(1024)
"""
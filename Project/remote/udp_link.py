"""Small non-blocking UDP sender/receiver used by the remote."""
from __future__ import annotations

import socket

from config_loader import UdpSettings


class LinkError(Exception):
    pass


class UdpLink:
    def __init__(self):
        self._sock: socket.socket | None = None
        self._target: tuple[str, int] | None = None
        self.description = ""

    @property
    def is_open(self) -> bool:
        return self._sock is not None

    def open(self, host: str, port: int, settings: UdpSettings):
        self.close()
        host = host.strip()
        if not host:
            raise LinkError("No target IP/host given")
        if not 1 <= port <= 65535:
            raise LinkError(f"Invalid target port {port}")
        try:
            infos = socket.getaddrinfo(host, port, socket.AF_INET, socket.SOCK_DGRAM)
        except socket.gaierror as e:
            raise LinkError(f"Cannot resolve {host!r}: {e}") from None
        target = infos[0][4][:2]

        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        try:
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
            if hasattr(socket, "SIO_UDP_CONNRESET"):
                # Windows otherwise reports ICMP "port unreachable" as an error on the next recv
                sock.ioctl(socket.SIO_UDP_CONNRESET, False)
            sock.bind(("", settings.local_port))
            sock.setblocking(False)
        except OSError as e:
            sock.close()
            raise LinkError(f"Cannot bind local UDP port {settings.local_port}: {e}") from None
        self._sock, self._target = sock, target
        local = sock.getsockname()[1]
        self.description = f"{target[0]}:{target[1]} (listening on UDP {local})"

    def close(self):
        if self._sock is not None:
            try:
                self._sock.close()
            except OSError:
                pass
        self._sock = None
        self._target = None

    def write(self, data: bytes):
        if self._sock is None:
            raise LinkError("Link is not open")
        try:
            self._sock.sendto(data, self._target)
        except BlockingIOError:
            raise LinkError("Send buffer full, packet dropped") from None
        except OSError as e:
            raise LinkError(str(e)) from None

    def read_available(self) -> list[tuple[bytes, tuple[str, int]]]:
        packets = []
        while self._sock is not None:
            try:
                data, addr = self._sock.recvfrom(65535)
            except (BlockingIOError, ConnectionResetError):
                break
            packets.append((data, addr[:2]))
        return packets

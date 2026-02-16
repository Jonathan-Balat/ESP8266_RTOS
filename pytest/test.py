import pytest
import socket
import enum
import common.byte_conversion as bc

START_BYTE = 0xBD


class Command(enum.Enum):
    SUM = 0x00
    BLINK = 0x01


def setup():
    # Setup code to initialize connection to the device, if necessary
    sock = socket.create_connection(("192.168.1.10", 3333), timeout=5)
    _ = sock.recv(64)  # NOTE: Captures welcome message.
    return sock

def teardown(socket_instance):
    socket_instance.close()

def send_bytes(socket_instance, data_bytes):
    socket_instance.sendall(data_bytes)
    return socket_instance.recv(1024)

def test_command_sum():

    socket_instance = setup()

    # Data sample
    data = [1, 2, 3, 4, 5]
    cmd_frame = [START_BYTE, Command.SUM.value, len(data)] + data

    # Convert to byte array
    cmd_frame_bytes = bc.to_bytearray(cmd_frame)

    # Send to device and receive response
    ret_val = send_bytes(socket_instance, cmd_frame_bytes)
    data_sum = bc.from_bytearray(ret_val[3:], 4)[0]

    assert data_sum == sum(data)
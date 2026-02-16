

def to_bytearray(data, data_width=1):
    """
    data_width: Number of bytes per data element (e.g., 1 for byte, 2 for half-word, 4 for word)
    """
    data_frame = bytearray(len(data * data_width))
    index = 0
    for byte in data:
        for byte_index in range(data_width):
            data_frame[index + byte_index] = byte & (0xFF << (8 * byte_index))
        index += data_width

    return data_frame

def from_bytearray(data_frame, data_width=1):
    data = []
    for i in range(0, len(data_frame), data_width):
        value = 0
        for byte_index in range(data_width):
            value |= data_frame[i + byte_index] << (8 * byte_index)
        data.append(value)

    return data
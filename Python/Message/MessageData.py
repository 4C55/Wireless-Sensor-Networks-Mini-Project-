class MessageDataBuilder:
    def __init__(
            self):
        self.data = []

    def add_uint32_t(self, value):
        self.data.append((value >> 0) & 0xff)
        self.data.append((value >> 8) & 0xff)
        self.data.append((value >> 16) & 0xff)
        self.data.append((value >> 24) & 0xff)

    def add_uint8_t(self, value):
        self.data.append((value >> 0) & 0xff)

    def get_data(self):
        return self.data


class MessageDataReader:
    def __init__(
            self,
            data):
        self.data = data
        self.current_index = 0

    def get_uint32_t(self):
        result = 0
        result = result | (self.data[self.current_index] << 0)
        self.current_index = self.current_index + 1
        result = result | (self.data[self.current_index] << 8)
        self.current_index = self.current_index + 1
        result = result | (self.data[self.current_index] << 16)
        self.current_index = self.current_index + 1
        result = result | (self.data[self.current_index] << 24)
        self.current_index = self.current_index + 1
        return result

    def get_uint8_t(self):
        self.current_index = self.current_index + 1
        return self.data[self.current_index - 1]


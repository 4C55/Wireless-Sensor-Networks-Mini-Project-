def reconstruct(data):
    output = []
    index = 0

    while index < len(data):
        if data[index] != 0xff:
            output.append(data[index])
            continue
        value = data[index + 1]
        repeat = data[index + 2]
        for i in range(repeat):
            output.append(value)
        index = index + 3
def reconstruct(data, target_columns):
    output = [0] * (len(data) * 4)
    source_columns = target_columns / 2

    for i in range(len(data)):
        row = i // source_columns
        column = i - row * source_columns

        index_1 = int((row * target_columns * 2) + (column * 2))
        index_2 = index_1 + 1
        index_3 = index_1 + target_columns
        index_4 = index_3 + 1

        output[index_1] = data[i]
        output[index_2] = data[i]
        output[index_3] = data[i]
        output[index_4] = data[i]
    return output
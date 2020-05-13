
/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "compression.h"

/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/

/*****************************************************************************/
/* PRIVATE ENUMERATIONS                                                      */
/*****************************************************************************/

/*****************************************************************************/
/* PRIVATE STRUCTS                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* PRIVATE FUNCTION DECLARATION                                              */
/*****************************************************************************/

/*****************************************************************************/
/* PRIVATE VARIABLES                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* PRIVATE FUNCTIONS                                                         */
/*****************************************************************************/

/*****************************************************************************/
/* PUBLIC FUNCTIONS                                                          */
/*****************************************************************************/

uint32_t compression_runlength_lossy(
    uint8_t *buffer,
    const uint32_t count,
    uint8_t margin)
{
    uint8_t i;
    uint32_t parse_index;
    uint32_t write_index = 0;
    uint8_t length = 1;
    uint8_t last_value = buffer[0];

    for (parse_index = 1; parse_index <= count; parse_index++) {

        bool_t match = false;

        if (last_value > buffer[parse_index]) {
            match = (last_value - buffer[parse_index]) <= margin;
        } else {
            match = (buffer[parse_index] - last_value) <= margin;
        }

        if (match && length < 254 && parse_index < count) {
            length = length + 1;
            continue;
        }

        if (length > 3) {
            buffer[write_index] = 0xff;
            write_index = write_index + 1;
            buffer[write_index] = last_value;
            write_index = write_index + 1;
            buffer[write_index] = length;
            write_index = write_index + 1;
        }
        else {
            for (i = 0; i < length; i++) {
                buffer[write_index] = last_value;
                write_index = write_index + 1;
            }
        }

        length = 1;
        last_value = buffer[parse_index];
    }

    return write_index;
}

uint32_t compression_runlength(uint8_t *buffer, const uint32_t count)
{
    return compression_runlength_lossy(
        buffer,
        count,
        0);
}

uint32_t compression_scale(uint8_t *buffer, const uint32_t length, const uint32_t columns)
{
    uint32_t final_length = length / 4;
    uint32_t final_columns = columns / 2;
    uint32_t i;

    for (i = 0; i < final_length; i++) {
        /*Find compressed pixel image row and column*/
        uint32_t row = i / final_columns;
        uint32_t column = i - row * final_columns;

        /*Find first source pixel of the compressed pixel*/
        uint32_t index_1 = (row * columns * 2) + (column * 2);
        uint32_t index_2 = index_1 + 1;
        uint32_t index_3 = index_1 + columns;
        uint32_t index_4 = index_3 + 1;

        uint32_t value = buffer[index_1];
        value = value + buffer[index_2];
        value = value + buffer[index_3];
        value = value + buffer[index_4];
        value = value / 4;
        buffer[i] = value;
    }

    return final_length;
}
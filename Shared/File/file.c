
/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "file.h"

/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/

#define FILENAME "data"
#define MAX_NUMBER_OF_ATTEMPTS 3

#define LOG_MODULE "File"
#define LOG_LEVEL LOG_CONF_LEVEL_FILE

#define FURTHEST_SEEK 30000

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

static int file_id;
static uint32_t current_address;

/*****************************************************************************/
/* PRIVATE FUNCTIONS                                                         */
/*****************************************************************************/

static int attempt_to_seek(int direction) {
    LOG_DBG("Seeking %d from current position %ld.\n", direction, current_address);
    int result = cfs_seek(file_id, direction, CFS_SEEK_CUR);

    if (result == -1) {
        LOG_DBG("Possible seek error\n");
    }

    return result;
}

static void seek_backwards(const uint32_t address) {
    while (current_address > address) {
        if (current_address - address > FURTHEST_SEEK) {
            (void)attempt_to_seek(-FURTHEST_SEEK);
            current_address = current_address - FURTHEST_SEEK;
        } else {
            uint32_t remaining = current_address - address;
            (void)attempt_to_seek(-remaining);
            current_address = current_address - remaining;
        }
    }
}

static void seek_forward(const uint32_t address) {
    while (current_address < address) {
        if (address - current_address > FURTHEST_SEEK) {
            (void)attempt_to_seek(FURTHEST_SEEK);
            current_address = current_address + FURTHEST_SEEK;
        } else {
            uint32_t remaining = address - current_address;
            (void)attempt_to_seek(remaining);
            current_address = current_address + remaining;
        }
    }
}

static void seek_to(const uint32_t address)
{
    LOG_DBG("Seeking to address %ld.\n", address);

    if (address > current_address) {
        seek_forward(address);
    } else if (address < current_address) {
        seek_backwards(address);
    } else {
        LOG_DBG("Seeking - already at the address.\n");
    }
}

static int attempt_to_read(uint8_t *buffer, uint16_t length)
{
    uint8_t i = 0;
    int read_result = 0;

    for (i = 0; i < MAX_NUMBER_OF_ATTEMPTS; i++) {
        read_result = cfs_read(file_id, buffer, length);

        if (read_result > 0) {
            current_address = current_address + (uint32_t)read_result;
            return read_result;
        } else {
            LOG_DBG("Error when reading.\n");
        }
    }

    return read_result;
}

static int attempt_to_write(uint8_t *buffer, uint16_t length)
{
    uint8_t i = 0;
    int write_result = 0;

    for (i = 0; i < MAX_NUMBER_OF_ATTEMPTS; i++) {
        write_result = cfs_write(file_id, buffer, length);

        if (write_result > 0) {
            current_address = current_address + (uint32_t)write_result;
            return write_result;
        } else {
            LOG_DBG("Error when writing.\n");
        }
    }

    return write_result;
}

static bool_t can_work_on_file(void)
{
    if (file_id < 0) {
        LOG_ERR("Failed to open\n");
        return false;
    } else {
        return true;
    }
}

/*****************************************************************************/
/* PUBLIC FUNCTIONS                                                          */
/*****************************************************************************/

bool_t file_init(void)
{
    file_id = cfs_open(FILENAME, CFS_READ | CFS_WRITE);
    current_address = 0;

    if (file_id >= 0) {
        LOG_DBG("Successfully opened\n");
        return true;
    } else {
        LOG_ERR("Failed to open\n");
        return false;
    }
}

bool_t file_write(const uint32_t address, uint8_t *buffer, uint16_t length)
{
    int write_result;
    uint16_t bytes_written = 0;

    if (!can_work_on_file()) {
        return false;
    }

    LOG_DBG("Attempting to write %d bytes starting at %ld\n", length, address);

    seek_to(address);

    while (bytes_written < length) {
        write_result = attempt_to_write(buffer + bytes_written, length - bytes_written);

        /*Failed to read*/
        if (write_result < 0) {
            LOG_DBG("Failed to write\n");
            return false;
        }

        bytes_written = bytes_written + (uint16_t)write_result;
    }

    LOG_DBG("Successfully completed wrting\n");

    return true;
}

bool_t file_read(const uint32_t address, uint8_t *buffer, uint16_t length)
{
    int read_result;
    uint16_t bytes_read = 0;

    if (!can_work_on_file()) {
        return false;
    }

    LOG_DBG("Attempting to read %d bytes starting from %ld\n", length, address);

    seek_to(address);

    while (bytes_read < length) {
        read_result = attempt_to_read(buffer + bytes_read, length - bytes_read);

        /*Failed to read*/
        if (read_result < 0) {
            LOG_DBG("Failed to read\n");
            return false;
        }

        bytes_read = bytes_read + (uint16_t)read_result;
    }

    LOG_DBG("Successfully completed reading\n");

    return true;
}

bool_t file_format_memory(void)
{
    LOG_DBG("Starting to format the memory\n");
    cfs_coffee_format();
    LOG_DBG("Finished formatting the memory\n");

    return file_init();
}

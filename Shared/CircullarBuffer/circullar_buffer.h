#ifndef INC_BUFFER_BUFFER_H_
#define INC_BUFFER_BUFFER_H_

//------------------------------------------------------INCLUDES------------------------------------------------------//

#include "types.h"

//-------------------------------------------------------STRUCTS------------------------------------------------------//

struct circullar_buffer {
	uint8_t *buffer;
	uint8_t buffer_size;
	uint8_t element_size;
	uint8_t head;
	uint8_t tail;
	uint8_t count;
};

//---------------------------------------------------PUBLIC FUNCTIONS-------------------------------------------------//

void circullar_buffer_init(
	struct circullar_buffer *buffer,
    uint8_t *data_buffer,
	const uint8_t data_buffer_size,
	const uint8_t item_size);

/**
 * @brief Places the byte into the buffer.
 * @param buffer The buffer to place the byte into.
 * @param item The item to place into the buffer.
 * @return True if the byte was placed in the buffer, false otherwise.
 */
bool_t circullar_buffer_put(
	struct circullar_buffer *buffer,
	const void *item);

/**
 * @brief Gets the next available byte from the buffer.
 * @param buffer The buffer to get the byte from.
 * @param item Pointer to put the item into.
 * @return True if the next available byte from the buffer was placed in the pointer, false otherwise.
 */
bool_t circullar_buffer_get(
	struct circullar_buffer *buffer,
	void *item);

/**
 * @brief Gets the number of elements in the buffer.
 * @param buffer The buffer to get the count of.
 * @return The number of elements in the buffer.
 */
uint8_t circullar_buffer_get_count(const struct circullar_buffer *buffer);

/**
 * @brief Checks if the buffer has space for the specified number of bytes.
 * @param buffer The buffer to check if it has enough space.
 * @param number The number of bytes to check if they can fit in the buffer.
 * @return True if the specified number of bytes can fit in the buffer, false otherwise.
 */
bool_t circullar_buffer_has_space_for(const struct circullar_buffer *buffer, const uint8_t number);

/**
 * @brief Checks if the buffer has any elements in it.
 * @param buffer The buffer to check.
 * @return True if the buffer has any elements, false otherwise.
 */
bool_t circullar_buffer_has_any(const struct circullar_buffer *buffer);

#endif /* INC_BUFFER_BUFFER_H_ */

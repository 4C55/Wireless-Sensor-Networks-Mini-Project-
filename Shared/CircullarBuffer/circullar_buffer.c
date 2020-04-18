//------------------------------------------------------INCLUDES------------------------------------------------------//

#include "circullar_buffer.h"

//----------------------------------------------PRIVATE FUNCTION DECLERATIONS-----------------------------------------//

/**
 * @brief Increments the index.
 * Increments the provided index by one and wraps it around when the maximum limit is reached.
 * @param index Index to increment.
 * @param max The maximum limit.
 */
static void increment_index(uint8_t *index, const uint8_t max);

//---------------------------------------------------PRIVATE FUNCTIONS------------------------------------------------//

static void increment_index(uint8_t *index, const uint8_t max)
{
	*index = (*index + 1) % max;
}

void copy_memory(void *destination, const void *source, uint16_t num)
{
    uint16_t i = 0;
    for (i = 0; i < num; i++) {
        ((uint8_t *)destination)[i] = ((uint8_t*)source)[i];
    }
}

//---------------------------------------------------PUBLIC FUNCTIONS-------------------------------------------------//

void circullar_buffer_init(
	struct circullar_buffer *buffer,
    uint8_t *data_buffer,
	const uint8_t data_buffer_size,
	const uint8_t item_size)
{
	buffer->buffer = data_buffer;
	buffer->buffer_size = data_buffer_size / item_size;
	buffer->element_size = item_size;
	buffer->count = 0;
	buffer->head = 0;
	buffer->tail = 0;
}

bool_t circullar_buffer_put(
	struct circullar_buffer *buffer,
	const void *item)
{
	if (buffer->count < buffer->buffer_size) {
		copy_memory(&buffer->buffer[buffer->head * buffer->element_size], item, buffer->element_size);
		increment_index(&buffer->head, buffer->buffer_size);
		buffer->count++;
		return true;
	}

	return false;
}

bool_t circullar_buffer_get(
	struct circullar_buffer *buffer,
	void *item)
{
	if (buffer->count > 0) {
		copy_memory(item, &(buffer->buffer[buffer->tail * buffer->element_size]), buffer->element_size);
		increment_index(&buffer->tail, buffer->buffer_size);
		buffer->count--;
		return true;
	}

	return false;
}

uint8_t circullar_buffer_get_count(const struct circullar_buffer *buffer)
{
	return buffer->count;
}

bool_t circullar_buffer_has_space_for(const struct circullar_buffer *buffer, const uint8_t number)
{
	uint8_t left = buffer->buffer_size - buffer->count;
	return (number <= left);
}

bool_t circullar_buffer_has_any(const struct circullar_buffer *buffer)
{
	return (buffer->count != 0);
}

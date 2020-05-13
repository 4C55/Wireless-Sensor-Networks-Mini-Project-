#ifndef COMPRESSION_H_
#define COMPRESSION_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "types.h"

/*****************************************************************************/
/* PUBLIC DEFINES                                                            */
/*****************************************************************************/

enum compression_type
{
    COMPRESSION_TYPE_NONE,
    COMPRESSION_TYPE_SCALE,
    COMPRESSION_TYPE_RUN_LENGTH,
    COMPRESSION_TYPE_RUN_LENGTH_LOSSY
};

/*****************************************************************************/
/* PUBLIC ENUMERATIONS                                                       */
/*****************************************************************************/

/*****************************************************************************/
/* PUBLIC STRUCTS                                                            */
/*****************************************************************************/

/*****************************************************************************/
/* PUBLIC VARIABLES                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* PUBLIC FUNCTION DECLARATION                                               */
/*****************************************************************************/

uint32_t compression_runlength_lossy(
    uint8_t *buffer,
    const uint32_t count,
    uint8_t margin);

uint32_t compression_runlength(
    uint8_t *buffer,
    const uint32_t length);

uint32_t compression_scale(uint8_t *buffer, const uint32_t length, const uint32_t columns);

#endif /* COMPRESSION_H_ */
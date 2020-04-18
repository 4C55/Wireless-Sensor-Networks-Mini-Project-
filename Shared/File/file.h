#ifndef FILE_H_
#define FILE_H_

#ifdef	__cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include "types.h"
#include "cfs/cfs.h"
#include "cfs/cfs-coffee.h"
#include "sys/log.h"

/*****************************************************************************/
/* PUBLIC DEFINES                                                            */
/*****************************************************************************/

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

bool_t file_init(void);

bool_t file_read(const uint32_t address, uint8_t *buffer, uint16_t length);

bool_t file_write(const uint32_t address, uint8_t *buffer, uint16_t length);

void file_format_memory(void);

#ifdef	__cplusplus
}
#endif

#endif /* FILE_H_ */
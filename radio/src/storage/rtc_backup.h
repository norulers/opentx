/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _RTC_BACKUP_H_
#define _RTC_BACKUP_H_

#include "definitions.h"

PACK(struct RamBackup {
  uint16_t size;
  uint8_t data[4094];
});

extern RamBackup * ramBackup;

void rambackupWrite();
bool rambackupRestore();
unsigned int compress(uint8_t * dst, unsigned int dstsize, const uint8_t * src, unsigned int len);
unsigned int uncompress(uint8_t * dst, unsigned int dstsize, const uint8_t * src, unsigned int len);

#endif // _RTC_BACKUP_H_

#pragma once

#ifndef __OKIM6376_H__
#define __OKIM6376_H__

#include "devlegcy.h"

/* an interface for the OKIM6376 and similar chips */

READ8_DEVICE_HANDLER( okim6376_r );
WRITE8_DEVICE_HANDLER( okim6376_w );

DECLARE_LEGACY_SOUND_DEVICE(OKIM6376, okim6376);

WRITE_LINE_DEVICE_HANDLER( okim6376_st_w );
WRITE_LINE_DEVICE_HANDLER( okim6376_ch2_w );

READ_LINE_DEVICE_HANDLER( okim6376_busy_r );
READ_LINE_DEVICE_HANDLER( okim6376_nar_r );

DECLARE_LEGACY_SOUND_DEVICE(OKIM6376, okim6376);

void okim6376_set_frequency(device_t *device, int frequency);

#endif /* __OKIM6376_H__ */

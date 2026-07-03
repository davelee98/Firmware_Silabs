#ifndef OPENDISPLAY_FORTUNE_H
#define OPENDISPLAY_FORTUNE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Call on every button rising-edge (press). Tracks the double-press window
 * internally and sets a pending flag when a double-press is detected. */
void opendisplay_fortune_on_button_press(uint32_t now_ms);

/* Call from the main process loop. Renders a fortune to the display when a
 * double-press is pending. Blocks while the EPD refreshes. */
void opendisplay_fortune_process(void);

#ifdef __cplusplus
}
#endif

#endif

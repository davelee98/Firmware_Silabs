#include "opendisplay_fortune.h"
#include "opendisplay_display.h"
#include "sl_sleeptimer.h"
#include <stddef.h>
#include <stdio.h>

/* Maximum time between two presses that counts as a double-press. */
#define FORTUNE_DOUBLE_PRESS_MS 500u

/* Each fortune is stored as up to 3 pre-split lines (NULL = unused).
 * Lines are kept to ~20 characters so they fit at scale-2 on typical EPDs. */
typedef struct {
  const char *lines[3];
} OdFortune;

static const OdFortune s_fortunes[] = {
  { { "THE BEST IS",          "YET TO COME.",          NULL } },
  { { "YOUR FUTURE HOLDS",    "GREAT PROMISE.",         NULL } },
  { { "KINDNESS WILL",        "RETURN TO YOU.",         NULL } },
  { { "A PLEASANT SURPRISE",  "IS COMING.",             NULL } },
  { { "SUCCESS COMES TO",     "THOSE WHO PERSIST.",     NULL } },
  { { "HARD WORK WILL",       "SOON PAY OFF.",          NULL } },
  { { "GOOD THINGS COME",     "TO THOSE WHO WAIT.",     NULL } },
  { { "HAPPINESS IS",         "A CHOICE.",              NULL } },
  { { "TODAY IS A GREAT",     "DAY FOR A NEW START.",   NULL } },
  { { "YOUR CREATIVITY",      "SOLVES ALL PROBLEMS.",   NULL } },
  { { "FORTUNE FAVORS",       "THE BOLD.",              NULL } },
  { { "LAUGHTER IS THE",      "BEST MEDICINE.",         NULL } },
  { { "THE ANSWER YOU SEEK",  "IS WITHIN.",             NULL } },
  { { "PREPARE FOR A",        "WONDERFUL SURPRISE.",    NULL } },
  { { "NOTHING VENTURED,",    "NOTHING GAINED.",        NULL } },
  { { "YOU ARE",              "NEVER ALONE.",           NULL } },
  { { "A DREAM YOU HAVE",     "WILL COME TRUE.",        NULL } },
  { { "NEW OPPORTUNITIES",    "AWAIT YOU.",             NULL } },
  { { "YOUR PATIENCE",        "IS YOUR STRENGTH.",      NULL } },
  { { "GREAT THINGS TAKE",    "TIME.",                  NULL } },
};

#define FORTUNE_COUNT ((uint8_t)(sizeof(s_fortunes) / sizeof(s_fortunes[0])))

static uint32_t s_first_press_ms = 0u;
static bool s_fortune_pending = false;

void opendisplay_fortune_on_button_press(uint32_t now_ms)
{
  if (s_first_press_ms == 0u || (now_ms - s_first_press_ms) > FORTUNE_DOUBLE_PRESS_MS) {
    /* First press (or previous first press expired) — arm the window. */
    s_first_press_ms = now_ms;
    return;
  }
  /* Second press arrived within the window — trigger a fortune. */
  s_first_press_ms = 0u;
  s_fortune_pending = true;
}

void opendisplay_fortune_process(void)
{
  uint8_t idx;

  if (!s_fortune_pending) {
    return;
  }
  s_fortune_pending = false;

  /* Use the low bits of the tick counter as a cheap random index.
   * The user presses the button at an essentially unpredictable time,
   * so the tick value is sufficiently random for this purpose. */
  idx = (uint8_t)(sl_sleeptimer_get_tick_count() % FORTUNE_COUNT);
  printf("[OD][FORTUNE] showing fortune %u: %s\r\n",
         (unsigned)idx, s_fortunes[idx].lines[0]);

  opendisplay_display_show_fortune(s_fortunes[idx].lines);
}

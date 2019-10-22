#include "delta.h"

#include <stdio.h>

enum Meeting_delta get_meeting_delta(Meeting *meeting, time_t origin) {
  time_t delta = meeting->when - origin;
  if (delta < 0) {
    return PAST;
  } else if (delta > (2 * HOUR)) {
    return LONGER_THAN_2_HOURS;
  } else if (delta > (1 * HOUR)) {
    return LESS_THAN_2_HOURS;
  } else if (delta > (15 * MINUTE)) {
    return LESS_THAN_1_HOUR;
  }

  return LESS_THAN_15_MINUTES;
}

time_t get_meeting_time_minus_time(Meeting *meeting, time_t delta) {
  return meeting->when - delta;
}

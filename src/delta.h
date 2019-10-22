#ifndef REMEMBRALL_DELTA_H
#define REMEMBRALL_DELTA_H

#include <time.h>

#include "meeting.h"

#define HOUR 3600
#define MINUTE 60

enum Meeting_delta {
  PAST,
  LESS_THAN_15_MINUTES,
  LESS_THAN_1_HOUR,
  LESS_THAN_2_HOURS,
  LONGER_THAN_2_HOURS
};

enum Meeting_delta get_meeting_delta(Meeting *, time_t);
time_t get_meeting_time_minus_time(Meeting *, time_t);

#endif

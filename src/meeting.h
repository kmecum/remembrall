#ifndef REMEMBRALL_MEETINGS_H
#define REMEMBRALL_MEETINGS_H

#define MAX_STRING_LENGTH 1024

#include <time.h>

typedef struct Meeting {
  time_t when;
  char *room;
} Meeting;

Meeting *meeting_new(time_t, const char *);
Meeting *meeting_clone(Meeting *);
void meeting_destroy(Meeting *);

typedef struct Meeting_list {
  Meeting *meetings;
  size_t count;
  size_t _capacity;
  size_t _first;
} Meeting_list;

Meeting_list *meeting_list_new(void);
Meeting_list *meeting_list_add_meeting(Meeting_list *, Meeting *);
void meeting_list_destroy(Meeting_list *);

Meeting *meeting_list_get_next_meeting(Meeting_list *);
void meeting_list_pop(Meeting_list *);

#endif

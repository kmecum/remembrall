#include <assert.h>
#include <config.h>
#include <stdio.h>
#include <string.h>

#ifdef ENABLE_COCOA
#include "cocoa_view.h"
#endif

#include "data.h"
#include "meeting.h"

#ifndef ENABLE_COCOA
static void _list_meetings(void);
#endif

int main(void) {
#ifdef ENABLE_COCOA
  Meeting_list *meeting_list = start_cocoa_init();
  start_cocoa_ui(meeting_list);

  meeting_list_destroy(meeting_list);
#else
  _list_meetings();
#endif
}

#ifndef ENABLE_COCOA
void _list_meetings(void) {
  Meeting_list *meeting_list = meeting_list_from_file("TESTDATA");

  while (meeting_list->count > 0) {
    Meeting *meeting = meeting_list_get_next_meeting(meeting_list);
    printf("Next meeting found at %ld in room %s", meeting->when,
           meeting->room);

    meeting_list_pop(meeting_list);
  }

  meeting_list_destroy(meeting_list);
}
#endif

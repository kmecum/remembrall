#include "meeting.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Meeting_list *_meeting_list_resize(Meeting_list *, size_t);

Meeting *meeting_new(time_t when, const char *room) {
  Meeting *meeting = malloc(sizeof(Meeting));
  meeting->when = when;
  meeting->room = malloc(strlen(room) + 1);
  if (snprintf(meeting->room, MAX_STRING_LENGTH, "%s", room) == -1) {
    fprintf(stderr, "Out of memory. Dying..");
    exit(1);
  }

  return meeting;
}

Meeting *meeting_clone(Meeting *other) {
  return meeting_new(other->when, other->room);
}

void meeting_destroy(Meeting *meeting) {
  free(meeting->room);
  free(meeting);
}

Meeting_list *meeting_list_new() {
  Meeting_list *meeting_list = malloc(sizeof(Meeting_list));
  if (meeting_list == NULL) {
    fprintf(stderr, "Out of memory. Dying..");
    exit(1);
  }
  meeting_list->count = 0;

  // malloc 10 * Meetings so we (probably) never malloc again
  meeting_list->meetings = calloc(10, sizeof(Meeting));
  if (meeting_list->meetings == NULL) {
    fprintf(stderr, "Out of memory. Dying..");
    exit(1);
  }
  meeting_list->_capacity = 10;
  meeting_list->_first = 0;

  return meeting_list;
}

Meeting_list *meeting_list_add_meeting(Meeting_list *meeting_list,
                                       Meeting *meeting) {
  if (meeting_list->count >= meeting_list->_capacity - 1) {
    meeting_list =
        _meeting_list_resize(meeting_list, meeting_list->_capacity + 10);
  }
  memcpy(meeting_list->meetings + meeting_list->_first + meeting_list->count,
         meeting, sizeof(Meeting));
  char **room =
      &meeting_list->meetings[meeting_list->_first + meeting_list->count].room;
  *room = malloc(strlen(meeting->room) + 1);
  if (snprintf(*room, MAX_STRING_LENGTH, "%s", meeting->room) == -1) {
    fprintf(stderr, "Out of memory. Dying..");
    exit(1);
  }
  meeting_list->count++;

  // sort here

  return meeting_list;
}

void meeting_list_destroy(Meeting_list *meeting_list) {
  for (size_t i = 0; i < meeting_list->_capacity; i++) {
    if (meeting_list->meetings[i].room != NULL)
      free(meeting_list->meetings[i].room);
  }
  free(meeting_list->meetings);
  free(meeting_list);
}

Meeting *meeting_list_get_next_meeting(Meeting_list *meeting_list) {
  if (meeting_list->count <= 0) {
    return NULL;
  }
  return &(meeting_list->meetings[meeting_list->_first]);
}

void meeting_list_pop(Meeting_list *meeting_list) {
  /* We lose memory here because we don't *actually* free the meeting. We have a
   * buffer of these things that we own. This is really a write-once and then
   * erase kind of problem so we can just leave the memory as-is. In a normal
   * run I expect this buffer to be malloc'd once, never resized, and free'd at
   * the end of the program when the whole list gets destroyed. Technically,
   * your meeting list is available all day somewhere in RAM. How nice :\ */
  meeting_list->count--;
  meeting_list->_first++;
}

Meeting_list *_meeting_list_resize(Meeting_list *meeting_list,
                                   size_t new_capacity) {
  // We only go one way folks.
  assert(new_capacity > meeting_list->_capacity);

  // linear malloc algorithm since problem is simple
  Meeting *meetings_new = calloc(new_capacity, sizeof(Meeting));
  if (meetings_new == NULL) {
    fprintf(stderr, "Out of memory. Dying..");
    exit(1);
  }
  memcpy(meetings_new, meeting_list->meetings,
         meeting_list->_capacity * sizeof(Meeting));
  free(meeting_list->meetings);
  meeting_list->meetings = meetings_new;
  meeting_list->_capacity = new_capacity;

  return meeting_list;
}
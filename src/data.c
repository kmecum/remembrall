#include "data.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

Meeting_list *meeting_list_from_file(const char *file) {
  Meeting_list *meeting_list = meeting_list_new();

  FILE *fp = fopen(file, "r");

  if (fp == NULL) {
    fprintf(stderr, "Could not open file: %s. Dying..\n", file);
    exit(EXIT_FAILURE);
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  while ((nread = getline(&line, &len, fp)) != -1) {
    char delim[] = ":";

    char *ptr = strtok(line, delim);
    time_t when = atoi(ptr);

    ptr = strtok(NULL, delim);

    Meeting *meeting = meeting_new(when, ptr);
    meeting_list_add_meeting(meeting_list, meeting);

    meeting_destroy(meeting);
  }

  free(line);
  fclose(fp);

  return meeting_list;
}

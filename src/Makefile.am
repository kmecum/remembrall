bin_PROGRAMS = remembrall
remembrall_SOURCES = main.c data.c data.h delta.c delta.h meeting.c meeting.h
remembrall_CFLAGS = -Wall -Werror $(REMEMBRALL_CFLAG_GNU11)

ACLOCAL_AMFLAGS = -I ../autoconf

if REMEMBRALL_COCOA
remembrall_OBJCFLAGS = $(REMEMBRALL_CFLAG_COCOA)
remembrall_SOURCES += cocoa_view.m cocoa_view.h
endif

#include "cocoa_view.h"

#import <Cocoa/Cocoa.h>

#include <stdio.h>

#include "delta.h"

struct Cocoa_context {
  Meeting_list *meeting_list;
  id window;
};

static id _start_window(void);
static void _start_timer(struct Cocoa_context *);
static void _timer_callback(CFRunLoopTimerRef, void *);
static CFAbsoluteTime _time_t_to_CFAbsoluteTime(time_t);

Meeting_list *start_cocoa_init(void) {
  // Replace with actual UI lol
  Meeting_list *meeting_list = meeting_list_from_file("TESTDATA");

  return meeting_list;
}

void start_cocoa_ui(Meeting_list *meeting_list) {
  id window = _start_window();

  struct Cocoa_context *context = malloc(sizeof(struct Cocoa_context));
  context->meeting_list = meeting_list;
  context->window = window;

  _start_timer(context);

  [NSApp run];
}

id _start_window(void) {
  [NSAutoreleasePool new];
  [NSApplication sharedApplication];
  [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

  id menubar = [[NSMenu new] autorelease];
  id appMenuItem = [[NSMenuItem new] autorelease];
  [menubar addItem:appMenuItem];
  [NSApp setMainMenu:menubar];
  id appMenu = [[NSMenu new] autorelease];
  id appName = [[NSProcessInfo processInfo] processName];
  id quitTitle = [@"Quit " stringByAppendingString:appName];
  id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle
                                                action:@selector(terminate:)
                                         keyEquivalent:@"q"] autorelease];
  [appMenu addItem:quitMenuItem];
  [appMenuItem setSubmenu:appMenu];

  NSRect screenSize = [[NSScreen mainScreen] frame];

  CGFloat percent = 0.8;
  CGFloat offset = (1.0 - percent) / 2.0;

  id window = [[NSWindow alloc]
      initWithContentRect:NSMakeRect(screenSize.size.width * offset,
                                     screenSize.size.height * offset,
                                     screenSize.size.width * percent,
                                     screenSize.size.height * percent)
                styleMask:NSWindowStyleMaskBorderless
                  backing:NSBackingStoreBuffered
                    defer:YES];
  [window setTitle:appName];
  [window setOpaque:YES];
  [window setAlphaValue:0.0];
  [window setHasShadow:YES];
  [window setLevel:NSFloatingWindowLevel];
  [window setIgnoresMouseEvents:YES];
  [window makeKeyAndOrderFront:nil];
  [NSApp activateIgnoringOtherApps:YES];

  return window;
}

static void _start_timer(struct Cocoa_context *context) {
  CFRunLoopRef runLoop = CFRunLoopGetCurrent();
  CFRunLoopTimerContext timer_context = {.copyDescription = NULL,
                                         .info = &context,
                                         .release = NULL,
                                         .retain = NULL,
                                         .version = 0};

  CFRunLoopTimerRef timer =
      CFRunLoopTimerCreate(kCFAllocatorDefault, CFAbsoluteTimeGetCurrent(), 0.1,
                           0, 0, _timer_callback, &timer_context);

  CFRunLoopAddTimer(runLoop, timer, kCFRunLoopCommonModes);
}

void _timer_callback(CFRunLoopTimerRef timer, void *info) {
  struct Cocoa_context *context = *((struct Cocoa_context **)info);
  Meeting *meeting = meeting_list_get_next_meeting(context->meeting_list);

  if (meeting == NULL) {
    NSLog(@"No more meetings. Dying..");
    exit(1);
  }

  time_t now = time(NULL);
  time_t timer_delta = 0;

  switch (get_meeting_delta(meeting, now)) {
  case PAST:
    NSLog(@"Next meeting is in the past. Popping..");
    meeting_list_pop(context->meeting_list);
    break;
  case LESS_THAN_15_MINUTES:
    NSLog(@"Meeting found in next 15 minutes.");

    [context->window setAlphaValue:0.2];
    [context->window setBackgroundColor:NSColor.redColor];

    // This sets the next check one second after the current
    // meeting. If this is 0 it introduces jitter and the timer fires
    // hundreds of times before the next second.
    timer_delta = -1;
    break;
  case LESS_THAN_1_HOUR:
    NSLog(@"Meeting found in next hour.");

    [context->window setAlphaValue:0.1];
    [context->window setBackgroundColor:NSColor.redColor];

    timer_delta = 15 * MINUTE;
    break;
  case LESS_THAN_2_HOURS:
    NSLog(@"Meeting found in next 2 hours.");

    [context->window setAlphaValue:0.1];
    [context->window setBackgroundColor:NSColor.yellowColor];

    timer_delta = 1 * HOUR;
    break;
  default:
    NSLog(@"No meetings in the next two hours. See you later!");

    [context->window setAlphaValue:0.0];

    timer_delta = 2 * HOUR;
  }

  time_t next_timer_fire = get_meeting_time_minus_time(meeting, timer_delta);
  CFRunLoopTimerSetNextFireDate(timer,
                                _time_t_to_CFAbsoluteTime(next_timer_fire));
  time_t next_delta = next_timer_fire - time(NULL);

  NSLog(@"Next timer fire in %ld seconds.", next_delta > 0 ? next_delta : 0);
}

static CFAbsoluteTime _time_t_to_CFAbsoluteTime(time_t time) {
  return time - kCFAbsoluteTimeIntervalSince1970;
}

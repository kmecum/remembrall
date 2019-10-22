AC_PREREQ([2.69])
AC_INIT([remembrall]. [0.1], [k@kenmecum.com])
AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC

AX_CHECK_COMPILE_FLAG([-std=gnu11], [
    AC_SUBST([REMEMBRALL_CFLAG_GNU11], [-std=gnu11])
])

AC_PROG_OBJC
AC_PROG_OBJCXX

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile
	src/Makefile
])

#
# Cocoa support
#
AC_MSG_CHECKING([whether to build Cocoa support])
AC_ARG_ENABLE(cocoa,
    [AS_HELP_STRING([--enable-cocoa],[Enable the Cocoa UI for Apple Macintosh computers. @<:@default=no@:>@.])],
    [
        case "${enableval}" in

        no|yes)
            enable_cocoa=${enableval}
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${enableval} for --enable-cocoa])
            ;;

        esac
    ],
    [enable_cocoa=no])
AC_MSG_RESULT(${enable_cocoa})

AM_CONDITIONAL([REMEMBRALL_COCOA], [test "${enable_cocoa}" = "yes"])

if test "${enable_cocoa}" = "yes" ; then
    AC_LANG_PUSH([Objective C++])

        # Check for required Cocoa headers.

        AC_CHECK_HEADERS([Cocoa/Cocoa.h],
        [],
        [
            AC_MSG_ERROR(The Cocoa header "$ac_header" is required but cannot be found.)
        ])

        AX_CHECK_COMPILE_FLAG([-framework Cocoa], [
            AC_SUBST([REMEMBRALL_CFLAG_COCOA], ["-framework Cocoa"])
        ])

        AC_DEFINE([ENABLE_COCOA], [1], [Define if enabling Cocoa support.])

    AC_LANG_POP([Objective C++])
fi

AC_OUTPUT

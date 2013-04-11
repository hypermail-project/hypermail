dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_REVISION($Revision: 1.1 $)dnl
AC_INIT
AC_CONFIG_SRCDIR([src/hypermail.c])
AC_CONFIG_HEADER(config.h)
AC_PREFIX_DEFAULT(/usr/local)
LDFLAGS=""
LIBS=""
EXTRA_LIBS=""
INCLUDES=""
GDBM_INCLUDE=""
GDBM_LIB=""
PCRE_DEP=""
FNV_DEP=""

dnl These are the subdirs that are compiled systematically
dnl For subdirs that can be disabled thru configure options,
dnl we configure them in a dedicated section near the test
SUBDIRS="src/trio"

dnl ===========================================================================
dnl Get host, target and build variables filled with appropriate info.
dnl ===========================================================================

AC_CANONICAL_TARGET

dnl ===========================================================================
dnl Check to assure the cached information is valid.
dnl ===========================================================================

AC_MSG_CHECKING(cached information)
hostcheck="$host"
AC_CACHE_VAL(ac_cv_hostcheck, [ ac_cv_hostcheck="$hostcheck" ])
if test "$ac_cv_hostcheck" != "$hostcheck"; then
  AC_MSG_RESULT(changed)
  AC_MSG_WARN(config.cache exists!)
  AC_MSG_ERROR(you must do 'make clobber' first to compile for different host or different parameters.)
else
  AC_MSG_RESULT(ok)
fi

dnl ===========================================================================
dnl Checks for programs.
dnl ===========================================================================

AC_PROG_CC
AC_PROG_CPP
AC_PROG_YACC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_CHECK_PROG(AR, ar, ar, echo)
if test -z "$no_ranlib"; then
  AC_PROG_RANLIB
else
  RANLIB=":"
fi

dnl ===========================================================================
dnl Determine the host type and set compliation flags as needed
dnl ===========================================================================

INSTALL="install-sh"

case "$host" in
  *-*-solaris*)
    # Solaris flags 
    AC_DEFINE(NO_MACRO)
    ;;

  *-*-hpux*)
    # HPUX flags 
    if test -z "$GCC"; then
      CFLAGS="$CFLAGS -Ae"
    fi
    ;;
  *-dec-osf*)
    # DEC OSF flags 
    CFLAGS="$CFLAGS -mieee"
    INSTALL="install-sh"
    ;;
esac

export CFLAGS CC

AC_SUBST(INSTALL)

AC_MSG_CHECKING([that the compiler works])
AC_RUN_IFELSE([AC_LANG_SOURCE([[ main(int ac, char **av) { return 0; } ]])],[AC_MSG_RESULT(yes)],[AC_MSG_RESULT(no)
    AC_MSG_ERROR(Could not compile and run even a trivial ANSI C program - check CC.)],[AC_MSG_ERROR(Could not compile and run even a trivial ANSI C program - check CC.)])

dnl ===========================================================================
dnl Determine if building on a Cygwin system so that 
dnl etc/passwd checks are to be disabled.
dnl ===========================================================================

case "$build" in
  *-*-cygwin*)
      suffix=.exe
      AC_SUBST(suffix)
    ;;
esac

dnl ===========================================================================
dnl Accepted arguments to ./configure...
dnl ===========================================================================

AC_MSG_CHECKING([whether to enable -Wall])
AC_ARG_ENABLE(warnings,
[  --enable-warnings       Enable -Wall if using gcc.],
[ if test -n "$GCC"; then 
    AC_MSG_RESULT(adding -Wall to CFLAGS.)
    CFLAGS="$CFLAGS -Wall"
  fi],AC_MSG_RESULT(no))

AC_ARG_WITH(httpddir, 
    [  --with-httpddir=DIR	  webserver's root directory [/usr/local/apache]],
    httpddir=$with_httpddir,	  httpddir=/usr/local/apache)
AC_SUBST(httpddir)

AC_ARG_WITH(cgidir, 
    [  --with-cgidir=DIR	  where to install CGI scripts ],
    cgidir=$with_cgidir,	  cgidir=$httpddir/cgi-bin)
AC_SUBST(cgidir)

AC_ARG_WITH(htmldir, 
    [  --with-htmldir=DIR	  where to install Hypermail HTML pages ],
    htmldir=$with_htmldir,	  htmldir=$httpddir/htdocs/hypermail)
AC_SUBST(htmldir)

AC_ARG_WITH(language, 
    [  --with-language=xx	  two character language indicator [en] ],
    language=$with_language,	  language=en)
AC_SUBST(language)

AC_ARG_WITH(htmlsuffix, 
    [  --with-htmlsuffix=xx	  two character language indicator [html] ],
    htmlsuffix=$with_htmlsuffix,  htmlsuffix=html)
AC_SUBST(htmlsuffix)

AC_ARG_ENABLE(defaultindex,
    [  --enable-defaultindex=type	Default index page type [thread] ],
    [ defaultindex=$enableval ], [ defaultindex="thread" ])
AC_SUBST(defaultindex)

AC_ARG_WITH(domainaddr, 
    [  --with-domainaddr=YOURDOMAIN	  domain address of local domain ],
    domainaddr=$with_domainaddr,  domainaddr=NONE)
AC_SUBST(domainaddr)

dnl ===========================================================================
dnl Configure the subdirectories
dnl ===========================================================================
AC_DISABLE_OPTION_CHECKING
AC_CONFIG_SUBDIRS([SUBDIRS])

dnl ===========================================================================
dnl Checks headers
dnl ===========================================================================

AC_HEADER_STDC

AC_CHECK_HEADERS(alloca.h arpa/inet.h ctype.h dirent.h errno.h \
	fcntl.h locale.h malloc.h netdb.h netinet/in.h pwd.h stdarg.h \
	stdio.h stdlib.h string.h sys/dir.h sys/param.h sys/socket.h \
	sys/stat.h sys/time.h sys/types.h time.h unistd.h)

AC_HEADER_STAT
AC_HEADER_DIRENT
AC_HEADER_TIME

dnl ===========================================================================
dnl Checks for library functions.
dnl Check for typedefs, structures, and compiler characteristics.
dnl ===========================================================================

AC_STRUCT_TM

AC_FUNC_STRFTIME
AC_CHECK_FUNCS(mkdir strdup strstr strtol memcpy memset lstat strcasecmp \
               strcasestr getpwuid getopt snprintf memmove strerror)

AC_TYPE_SIZE_T

if test $ac_cv_func_snprintf != no; then
    AC_DEFINE(HAVE_SNPRINTF)
fi


dnl gdbm checks

AC_DEFUN([AC_TEMP_LDFLAGS],[
  old_LDFLAGS="$LDFLAGS"
  LDFLAGS="$1 $LDFLAGS"
  $2
  LDFLAGS="$old_LDFLAGS"
])

dnl
dnl AC_HMAIL_ONCE(namespace, variable, code)
dnl
dnl execute code, if variable is not set in namespace
dnl
AC_DEFUN([AC_HMAIL_ONCE],[
  changequote({,})
  unique=`echo $2|sed 's/[^a-zA-Z0-9]/_/g'`
  changequote([,])
  cmd="echo $ac_n \"\$$1$unique$ac_c\""
  if test -n "$unique" && test "`eval $cmd`" = "" ; then
    eval "$1$unique=set"
    $3
  fi
])

dnl
dnl AC_EXPAND_PATH(path, variable)
dnl
dnl expands path to an absolute path and assigns it to variable
dnl
AC_DEFUN([AC_EXPAND_PATH],[
  if test -z "$1" || echo "$1" | grep '^/' >/dev/null ; then
    $2="$1"
  else
    changequote({,})
    ep_dir="`echo $1|sed 's%/*[^/][^/]*/*$%%'`"
    changequote([,])
    ep_realdir="`(cd \"$ep_dir\" && pwd)`"
    $2="$ep_realdir/`basename \"$1\"`"
  fi
])

AC_DEFUN([AC_ADD_INCLUDE],[
  if test "$1" != "/usr/include"; then
    AC_EXPAND_PATH($1, ai_p)
    AC_HMAIL_ONCE(INCLUDEPATH, $ai_p, [
      INCLUDES="$INCLUDES -I$ai_p"
    ])
  fi
])

dnl
dnl AC_ADD_LIBPATH(path[, shared-libadd])
dnl
dnl add a library to linkpath
dnl
AC_DEFUN([AC_ADD_LIBPATH],[
  if test "$1" != "/usr/lib"; then
    AC_EXPAND_PATH($1, ai_p)
    AC_HMAIL_ONCE(LIBPATH, $ai_p, [
      LDFLAGS="$LDFLAGS -L$ai_p"
    ])
  fi
])

dnl
dnl AC_ADD_LIBRARY_WITH_PATH(library, path[, shared-libadd])
dnl
dnl add a library to the link line and path to linkpath/runpath.
dnl if shared-libadd is not empty and $ext_shared is yes,
dnl shared-libadd will be assigned the library information
dnl
AC_DEFUN([AC_ADD_LIBRARY_WITH_PATH],[
ifelse($3,,[
  if test -n "$2"; then
    AC_ADD_LIBPATH($2)
  fi
  EXTRA_LIBS="$EXTRA_LIBS $1" ],[
    AC_ADD_LIBRARY_WITH_PATH($1,$2)
  ])
])

AC_ARG_WITH(gdbm,
   AS_HELP_STRING([--with-gdbm=[DIR]],
                  [Include GDBM support]),
   [ given_gdbm=$withval])

if test "$given_gdbm" != "no"; then
    for i in /usr/local /usr $withval; do
      if test -f "$i/include/gdbm.h"; then
        GDBM_INCLUDE="$i/include"
        THIS_PREFIX="$i"
      fi
    done

    unset ac_cv_lib_gdbm_gdbm_open
    AC_TEMP_LDFLAGS(-L$THIS_PREFIX/lib,[
       AC_CHECK_LIB(gdbm, gdbm_open, [GDBM_LIB="-lgdbm"])
    ])

    if test "$THIS_PREFIX" != "" && test "$THIS_PREFIX" != "/usr"; then
      THIS_LFLAGS="$THIS_PREFIX/lib"
    fi
fi

AC_MSG_CHECKING(for GDBM support)

if test "$GDBM_LIB" = "" && test "$given_gdbm" != "no"; then
  AC_CHECK_LIB(gdbm, gdbm_open,[AC_DEFINE(GDBM,1, [Whether you have GDBM]) DBM_TYPE=gdbm; GDBM_LIB=-lgdbm],
     [DBM_TYPE=""])
  AC_MSG_CHECKING([gdbm library])
  if test "a$DBM_TYPE" = a; then
    AC_MSG_RESULT(none found)
    AC_MSG_WARN(No gdbm library found - will limit a few features)
  else
    AC_MSG_RESULT($DBM_TYPE found)
  fi
else
    AC_MSG_RESULT(no)
fi

if test "$GDBM_LIB" = "-lgdbm" && test "$GDBM_INCLUDE" = ""; then
  AC_CHECK_HEADER(gdbm.h, [ GDBM_INCLUDE="" ], [ 
      AC_MSG_RESULT(Try /usr/local/include/gdbm.h)
      AC_CHECK_HEADER(/usr/local/include/gdbm.h, [ GDBM_INCLUDE="-I/usr/local/include" ],[
        AC_MSG_RESULT(Try /opt/local/include/gdbm.h)
        AC_CHECK_HEADER(/opt/local/include/gdbm.h, [ GDBM_INCLUDE="-I/opt/local/include" ],[
          dnl if in /usr/pkg/include, do not add anything.  See above.
          AC_MSG_RESULT(Try /usr/pkg/include/gdbm.h)
          AC_CHECK_HEADER(/usr/pkg/include/gdbm.h, [ GDBM_INCLUDE="" ],[
            AC_MSG_RESULT([Giving up - You need to install gdbm.h somewhere])
            exit
          ])
        ])
      ])  
    ])
fi

if test -n "$GDBM_LIB"; then
    AC_ADD_INCLUDE($GDBM_INCLUDE)
    AC_DEFINE(HAVE_GDBM_H)
    EXTRA_LIBS="$EXTRA_LIBS $GDBM_LIB"
fi


dnl
dnl iconv check
dnl

AC_ARG_ENABLE(i18n, [  --disable-i18n           Disable I18N support], [given_iconv=$enableval])
if test "$given_iconv" = "no"; then
  echo "disabled I18N support."
else
  AC_CHECK_FUNCS(iconv)
  AC_CHECK_HEADERS(iconv.h)
fi

dnl
dnl PCRE: select whether to use an external or the bundled PCRE lib
dnl borrowed from Apache's configure.in.
dnl

AC_MSG_NOTICE(Checking use of an external PCRE)
AC_ARG_WITH(external_pcre, 
	    AS_HELP_STRING([--with-external_pcre[=DIR]],
                           [Use an external PCRE library instead of the bundled one (default /usr)])
)

case $with_external_pcre in
yes) AC_PATH_PROG([PCRE_CONFIG], [pcre-config], [false]) ;;
 /*) if test -d "$with_external_pcre" && test -x "$with_external_pcre/bin/pcre-config"; then
       PCRE_CONFIG=$with_external_pcre/bin/pcre-config
     elif test -x "$with_external_pcre"; then
       PCRE_CONFIG=$with_external_pcre
     fi

     if $PCRE_CONFIG --version >/dev/null 2>&1; then :; 
     else
        PCRE_CONFIG=false
     fi
     ;;
*) PCRE_CONFIG=false ;;
esac

if test "$PCRE_CONFIG" != "false"; then
  PCRE_PATH=`$PCRE_CONFIG --prefix`
  AC_CHECK_HEADER(["$PCRE_PATH/include/pcre.h"], [ ],[PCRE_CONFIG=false])
fi

if test "$PCRE_CONFIG" != "false"; then
  AC_MSG_NOTICE([using external PCRE library])
  APR_ADDTO(CFLAGS, [`$PCRE_CONFIG --cflags`])
  APR_ADDTO(LIBS, [`$PCRE_CONFIG --libs`])
else
  AC_MSG_NOTICE([using bundled PCRE regular expression library])
  APR_ADDTO(INCLUDES, [-Ipcre])
  APR_ADDTO(LDFLAGS, "[-Lpcre/.libs]")
  PCRE_DEP="pcre/.libs/libpcre.a"
  APR_SUBDIR_CONFIG([src/pcre], 
                    [],
  		    [--with-pcre=*|\'--with-pcre=*])
fi
AC_SUBST([PCRE_DEP])

dnl
dnl The FNV hash library used for the nonsequential filenames
dnl

AC_MSG_CHECKING([whether to use fnv hash library for non-sequential filenames])
AC_ARG_ENABLE(libfnv, 
     [	    AS_HELP_STRING([--enable-libfnv],
                           [use the fnv hash library for generating non-sequential filenames [no]])
     ],
     [ given_libfnv=$enableval ])
     
if test "$given_libfnv" != "yes"; then
  AC_MSG_RESULT([no])
else
  AC_MSG_RESULT([yes])
  AC_DEFINE([HAVE_LIBFNV])
  APR_ADDTO(INCLUDES, [-Ifnv])
  dnl APR_ADDTO(LDFLAGS, "[-Lfnv]")
  AC_ADD_LIBRARY_WITH_PATH([-lfnv],[src/fnv])
  FNV_DEP="fnv/libfnv.a"
  APR_SUBDIR_CONFIG([src/fnv])
fi
AC_SUBST(FNV_DEP)


dnl ===========================================================================
dnl Checks for libraries.
dnl nsl socket lib?
dnl ===========================================================================

USENSL=no
AC_CHECK_LIB(socket,gethostbyaddr,result=yes,result=no)
if test $result = yes; then
        LIBS="$LIBS -lsocket"
else
        AC_CHECK_LIB(socket,gethostbyaddr,result=yes,result=no,-lnsl)
        if test $result = yes; then
                LIBS = "$LIBS -lsocket -lnsl"
                USENSL=yes
        else
                AC_CHECK_LIB(socket,inet_addr,result=yes,result=no)
                if test $result = yes; then
                        LIBS="$LIBS -lsocket"
                else
                        AC_CHECK_LIB(socket,inet_addr,result=yes,result=no,-lnsl)
                        if test $result = yes; then
                                LIBS="$LIBS -lsocket -lnsl"
                                USENSL=yes
                        fi
                fi
        fi
fi
if test $USENSL != yes; then
        AC_CHECK_LIB(nsl,inet_addr,result=yes,result=no)
        if test $result = yes; then
                LIBS="$LIBS -lnsl"
        fi
fi

dnl ===========================================================================
dnl Makefile variable substitution 
dnl ===========================================================================

AC_SUBST(HAVE_MEMMOVE)
AC_SUBST(HAVE_STRERROR)
AC_SUBST(LDFLAGS)
AC_SUBST(LIBS)
AC_SUBST(EXTRA_LIBS)
AC_SUBST(INCLUDES)

AC_CONFIG_FILES([Makefile archive/Makefile docs/Makefile libcgi/Makefile src/Makefile tests/testhm src/defaults.h])
AC_OUTPUT


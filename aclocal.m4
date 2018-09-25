dnl Some macros borrowed from Apache's httpd config file. Thanks!

dnl
dnl APR_SUBDIR_CONFIG(dir [, sub-package-cmdline-args, args-to-drop])
dnl
dnl dir: directory to find configure in
dnl sub-package-cmdline-args: arguments to add to the invocation (optional)
dnl args-to-drop: arguments to drop from the invocation (optional)
dnl
dnl Note: This macro relies on ac_configure_args being set properly.
dnl
dnl The args-to-drop argument is shoved into a case statement, so
dnl multiple arguments can be separated with a |.
dnl
dnl Note: Older versions of autoconf do not single-quote args, while 2.54+
dnl places quotes around every argument.  So, if you want to drop the
dnl argument called --enable-layout, you must pass the third argument as:
dnl [--enable-layout=*|\'--enable-layout=*]
dnl
dnl Trying to optimize this is left as an exercise to the reader who wants
dnl to put up with more autoconf craziness.  I give up.
dnl
AC_DEFUN([APR_SUBDIR_CONFIG], [
  # save our work to this point; this allows the sub-package to use it
  AC_CACHE_SAVE

  echo "configuring package in $1 now"
  ac_popdir=`pwd`
  apr_config_subdirs="$1"
  test -d $1 || $mkdir_p $1
  ac_abs_srcdir=`(cd $srcdir/$1 && pwd)`
  cd $1

changequote(, )dnl
      # A "../" for each directory in /$config_subdirs.
      ac_dots=`echo $apr_config_subdirs|sed -e 's%^\./%%' -e 's%[^/]$%&/%' -e 's%[^/]*/%../%g'`
changequote([, ])dnl

  # Make the cache file pathname absolute for the subdirs
  # required to correctly handle subdirs that might actually
  # be symlinks
  case "$cache_file" in
  /*) # already absolute
    ac_sub_cache_file=$cache_file ;;
  *)  # Was relative path.
    ac_sub_cache_file="$ac_popdir/$cache_file" ;;
  esac

  ifelse($3, [], [apr_configure_args=$ac_configure_args],[
  apr_configure_args=
  apr_sep=
  for apr_configure_arg in $ac_configure_args
  do
    case "$apr_configure_arg" in
      $3)
        continue ;;
    esac
    apr_configure_args="$apr_configure_args$apr_sep'$apr_configure_arg'"
    apr_sep=" "
  done
  ])

  dnl autoconf doesn't add --silent to ac_configure_args; explicitly pass it
  test "x$silent" = "xyes" && apr_configure_args="$apr_configure_args --silent"

  dnl AC_CONFIG_SUBDIRS silences option warnings, emulate this for 2.62
  apr_configure_args="--disable-option-checking $apr_configure_args" 

  dnl The eval makes quoting arguments work - specifically the second argument
  dnl where the quoting mechanisms used is "" rather than [].
  dnl
  dnl We need to execute another shell because some autoconf/shell combinations
  dnl will choke after doing repeated APR_SUBDIR_CONFIG()s.  (Namely Solaris
  dnl and autoconf-2.54+)
  if eval $SHELL $ac_abs_srcdir/configure $apr_configure_args --cache-file=$ac_sub_cache_file --srcdir=$ac_abs_srcdir $2
  then :
    echo "$1 configured properly"
  else
    echo "configure failed for $1"
    exit 1
  fi

  cd $ac_popdir

  # grab any updates from the sub-package
  AC_CACHE_LOAD
])dnl

dnl
dnl APR_ADDTO(variable, value)
dnl
dnl  Add value to variable
dnl
AC_DEFUN([APR_ADDTO], [
  if test "x$$1" = "x"; then
    test "x$silent" != "xyes" && echo "  setting $1 to \"$2\""
    $1="$2"
  else
    apr_addto_bugger="$2"
    for i in $apr_addto_bugger; do
      apr_addto_duplicate="0"
      for j in $$1; do
        if test "x$i" = "x$j"; then
          apr_addto_duplicate="1"
          break
        fi
      done
      if test $apr_addto_duplicate = "0"; then
        test "x$silent" != "xyes" && echo "  adding \"$i\" to $1"
        $1="$$1 $i"
      fi
    done
  fi
])dnl

dnl
dnl APR_REMOVEFROM(variable, value)
dnl
dnl Remove a value from a variable
dnl
AC_DEFUN([APR_REMOVEFROM], [
  if test "x$$1" = "x$2"; then
    test "x$silent" != "xyes" && echo "  nulling $1"
    $1=""
  else
    apr_new_bugger=""
    apr_removed=0
    for i in $$1; do
      if test "x$i" != "x$2"; then
        apr_new_bugger="$apr_new_bugger $i"
      else
        apr_removed=1
      fi
    done
    if test $apr_removed = "1"; then
      test "x$silent" != "xyes" && echo "  removed \"$2\" from $1"
      $1=$apr_new_bugger
    fi
  fi
]) dnl

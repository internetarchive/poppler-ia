AC_PREREQ(2.59)
AC_INIT(poppler, 0.10.0)
AM_INIT_AUTOMAKE([foreign])
AM_CONFIG_HEADER(config.h)
AM_CONFIG_HEADER(poppler/poppler-config.h)

dnl ##### Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CC
AC_PROG_CXX
AC_ISC_POSIX
AC_PROG_CC_STDC
#if test -z "$CXX" -a "$CC" = "gcc"; then
#  CXX="gcc"
#fi
AC_PROG_CXX
AC_PROG_INSTALL
AC_CHECK_FUNC(gettimeofday, AC_DEFINE(HAVE_GETTIMEOFDAY, 1, [Defines if gettimeofday is available on your system]))
AC_CHECK_FUNC(localtime_r, AC_DEFINE(HAVE_LOCALTIME_R, 1, [Defines if localtime_r is available on your system]))

dnl Enable these unconditionally.
AC_DEFINE([OPI_SUPPORT], [1], [Generate OPI comments in PS output.])
AC_DEFINE([MULTITHREADED], [1], [Enable multithreading support.])
AC_DEFINE([TEXTOUT_WORD_LIST], [1], [Enable word list support.])

dnl Check for OS specific flags
win32_libs=""
create_shared_lib=""
case "$host_os" in
  mingw*)
    win32_libs="-lgdi32"
    create_shared_lib="-no-undefined"
  ;;
esac

AC_SUBST(win32_libs)
AC_SUBST(create_shared_lib)

ACX_PTHREAD()

dnl Install xpdf headers
AC_ARG_ENABLE(xpdf-headers,
	      AC_HELP_STRING([--enable-xpdf-headers],
	                     [Install unsupported xpdf headers.]),
              enable_xpdf_headers=$enableval,
              enable_xpdf_headers="no")
AM_CONDITIONAL(ENABLE_XPDF_HEADERS, test x$enable_xpdf_headers = xyes)

AC_ARG_ENABLE(fixedpoint,
[  --enable-fixedpoint     use fixed point (instead of floating point) arithmetic],
AC_DEFINE(USE_FIXEDPOINT, [1], [Use fixed point arithmetic]))

AC_DEFINE_DIR(POPPLER_DATADIR, "{datarootdir}/poppler", [Poppler data dir])

dnl ##### Checks for header files.
AC_PATH_XTRA
AC_HEADER_DIRENT

AC_ARG_ENABLE(exceptions,
	      [  --enable-exceptions     use C++ exceptions],
	      AC_DEFINE([USE_EXCEPTIONS], [1], [Throw exceptions to deal with not enough memory and similar problems]))

dnl ##### Switch over to C++.  This will make the checks below a little
dnl ##### bit stricter (requiring function prototypes in include files).
dnl ##### (99% of xpdf is written in C++.)
AC_LANG_CPLUSPLUS

dnl ##### Check for extra libraries needed by X.  (LynxOS needs this.)
AC_CHECK_FUNC(gethostbyname)
if test $ac_cv_func_gethostbyname = no; then
  AC_CHECK_LIB(bsd, gethostbyname, X_EXTRA_LIBS="$X_EXTRA_LIBS -lbsd")
fi

dnl ##### Checks for library functions.
AC_CHECK_FUNCS(popen mkstemp mkstemps)

dnl ##### Back to C for the library tests.
AC_LANG_C

dnl ##### Check for fseeko/ftello or fseek64/ftell64
dnl The LARGEFILE and FSEEKO macros have to be called in C, not C++, mode.
AC_SYS_LARGEFILE
AC_FUNC_FSEEKO
AC_CHECK_FUNCS(fseek64, xpdf_cv_func_fseek64=yes, xpdf_cv_func_fseek64=no)
AC_CHECK_FUNCS(ftell64, xpdf_cv_func_ftell64=yes, xpdf_cv_func_ftell64=no)
if test "$xpdf_cv_func_fseek64" = yes -a "$xpdf_cv_func_ftell64" = yes; then
  AC_DEFINE(HAVE_FSEEK64)
fi

dnl Test for zlib
AC_ARG_ENABLE([zlib],
  [AS_HELP_STRING([--enable-zlib],[Build with zlib])],
  [],[enable_zlib="no"])
if test x$enable_zlib = xyes; then
  AC_CHECK_LIB([z], [inflate],,
	       AC_MSG_ERROR("*** zlib library not found ***"))
  AC_CHECK_HEADERS([zlib.h],,
		   AC_MSG_ERROR("*** zlib headers not found ***"))
elif test x$enable_zlib = xtry; then
  AC_CHECK_LIB([z], [inflate],
               [enable_zlib="yes"],
	       [enable_zlib="no"])
  AC_CHECK_HEADERS([zlib.h],,
		   [enable_zlib="no"])
fi

if test x$enable_zlib = xyes; then
  ZLIB_LIBS="-lz"
  AC_SUBST(ZLIB_LIBS)
  AC_DEFINE(ENABLE_ZLIB)
fi

AM_CONDITIONAL(BUILD_ZLIB, test x$enable_zlib = xyes)
AH_TEMPLATE([ENABLE_ZLIB],
	    [Use zlib instead of builtin zlib decoder.])


dnl Test for libopenjpeg
AC_ARG_ENABLE(libopenjpeg,
	      AC_HELP_STRING([--disable-libopenjpeg],
	                     [Don't build against libopenjpeg.]),
              enable_libopenjpeg=$enableval,
              enable_libopenjpeg="try")
if test x$enable_libopenjpeg = xyes; then
  AC_CHECK_LIB([openjpeg], [opj_cio_open],,
	       AC_MSG_ERROR("*** libopenjpeg library not found ***"))
  AC_CHECK_HEADERS([openjpeg.h],,
		   AC_MSG_ERROR("*** libopenjpeg headers not found ***"))
elif test x$enable_libopenjpeg = xtry; then
  AC_CHECK_LIB([openjpeg], [opj_cio_open],
               [enable_libopenjpeg="yes"],
	       [enable_libopenjpeg="no"])
  AC_CHECK_HEADERS([openjpeg.h],,
		   [enable_libopenjpeg="no"])
fi

if test x$enable_libopenjpeg = xyes; then
  LIBOPENJPEG_LIBS="-lopenjpeg"
  AC_SUBST(LIBOPENJPEG_LIBS)
  AC_DEFINE(ENABLE_LIBOPENJPEG)
fi

AM_CONDITIONAL(BUILD_LIBOPENJPEG, test x$enable_libopenjpeg = xyes)
AH_TEMPLATE([ENABLE_LIBOPENJPEG],
	    [Use libopenjpeg instead of builtin jpeg2000 decoder.])



dnl Test for libjpeg
AC_ARG_ENABLE(libjpeg,
	      AC_HELP_STRING([--disable-libjpeg],
	                     [Don't build against libjpeg.]),
              enable_libjpeg=$enableval,
              enable_libjpeg="try")
if test x$enable_libjpeg != xno; then
  POPPLER_FIND_JPEG
fi

AM_CONDITIONAL(BUILD_LIBJPEG, test x$enable_libjpeg = xyes)
AH_TEMPLATE([ENABLE_LIBJPEG],
            [Use libjpeg instead of builtin jpeg decoder.])


dnl Check for freetype headers
FREETYPE_LIBS=
FREETYPE_CFLAGS=

PKG_CHECK_MODULES(FREETYPE, freetype2,
                  [freetype_pkgconfig=yes], [freetype_pkgconfig=no])

if test "x$freetype_pkgconfig" = "xyes"; then

  AC_DEFINE(HAVE_FREETYPE_H, 1, [Have FreeType2 include files])

else

  AC_PATH_PROG(FREETYPE_CONFIG, freetype-config, no)
  if test "x$FREETYPE_CONFIG" != "xno" ; then

    FREETYPE_CFLAGS=`$FREETYPE_CONFIG --cflags`
    FREETYPE_LIBS=`$FREETYPE_CONFIG --libs`
    AC_DEFINE(HAVE_FREETYPE_H, 1, [Have FreeType2 include files])

  fi

fi

AC_SUBST(FREETYPE_CFLAGS)
AC_SUBST(FREETYPE_LIBS)

PKG_CHECK_MODULES(FONTCONFIG, fontconfig >= 2.0.0)

AC_ARG_ENABLE(splash-output,
              AC_HELP_STRING([--disable-splash-output],
	                     [Don't build the Splash graphics backend.]),,
              enable_splash_output="yes")
AM_CONDITIONAL(BUILD_SPLASH_OUTPUT, test x$enable_splash_output = xyes)
AH_TEMPLATE([HAVE_SPLASH], [Use splash for rendering.])
if test x$enable_splash_output = xyes; then
  AC_DEFINE(HAVE_SPLASH)
fi

CAIRO_VERSION="1.4"
AC_SUBST(CAIRO_VERSION)
AC_ARG_ENABLE(cairo-output,
              AC_HELP_STRING([--disable-cairo-output],
                             [Don't build the cairo graphics backend.]),
              enable_cairo_output=$enableval,
              enable_cairo_output="try")
if test x$enable_cairo_output = xyes; then
  PKG_CHECK_MODULES(CAIRO, cairo >= $CAIRO_VERSION)
elif test x$enable_cairo_output = xtry; then
  PKG_CHECK_MODULES(CAIRO, cairo >= $CAIRO_VERSION,
                    [enable_cairo_output="yes"],
                    [enable_cairo_output="no"])
fi

AM_CONDITIONAL(BUILD_CAIRO_OUTPUT, test x$enable_cairo_output = xyes)
AH_TEMPLATE([HAVE_CAIRO], [Use cairo for rendering.])
if test x$enable_cairo_output = xyes; then
  AC_DEFINE(HAVE_CAIRO)
  CAIRO_FEATURE="#define POPPLER_HAS_CAIRO 1"
  CAIRO_REQ="cairo"
  AC_CHECK_HEADERS(fcntl.h sys/mman.h sys/stat.h)
else
  CAIRO_FEATURE="#undef POPPLER_HAS_CAIRO"
  CAIRO_REQ=""
fi
AC_SUBST(CAIRO_FEATURE)
AC_SUBST(CAIRO_REQ)

POPPLER_GLIB_DISABLE_DEPRECATED=""
POPPLER_GLIB_DISABLE_SINGLE_INCLUDES=""

GLIB_REQUIRED=2.6
AC_ARG_ENABLE(poppler-glib,
              AC_HELP_STRING([--disable-poppler-glib],
	                     [Don't compile poppler glib wrapper.]),
              enable_poppler_glib=$enableval,
              enable_poppler_glib="try")
if test x$enable_poppler_glib = xyes; then
  PKG_CHECK_MODULES(POPPLER_GLIB, glib-2.0 >= $GLIB_REQUIRED gobject-2.0 >= $GLIB_REQUIRED)
elif test x$enable_poppler_glib = xtry; then
  PKG_CHECK_MODULES(POPPLER_GLIB, glib-2.0 >= $GLIB_REQUIRED gobject-2.0 >= $GLIB_REQUIRED, 
                    [enable_poppler_glib="yes"],
                    [enable_poppler_glib="no"])
fi
if test x$enable_poppler_glib = xyes; then
  AC_PATH_PROG([GLIB_MKENUMS],[glib-mkenums])
  POPPLER_GLIB_DISABLE_DEPRECATED="$POPPLER_GLIB_DISABLE_DEPRECATED -DG_DISABLE_DEPRECATED"
  POPPLER_GLIB_DISABLE_SINGLE_INCLUDES="$POPPLER_GLIB_DISABLE_SINGLE_INCLUDES -DG_DISABLE_SINGLE_INCLUDES"
fi
AM_CONDITIONAL(BUILD_POPPLER_GLIB, test x$enable_poppler_glib = xyes)

GTK_DOC_CHECK([1.0])

dnl
dnl GDK
dnl
AC_ARG_ENABLE(gdk,
              AC_HELP_STRING([--disable-gdk],
	                     [Compile without gdk.]),
              enable_gdk=$enableval,
	      enable_gdk="try")

if test x$enable_poppler_glib = xyes; then
  if test x$enable_gdk = xyes; then
    PKG_CHECK_MODULES(GDK, gdk-2.0)
  elif test x$enable_gdk = xtry; then
    if test x$enable_splash_output = xyes; then
      PKG_CHECK_MODULES(GDK, gdk-2.0,
                        [enable_gdk="yes"],
                        [enable_gdk="no"])
    elif test x$enable_cairo_output = xyes; then
      PKG_CHECK_MODULES(GDK, gdk-2.0,
                        [enable_gdk="yes"],
			[enable_gdk="no"])
    fi
  elif test x$enable_splash_output = xyes; then
    PKG_CHECK_MODULES(GDK, gdk-2.0)
    AC_MSG_WARN("GDK is required by the glib wrapper when splash output device is
                 enabled. If you want to comppile popler without GDK use --disable-splash-output too")
    enable_gdk=yes
  fi
  if test x$enable_gdk = xyes; then
    AC_DEFINE(POPPLER_WITH_GDK,[1], [Have GDK])
    GDK_FEATURE="#define POPPLER_WITH_GDK 1"
    POPPLER_GLIB_DISABLE_DEPRECATED="$POPPLER_GLIB_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGDK_PIXBUF_DISABLE_DEPRECATED"
    POPPLER_GLIB_DISABLE_SINGLE_INCLUDES="$POPPLER_GLIB_DISABLE_SINGLE_INCLUDES -DGTK_DISABLE_SINGLE_INCLUDES"
  else
    GDK_FEATURE=""
  fi
fi
AM_CONDITIONAL(BUILD_WITH_GDK, test x$enable_gdk = xyes)
AC_SUBST(GDK_CFLAGS)
AC_SUBST(GDK_LIBS)
AC_SUBST(GDK_FEATURE)

AC_SUBST(POPPLER_GLIB_DISABLE_DEPRECATED)
AC_SUBST(POPPLER_GLIB_DISABLE_SINGLE_INCLUDES)

AC_ARG_ENABLE(poppler-qt,
	      AC_HELP_STRING([--disable-poppler-qt],
			     [Don't compile poppler qt wrapper.]),
	      enable_poppler_qt=$enableval,
	      enable_poppler_qt="try")

if test x$enable_poppler_qt = xyes; then
  POPPLER_FIND_QT(POPPLER_QT,,)
elif test x$enable_poppler_qt = xtry; then
  POPPLER_FIND_QT(POPPLER_QT,
		  [enable_poppler_qt="yes"],
                  [enable_poppler_qt="no"])
fi
AC_SUBST(POPPLER_QT_CXXFLAGS)
AC_SUBST(POPPLER_QT_LIBS)

AM_CONDITIONAL(BUILD_POPPLER_QT, test "x$enable_poppler_qt" = "xyes")

dnl 
dnl Try Qt4
dnl

AC_ARG_ENABLE(poppler-qt4,
              AC_HELP_STRING([--disable-poppler-qt4],
	                     [Don't compile poppler Qt4 wrapper.]),
              enable_poppler_qt4=$enableval,
              enable_poppler_qt4="try")
if test x$enable_poppler_qt4 = xyes; then
  PKG_CHECK_MODULES(POPPLER_QT4, 
	            QtCore >= 4.1.0 QtGui >= 4.1.0 QtXml >= 4.1.0)
  PKG_CHECK_MODULES(POPPLER_QT4_TEST, 
	            QtTest >= 4.1.0)
elif test x$enable_poppler_qt4 = xtry; then
  PKG_CHECK_MODULES(POPPLER_QT4,
                    QtCore >= 4.1.0 QtGui >= 4.1.0 QtXml >= 4.1.0,
                    [enable_poppler_qt4="yes"],
                    [enable_poppler_qt4="no"])
  if test x$enable_poppler_qt4 = xyes; then
    PKG_CHECK_MODULES(POPPLER_QT4_TEST,
                    QtTest >= 4.1.0,
                    [enable_poppler_qt4="yes"],
                    [enable_poppler_qt4="no"])
  fi
fi

AC_SUBST(POPPLER_QT4_CXXFLAGS)
AC_SUBST(POPPLER_QT4_LIBS)
AC_SUBST(POPPLER_QT4_TEST_LIBS)

if test x$enable_poppler_qt4 = xyes; then
  AC_MSG_CHECKING([for Qt4 moc])
  MOCQT4=`which moc`
  mocversion=`$MOCQT4 -v 2>&1`
  mocversiongrep=`echo $mocversion | grep "Qt 4"`
  if test x"$mocversiongrep" != x"$mocversion"; then
    # moc was not the qt4 one, try with moc-qt4
    MOCQT4=`which moc-qt4`
    mocversion=`$MOCQT4 -v 2>&1`
    mocversiongrep=`echo $mocversion | grep "Qt 4"`
    if test x"$mocversiongrep" != x"$mocversion"; then
      # no valid moc found
      enable_poppler_qt4=no;
      MOCQT4="not found"
    fi
  fi
  AC_SUBST(MOCQT4)
  AC_MSG_RESULT([$MOCQT4])
fi

AM_CONDITIONAL(BUILD_POPPLER_QT4, test "x$enable_poppler_qt4" = "xyes")

AC_ARG_ENABLE(gtk-test,
              AC_HELP_STRING([--disable-gtk-test],
	                     [Don't compile GTK+ test program.]),
              enable_gtk_test=$enableval,
              enable_gtk_test="try")
if test x$enable_gtk_test = xyes; then
  PKG_CHECK_MODULES(GTK_TEST, gtk+-2.0 >= 2.8.0 gdk-pixbuf-2.0 libglade-2.0 gthread-2.0)
elif test x$enable_gtk_test = xtry; then
  PKG_CHECK_MODULES(GTK_TEST, gtk+-2.0 >= 2.8.0 gdk-pixbuf-2.0 libglade-2.0 gthread-2.0,
                    [enable_gtk_test="yes"],
                    [enable_gtk_test="no"])
fi
AM_CONDITIONAL(BUILD_GTK_TEST, test x$enable_gtk_test = xyes -a x$enable_poppler_glib = xyes)

AC_ARG_ENABLE(abiword-output,
              AC_HELP_STRING([--disable-abiword-output],
                             [Don't build the abiword backend.]),
              enable_abiword_output=$enableval,
              enable_abiword_output="try")
if test x$enable_abiword_output = xyes; then
  PKG_CHECK_MODULES(ABIWORD, libxml-2.0)
elif test x$enable_abiword_output = xtry; then
  PKG_CHECK_MODULES(ABIWORD, libxml-2.0,
                    [enable_abiword_output="yes"],
                    [enable_abiword_output="no"])
fi

AM_CONDITIONAL(BUILD_ABIWORD_OUTPUT, test x$enable_abiword_output = xyes)

AC_ARG_ENABLE(utils,
              AC_HELP_STRING([--disable-utils],
	                     [Don't compile poppler command line utils.]),
              enable_utils=$enableval,
              enable_utils="yes")
AM_CONDITIONAL(BUILD_UTILS, test x$enable_utils = xyes)

AC_ARG_ENABLE(compile-warnings,
              AC_HELP_STRING([--enable-compile-warnings=@<:@no/yes/kde@:>@]
                             [Turn on compiler warnings.]),,
              [enable_compile_warnings="yes"])

if test "x$GCC" != xyes; then
  enable_compile_warnings=no
fi
case "$enable_compile_warnings" in
  no)	;;
  yes)	CXXFLAGS="-Wall -Wno-write-strings $CXXFLAGS" ;;
  kde)	CXXFLAGS="-Wnon-virtual-dtor -Wno-long-long -Wundef -ansi \
		-D_XOPEN_SOURCE=500 -D_BSD_SOURCE -Wcast-align \
		-Wconversion -Wchar-subscripts -Wall -W -Wpointer-arith \
		-Wwrite-strings -O2 -Wformat-security \
		-Wmissing-format-attribute -fno-exceptions -fno-check-new \
		-fno-common $CXXFLAGS" ;;
esac

case $($PKG_CONFIG --version) in
  0.?|0.1[0-7])
    PC_REQUIRES_PRIVATE="";
    PC_REQUIRES="poppler = $VERSION";;
  *)
    PC_REQUIRES_PRIVATE="Requires.private: poppler = $VERSION";
    PC_REQUIRES="";;
esac

AC_SUBST(PC_REQUIRES)
AC_SUBST(PC_REQUIRES_PRIVATE)

AC_OUTPUT([
Makefile
goo/Makefile
fofi/Makefile
splash/Makefile
poppler/Makefile
utils/Makefile
glib/Makefile
glib/poppler-features.h
glib/reference/Makefile
glib/demo/Makefile
test/Makefile
qt/Makefile
qt4/Makefile
qt4/src/Makefile
qt4/tests/Makefile
qt4/demos/Makefile
poppler.pc
poppler-cairo.pc
poppler-splash.pc
poppler-glib.pc
poppler-qt.pc
poppler-qt4.pc])


echo ""
echo "Building poppler with support for:"
echo "  splash output:      $enable_splash_output"
echo "  cairo output:       $enable_cairo_output"
echo "  abiword output:     $enable_abiword_output"
echo "  qt wrapper:         $enable_poppler_qt"
echo "  qt4 wrapper:        $enable_poppler_qt4"
echo "  glib wrapper:       $enable_poppler_glib"
echo "    use GDK:          $enable_gdk"
echo "  use gtk-doc:        $enable_gtk_doc"
echo "  use libjpeg:        $enable_libjpeg"
echo "  use zlib:           $enable_zlib"
echo "  use libopenjpeg:    $enable_libopenjpeg"
echo "  command line utils: $enable_utils"
echo ""

if test x$enable_splash_output = xno -a x$enable_cairo_output = xno; then
	echo "  Warning: There is no rendering backend enabled"
fi

if test x$enable_libjpeg != xyes; then
	echo "  Warning: Using libjpeg is recommended"
fi

if test x$enable_zlib != xno; then
	echo "  Warning: Using zlib is not totally safe"
fi

if test x$enable_libopenjpeg != xyes; then
	echo "  Warning: Using libopenjpeg is recommended"
fi

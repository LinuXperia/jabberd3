# jabberd2 configure script
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(jabberd, 2.1, jabberd2@xiaoka.com)
AC_CONFIG_SRCDIR([sx/sx.h])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
# libtool for internal linking
AC_DISABLE_STATIC
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

# extra paths
AC_ARG_WITH([extra_include_path], AC_HELP_STRING([--with-extra-include-path],
                                           [use additional include paths]),
            extra_include_path=$withval)
split_includes="`echo $extra_include_path | sed -e 's/:/ /g'`"
for incpath in $split_includes ; do
    CPPFLAGS="-I$incpath $CPPFLAGS"
done

AC_ARG_WITH([extra_library_path], AC_HELP_STRING([--with-extra-library-path],
                                               [use additional library paths]),
            extra_library_path=$withval)
split_libs="`echo $extra_library_path | sed -e 's/:/ /g'`"
for libpath in $split_libs ; do
    LDFLAGS="-L$libpath $LDFLAGS"
done

# developer flags
AC_ARG_ENABLE([developer], AC_HELP_STRING([--enable-developer], [Compile with warnings and debugging symbols]),
              CFLAGS="-Wall -g $CFLAGS")
# debugging
AC_ARG_ENABLE(debug, AC_HELP_STRING([--enable-debug], [enable debug messages]),
              want_debug=$enableval, want_debug=no)
if test "x-$want_debug" = "x-yes" ; then
    AC_DEFINE(DEBUG,1,[Define to 1 if you want to get debug output with -D.])
    AC_DEFINE(SX_DEBUG,1,[Define to 1 if you want to get SX debug output with -D.])
fi

AC_ARG_ENABLE(nad_debug, AC_HELP_STRING([--enable-nad-debug], [enable NAD pointer tracking]),
              want_nad_debug=$enableval, want_nad_debug=no)
if test "x-$want_nad_debug" = "x-yes" ; then
    AC_DEFINE(NAD_DEBUG,1,[Define to 1 if you want to enable NAD pointer tracking.])
fi

AC_ARG_ENABLE(pool_debug, AC_HELP_STRING([--enable-pool-debug], [enable memory pool statistics]),
              want_pool_debug=$enableval, want_pool_debug=no)
if test "x-$want_pool_debug" = "x-yes" ; then
    AC_DEFINE(POOL_DEBUG,1,[Define to 1 if you want to enable memory pool statistics.])
fi

AC_ARG_ENABLE(mio_debug, AC_HELP_STRING([--enable-mio-debug], [enable managed IO debug output]),
              want_mio_debug=$enableval, want_mio_debug=no)
if test "x-$want_mio_debug" = "x-yes" ; then
    AC_DEFINE(MIO_DEBUG,1,[Define to 1 if you want to enable managed IO debug output.])
fi

# Two-step header checking.  First check for headers which don't
# require any other headers.
AC_HEADER_DIRENT
AC_HEADER_RESOLV
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_HEADER_TIME
AC_CHECK_HEADERS([arpa/inet.h \
                  arpa/nameser.h \
                  fcntl.h \
                  netinet/in.h \
                  signal.h \
                  stdint.h \
                  stdlib.h \
                  string.h \
                  sys/filio.h \
                  sys/ioctl.h \
                  sys/socket.h \
                  sys/time.h \
                  sys/timeb.h \
                  sys/types.h \
                  sys/stat.h \
                  sys/utsname.h \
                  syslog.h \
                  unistd.h \
                  windows.h \
                  winsock2.h \
])
 
# Now check for those headers that do, including all the required
# headers.
AC_CHECK_HEADERS([resolv.h windns.h],,,
[#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifdef HAVE_WINSOCK2_H
# include <winsock2.h>
#endif
#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_STRUCT_TIMEZONE
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# generic system types 
AC_CREATE_STDINT_H(ac-stdint.h) 

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_REPLACE_FNMATCH
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_FUNC_SELECT_ARGTYPES
AC_CHECK_FUNCS([close \
                dup2 \
                fcntl \
                _findfirst \
                gethostname \
                getopt \
                getpagesize \
                getpid \
                gettimeofday \
                inet_aton \
                inet_ntoa \
                ioctl \
                isascii \
                memchr \
                memmove \
                memset \
                mkdir \
                _mkdir \
                modf \
                select \
                setenv \
                sleep \
                Sleep \
                socket \
                strcasecmp \
                strchr \
                strdup \
                strerror \
                stricmp \
                strncasecmp \
                strndup \
                strnicmp \
                strstr \
                tzset \
                uname \
])

AC_CHECK_FUNC([connect], ,[AC_CHECK_LIB([socket], [connect])])
AC_CHECK_LIB([ws2_32], [_head_libws2_32_a])

AC_CHECK_FUNC(gethostbyname, ,[AC_CHECK_LIB([resolv], [gethostbyname])])
AC_CHECK_FUNC(gethostbyname, ,[AC_CHECK_LIB([nsl], [gethostbyname])])
if test "x$ac_cv_lib_nsl_gethostbyname" != "xyes" && test "x$ac_cv_func_gethostbyname" != "xyes" ; then
  AC_CHECK_FUNC([gethostbyname], , [AC_CHECK_LIB([socket], [gethostbyname])])
fi
if test "$ac_cv_lib_nsl_gethostbyname" = "$ac_cv_func_gethostbyname" ; then
  AC_MSG_CHECKING([if we can include libnsl + libsocket])
  LIBS="-lnsl -lsocket $LIBS"
  AC_LINK_IFELSE([AC_LANG_PROGRAM([[]], [[(void) gethostbyname]])],[my_ac_link_result=yes],[my_ac_link_result=no ])
  if test "$my_ac_link_result" = "no" ; then
    AC_MSG_RESULT([failure])
    AC_MSG_ERROR([unable to use gethostbyname()])
  else
    AC_MSG_RESULT([success])
  fi
fi

# windows has different names for a few basic things
if test "x-$ac_cv_func_getpid" != "x-yes" -a "x-$ac_cv_func__getpid" = "x-yes" ; then
    AC_DEFINE(getpid,_getpid,[Define to a function than can provide getpid(2) functionality.])
fi

if test "x-$ac_cv_func_sleep" != "x-yes" -a "x-$ac_cv_func_Sleep" = "x-yes" ; then
    AC_DEFINE(sleep,Sleep,[Define to a function than can provide sleep(2) functionality.])
fi

if test "x-$ac_cv_func_strcasecmp" != "x-yes" -a "x-$ac_cv_func_stricmp" = "x-yes" ; then
    AC_DEFINE(strcasecmp,stricmp,[Define to a function than can provide strcasecmp(3) functionality.])
fi

if test "x-$ac_cv_func_strncasecmp" != "x-yes" -a "x-$ac_cv_func_strnicmp" = "x-yes" ; then
    AC_DEFINE(strncasecmp,strnicmp,[Define to a function than can provide strncasecmp(3) functionality.])
fi

# winsock substitutions
if test "x-$ac_cv_func_close" != "x-yes" ; then
    AC_MSG_CHECKING(for closesocket)
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <winsock2.h>]],
                                    [[closesocket(0)]])],
                   [AC_MSG_RESULT(yes)
                    AC_DEFINE(close,closesocket,[Define to a function than can provide close(2) functionality.])],
                    AC_DEFINE(HAVE_CLOSE,1,[Define to 1 if you have the 'close' function.])
                   AC_MSG_RESULT(no))
fi

if test "x-$ac_cv_func_ioctl" != "x-yes" ; then
    AC_MSG_CHECKING(for ioctlsocket)
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <winsock2.h>]],
                                    [[ioctlsocket(0,0,0)]])],
                   [AC_MSG_RESULT(yes)
                    AC_DEFINE(ioctl,ioctlsocket,[Define to a function than can provide ioctl(2) functionality.])],
                    AC_DEFINE(HAVE_IOCTL,1,[Define to 1 if you have the 'ioctl' function.])
                   AC_MSG_RESULT(no))
fi

# res_query has been seen in libc, libbind and libresolv
if test "x-$ac_cv_header_resolv_h" = "x-yes" ; then
    AC_CHECK_FUNCS(res_query)
    if test "x-$ac_cv_func_res_query" = "x-yes" ; then
        have_res_query=yes
    else
        AC_CHECK_LIB(resolv, res_query)
        if test "x-$ac_cv_lib_resolv_res_query" = "x-yes" ; then
            have_res_query=yes
        else
            AC_CHECK_LIB(bind, res_query)
            if test "x-$ac_cv_lib_bind_res_query" = "x-yes" ; then
                have_res_query=yes
            else
                dnl some glibcs have res_query as a macro, so work around it
                AC_MSG_CHECKING([for res_query in -lresolv (alternate version)])
                save_libs="$LIBS"
                LIBS="-lresolv $LIBS"
                AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <resolv.h>]],
                                                [[res_query(0,0,0,0,0)]])],
                               [AC_MSG_RESULT(yes)
                                have_res_query=yes],
                               [AC_MSG_RESULT(no)
                                LIBS="$save_libs"])
            fi
        fi
    fi
fi
# windows calls it DnsQuery
if test "x-$ac_cv_header_windns_h" = "x-yes" ; then
    AC_MSG_CHECKING([for DnsQuery in -ldnsapi])
    save_libs="$LIBS"
    LIBS="-ldnsapi $LIBS"
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <windows.h>
                                      #include <windns.h>]],
                                    [[DnsQuery(0,0,0,0,0,0)]])],
                   [AC_MSG_RESULT(yes)
                    have_dnsquery=yes],
                   [AC_MSG_RESULT(no)
                    LIBS="$save_libs"])
fi
if test "x-$have_res_query" = "x-yes" ; then
    AC_DEFINE(HAVE_RES_QUERY,1,[Define to 1 if you have the 'res_query' function.])
elif test "x-$have_dnsquery" = "x-yes" ; then
    AC_DEFINE(HAVE_DNSQUERY,1,[Define to 1 if you have the 'DnsQuery' function.])
else
    AC_MSG_ERROR([no DNS resolver interface (res_query or DnsQuery) found])
fi

# inet_ntop/inet_pton have been seen in -lnsl, and sometimes not at all
AC_CHECK_FUNC(inet_ntop, ,[AC_CHECK_LIB(nsl, inet_ntop)])
unset ac_cv_func_inet_ntop
AC_CHECK_FUNCS(inet_ntop inet_pton)

# some glibcs have broken sockaddr_storage members
if test "x-$ac_cv_type_struct_sockaddr_storage" = "x-yes" ; then
    AC_MSG_CHECKING(for broken __ss_family member in struct sockaddr_storage)
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM(_IP6_INCLUDES,
                                       [[do {
                                             struct sockaddr_storage s;
                                             s.__ss_family = 0;
                                         } while(0)]])],
                      [AC_MSG_RESULT(yes)
                       AC_DEFINE(ss_family, __ss_family,
                                 [Define to '__ss_family' if 'struct sockaddr_storage' defines '__ss_family' instead of 'ss_family'.])],
                      AC_MSG_RESULT(no))

    AC_MSG_CHECKING(for broken __ss_len member in struct sockaddr_storage)
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM(_IP6_INCLUDES,
                                       [[do {
                                             struct sockaddr_storage s;
                                             s.__ss_len = 0;
                                         } while(0)]])],
                      [AC_MSG_RESULT(yes)
                       AC_DEFINE(ss_len, __ss_len,
                                 [Define to '__ss_len' if 'struct sockaddr_storage' defines '__ss_len' instead of 'ss_len'.])],
                      AC_MSG_RESULT(no))
fi


# syslog
if test "x-$ac_cv_header_syslog_h" = "x-yes" ; then
    AC_CHECK_FUNCS(syslog vsyslog)
fi

if test "x-$ac_cv_header_windows_h" = "x-yes" ; then
    AC_MSG_CHECKING(for ReportEvent)
    AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <windows.h>]],
                                    [[ReportEvent(0,0,0,0,0,0,0,0,0)]])],
                   [AC_MSG_RESULT(yes)
                    AC_DEFINE(HAVE_REPORTEVENT,1,[Define to 1 if you have the 'ReportEvent' function.])],
                   AC_MSG_RESULT(no))
fi

# snprintf/vsnprintf don't exist everywhere. additionally, we require
# them to gracefully accept NULLs, which is non-standard
AC_CHECK_FUNCS(snprintf vsnprintf)
if test "x-$ac_cv_func_snprintf" = "x-yes" ; then
    AC_MSG_CHECKING([if snprintf can handle NULL arguments])
    AC_RUN_IFELSE([AC_LANG_SOURCE([[#include <stdio.h>
                                    #include <signal.h>
                                    segv() { exit(1); }
                                    main() { char b[10]; signal(SIGSEGV,segv); snprintf(b,10,"%s",NULL); exit(0); }]])],
                  AC_MSG_RESULT(yes),
                  [AC_MSG_RESULT(no)
                   AC_DEFINE(HAVE_BROKEN_SNPRINTF,1,[Define to 1 if 'snprintf' cannot handle NULL arguments.])])
fi
if test "x-$ac_cv_func_vsnprintf" = "x-yes" ; then
    AC_MSG_CHECKING([if vsnprintf can handle NULL arguments])
    AC_RUN_IFELSE([AC_LANG_SOURCE([[#include <stdio.h>
                                    #include <signal.h>
                                    #include <stdarg.h>
                                    segv() { exit(1); }
                                    expand(char *f,...) { va_list ap; char b[10]; va_start(ap,f); vsnprintf(b,10,f,ap); va_end(ap); }
                                    main() { char b[10]; signal(SIGSEGV,segv); expand("%s", NULL); exit(0); }]])],
                  AC_MSG_RESULT(yes),
                  [AC_MSG_RESULT(no)
                   AC_DEFINE(HAVE_BROKEN_VSNPRINTF,1,[Define to 1 if 'vsnprintf' cannot handle NULL arguments.])])
fi

#
# Checks for libraries.

# Expat
AC_CHECK_LIB([expat], [XML_ParserCreate])
if test "x-$ac_cv_lib_expat_XML_ParserCreate" = "x-" ; then
    AC_MSG_ERROR([Expat not found])
fi

# libidn >= 0.3.0
AC_CHECK_LIB(idn, stringprep_check_version)
if test "x-$ac_cv_lib_idn_stringprep_check_version" = "x-yes" ; then
    AC_MSG_CHECKING(for Libidn version >= 0.3.0)
    AC_RUN_IFELSE([AC_LANG_PROGRAM([[#include <stringprep.h>]],
                                   [[return !(stringprep_check_version("0.3.0"))]])],
                  [AC_MSG_RESULT(yes)
                    have_idn=yes],
                  AC_MSG_RESULT(no))
fi
if test "x-$have_idn" = "x-" ; then
    AC_MSG_ERROR([Libidn >= 0.3.0 not found])
fi

# GnuSASL or Cyrus-SASL
AC_ARG_ENABLE(gsasl, AC_HELP_STRING([--enable-gsasl], [enable GnuSASL support (yes)]), want_gsasl=$enableval, want_gsasl=yes)
AC_ARG_ENABLE(cyrus, AC_HELP_STRING([--enable-cyrus], [enable Cyrus-SASL support (no)]), want_cyrus=$enableval, want_cyrus=no)
if test "x-$want_gsasl" != "x-yes" -a "x-$want_cyrus" != "x-yes" ; then
    AC_MSG_ERROR([GnuSASL or Cyrus-SASL library is required])
fi
if test "x-$want_gsasl" = "x-yes" ; then
    if test "x-$want_cyrus" = "x-yes" ; then
        AC_MSG_WARN(Requested both GnuSASL and Cyrus-SASL. Choosing GnuSASL.)
        want_cyrus=no
    fi
    AC_CHECK_HEADERS(gsasl.h)
    if test "x-$ac_cv_header_gsasl_h" = "x-yes" ; then
        AC_CHECK_LIB(gsasl, gsasl_check_version)
    fi
    if test "x-$ac_cv_lib_gsasl_gsasl_check_version" = "x-yes" ; then
        AC_MSG_CHECKING(for GnuSASL version >= 0.2.14)
        AC_RUN_IFELSE([AC_LANG_PROGRAM([[#include <gsasl.h>]],
                                       [[return !(gsasl_check_version("0.2.14"))]])],
                      [AC_MSG_RESULT(yes)
                        have_gsasl=yes],
                      AC_MSG_RESULT(no))
    fi
fi

if test "x-$want_cyrus" = "x-yes" ; then
    AC_CHECK_LIB(sasl2, sasl_client_init)
    if test "x-$ac_cv_lib_sasl2_sasl_client_init" = "x-yes" ; then
        AC_CHECK_HEADERS(sasl/sasl.h)
        if test "x-$ac_cv_header_sasl_sasl_h" = "x-yes" ; then
            have_cyrus=yes
        fi
    fi
fi

if test "x-$want_gsasl" = "x-yes" -a "x-$have_gsasl" != "x-yes" ; then
    AC_MSG_ERROR([GnuSASL >= 0.2.14 not found])
fi
if test "x-$want_cyrus" = "x-yes" -a "x-$have_cyrus" != "x-yes" ; then
    AC_MSG_ERROR([Cyrus-SASL not found])
fi
AM_CONDITIONAL(SASL_GSASL, [test "x-$have_gsasl" = "x-yes"])
AM_CONDITIONAL(SASL_CYRUS, [test "x-$have_cyrus" = "x-yes"])

#
# optional libs

# OpenSSL >= 0.9.6b
AC_ARG_ENABLE(ssl, AC_HELP_STRING([--enable-ssl], [enable SSL/TLS support (yes)]), want_ssl=$enableval, want_ssl=yes)
if test "x-$want_ssl" = "x-yes" ; then
    AC_CHECK_HEADERS(openssl/crypto.h)
    if test "x-$ac_cv_header_openssl_crypto_h" = "x-yes" ; then
        AC_CHECK_LIB(crypto, CRYPTO_lock)
    fi
    if test "x-$ac_cv_lib_crypto_CRYPTO_lock" = "x-yes" ; then
        AC_CHECK_HEADERS(openssl/ssl.h)
    fi
    if test "x-$ac_cv_header_openssl_ssl_h" = "x-yes" ; then
        AC_CHECK_LIB(ssl, SSL_connect)
    fi
    if test "x-$ac_cv_lib_ssl_SSL_connect" = "x-yes" ; then
        AC_MSG_CHECKING(for OpenSSL version >= 0.9.6b)
        AC_RUN_IFELSE([AC_LANG_PROGRAM([[#include <openssl/opensslv.h>]],
                                       [[return !(SSLeay() >= 0x000906020L)]])],
                      [AC_MSG_RESULT(yes)
                       have_openssl=yes],
                      AC_MSG_RESULT(no))
    fi
    if test "x-$have_openssl" = "x-" ; then
        AC_MSG_ERROR([OpenSSL >= 0.9.6b not found])
    fi

    AC_DEFINE(HAVE_SSL,1,[Define to 1 if OpenSSL is available.])
fi

# MySQL
AC_ARG_ENABLE([mysql], 
        AC_HELP_STRING([--enable-mysql], [enable MySQL auth/reg/storage support (no)]),
        [enable_mysql=$enableval have_mysql=no],
        [enable_mysql=no         have_mysql=no])
if test "x-$enable_mysql" = "x-yes" ; then
        for i in /usr /usr/local /usr/local/mysql ; do
                for j in include include/mysql "" ; do
                        if test -r "$i/$j/mysql.h" ; then
                                MYSQL_INCLUDE=$i/$j
                        fi
                done
                for j in lib lib/mysql lib64 lib64/mysql "" ; do
                        if test -f "$i/$j/libmysqlclient.so" || test -f "$i/$j/libmysqlclient.a" ; then
                                MYSQL_LIBDIR=$i/$j
                        fi
                done
        done
        AC_CHECK_LIB([mysqlclient], [mysql_init], [
                have_mysql=yes
                if test "x-$MYSQL_INCLUDE" != "x-" ; then
                        MYSQL_CFLAGS="-I $MYSQL_INCLUDE"
                fi
                if test "x-$MYSQL_LIBDIR" != "x-" ; then
                        MYSQL_LIBS="-L$MYSQL_LIBDIR -lmysqlclient"
                fi
                AC_DEFINE(STORAGE_MYSQL, 1, [Define to 1 if you want to use MySQL for storage.])
        ])
        if test "x-$have_mysql" != "x-yes" ; then
                AC_MSG_ERROR([MySQL support requested, but headers/libraries not found.])
        fi
fi
AC_SUBST(MYSQL_CFLAGS)
AC_SUBST(MYSQL_LIBS)
AM_CONDITIONAL(STORAGE_MYSQL, [test "x-$have_mysql" = "x-yes"])

# PostgreSQL
AC_ARG_ENABLE([pgsql],
        AC_HELP_STRING([--enable-pgsql], [enable PostgreSQL auth/reg/storage support (no)]),
        [enable_pgsql=$enableval have_pgsql=no],
        [enable_pgsql=no         have_pgsql=no])
if test "x-$enable_pgsql" = "x-yes" ; then
        for i in /usr /usr/local /usr/local/pgsql ; do
                for j in include include/pgsql include/postgres include/postgresql "" ; do
                        if test -r "$i/$j/libpq-fe.h" ; then
                                PGSQL_INCLUDE=$i/$j
                        fi
                done
                for lib in lib lib64 ; do
                        for j in $lib $lib/pgsql $lib/postgres $lib/postgresql "" ; do
                                if test -f "$i/$j/libpq.so" || test -f "$i/$j/libpq.a" ; then
                                        PGSQL_LIBDIR=$i/$j
                                fi
                        done
                done
        done
        AC_CHECK_LIB([pq], [PQsetdbLogin], [
                have_pgsql=yes
                if test "x-$PGSQL_INCLUDE" != "x-"; then
                        PGSQL_CFLAGS="-I $PGSQL_INCLUDE"
                fi
                if test "x-$PGSQL_LIBDIR" != "x-"; then
                        PGSQL_LIBS="-L$PGSQL_LIBDIR -lpq"
                fi
                AC_DEFINE(STORAGE_POSTGRES, 1, [Define to 1 if you want to use PostgreSQL for storage.])
        ])
        if test "x-$have_pgsql" != "x-yes" ; then
                AC_MSG_ERROR([PostgreSQL support requested, but headers/libraries not found.])
        fi
fi
AC_SUBST(PGSQL_CFLAGS)
AC_SUBST(PGSQL_LIBS)
AM_CONDITIONAL(STORAGE_PGSQL, [test "x-$have_pgsql" = "x-yes"])

# SQLite 3
AC_ARG_ENABLE([sqlite],
        AS_HELP_STRING([--enable-sqlite], [enable SQLite3 auth/reg/storage support (no)]),
        [enable_sqlite=$enableval have_sqlite=no],
        [enable_sqlite=no         have_sqlite=no])
if test "x-$enable_sqlite" = "x-yes" ; then
    AC_CHECK_HEADERS([sqlite3.h], [
                AC_CHECK_LIB([sqlite3], [sqlite3_open], [
                        have_sqlite=yes
                        SQLITE_LIBS="-lsqlite3"
                        AC_DEFINE(STORAGE_SQLITE, 1, [Define to 1 if you want to use SQLite 3 for storage.])
                ])
        ])
        if test "x-$have_sqlite" != "x-yes" ; then
                AC_MSG_ERROR([SQLite3 support requested, but headers/libraries not found.])
        fi
fi
AC_SUBST(SQLITE_LIBS)
AM_CONDITIONAL(STORAGE_SQLITE, [test "x-$have_sqlite" = "x-yes"])

# Berkeley DB
AC_ARG_ENABLE(db, AC_HELP_STRING([--enable-db], [enable Berkeley DB auth/reg/storage support (no)]),
              want_db=$enableval, want_db=no)
if test "x-$want_db" = "x-yes" ; then
    AC_CHECK_HEADERS(db.h)
    if test "x-$ac_cv_header_db_h" = "x-yes" ; then
        for lib in db-4.3 db-4.2 db-4.1 db-4 db4 db41 db ; do
            if test "x-$have_db_version" != "x-yes" ; then
                AC_MSG_CHECKING([for db_create in -l$lib])
                save_libs="$LIBS"
                LIBS="-l$lib $LIBS"
                AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <db.h>]],
                                                [[db_create(0,0,0)]])],
                               [AC_MSG_RESULT(yes)
                                AC_MSG_CHECKING(for Berkeley DB version >= 4.1.25)
                                AC_RUN_IFELSE([AC_LANG_PROGRAM([[#include <db.h>]],
                                                               [[do {
                                                                     int major, minor, patch;
                                                                     db_version(&major, &minor, &patch);
                                                                     if(major < 4 ||
                                                                         (major == 4 && minor < 1) ||
                                                                         (major == 4 && minor == 1 && patch < 24))
                                                                         return 1;
                                                                 } while(0)]])],
                                              [AC_MSG_RESULT(yes)
                                               have_db_version=yes],
                                              AC_MSG_RESULT(no))],
                                AC_MSG_RESULT(no))
                if test "x-$have_db_version" = "x-" ; then
                    LIBS="$save_libs"
                fi
            fi
        done
    fi
    if test "x-$have_db_version" != "x-yes" ; then
        AC_MSG_ERROR([Berkeley DB >= 4.1.24 not found])
    else
        DB_LIBS="-l$lib"
        AC_DEFINE(STORAGE_DB,1,[Define to 1 if you want to use Berkeley DB for auth/reg/storage.])
    fi
fi
AC_SUBST(DB_LIBS)
AM_CONDITIONAL(STORAGE_DB, [test "x-$have_db_version" = "x-yes"])

# Oracle
AC_ARG_WITH(oracle-home,
        [ --with-oracle-home=DIR   the Oracle home directory, for includes and libs. ],
        [ ac_oracle_home="$withval" ])

AC_ARG_ENABLE(oracle, AC_HELP_STRING([--enable-oracle], [enable Oracle auth/reg/storage support (no)]),
              want_oracle=$enableval, want_oracle=no)
if test "x-$want_oracle" = "x-yes" ; then
    AC_CHECK_HEADERS(oci.h)
    if test "x-$ac_cv_header_oci_h" != "x-yes" ; then
       if test -n $ac_oracle_home ; then
          AC_MSG_CHECKING([for oci.h in $ac_oracle_home])
                save_cppflags="$CPPFLAGS"
                CPPFLAGS="-I$ac_oracle_home/rdbms/demo -I$ac_oracle_home/rdbms/public $CPPFLAGS"
                save_libs="$LIBS"
                LIBS="-L$ac_oracle_home/lib $LIBS"
                save_ldflags="$LDFLAGS"
                LDFLAGS="-Wl,-rpath,$ac_oracle_home/lib $LDFLAGS"
                AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <oci.h>]])],
                                  [AC_MSG_RESULT(yes)
                                   AC_DEFINE(HAVE_OCI_H,,[Define if you have oci.h])
                                   ac_cv_header_oci_h=yes],
                                  AC_MSG_RESULT(no))
                if test "x-$ac_cv_header_oci_h" != "x-yes" ; then
                    CPPFLAGS="$save_cppflags"
                    LIBS="$save_libs"
                    LDFLAGS="$save_ldflags"
                fi
       fi
    fi
    if test "x-$ac_cv_header_oci_h" = "x-yes" ; then
        AC_CHECK_LIB(clntsh, OCIInitialize)
    fi
    if test "x-$ac_cv_lib_clntsh_OCIInitialize" != "x-yes" ; then
        AC_MSG_ERROR([Oracle client libraries not found])
    else
        have_oracle="yes"
        ORACLE_CPPFLAGS="-I$ac_oracle_home/rdbms/demo -I$ac_oracle_home/rdbms/public"
        ORACLE_LIBS="-L$ac_oracle_home/lib"
        ORACLE_LDFLAGS="-Wl,-rpath,$ac_oracle_home/lib"
        AC_DEFINE(STORAGE_ORACLE,1,[Define to 1 if you want to use Oracle for auth/reg/storage.])
    fi
fi
AC_SUBST(ORACLE_CPPFLAGS)
AC_SUBST(ORACLE_LIBS)
AC_SUBST(ORACLE_LDFLAGS)
AM_CONDITIONAL(STORAGE_ORACLE, [test "x-$have_oracle" = "x-yes"])

# OpenLDAP
AC_ARG_ENABLE(ldap, AC_HELP_STRING([--enable-ldap], [enable OpenLDAP auth/reg support (no)]),
              want_ldap=$enableval, want_ldap=no)
if test "x-$want_ldap" = "x-yes" ; then
    AC_CHECK_HEADERS(lber.h ldap.h)
    if test "x-$ac_cv_header_ldap_h" = "x-yes" -a "x-$ac_cv_header_lber_h" = "x-yes" ; then
        AC_CHECK_LIB(lber, ber_alloc)
        AC_CHECK_LIB(ldap, ldap_init)
    fi
    if test "x-$ac_cv_lib_lber_ber_alloc" = "x-yes" -a "x-$ac_cv_lib_ldap_ldap_init" = "x-yes" ; then
        AC_MSG_CHECKING(for OpenLDAP version >= 2.1.0)
        AC_RUN_IFELSE([AC_LANG_PROGRAM([[#include <lber.h>
                                         #include <ldap.h>]],
                                       [[do {
                                             LDAPAPIInfo info;
                                             info.ldapai_info_version = LDAP_API_INFO_VERSION;
                                             ldap_get_option(0, LDAP_OPT_API_INFO, &info);
                                             if(info.ldapai_vendor_version != LDAP_VENDOR_VERSION || LDAP_VENDOR_VERSION < 2004)
                                                 return 1;
                                         } while(0)]])],
                                       [AC_MSG_RESULT(yes)
                                        have_ldap_version=yes],
                                       AC_MSG_RESULT(no))
    fi
    if test "x-$have_ldap_version" = "x-" ; then
        AC_MSG_ERROR([OpenLDAP client libraries >= 2.1.0 not found])
    else
        LDAP_LIBS="-llber -lldap"
        AC_DEFINE(STORAGE_LDAP,1,[Define to 1 if you want to use OpenLDAP for auth/reg.])
    fi
fi
AC_SUBST(LDAP_LIBS)
AM_CONDITIONAL(STORAGE_LDAP, [test "x-$have_ldap_version" = "x-yes"])

# Plugabble Authentication Modules (PAM)
AC_ARG_ENABLE(pam, AC_HELP_STRING([--enable-pam], [enable PAM auth/reg support (no)]),
              want_pam=$enableval, want_pam=no)
if test "x-$want_pam" = "x-yes" ; then
    AC_CHECK_HEADERS(security/pam_appl.h)
    if test "x-$ac_cv_header_security_pam_appl_h" = "x-yes" ; then
        AC_CHECK_LIB(pam, pam_start)
    fi
    if test "x-$ac_cv_lib_pam_pam_start" != "x-yes" ; then
        AC_MSG_ERROR([PAM application libraries not found])
    else
        have_pam="yes"
        PAM_LIBS="-lpam"
        AC_DEFINE(STORAGE_PAM,1,[Define to 1 if you want to use PAM for auth/reg.])
    fi
fi
AC_SUBST(PAM_LIBS)
AM_CONDITIONAL(STORAGE_PAM, [test "x-$have_pam" = "x-yes"])

# pipe (not really an external package, but does need some checks)
AC_ARG_ENABLE(pipe, AC_HELP_STRING([--enable-pipe], [enable pipe auth/reg support (no)]),
              want_pipe=$enableval, want_pipe=no)
if test "x-$want_pipe" = "x-yes" ; then
    AC_CHECK_HEADERS(sys/wait.h)
    AC_FUNC_FORK
    AC_CHECK_FUNCS(pipe wait)
    if test "x-$ac_cv_header_sys_wait_h" != "x-yes" -o \
            "x-$ac_cv_func_fork" != "x-yes" -o \
            "x-$ac_cv_func_pipe" != "x-yes" -o \
            "x-$ac_cv_func_wait" != "x-yes" ; then
        AC_MSG_ERROR([Pipe auth/reg requirements (sys/wait.h, fork(), pipe(), wait()) not found])
    else
        have_pipe="yes"
        AC_DEFINE(STORAGE_PIPE,1,[Define to 1 if you want to use pipes for auth/reg.])
    fi
fi
AM_CONDITIONAL(STORAGE_PIPE, [test "x-$have_pipe" = "x-yes"])

# Anonymous
AC_ARG_ENABLE(anon, AC_HELP_STRING([--enable-anon], [enable anonymous auth/reg support (no)]),
              want_anon=$enableval, want_anon=no)
if test "x-$want_anon" = "x-yes" ; then
    AC_DEFINE(STORAGE_ANON,1,[Define to 1 if you want anonymous auth.])
fi
AM_CONDITIONAL(STORAGE_ANON, [test "x-$want_anon" = "x-yes"])


# Filesystem storage
AC_ARG_ENABLE(fs, AC_HELP_STRING([--enable-fs], [enable filesystem storage support (no)]),
              want_fs=$enableval, want_fs=no)
if test "x-$want_fs" = "x-yes" ; then
    AC_DEFINE(STORAGE_FS,1,[Define to 1 if you want to use the filesystem for storage.])
fi
AM_CONDITIONAL(STORAGE_FS, [test "x-$want_fs" = "x-yes"])

#
# IPv6 checks
AC_DEFUN([_IP6_INCLUDES],[[
#include "ac-stdint.h"
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif
]])

# these types are sometimes missing
AC_CHECK_TYPES([in_port_t, sa_family_t, struct sockaddr_storage, struct sockaddr_in6, struct in6_addr],,, _IP6_INCLUDES)

#
# mio backend selection

AC_ARG_ENABLE(mio, AC_HELP_STRING([--enable-mio=BACKEND], [use BACKEND to drive MIO]),
              mio_check=$enableval, mio_check='epoll poll select')

mio_backend=''
for backend in $mio_check ; do
    case x-$backend in

        x-epoll)
            AC_CHECK_HEADERS(sys/epoll.h)
            if test "x-$ac_cv_header_sys_epoll_h" = "x-yes" ; then
                AC_CHECK_FUNCS(epoll_create,[
                    mio_backend='epoll'
                    AC_DEFINE(MIO_EPOLL,1,[Define to 1 if you want to use 'epoll' for non-blocking I/O.])])
            fi
            ;;

        x-poll)
            AC_CHECK_HEADERS(poll.h)
            if test "x-$ac_cv_header_poll_h" = "x-yes" ; then
                AC_CHECK_FUNCS(poll,[
                    mio_backend='poll'
                    AC_DEFINE(MIO_POLL,1,[Define to 1 if you want to use 'poll' for non-blocking I/O.])])
            fi
            ;;

        x-select)
            AC_CHECK_HEADERS(sys/select.h)
            if test "x-$ac_cv_header_sys_select_h" = "x-yes" ; then
                AC_CHECK_FUNCS(select, have_select=yes)
            fi

            if test "x-$have_select" != "x-yes" -a "x-$ac_cv_header_winsock2_h" = "x-yes" ; then
                AC_MSG_CHECKING([for select in ws2_32])
                AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <winsock2.h>]],
                                                [[select(0,0,0,0,0)]])],
                               [AC_MSG_RESULT(yes)
                                have_select=yes],
                               AC_MSG_RESULT(no))
            fi

            if test "x-$have_select" = "x-yes" ; then
                mio_backend='select'
                AC_DEFINE(MIO_SELECT,1,[Define to 1 if you want to use 'select' for non-blocking I/O.])
            fi
            ;;
    esac
done

if test "x-$mio_backend" = "x-" ; then
    AC_MSG_ERROR([no MIO backend available out of: $backend])
fi


# Generate Makefiles
AC_CONFIG_FILES([Doxyfile
                 Makefile
                 etc/Makefile
                 etc/templates/Makefile
                 man/Makefile
                 subst/Makefile
                 util/Makefile
                 mio/Makefile
                 sx/Makefile
                 storage/Makefile
                 c2s/Makefile
                 resolver/Makefile
                 router/Makefile
                 s2s/Makefile
                 sm/Makefile
                 tools/Makefile])
AC_OUTPUT
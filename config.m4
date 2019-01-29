dnl config.m4 for extension explosion

PHP_ARG_ENABLE(explosion, whether to enable explosion support,
dnl Make sure that the comment is aligned:
[  --enable-explosion           Enable explosion support])

if test "$PHP_EXPLOSION" != "no"; then
  PHP_REQUIRE_CXX()

  # --with-skeleton -> check with-path
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/re2/re2.h"
  if test -r $PHP_EXPLOSION/$SEARCH_FOR; then # path given as parameter
    RE2_DIR=$PHP_EXPLOSION
  else # search default path list
    AC_MSG_CHECKING([for re2 files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        RE2_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi

  if test -z "$RE2_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the re2 distribution])
  fi

  # --with-explosion -> add include path
  PHP_ADD_INCLUDE($RE2_DIR/include/re2)

  # --with-explosion -> check for lib and symbol presence
  LIBNAME="re2"
  LIBSYMBOL="RE2"

  PHP_SUBST(EXPLOSION_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, EXPLOSION_SHARED_LIBADD)
  PHP_ADD_LIBRARY(re2, 1, EXPLOSION_SHARED_LIBADD)
  CXXFLAGS="-std=c++14"

  PHP_NEW_EXTENSION(explosion, explosion.cc explosion_api.cc php_explosion.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi

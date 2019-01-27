dnl config.m4 for extension explosion

PHP_ARG_ENABLE(explosion, whether to enable explosion support,
dnl Make sure that the comment is aligned:
[  --enable-explosion           Enable explosion support])

if test "$PHP_EXPLOSION" != "no"; then
  PHP_SUBST(EXPLOSION_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, EXPLOSION_SHARED_LIBADD)
  CXXFLAGS="-std=c++11"

  PHP_NEW_EXTENSION(explosion, explosion.cc explosion_api.cc explosion.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi

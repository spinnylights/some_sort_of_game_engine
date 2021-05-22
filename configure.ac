m4_define([pname],
          [crypt_underworld])

m4_define(m4_defn([pname])[_VERSION],
          m4_esyscmd([build-aux/git-version-gen .tarball-version]))

AC_INIT([Crypt Underworld],
        m4_defn(m4_defn([pname])[_VERSION]),
        [https://github.com/spinnylights/]m4_defn([pname])[/issues],
        m4_defn([pname])[-]m4_defn(m4_defn([pname])[_VERSION]))

AC_CONFIG_SRCDIR([src/main.cpp])

AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects foreign])

AC_PROG_CXX

AC_CONFIG_FILES([Makefile])

AC_OUTPUT

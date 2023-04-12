m4_define([pname],
          [crypt_underworld])

m4_define(m4_defn([pname])[_VERSION],
          m4_esyscmd([build-aux/git-version-gen .tarball-version]))

AC_INIT([Crypt Underworld],
        m4_defn(m4_defn([pname])[_VERSION]),
        [https://github.com/spinnylights/]m4_defn([pname])[/issues],
        m4_defn([pname])[-]m4_defn(m4_defn([pname])[_VERSION]))

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_SRCDIR([examples/circ/main.comp])

AC_PROG_CXX

AX_CXX_COMPILE_STDCXX20

dnl Check for SDL2
SDL_VERSION=2.0.14
AM_PATH_SDL2($SDL_VERSION,
             :,
             AC_MSG_ERROR([SDL version $SDL_VERSION not found!]))

dnl Check for Vulkan
AC_CHECK_HEADERS([vulkan/vulkan.h vulkan/vulkan_core.h],
                 [pname_found_vulkan_headers=yes; break;])
AS_IF([test "x$pname_found_vulkan_headers" != "xyes"],
      [AC_MSG_ERROR([Unable to find the Vulkan headers])])

dnl Check for pthreads
AX_PTHREAD

AM_INIT_AUTOMAKE([subdir-objects foreign])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT

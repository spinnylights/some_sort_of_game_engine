# SYNOPSIS
#
#   AX_CXX_COMPILE_STDCXX20
#
# DESCRIPTION
#
#   Checks for general C++20 support and the specific C++20
#   features needed by Crypt Underworld. Note that if you are
#   using this in a different project you may need to modify it
#   to check for the features your codebase needs.
#
# LICENSE
#
#   This file is part of Crypt Underworld.
#
#   Crypt Underworld is free software: you can redistribute it
#   and/or modify it under the terms of the GNU General Public
#   License as published by the Free Software Foundation, either
#   version 3 of the License, or (at your option) any later
#   version.
#
#   Crypt Underworld is distributed in the hope that it will be
#   useful, but WITHOUT ANY WARRANTY; without even the implied
#   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#   PURPOSE. See the GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with Crypt Underworld. If not, see
#   <https://www.gnu.org/licenses/>.
#
#   Copyright (c) 2021 Zoë Sparks <zoe@milky.flowers>

#serial 1

dnl This marco is based on the AX_CXX_COMPILE_STDCXX macro
dnl (serial version number 12) available at
dnl https://www.gnu.org/software/autoconf-archive/ax_cxx_compile_stdcxx.html.

AC_DEFUN_ONCE([AX_CXX_COMPILE_STDCXX20], [dnl
    AC_LANG_PUSH([C++])dnl
    ac_success=no

    switch="-std=c++20"
    AC_CACHE_CHECK(whether $CXX supports the required C++20 features,
                   [ax_cv_cxx_compile_cxx20],
        [ac_save_CXX="$CXX"
         CXX="$CXX $switch"
         AC_COMPILE_IFELSE([AC_LANG_SOURCE([_AX_CXX_COMPILE_STDCXX_testbody_20])],
            [ax_cv_cxx_compile_cxx20=yes],
            [ax_cv_cxx_compile_cxx20=no])
         CXX="$ac_save_CXX"])
    if test "x$ax_cv_cxx_compile_cxx20" = xyes; then
        CXX="$CXX $switch"
        if test -n "$CXXCPP" ; then
            CXXCPP="$CXXCPP $switch"
        fi
        ac_success=yes
    fi

    AC_LANG_POP([C++])
    if test "x$ac_success" = xno; then
        AC_MSG_ERROR([*** A compiler with support for certain C++20 features is required.])
    fi
])

dnl Tests for the required C++20 features

m4_define([_AX_CXX_COMPILE_STDCXX_testbody_20], [[

#ifndef __cplusplus

#error "This is not a C++ compiler"

#elif __cpp_designated_initializers < 201707

#error "This compiler does not supported designated initializers"

#endif

]])

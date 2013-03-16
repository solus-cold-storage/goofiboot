#
#  This file is part of gummiboot.
#
#  Copyright (C) 2013 Karel Zak <kzak@redhat.com>
#
#  gummiboot is free software; you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation; either version 2.1 of the License, or
#  (at your option) any later version.
#
#  gummiboot is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with systemd; If not, see <http://www.gnu.org/licenses/>.

AC_INIT([gummiboot],
        [28],
        [systemd-devel@lists.freedesktop.org],
        [gummiboot],
        [http://freedesktop.org/wiki/Software/gummiboot])

AC_CONFIG_SRCDIR([src/setup/setup.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_PREFIX_DEFAULT([/usr])

AM_INIT_AUTOMAKE([foreign 1.11 -Wall -Wno-portability silent-rules tar-pax no-dist-gzip dist-xz subdir-objects])
AM_SILENT_RULES([yes])

AC_CANONICAL_HOST
AC_SYS_LARGEFILE

AC_PROG_CC

dnl Don't try to use things like -std=c99 for efi compilation
GNUEFI_CC=$CC
AC_SUBST([GNUEFI_CC])

AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_PROG_GCC_TRADITIONAL

AC_PROG_MKDIR_P
AC_PATH_PROG([XSLTPROC], [xsltproc])
AC_PATH_PROG([QEMU_KVM], [qemu-kvm])

dnl Define ARCH_<NAME> conditionals
SET_ARCH(I686, i686*)
SET_ARCH(X86_64, x86_64*)
SET_ARCH(IA64, ia64*)

ARCH=`echo $host | sed "s/\(-\).*$//"`

AM_COND_IF(ARCH_I686, [
        ARCH=ia32
        MACHINE_TYPE_NAME=ia32])

AM_COND_IF(ARCH_X86_64, [
        MACHINE_TYPE_NAME=x64])

AC_SUBST([ARCH])
AC_SUBST([MACHINE_TYPE_NAME])

# test bios
AC_CHECK_FILE([/usr/lib/qemu-bios],        [BIOS=/usr/lib/qemu-bios])
AC_CHECK_FILE([/usr/share/qemu-ovmf/bios], [BIOS=/usr/share/qemu-ovmf/bios])
AC_SUBST([BIOS])

# ------------------------------------------------------------------------------
dnl Compile EFI stuff is so tricky that it's probably better to check for the
dnl include files that try to use AC_CHECK_HEADERS to compile any conftest.c
AC_CHECK_FILES([/usr/include/efi/efi.h
                /usr/include/efi/${ARCH}/efibind.h], [],
        [AC_MSG_ERROR([*** GNU Efi headers not found])])

efiroot=$(echo $(cd /usr/lib/$(gcc -print-multi-os-directory); pwd))
GNUEFI_LIBS="-L $efiroot"

dnl extra objects and linker scripts
GNUEFI_LDS_DIR="$efiroot"
if test -d "${efiroot}/gnuefi"; then
        GNUEFI_LDS_DIR="${efiroot}/gnuefi"
fi
AC_SUBST([GNUEFI_LIBS])
AC_SUBST([GNUEFI_LDS_DIR])

# ------------------------------------------------------------------------------
AC_ARG_ENABLE(blkid, AS_HELP_STRING([--disable-blkid], [disable blkid support]))
if test "x$enable_blkid" != "xno"; then
        PKG_CHECK_MODULES(BLKID, [ blkid >= 2.20 ],
                [AC_DEFINE(HAVE_BLKID, 1, [Define if blkid is available]) have_blkid=yes], have_blkid=no)
        if test "x$have_blkid" = xno -a "x$enable_blkid" = xyes; then
                AC_MSG_ERROR([*** blkid support requested but libraries not found])
        fi
fi
AM_CONDITIONAL(HAVE_BLKID, [test "$have_blkid" = "yes"])

# ------------------------------------------------------------------------------
have_manpages=no
AC_ARG_ENABLE(manpages, AS_HELP_STRING([--disable-manpages], [disable manpages]))
AS_IF([test "x$enable_manpages" != xno], [
        AS_IF([test "x$enable_manpages" = xyes -a "x$XSLTPROC" = x], [
                AC_MSG_ERROR([*** Manpages requested but xsltproc not found])
        ])
        AC_CHECK_FILES([
                /usr/share/sgml/docbook/xsl-stylesheets/manpages/docbook.xsl
                /usr/share/sgml/docbook/stylesheet/xsl/nwalsh/manpages/docbook.xsl
                ],[have_docbook=yes])
        AS_IF([test "x$enable_manpages" = xyes -a "x$have_docbook" = x], [
                AC_MSG_ERROR([*** Manpages requested but docbook not found])
        ])
        AS_IF([test "x$XSLTPROC" != x -a "x$have_docbook" != x], [have_manpages=yes])
])
AM_CONDITIONAL(ENABLE_MANPAGES, [test "x$have_manpages" = "xyes"])

# ------------------------------------------------------------------------------
AC_CONFIG_FILES([
        Makefile
])

AC_OUTPUT
AC_MSG_RESULT([
        $PACKAGE_NAME $VERSION

        arch:                    $ARCH
        EFI machine type:        $MACHINE_TYPE_NAME

        prefix:                  ${prefix}
        efi libs:                ${GNUEFI_LIBS}
        efi lds:                 ${GNUEFI_LDS_DIR}
        blkid:                   ${have_blkid}
        man pages:               ${have_manpages}

        test QEMU bios:          ${BIOS}
])

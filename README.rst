goofiboot Simple UEFI boot manager
----------------------------------

goofiboot is a fork of gummiboot - which is now dead upstream.

This fork is now being used for Solus (and anyone else if they're interested)
because we need the full features of gummiboot, outside of the context of
systemd.

It's been stated on several occassions that systemd-boot is "not a direct
successor", and "systemd-boot does not support some of the features gummiboot
supported". As such, this fork becomes a separate and independent bootloader.

**Note**: This fork is not yet ready for use.

License
=======

goofiboot is available under the same license as gummiboot, the GNU
Lesser General Public License 2.1 (LGPL-2.1)

About
=====

goofiboot executes EFI images. The default entry is selected by a configured
pattern (glob) or an on-screen menu.

goofiboot operates on the EFI System Partition (ESP) only. Configuration
file fragments, kernels, initrds, other EFI images need to reside on the
ESP. Linux kernels must be built with CONFIG_EFI_STUB to be able to be
directly executed as an EFI image.

goofiboot reads simple and entirely generic configurion files; one file
per boot entry to select from.

Pressing Space (or most other) keys during bootup will show an on-screen
menu with all configured entries to select from. Pressing enter on the
selected entry loads and starts the EFI image.

If no timeout is configured and no key pressed during bootup, the default
entry is booted right away.

/*-*- Mode: C; c-basic-offset: 8; indent-tabs-mode: nil -*-*/

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Copyright (C) 2015 Kay Sievers <kay@vrfy.org>
 */

#include <efi.h>
#include <efilib.h>

#include "util.h"
#include "pefile.h"
#include "linux.h"

EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *sys_table) {
        EFI_LOADED_IMAGE *loaded_image;
        EFI_FILE *root_dir;
        CHAR16 *loaded_image_path;
        CHAR8 *sections[] = {
                (UINT8 *)".cmdline",
                (UINT8 *)".linux",
                (UINT8 *)".initrd",
                NULL
        };
        UINTN addrs[3] = {};
        UINTN offs[3] = {};
        UINTN szs[3] = {};
        EFI_STATUS err;

        InitializeLib(image, sys_table);

        err = uefi_call_wrapper(BS->OpenProtocol, 6, image, &LoadedImageProtocol, (VOID **)&loaded_image,
                                image, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
        if (EFI_ERROR(err)) {
                Print(L"Error getting a LoadedImageProtocol handle: %r ", err);
                uefi_call_wrapper(BS->Stall, 1, 3 * 1000 * 1000);
                return err;
        }

        root_dir = LibOpenRoot(loaded_image->DeviceHandle);
        if (!root_dir) {
                Print(L"Unable to open root directory: %r ", err);
                uefi_call_wrapper(BS->Stall, 1, 3 * 1000 * 1000);
                return EFI_LOAD_ERROR;
        }

        loaded_image_path = DevicePathToStr(loaded_image->FilePath);

        err = pefile_locate_sections(root_dir, loaded_image_path, sections, addrs, offs, szs);
        if (EFI_ERROR(err)) {
                Print(L"Unable to locate embedded .linux section: %r ", err);
                uefi_call_wrapper(BS->Stall, 1, 3 * 1000 * 1000);
                return err;
        }

        err = linux_exec(image, (CHAR8 *)"",
                         (UINTN)loaded_image->ImageBase + addrs[1],
                         (UINTN)loaded_image->ImageBase + addrs[2], szs[2]);

        Print(L"Execution of embedded linux image failed: %r\n", err);
        uefi_call_wrapper(BS->Stall, 1, 3 * 1000 * 1000);
        return err;
}

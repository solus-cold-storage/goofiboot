/**
 * This file is part of goofiboot.
 *
 * Copyright (C) 2015-2016 Ikey Doherty
 *
 * goofiboot is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 */

#ifndef _GOOFIBOOT_COMMON_H
#define _GOOFIBOOT_COMMON_H

#define __unused__ __attribute__((unused))

#define DEF_AUTOFREE(N, C)                                                                         \
        static inline void _autofree_func_##N(void *p)                                             \
        {                                                                                          \
                if (p && *(N **)p) {                                                               \
                        C(*(N **)p);                                                               \
                        (*(void **)p) = NULL;                                                      \
                }                                                                                  \
        }

#define autofree(N) __attribute__((cleanup(_autofree_func_##N))) N

#endif /*_GOOFIBOOT_COMMON_H */

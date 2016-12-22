//--------------------------------------------------------
//  Zelda Classic
//
//  alleg_compat.h
//
//  Compatibility between allegro versions.
//
//--------------------------------------------------------

#ifndef _ZC_ALLEG_COMPAT_H_
#define _ZC_ALLEG_COMPAT_H_

// If we're not using 4.2.modded, add the missing functions.
#if ALLEGRO_SUB_VERSION != 2
PACKFILE *pack_fopen_password(const char *filename, const char *mode, const char *password);
uint64_t file_size_ex_password(const char *filename, const char *password);
#endif

#endif                                                      // _ZC_ALLEG_COMPAT_H_

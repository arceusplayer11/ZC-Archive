//--------------------------------------------------------
//  Zelda Classic
//
//  alleg_compat.cpp
//
//  Compatibility between allegro versions.
//
//--------------------------------------------------------

#include <allegro.h>

// If we're not using 4.2.modded, add the missing functions.
#if ALLEGRO_SUB_VERSION != 2

PACKFILE *pack_fopen_password(const char *filename, const char *mode, const char *password) {
	packfile_password(password);
	return pack_fopen(filename, mode);
}

uint64_t file_size_ex_password(const char *filename, const char *password) {
	packfile_password(password);
	return file_size_ex(filename);
}

#endif

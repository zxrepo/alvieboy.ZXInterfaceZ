#include "command.h"

#define ROM_PATCHED_SNALOAD_ADDRESS 0x1F00

int sna__uploadsna(command_t *cmdt, int argc, char **argv);
int sna__save_from_extram(const char *name);
const char *sna__get_error_string();
int sna__load_sna_extram(const char *file);



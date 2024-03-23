#pragma once

#include "../typedefs.h"
#include "../strings/strings.h"

void console_init(const PutCharFct put_char_fct);
void console_print(const char* string);

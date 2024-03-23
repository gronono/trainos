#pragma once

typedef void (*PutCharFct)(const char character);

void print(const PutCharFct put_char_fct, const char* string);
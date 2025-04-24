#pragma once

#include <ctype.h>
#include <list>
#include <string>

#define CTRL_A 1
#define CTRL_B 2
#define CTRL_C 3
#define CTRL_D 4
#define CTRL_E 5
#define CTRL_F 6
#define CTRL_G 7
#define CTRL_H 8
#define CTRL_I 9
#define CTRL_J 10
#define CTRL_K 11
#define CTRL_L 12
#define CTRL_M 13
#define CTRL_N 14
#define CTRL_O 15
#define CTRL_P 16
#define CTRL_Q 17
#define CTRL_R 18
#define CTRL_S 19
#define CTRL_T 20
#define CTRL_U 21
#define CTRL_V 22
#define CTRL_W 23
#define CTRL_X 24
#define CTRL_Y 25
#define CTRL_Z 26

#define CTRL_BACKSLASH 28
#define CTRL_BACKSPACE 8
#define CTRL_CARET 30
#define CTRL_DEL 127
#define CTRL_ENTER 10
#define CTRL_ESCAPE 27
#define CTRL_LEFT_BRACE 27
#define CTRL_RIGHT_BRACE 29
#define CTRL_TAB 9
#define CTRL_UNDERSCORE 31

#define PRINTABLE(c) ((c) > 0x1f && (c) < 0x7f)
#define MIN_CAPACITY 8
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

enum
{
    C_UNDEFINED = 0x100,
    C_WHITESPACE = 0x200,
    C_ALPHANUM = 0x400,
    C_SPECIAL = 0x800,
    C_CONTROL = 0x1000
};

enum Mode
{
    NORMAL_MODE,
    INSERT_MODE,
    COMMAND_MODE
};

static std::map<Mode, const char*> mode_names = {
    {NORMAL_MODE, "NORMAL"},
    {COMMAND_MODE, "COMMAND"},
    {INSERT_MODE, "INSERT"},
};

int char_type(char c);

std::list<std::string> readlines(const char* filename);

std::vector<std::string> splitwords(const std::string& line, const std::string& delim);

bool is_number(const std::string& str);

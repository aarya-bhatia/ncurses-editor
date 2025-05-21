#pragma once
#include "log.h"
#include <vector>
#include <list>
#include "Cursor.h"
#include "Scroll.h"
#include "FileSubscriber.h"

typedef unsigned int FileID;

struct File
{
    const FileID id;
    std::string filename = "";
    std::list<std::list<char>> lines;
    Cursor cursor;

    std::vector<FileSubscriber*> subscribers;
    std::string normal_mode_buffer = "";

    File(FileID _id = 0, const std::string& _filename = "");
    ~File();

    FileID get_id() const { return this->id; }

    bool operator==(const File& other) { return this->id == other.id; }
    bool operator!=(const File& other) { return !(*this == other); }

    void add_subscriber(FileSubscriber* subscriber)
    {
        subscribers.push_back(subscriber);
    }

    int load_file();
    int save_file();

    int count_lines() const
    {
        return lines.size();
    }

    bool _move_cursor_y(int dy);
    bool _move_cursor_x(int dx);

    void cursor_up();
    void cursor_down();
    void cursor_left();
    void cursor_right();
    void move_cursor_eol();
    void move_cursor_eof();
    void goto_line(int line_no);
    void goto_column(int col);
    void move_begin();

    void insert_character(int c);
};
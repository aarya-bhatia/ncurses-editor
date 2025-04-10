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
    FileID id;
    char* filename;
    std::list<std::list<char>> lines;
    Cursor cursor;

    std::vector<FileSubscriber*> subscribers;

    File(FileID id, const char* filename);

    ~File()
    {
        free(this->filename);
        this->filename = nullptr;
        this->subscribers.clear();
    }

    FileID get_id() const { return this->id; }

    bool operator==(const File& other) { return this->id == other.id; }
    bool operator!=(const File& other) { return !(*this == other); }

    void add_subscriber(FileSubscriber* subscriber)
    {
        subscribers.push_back(subscriber);
    }

    void set_filename(const char* filename)
    {
        free(this->filename);
        this->filename = strdup(filename);
    }

    const char* get_filename()
    {
        return this->filename;
    }

    bool is_named()
    {
        return this->filename != nullptr;
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
    void move_begin();

    void insert_character(int c);
};
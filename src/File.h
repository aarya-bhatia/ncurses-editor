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

    std::function<bool(unsigned)> event_handler;

    std::vector<FileSubscriber*> subscribers;
    std::string normal_mode_buffer = "";

    bool readonly = false;

    File(FileID _id = 0, const std::string& _filename = "");
    ~File();

    FileID get_id() const { return this->id; }

    bool operator==(const File& other) { return this->id == other.id; }
    bool operator!=(const File& other) { return !(*this == other); }

    void add_subscriber(FileSubscriber* subscriber)
    {
        for (auto itr = subscribers.begin(); itr != subscribers.end(); itr++) {
            if (*itr == subscriber) {
                return;
            }
        }

        subscribers.push_back(subscriber);
    }

    bool remove_subscriber(FileSubscriber* subscriber)
    {
        for (auto itr = subscribers.begin(); itr != subscribers.end(); itr++) {
            if (*itr == subscriber) {
                subscribers.erase(itr);
                return true;
            }
        }

        return false;
    }

    std::string get_current_line() {
        const std::list<char>& it = *(cursor.line);
        return { it.begin(), it.end() };
    }

    int load_file();
    int save_file();

    int count_lines() const
    {
        return lines.size();
    }

    // Motions

    void cursor_up();
    void cursor_down();
    void cursor_left();
    void cursor_right();
    void move_cursor_eol();
    void move_cursor_eof();
    void goto_line(int line_no);
    void goto_column(int col);
    void move_begin();

    // Editing ops

    void remove_character();
    void remove_line();
    void insert_character(int c);
    void insert_line_below();

    void paste_below(std::string& line);
    void paste_above(std::string& line);

    std::string copy_line() const;

private:
    bool _move_cursor_y(int dy);
    bool _move_cursor_x(int dx);

};
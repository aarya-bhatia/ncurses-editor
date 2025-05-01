#pragma once
#include "FileSubscriber.h"
#include "File.h"

class MockFileSubscriber : public FileSubscriber
{
public:
    bool file_reloaded = false;
    bool character_inserted = false;
    bool line_inserted = false;
    bool line_removed = false;
    bool character_erased = false;
    bool character_replaced = false;
    
    char last_inserted_char = 0;
    Cursor last_insert_position;
    Cursor last_line_insert_position;
    int last_removed_line = -1;
    Cursor last_erase_position;
    Cursor last_replace_position;
    
    void reset() {
        file_reloaded = false;
        character_inserted = false;
        line_inserted = false;
        line_removed = false;
        character_erased = false;
        character_replaced = false;
        
        last_inserted_char = 0;
        last_removed_line = -1;
    }
    
    void on_file_reload(File& file) override {
        file_reloaded = true;
    }
    
    void on_insert_character(File& file, Cursor position, char c) override {
        character_inserted = true;
        last_insert_position = position;
        last_inserted_char = c;
    }
    
    void on_insert_line(File& file, Cursor position) override {
        line_inserted = true;
        last_line_insert_position = position;
    }
    
    void on_remove_line(File& file, int line_no) override {
        line_removed = true;
        last_removed_line = line_no;
    }
    
    void on_erase_character(File& file, Cursor position) override {
        character_erased = true;
        last_erase_position = position;
    }
    
    void on_replace_character(File& file, Cursor position) override {
        character_replaced = true;
        last_replace_position = position;
    }
};
#include "catch.hpp"
#include "File.h"
#include "MockFileSubscriber.h"
#include <fstream>
#include <string>

#define MY_TEST_SUITE "[file]"

// Helper function to create a temporary test file
std::string create_temp_file(const std::string& content) {
    std::string filename = "/tmp/test_file_" + std::to_string(rand()) + ".txt";
    std::ofstream file(filename);
    file << content;
    file.close();
    return filename;
}

TEST_CASE("File initialization", MY_TEST_SUITE) {
    // Default constructor with ID
    File file1(1);
    REQUIRE(file1.get_id() == 1);
    REQUIRE(file1.filename == "untitled-1");
    REQUIRE(file1.count_lines() == 1);
    REQUIRE(file1.cursor.x == 0);
    REQUIRE(file1.cursor.y == 0);
    
    // Constructor with ID and filename
    File file2(2, "test.txt");
    REQUIRE(file2.get_id() == 2);
    REQUIRE(file2.filename == "test.txt");
    REQUIRE(file2.count_lines() == 1);
}

TEST_CASE("File subscriber management", MY_TEST_SUITE) {
    File file(1);
    MockFileSubscriber subscriber;
    
    file.add_subscriber(&subscriber);
    REQUIRE(file.subscribers.size() == 1);
    REQUIRE(file.subscribers[0] == &subscriber);
}

TEST_CASE("File load_file", MY_TEST_SUITE) {
    // Create a temporary file with content
    std::string content = "Line 1\nLine 2\nLine 3";
    std::string filename = create_temp_file(content);
    
    File file(1, filename);
    MockFileSubscriber subscriber;
    file.add_subscriber(&subscriber);
    
    int result = file.load_file();
    REQUIRE(result == 0);
    REQUIRE(file.count_lines() == 3);
    REQUIRE(subscriber.file_reloaded == true);
    
    // Check content of lines
    auto line_it = file.lines.begin();
    std::string line1(line_it->begin(), line_it->end());
    REQUIRE(line1 == "Line 1");
    
    line_it++;
    std::string line2(line_it->begin(), line_it->end());
    REQUIRE(line2 == "Line 2");
    
    line_it++;
    std::string line3(line_it->begin(), line_it->end());
    REQUIRE(line3 == "Line 3");
    
    // Check cursor position after loading
    REQUIRE(file.cursor.x == 0);
    REQUIRE(file.cursor.y == 0);
}

TEST_CASE("File save_file", MY_TEST_SUITE) {
    // Create a temporary filename
    std::string filename = "/tmp/test_save_file_" + std::to_string(rand()) + ".txt";
    
    // Create a file with content
    File file(1, filename);
    
    // Add some content
    file.insert_character('H');
    file.insert_character('e');
    file.insert_character('l');
    file.insert_character('l');
    file.insert_character('o');
    
    // Insert a new line
    file.insert_line();
    
    // Add more content
    file.insert_character('W');
    file.insert_character('o');
    file.insert_character('r');
    file.insert_character('l');
    file.insert_character('d');
    
    // Save the file
    int result = file.save_file();
    REQUIRE(result == 0);
    
    // Read the file back and check content
    std::ifstream saved_file(filename);
    std::string line1, line2;
    std::getline(saved_file, line1);
    std::getline(saved_file, line2);
    
    REQUIRE(line1 == "Hello");
    REQUIRE(line2 == "World");
}

TEST_CASE("File cursor movement", MY_TEST_SUITE) {
    // Create a file with multiple lines
    File file(1);
    
    // Add some content
    file.insert_character('a');
    file.insert_character('b');
    file.insert_character('c');
    
    // Insert a new line
    file.insert_line();
    
    // Add more content
    file.insert_character('d');
    file.insert_character('e');
    file.insert_character('f');
    
    // Test cursor_up
    REQUIRE(file.cursor.y == 1);
    file.cursor_up();
    REQUIRE(file.cursor.y == 0);
    
    // Test cursor_down
    file.cursor_down();
    REQUIRE(file.cursor.y == 1);
    
    // Test cursor_left
    REQUIRE(file.cursor.x == 3);
    file.cursor_left();
    REQUIRE(file.cursor.x == 2);
    
    // Test cursor_right
    file.cursor_right();
    REQUIRE(file.cursor.x == 3);
    
    // Test move_begin
    file.move_begin();
    REQUIRE(file.cursor.x == 0);
    
    // Test move_cursor_eol
    file.move_cursor_eol();
    REQUIRE(file.cursor.x == 2);
    
    // Test goto_line
    file.goto_line(0);
    REQUIRE(file.cursor.y == 0);
    
    // Test move_cursor_eof
    file.move_cursor_eof();
    REQUIRE(file.cursor.y == 1);
}

TEST_CASE("File character insertion", MY_TEST_SUITE) {
    File file(1);
    MockFileSubscriber subscriber;
    file.add_subscriber(&subscriber);
    
    // Insert a character
    file.insert_character('a');
    REQUIRE(subscriber.character_inserted == true);
    REQUIRE(subscriber.last_inserted_char == 'a');
    REQUIRE(file.cursor.x == 1);
    
    // Check content
    std::string line(file.lines.front().begin(), file.lines.front().end());
    REQUIRE(line == "a");
}

TEST_CASE("File line operations", MY_TEST_SUITE) {
    File file(1);
    MockFileSubscriber subscriber;
    file.add_subscriber(&subscriber);
    
    // Insert a line
    file.insert_line();
    REQUIRE(subscriber.line_inserted == true);
    REQUIRE(file.cursor.y == 1);
    REQUIRE(file.count_lines() == 2);
    
    // Remove a line
    subscriber.reset();
    file.remove_line();
    REQUIRE(subscriber.line_removed == true);
    REQUIRE(subscriber.last_removed_line == 1);
    REQUIRE(file.cursor.y == 0);
    REQUIRE(file.count_lines() == 1);
    
    // Test removing the only line (should create an empty line)
    subscriber.reset();
    file.remove_line();
    REQUIRE(subscriber.line_removed == true);
    REQUIRE(file.count_lines() == 1);
    REQUIRE(file.lines.front().empty());
}
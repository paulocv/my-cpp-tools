#include "file_tools.hpp"

#include <iostream>
#include <fstream>
// #include <exception>
#include <cctype>       // isspace, 
#include <algorithm>    // find_if
#include <sstream>      // TODO: string streams
#include <iterator>     // istreambuf_iterator

#include "CustomException.hpp"


// ------- Globals not exposed to the extern
const std::string HEADER_END {"-----"};


// ------- FORWARD DECLARATIONS
void throw_file_not_found(std::string fname);
// void read_file_content(std::fstream fp);  # Instead, do this: std::string str(std::istreambuf_iterator<char>{ifs}, {});


/*
---------------------------------------------------------------------------------------------------
STRING OPERATIONS
---------------------------------------------------------------------------------------------------
*/


void trim_spaces(std::string& s){

    // Left trimming
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));

    // Right triming
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());

}


str_map_t& read_config_str(str_map_t& out, std::string s, std::string end_line, 
const char entry_char, const char attr_char, const char comment_char){

    std::istringstream s_stream {s};  // Create a stream from the input string. This copies the content.
    std::string line;
    bool use_end_line {end_line == ""};  // If end_line is empty, interpretation only stops at end of string.


    while (std::getline(s_stream, line)){
        
        // Line elimination criteria
        if ((line == end_line) and use_end_line) break;
        if (line.front() != entry_char) continue;

        line.erase(0, 1);  // Erases entry_char from string
        
        // Get substr before first comment char
        size_t index {line.find(comment_char)};
        if (index != std::string::npos){
            line.erase(index);
        }
        
        // Look for attr_char. Throw error if not found.
        index = line.find(attr_char);
        if (index == std::string::npos){
            throw CustomException("Hey, attr_char was not found in a given line.");
        }

        // Key/value separation
        std::string key     {line.substr(0, index)};
        std::string val   {line.substr(index+1)};

        trim_spaces(key);
        trim_spaces(val);

        // Finally adds the pair
        out[key] = val;

    }

    return out;
}


str_map_t& read_config_file(str_map_t& out, std::string fname, std::string end_line, 
const char entry_char, const char attr_char, const char comment_char){

    std::ifstream fp {};

    fp.open(fname, std::ios::in);
    if (!fp){
        throw_file_not_found(fname);
    }

    // Read whole file content into a string
    std::string file_content {std::istreambuf_iterator<char>{fp}, {}};
    fp.close();

    return read_config_str(out, file_content, end_line, entry_char, attr_char, comment_char);
}


// ------------------------------------------------------------------------------------------------
// FILE OPERATIONS
// ------------------------------------------------------------------------------------------------

void throw_file_not_found(std::string fname){
    std::stringstream err {};
    err << "Hey, file '" << fname << "' was not found.";
    throw CustomException(err.str());
}
#include "file_tools.hpp"

#include <algorithm>    // find_if
#include <cctype>       // isspace
#include <fstream>      // fstream, ifstream, ofstream
#include <iostream>     // cin, cout
#include <iterator>     // istreambuf_iterator
#include <sstream>      // stringstream
#include <sys/stat.h>   // stat

#include "custom_exception.hpp"


// ------- Globals not exposed to the extern
// const std::string HEADER_END {"-----"};


// ------- FORWARD DECLARATIONS
void throw_file_not_found(std::string fname);
// void read_file_content(std::fstream fp);  # Instead, do this: std::string str(std::istreambuf_iterator<char>{fp}, {});


/*
---------------------------------------------------------------------------------------------------
FILE, PATH AND DIRECTORY OPERATIONS
---------------------------------------------------------------------------------------------------
*/

void throw_file_not_found(std::string fname){
    std::stringstream err {};
    err << "Hey, file '" << fname << "' was not found.";
    throw CustomException(err.str());
}


bool path_exists(const std::string& path){
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}


int make_dir_recursive(const std::string& path){
    return system( (std::string("mkdir -p ") + path).c_str() );
}



/*
---------------------------------------------------------------------------------------------------
CONFIGURATION LANGUAGE INTERPRETATION
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


/* 
Parses entries of a InputMarkup-style C++ string into an ordered map of string: string. 

The function looks for lines starting with `entry_char`. Each one must contain a pair of strings 
(key and value, respectively) separated by an `attr_char`. Trailing whitespaces around each string 
are ignored. Besides, anything that comes after a `comment_char` (including itself) is ignored. 
Finally, the parsing stops after encountering a string `end_line` as a line.

EXAMPLE STRING (using entry_char = '>', attr_char = '=', comment_char = '#' and end_line = "-----")
--------------
"""
> hello = world
> foo = 1    # This is a comment
> bar = -2.0
This line is ignored. The next (empty) line is also ignored:

The parsing stops here (because of the next line)
-----
> ignore = this won't be reached, parsing has ended.

"""

@param out  A C++ map of string into string to collect the parsed key/value pairs.
@param s   Input C++ string to be parsed.
@param end_line   A string for a line that, when reached, stops the parsing.
@param entry_char   Character that denotes a meaningful entry line (must be first character in line)
@param attr_char   Attribution character. Separates the key and value strings (excluding whitespaces)
@param comment_char   In a valid entry line, everything after this character is ignored.

*/
str_map_t& read_config_str(str_map_t& out, const std::string& s, std::string end_line, 
const char entry_char, const char attr_char, const char comment_char){

    std::istringstream s_stream {s};  // Create a stream from the input string. This copies the content.
    std::string line;
    bool use_end_line {end_line != ""};  // If end_line is empty, interpretation only stops at end of string.

    int i_line = 0;
    while (std::getline(s_stream, line)){
        i_line++;  // Updated at the top. File lines starting at 1.
        
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
            std::stringstream err_msg{};
            err_msg << "Hey, attr_char was not found in line " << i_line << ".";
            throw CustomException(err_msg.str().c_str());
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


std::string& write_config_string(std::string& out, str_map_t& input_map, 
const char entry_char, const char attr_char){

    std::stringstream out_stream {};

    // Devnote: changes here must be repeated on write_config_file
    for (str_pair_t item : input_map){
        out_stream << entry_char << ' ' << item.first << ' ' << attr_char << ' ' << item.second << '\n';
    }
    
    out = out_stream.str();

    return out;
}


void write_config_file(std::string& fname, str_map_t& input_map, bool append,
const char entry_char, const char attr_char){

    std::ofstream fp {};

    if (append)
        fp.open(fname, std::ios::app);
    else
        fp.open(fname, std::ios::out);

    if (not fp)
        throw_file_not_found(fname);
    
    // Same syntax of write_config_string, now to a file stream.
    for (str_pair_t item : input_map){
        fp << entry_char << ' ' << item.first << ' ' << attr_char << ' ' << item.second << '\n';
    }

    fp.close();
}


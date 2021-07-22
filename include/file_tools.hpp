/*
File-related common operations, such as reading and writing a dictionary-like file, I/O of large array-like data, 
directory creation, etc.

The interface for text is mainly based on C++ std::string class.

Author: Paulo Cesar Ventura da Silva
https://github.com/paulocv

*/

#ifndef FILE_TOOLS_HPP  // This is the header guard. Prevents multiple definitions of this header.
#define FILE_TOOLS_HPP

#include <string>           // std::string
#include <utility>          // std::pair
#include <unordered_map>    // std::unordered_map


// ----------------------
// Name aliases
using str_pair_t = std::pair<std::string, std::string>;          // string/string pair for items of str_map_t
using str_map_t = std::unordered_map<std::string, std::string>;  // A string->string map


// ---------------------
// Declarations

// --- FILE, PATH AND DIRECTORY OPERATIONS

// Tests if a C++ string is a valid path to a file or directory.
bool path_exists(const std::string& path);

/* Create a directory if not existent (silently). Recursively creates needed parent directories. Uses linux syntax (unix mkdir command).
   Returns the result of the system command (0 if succeeded).*/
int make_dir_recursive(const std::string& path);


// --- CONFIGURATION LANGUAGE INTERPRETATION

// Remove trailing whitespaces from a string in place. Uses cctype isspace() function to find whitespace chars.
void trim_spaces(std::string& s);

// Interprets a string of dict-like inputs and returns a fresh unordered_map
str_map_t& read_config_str(str_map_t& out, std::string s, std::string end_line="-----", 
const char entry_char='>', const char attr_char='=', const char comment_char='#');

// Interprets a file with dict-like inputs and returns a fresh unordered_map
str_map_t& read_config_file(str_map_t& out, std::string fname, std::string end_line="-----", 
const char entry_char='>', const char attr_char='=', const char comment_char='#');

// Writes a map of string->string into a single dict-like string.
std::string& write_config_string(std::string& out, str_map_t& input_map, 
const char entry_char='>', const char attr_char='=');

// Writes a map of string->string into a file with a dict-like structure.
void write_config_file(std::string& fname, str_map_t& input_map, bool append=false,
const char entry_char='>', const char attr_char='=');


// --- ARRAY AND VECTOR DATA I/O


#endif

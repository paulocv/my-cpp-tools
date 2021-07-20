#ifndef FILE_TOOLS_H  // This is the header guard. Prevents multiple definitions of this header.
#define FILE_TOOLS_H

#include <string>
#include <unordered_map>


// ----------------------
// Name aliases
using str_map_t = std::unordered_map<std::string, std::string>;  // A string->string map


// --- String operations

// Remove trailing whitespaces from a string in place. Uses cctype isspace() function to find whitespace chars.
void trim_spaces(std::string& s);

// Interprets a string of dict-like inputs and returns a fresh unordered_map
str_map_t& read_config_str(str_map_t& out, std::string s, std::string end_line="-----", 
const char entry_char='>', const char attr_char='=', const char comment_char='#');

// Interprets a file with dict-like inputs and returns a fresh unordered_map
str_map_t& read_config_file(str_map_t& out, std::string fname, std::string end_line="-----", 
const char entry_char='>', const char attr_char='=', const char comment_char='#');



#endif

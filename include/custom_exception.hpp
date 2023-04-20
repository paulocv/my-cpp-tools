#ifndef CUSTOM_EXCEPTION_HPP_
#define CUSTOM_EXCEPTION_HPP_


class CustomException: public std::runtime_error{
public:
    CustomException(std::string msg) : std::runtime_error{msg.c_str()} {}
};


#endif

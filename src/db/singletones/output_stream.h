#ifndef _OUTPUT_STREAM_

#define _OUTPUT_STREAM_
#include <string>
#include <iostream>
class output_stream
{
protected:
    output_stream(const std::string value) : value_(value) {}
    static output_stream* singleton_;
    std::string value_;
public:
    output_stream(output_stream& other) = delete;
    void operator=(const output_stream&) = delete;
    output_stream* operator += (std::string value);
    static output_stream* GetInstance(const std::string& value);
    std::string value() const { return value_; }
    void set_value_to(const std::string&& value);
    void append_to_value(const std::string&& value);
    void clear_value() { value_.clear();}
};

#endif

#include "output_stream.h"

output_stream* output_stream::singleton_ = nullptr;

output_stream* output_stream::GetInstance(const std::string& value)
{

    if (singleton_ == nullptr) {
        singleton_ = new output_stream(value);
    }
    return singleton_;
}

void output_stream::set_value_to(const std::string&& value)
{
    value_ = value;
}

void output_stream::append_to_value(const std::string&& value)
{
    value_.append(value);
}

output_stream* output_stream:: operator += (std::string value)
{
    this -> value_.append(value);
    return this;
}
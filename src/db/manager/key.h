#ifndef  _KEY_
#define _KEY_

#include <string>
#include <vector>

class key {
private:
    int key_type_;
    char* key_;
    int length_;

public:
    key(int keytype, int length) {
        key_type_ = keytype;
        if (keytype == 2)
            length_ = length;
        else
            length_ = 4;
        key_ = new char[length_];
    }

    key(const key& t1) {
        key_type_ = t1.key_type_;
        length_ = t1.length_;
        key_ = new char[length_];
        memcpy(key_, t1.key_, length_);
    }

    void ReadValue(const char* content) {
        switch (key_type_) {
        case 0: {
            int a = std::atoi(content);
            memcpy(key_, &a, length_);
        } break;
        case 1: {
            float a = std::atof(content);
            memcpy(key_, &a, length_);
        } break;
        case 2: {
            memcpy(key_, content, length_);
        } break;
        }
    }

    void ReadValue(std::string str) {
        switch (key_type_) {
        case 0: {
            int a = std::atoi(str.c_str());
            memcpy(key_, &a, length_);
        } break;
        case 1: {
            float a = std::atof(str.c_str());
            memcpy(key_, &a, length_);
        } break;
        case 2: {
            memcpy(key_, str.c_str(), length_);
        } break;
        }
    }

    int key_type() { return key_type_; }
    char* get_key() { return key_; };
    int length() { return length_; }

    ~key() {
        if (key_ != NULL)
            delete[] key_;
    }

    friend std::ostream& operator<<(std::ostream& out, const key& object);

    bool operator<(const key t1) {
        switch (t1.key_type_) {
        case 0:
            return *(int*)key_ < *(int*)t1.key_;
        case 1:
            return *(float*)key_ < *(float*)t1.key_;
        case 2:
            return (strncmp(key_, t1.key_, length_) < 0);
        default:
            return false;
        }
    }

    bool operator>(const key t1) {
        switch (t1.key_type_) {
        case 0:
            return *(int*)key_ > * (int*)t1.key_;
        case 1:
            return *(float*)key_ > * (float*)t1.key_;
        case 2:
            return (strncmp(key_, t1.key_, length_) > 0);
        default:
            return false;
        }
    }

    bool operator<=(const key t1) { return !(operator>(t1)); }

    bool operator>=(const key t1) { return !(operator<(t1)); }

    bool operator==(const key t1) {
        switch (t1.key_type_) {
        case 0:
            return *(int*)key_ == *(int*)t1.key_;
        case 1:
            return *(float*)key_ == *(float*)t1.key_;
        case 2:
            return (strncmp(key_, t1.key_, length_) == 0);
        default:
            return false;
        }
    }

    bool operator!=(const key t1) {
        switch (t1.key_type_) {
        case 0:
            return *(int*)key_ != *(int*)t1.key_;
        case 1:
            return *(float*)key_ != *(float*)t1.key_;
        case 2:
            return (strncmp(key_, t1.key_, length_) != 0);
        default:
            return false;
        }
    }
};

#endif // ! _KEY_
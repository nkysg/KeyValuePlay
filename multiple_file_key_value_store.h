//
// Created by cheyulin on 8/10/16.
//

#ifndef KeyValueStore_KEY_VALUE_STORE_H
#define KeyValueStore_KEY_VALUE_STORE_H

#include <string>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <algorithm>

using namespace std;

#define SEPERATOR_STRING ","
#define SEPERATOR_END_STRING ";"
#define HASH_FUNC(x) str_hash_func_basic(x)
#define DB_FILE_NUM 1000

std::hash<string> str_hash_func_basic;

inline size_t get_hash_file_name(const string &to_persistent_string) {
    return HASH_FUNC(to_persistent_string) % DB_FILE_NUM;
}

class Answer {
private:
    inline pair<string, string> split(const string &str) {
        auto iter_begin = str.begin();
        auto iter_end = str.end();
        auto iter_middle = find(iter_begin, iter_end, ',');
        return make_pair(string(iter_begin, iter_middle),
                         string(iter_middle + 1, iter_end - 1));
    }

public:
    Answer() {
    }

    inline string get(string &&key) { //读取KV
        size_t file_hash_index = get_hash_file_name(key);
        ifstream input_file_stream(to_string(file_hash_index));

        string tmp_string;
        string *result_ptr = nullptr;
        pair<string, string> my_pair;
        for (; input_file_stream.good();) {
            getline(input_file_stream, tmp_string);
            if (tmp_string.size() > 0 && tmp_string.substr(tmp_string.size() - 1) == SEPERATOR_END_STRING
                && find(tmp_string.begin(), tmp_string.end(), ',') != tmp_string.end()) {
                my_pair = split(tmp_string);
                if (my_pair.first == key) {
                    result_ptr = &my_pair.second;
                }
            }
        }
        if (result_ptr != nullptr) {
            return *result_ptr;
        }
        input_file_stream.close();
        return "NULL";
    }

    //Can be optimized with first read, remove duplicate and then write whole
    inline void put(string &&key, string &&value) { //存储KV
        size_t file_hash_index = get_hash_file_name(key);
        ofstream output_stream(to_string(file_hash_index));
        output_stream << key << SEPERATOR_STRING << value << SEPERATOR_END_STRING << '\n' << flush;
        output_stream.close();
    }
};

#endif //INMEMORYKEYVALUESTOREWITHPERSISTENCE_KEY_VALUE_STORE_H
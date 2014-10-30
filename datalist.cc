#include "datalist.h"
#include <iostream>
#include <fstream.h>

using namespace std;

float byte_swap_float32(float temp){
    return ((temp >> 24) | ((temp & 0x00ff0000) >> 8) | ((temp & 0x0000ff00) << 8) | (temp << 24));
}

DataList::DataList(){
    data_size_ = -1;
    data_list_ = NULL;
}

DataList::DataList(const int data_size){
    data_size_ = data_size;
    data_list_ = new float[data_size_];
}

DataList::~DataList(){
    if(data_list_ != NULL){
        delete [] data_list_;
    }
}

bool DataList::set_size(const int size){
    if(size <= 0){
        return false;
    }
    if(data_list_ != NULL){
        delete [] data_list_;
    }
    data_size_ = size;
    data_list_ = new float[data_size_];
    return true;
}

inline float DataList::get_value(const int i){
    if(i < 0 || i >= data_size_ || data_list_ == NULL){
        return 0.0;
    }
    return data_list_[i];
}

inline bool DataList::set_value(const int i, const float v){
    if(i < 0 || i >= data_size_ || data_list_ == NULL){
        return false;
    }
    data_list_[i] = v;
    return true;
}

inline bool DataList::get_size(){
    return data_size_;
}

DataList* load_data_from_file(const char *filename){
    ifstream file(filename, ios::in|ios::binary|ios::ate);
    int data_size = file.tellg() / sizeof(float);
    file.seekg(0, ios:beg);

    DataList *list = new DataList(data_size);
    char *buf = new char[sizeof(float)];

    for(int i = 0;i < data_size;i++){
        file.read(buf, sizeof(float));
        list->set_value(i, byte_swap_float32(*(float *)buf));
    }
    
    delete [] buf;
    file.close();
    return list;
}

#include "data.h"
#include <iostream>
#include <fstream>
#include <assert.h>

using namespace std;

float byte_swap_float32(char *buf){
    char temp = buf[0];
    buf[0] = buf[3];
    buf[3] = temp;
    temp = buf[2];
    buf[2] = buf[1];
    buf[1] = temp;

    return *(float *)buf;
}

Data::Data(){
    dimension_ = -1;
    size_ = -1;
    data_ = NULL;
}

Data::Data(int d, int s){
    /*assert((d > 0) && (s > 0));*/
    assert((d == DIMENSION) && (s > 0) && (s <= 1000000));
    dimension_ = d;
    size_ = s;
    data_ = new float*[size_];
    for(int i = 0;i < size_;++i){
        data_[i] = new float[dimension_];
    }
}

Data::~Data(){
    if(data_ != NULL){
        for(int i = 0;i < size_;++i){
            delete [] data_[i];
        }
        delete [] data_;
    }
}

Data* load_data_from_file(const char *filename){
    ifstream file(filename, ios::in|ios::binary|ios::ate);
    int data_size = file.tellg() / sizeof(float) / DIMENSION;
    file.seekg(0, ios::beg);

    Data *data = new Data(DIMENSION, data_size);
    char *buf = new char[sizeof(float)];

    for(int i = 0;i < data_size;i++){
        for(int j = 0;j < DIMENSION;++j){
            file.read(buf, sizeof(float));
            data->data_[i][j] = byte_swap_float32(buf);
        }
    }
    
    delete [] buf;
    file.close();
    return data;
}

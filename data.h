#ifndef DATA_H
#define DATA_H

#define DIMENSION 128

struct MyVector{
    int index_;
    float *vector_;
};

class Data{
    public:
        MyVector *data_;
        int dimension_;
        int size_;
    
    public:
        Data();
        Data(int d, int s);
        ~Data();
        float *get_value(int i);
        float get_value(int i, int j);
        MyVector *get_vector(int i);
};

extern Data* load_data_from_file(const char *filename);

#endif

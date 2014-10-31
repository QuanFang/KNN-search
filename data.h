#ifndef DATA_H
#define DATA_H

#define DIMENSION 128

class Data{
    public:
        float **data_;
        int dimension_;
        int size_;
    
    public:
        Data();
        Data(int d, int s);
        ~Data();
};

extern Data* load_data_from_file(const char *filename);

#endif

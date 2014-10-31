#ifndef DATALIST_H
#define DATALIST_H

class DataList{
    private:
        float *data_list_;
        int data_size_;

    public:
        DataList();
        DataList(const int data_size);
        ~DataList();
        bool set_size(const int size);
        bool set_value(const int i, const float v);
        float get_value(const int i);
        int get_size();
};

extern DataList* load_data_from_file(const char *filename);

class Vectors{
    private:
        DataList* data_;
        int size_;
        
    public:
        Vectors();
        Vectors(int size);
        ~Vectors();
        
        
};

#endif

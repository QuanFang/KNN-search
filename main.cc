#include "data.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    char *filename = argv[1];
    Data *data = load_data_from_file(filename);
    cout << data->size_ << endl;
    return 0;
}

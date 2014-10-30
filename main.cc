#include "datalist.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    char *filename = argv[1];
    DataList *data = load_data_from_file(filename);
    cout << data->get_size() << endl;
    return 0;
}

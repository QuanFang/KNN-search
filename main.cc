#include "data.h"
#include "hashfunctions.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    char *filename = argv[1]; //data file path
    int projections_num = atoi(argv[2]); //l in the paper, number of projections
    int projection_size = atoi(argv[3]); //k in the paper, size of a projection
    
    Data *data = load_data_from_file(filename);  //load data
    Projections *projections = get_random_projections(projections_num, projection_size, 1000); //randomly generate projection coordinates
    HashFunctions *hash_functions = new HashFunctions(projections_num, projection_size); //init hash functions

    hash_insert(hash_functions, data, projections);//apply hash funtions to the data
    
    set<MyVector *> query_result = get_near_vectors(hash_functions, data->get_vector(0), projections); //example of query, returns a set of vectors
    
    cout << query_result.size() << endl;

    return 0;
}

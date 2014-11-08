#include "data.h"
#include "hashfunctions.h"

#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    char *mode = argv[1];
    if(mode[0] == 'p' or mode [0] == 'P'){
        char *filename = argv[2]; //data file path
        int projections_num = atoi(argv[3]); //l in the paper, number of projections
        int projection_size = atoi(argv[4]); //k in the paper, size of a projections
        int width = atoi(argv[5]); //width of p-stable hashing
        float d_mean = atof(argv[6]); //mean value of normal distribution
        float d_deviation = atof(argv[7]); //standard deviation of normal distribution
        
        Data *data = load_data_from_file(filename);  //load data
        PProjections *projections = get_random_p_projections(projections_num, projection_size, width, d_mean, d_deviation); //generate projection randomly
        PHashFunctions *hash_functions = new PHashFunctions(projections_num, projection_size); //init hash functions

        p_hash_insert(hash_functions, data, projections);//apply hash funtions to the data
        
        set<MyVector *> query_result = get_p_near_vectors(hash_functions, data->get_vector(0), projections); //example of query, returns a set of vectors
        
        cout << query_result.size() << endl;
    }
    else{
        char *filename = argv[2]; //data file path
        int projections_num = atoi(argv[3]); //l in the paper, number of projections
        int projection_size = atoi(argv[4]); //k in the paper, size of a projection
        
        Data *data = load_data_from_file(filename);  //load data
        Projections *projections = get_random_projections(projections_num, projection_size, 1000); //randomly generate projection coordinates
        HashFunctions *hash_functions = new HashFunctions(projections_num, projection_size); //init hash functions

        hash_insert(hash_functions, data, projections);//apply hash funtions to the data
        
        set<MyVector *> query_result = get_near_vectors(hash_functions, data->get_vector(0), projections); //example of query, returns a set of vectors
        
        cout << query_result.size() << endl;   
    }

    return 0;
}

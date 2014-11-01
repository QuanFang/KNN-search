#include "hashfunctions.h"
#include <cstdlib>
#include <ctime>
#include <assert.h>
#include <iostream>

using namespace std;

Projections::Projections(){
    l_ = -1;
    k_ = -1;
    projections_ = NULL;
}

Projections::Projections(int l, int k){
    assert((l >= 0) && (k >= 0));
    l_ = l;
    k_ = k;
    projections_ = new int*[l_];
    for(int i = 0;i < l_;++i){
        projections_[i] = new int[k_];
    }
}

Projections::~Projections(){
    if(projections_ != NULL){
        for(int i = 0;i < l_;++i){
            delete [] projections_[i];
        }
        delete [] projections_;
    }
}

HashFunctions::HashFunctions(){
    l_ = -1;
    k_ = -1;
    hashmaps_ = NULL;
}

HashFunctions::HashFunctions(int l, int k){
    assert((l >= 0) && (k >= 0));
    l_ = l;
    k_ = k;
    hashmaps_ = new unordered_map<int, list<MyVector *> >[l_];
}

HashFunctions::~HashFunctions(){
    if(hashmaps_ != NULL){
        delete [] hashmaps_;
    }
}

bool HashFunctions::insert(int hash_key, MyVector *value, int index){
    auto search_result = hashmaps_[index].find(hash_key);
    if(search_result == hashmaps_[index].end()){ // not found
        list<MyVector *> new_list;
        new_list.push_back(value);
        hashmaps_[index].insert(make_pair(hash_key, new_list));
    }
    else{
        search_result->second.push_back(value);
    }
    return true;
}

Projections *get_random_projections(int l, int k, int max_coordinate){
    srand((unsigned int)time(0));
    Projections *projections = new Projections(l, k);
    for(int i = 0;i < l;++i){
        for (int j = 0;j < k;++j){
            projections->projections_[i][j] = rand() % 128000;
        }
    }
    
    return projections;
}

int gen_hash_key(float *data_array, int *projection, int p_length){
    int hash_key = 0;
    for(int i = 0;i < p_length;++i){
        int data_index = projection[i] / 1000;
        int coordinate_index = projection[i] % 1000 + 1;
        if((int)data_array[data_index] >= coordinate_index){ //1
            hash_key = (hash_key | 0x01) << 1;
        }
        else{ //0
            hash_key = hash_key << 1;
        }
    }
    return (hash_key >> 1);
}

bool hash_insert(HashFunctions *hash_functions, Data *data, Projections *projections){
    for(int i = 0;i < hash_functions->l_;++i){
        for(int j = 0;j < data->size_;++j){
            int hash_key = gen_hash_key(data->get_vector(j)->vector_, projections->projections_[i], projections->k_);
            hash_functions->insert(hash_key, data->get_vector(j), i);
        }
    }
    return true;
}

set<MyVector *> get_near_vectors(HashFunctions *hash_functions, MyVector *vector, Projections *projections){
    set<MyVector *> result_set = set<MyVector *>();
    for(int i = 0;i < hash_functions->l_;++i){
        int hash_key = gen_hash_key(vector->vector_, projections->projections_[i], projections->k_);
        auto search_result = hash_functions->hashmaps_[i].find(hash_key);
        if(search_result != hash_functions->hashmaps_[i].end()){
            list<MyVector *> temp_list = search_result->second;
            list<MyVector *>::iterator temp_iterator = temp_list.begin();
            while(temp_iterator != temp_list.end()){
                result_set.insert(*temp_iterator);
                temp_iterator++;
            }
        }
    }
    
    result_set.erase(vector);
    return result_set;
}

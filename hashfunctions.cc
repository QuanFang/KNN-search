#include "hashfunctions.h"
#include <cstdlib>
#include <ctime>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <math.h>

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

PProjections::PProjections(){
    l_ = -1;
    k_ = -1;
    projections_ = NULL;
}

PProjections::PProjections(int l, int k, float width){
    assert((l >= 0) && (k >= 0));
    l_ = l;
    k_ = k;
    width_ = width;
    projections_ = new p_function[l_];
    for(int i = 0;i < l_;++i){
        projections_[i].vectors_ = new float*[k_];
        projections_[i].random_b_ = new float[k_];
        for(int j = 0;j < k_;++j){
            projections_[i].vectors_[j] = new float[DIMENSION];
        }
    }
}

PProjections::~PProjections(){
    if(projections_ != NULL){
        for(int i = 0;i < l_;++i){
            for(int j = 0;j < k_;++j){
                delete [] projections_[i].vectors_[j];
            }
            delete [] projections_[i].random_b_;
            delete [] projections_[i].vectors_;
        }
        delete [] projections_;
    }
}

float gaussrand(float mean, float standard_deviation)
{
    static float V1, V2, S;
    static int phase = 0;
    float X;
     
    if(phase == 0){
        do{
            float U1 = (float)rand() / RAND_MAX;
            float U2 = (float)rand() / RAND_MAX;
             
            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);
         
        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);
         
    phase = 1 - phase;
 
    return X;
}

p_function *PProjections::get_p_function(int i){
    assert((projections_ != NULL) && (i >= 0) && (i <= l_));
    return (projections_ + i);
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

PHashFunctions::PHashFunctions(){
    l_ = -1;
    k_ = -1;
    hashmaps_ = NULL;
}

PHashFunctions::PHashFunctions(int l, int k){
    assert((l >= 0) && (k >= 0));
    l_ = l;
    k_ = k;
    hashmaps_ = new unordered_map<string, list<MyVector *> >[l_];
}

PHashFunctions::~PHashFunctions(){
    if(hashmaps_ != NULL){
        delete [] hashmaps_;
    }
}

bool PHashFunctions::insert(string hash_key, MyVector *value, int index){
    auto search_result = hashmaps_[index].find(hash_key);
    if(search_result == hashmaps_[index].end()){ // not found
        list<MyVector *> new_list;
        new_list.push_back(value);
        hashmaps_[index].insert(make_pair(hash_key, new_list));
    }
    else{
        search_result->second.push_back(value);
        //cout << search_result->second.size() << endl;
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

PProjections *get_random_p_projections(int l, int k, int width, float mean, float standard_deviation){
    srand((unsigned int)time(0));
    PProjections *projections = new PProjections(l, k, width);
    for(int i = 0;i < l;++i){
        for(int j = 0;j < k;++j){
            projections->projections_[i].random_b_[j] = rand() % width;
            for(int m = 0;m < DIMENSION;++m){
                projections->projections_[i].vectors_[j][m] = gaussrand(mean, standard_deviation);
            }
        }
    }
    
    return projections;
}

float product(float *data_array, float *p_vector, int length){
    float result = 0;
    for(int i = 0;i < length;++i){
        result += data_array[i] * p_vector[i];
    }
    return result;
}

string gen_p_hash_key(float *data_array, p_function *projection, int p_length, float width){
    string hash_key = "";
    for(int i = 0;i < p_length;++i){
        int temp = (int)((product(data_array, projection->vectors_[i], DIMENSION) + projection->random_b_[i]) / width);
        stringstream n_str;
        n_str << temp;
        hash_key += "," + n_str.str();
    }
    cout << hash_key << endl;
    return hash_key;
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

bool p_hash_insert(PHashFunctions *hash_functions, Data *data, PProjections *projections){
    for(int i = 0;i < hash_functions->l_;++i){
        for(int j = 0;j < data->size_;++j){
            string hash_key = gen_p_hash_key(data->get_vector(j)->vector_, projections->get_p_function(i), projections->k_, projections->width_);
            hash_functions->insert(hash_key, data->get_vector(j), i);
        }
    }
    return true;
}

set<MyVector *> get_p_near_vectors(PHashFunctions *hash_functions, MyVector *vector, PProjections *projections){
    set<MyVector *> result_set = set<MyVector *>();
    for(int i = 0;i < hash_functions->l_;++i){
        string hash_key = gen_p_hash_key(vector->vector_, projections->get_p_function(i), projections->k_, projections->width_);
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

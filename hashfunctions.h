#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

#include "data.h"

#include <string>
#include <list>
#include <set>
#include <unordered_map>

using namespace std;

struct p_function{
    float **vectors_;
    float *random_b_;
};

class Projections{
    public:
        int l_;
        int k_;
        int **projections_;
        
    public:
        Projections();
        Projections(int l, int k);
        ~Projections();
};

class PProjections{
    public:
        int l_;
        int k_;
        float width_;
        p_function *projections_;
    
    public:
        PProjections();
        PProjections(int l, int k, float width);
        ~PProjections();
        p_function *get_p_function(int i);
};

class HashFunctions{
    public:
        int l_;  //number of function in LSH
        int k_;  //projection size in LSH
        unordered_map<int, list<MyVector *> > *hashmaps_;
        
    public:
        HashFunctions();
        HashFunctions(int l, int k);
        ~HashFunctions();
        bool insert(int hash_key, MyVector *value, int index);
};

class PHashFunctions{
    public:
        int l_;
        int k_;
        unordered_map<string, list<MyVector *> > *hashmaps_;
    
    public:
        PHashFunctions();
        PHashFunctions(int l, int k);
        ~PHashFunctions();
        bool insert(string hash_key, MyVector *value, int index);
};

extern Projections *get_random_projections(int l, int k, int max_coordinate);
extern PProjections *get_random_p_projections(int l, int k, int width, float mean, float standard_deviation);
extern bool hash_insert(HashFunctions *hash_functions, Data *data, Projections *projections);
extern bool p_hash_insert(PHashFunctions *hash_functions, Data *data, PProjections *projections);
extern set<MyVector *> get_near_vectors(HashFunctions *hash_functions, MyVector *vector, Projections *projections);
extern set<MyVector *> get_p_near_vectors(PHashFunctions *hash_functions, MyVector *vector, PProjections *projections);

#endif

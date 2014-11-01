#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

#include "data.h"

#include <string>
#include <list>
#include <set>
#include <unordered_map>

using namespace std;

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

extern Projections *get_random_projections(int l, int k, int max_coordinate);
extern bool hash_insert(HashFunctions *hash_functions, Data *data, Projections *projections);
extern set<MyVector *> get_near_vectors(HashFunctions *hash_functions, MyVector *vector, Projections *projections);

#endif

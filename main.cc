#include "data.h"
#include "hashfunctions.h"

#include <iostream>
#include <time.h>

using namespace std;

double getDistanceOfTwoVector(MyVector* a, MyVector* b);
int getPosition(double* array,double d,int l,int r);

int main(int argc, char *argv[]){
    char *filename = argv[1]; //data file path
    int projections_num = atoi(argv[2]); //l in the paper, number of projections
    int projection_size = atoi(argv[3]); //k in the paper, size of a projection
    int k_nearest_neighbour = atoi(argv[4]);
    
    Data *data = load_data_from_file(filename);  //load data
    Projections *projections = get_random_projections(projections_num, projection_size, 1000); //randomly generate projection coordinates
    HashFunctions *hash_functions = new HashFunctions(projections_num, projection_size); //init hash functions

    hash_insert(hash_functions, data, projections);//apply hash funtions to the data
    


    set<MyVector *> query_result = get_near_vectors(hash_functions, data->get_vector(0), projections); //example of query, returns a set of vectors
    

    MyVector* a = data->get_vector(0);

    set<MyVector *>::iterator it;

    int getNum = 0;
    int* indexArray = new int[k_nearest_neighbour];
    double* distanceArray = new double[k_nearest_neighbour];
    double maxDistanceInArray = 0;
    for (it = query_result.begin();it != query_result.end();it ++){
        MyVector* tempVector = (*it);
        double tempDistance = getDistanceOfTwoVector(a,tempVector);
        cout<<tempDistance<<endl;
        if (getNum == 0){
            getNum++;
            distanceArray[0] = tempDistance;
            indexArray[0] = (*it)->index_;
            maxDistanceInArray = tempDistance;
        }else{
            if (getNum == k_nearest_neighbour && tempDistance >= maxDistanceInArray)
                continue;
            int trueIndex = getPosition(distanceArray,tempDistance,0,getNum-1);            
            cout<<"trueIndex:  "<<trueIndex<<endl;
            cout<<"getNum:   "<<getNum<<endl;
            for (int i = getNum-1;i > trueIndex ;i--){
                distanceArray[i] = distanceArray[i-1];
                indexArray[i] = indexArray[i-1];
            }

            if (getNum < k_nearest_neighbour)
                getNum++;

            distanceArray[trueIndex] = tempDistance;
            indexArray[trueIndex] = (*it)->index_;
            maxDistanceInArray = distanceArray[getNum-1];
        }
    }
    
    for (int i = 0;i < k_nearest_neighbour;i++){
        cout<<"index: "<<indexArray[i]<<endl;
    }

    cout << query_result.size() << endl;

    //getPosition  Test
    // srand((unsigned int)time(0));
    // for (int i = 0;i < 10;i++)
    // {
    //     double* a = new double[5];
    //     double* b = new double[4];
    //     for (int j = 0; j < 5;j++){
    //         a[j] = j * 2;
    //     }
    //     int aTest = rand() % 10;
    //     for (int j = 0;j < 4;j++){
    //         b[j] = j * 2;
    //     }

    //     int bTest = rand() % 10;

    //     int position_a = getPosition(a,aTest,0,4);
    //     int position_b = getPosition(b,bTest,0,3);

    //     cout<<"a:"<<endl;
    //     for (int j = 0;j < 5;j++){
    //         cout<<(int)a[j]<<" ";
    //     }
    //     cout<<endl;
    //     cout<<"Test Number:"<<aTest;
    //     cout<<" Position:"<<position_a;
    //     cout<<endl;
    //     cout<<"b:"<<endl;
    //     for (int j = 0;j < 4;j++){
    //         cout<<(int)b[j]<<" ";
    //     }
    //     cout<<"Test Number:"<<bTest;
    //     cout<<" Position:"<<position_b;
    //     cout<<endl;
    //     delete a;
    //     delete b;
    // }
    return 0;
}


double getDistanceOfTwoVector(MyVector* a, MyVector* b){
    double distance = 0;
    for (int i = 0;i < DIMENSION;i++){
        distance += (a->vector_[i] - b->vector_[i]) * (a->vector_[i] - b->vector_[i]);
    }
    distance = sqrt(distance);
    return distance;
}

int getPosition(double* array,double d,int l,int r){
    if (l >= r){
        if (array[l] >= d)
            return l;
        else
            return l + 1;
    }
    int mid = (l+r) / 2;
    if (array[mid] == d){
        return mid;
    }else if (array[mid] > d){
        return getPosition(array,d,l,mid-1);
    }else{
        return getPosition(array,d,mid+1,r);
    }
}
#include "data.h"
#include "hashfunctions.h"

#include <iostream>
#include <time.h>

using namespace std;

void myQuickSort(double* array, int l,int r);
double getDistanceOfTwoVector(MyVector* a, MyVector* b);
int getPosition(double* array,double d,int l,int r);
void testQuickSort();
pair<int*,int> myOldKNN(MyVector* v,set<MyVector* >* condidateSet, int k);
pair<int*,int> myNewKNN(MyVector* v,set<MyVector* >* condidateSet, int k);
void myNewQuickSort(int* indexArray,double* disArray,int l,int r);
void testMyNewKNN();
int myNewGetPosition(double* array,double d,int l,int r);

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
    int i;
    pair<int*, int> answer = myOldKNN(a,&query_result,k_nearest_neighbour);
    for (i = 0;i < answer.second;i++){
        cout<<" "<<answer.first[i];
    }
    cout<<endl;

    pair<int*, int> newAnswer = myNewKNN(a,&query_result,k_nearest_neighbour);
    for (i = 0;i < answer.second;i++){
        cout<<" "<<newAnswer.first[i];
    }
    cout<<endl;




    // set<MyVector *>::iterator it;

    // int getNum = 0;
    // int* indexArray = new int[k_nearest_neighbour];
    // double* distanceArray = new double[k_nearest_neighbour];
    // double maxDistanceInArray = 0;
    // for (it = query_result.begin();it != query_result.end();it ++){
    //     MyVector* tempVector = (*it);
    //     double tempDistance = getDistanceOfTwoVector(a,tempVector);
    //     cout<<tempDistance<<endl;
    //     if (getNum == 0){
    //         getNum++;
    //         distanceArray[0] = tempDistance;
    //         indexArray[0] = (*it)->index_;
    //         maxDistanceInArray = tempDistance;
    //     }else{
    //         if (getNum == k_nearest_neighbour && tempDistance >= maxDistanceInArray)
    //             continue;
    //         int trueIndex = getPosition(distanceArray,tempDistance,0,getNum-1);            
    //         cout<<"trueIndex:  "<<trueIndex<<endl;
    //         cout<<"getNum:   "<<getNum<<endl;
    //         for (int i = getNum-1;i > trueIndex ;i--){
    //             distanceArray[i] = distanceArray[i-1];
    //             indexArray[i] = indexArray[i-1];
    //         }

    //         if (getNum < k_nearest_neighbour)
    //             getNum++;

    //         distanceArray[trueIndex] = tempDistance;
    //         indexArray[trueIndex] = (*it)->index_;
    //         maxDistanceInArray = distanceArray[getNum-1];
    //     }
    // }
    
    // for (int i = 0;i < k_nearest_neighbour;i++){
    //     cout<<"index: "<<indexArray[i]<<endl;
    // }

    // cout << query_result.size() << endl;
    // srand((unsigned int)time(0));
    // for (int i = 0;i < 5;i++){
    //     testQuickSort();
    // }

    //getPosition  Test
    //srand((unsigned int)time(0));
    //testMyNewKNN();
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

pair<int*,int> myOldKNN(MyVector* v,set<MyVector* >* condidateSet, int k){
    int size = (*condidateSet).size();
    double *allDis = new double[size];
    int *allIndex = new int[size];
    set<MyVector* >::iterator it;
    int i = 0;
    for (it = condidateSet->begin();it != condidateSet->end();it++){
        allIndex[i] = (*it)->index_;
        allDis[i] = getDistanceOfTwoVector(v,(*it));
        i++;
    }

    myNewQuickSort(allIndex,allDis,0,size-1);

    if (k > size){
        k = size;
    }
    
    int* returnIndex;
    returnIndex = new int[k];
    for (int m = 0;m < k;m++){
        returnIndex[m] = allIndex[m];
    }
    delete allDis;
    delete allIndex;
    return make_pair(returnIndex,k);
}

void testMyNewKNN(){
    double* t = new double[10];
    for (int i = 0; i < 10;i ++){
        t[i] = rand() % 100;
        cout<<" "<<t[i];
    }
    cout<<endl;
    double* r = new double[5];
    int getNum = 0;
    double max = 0;

    for (int i = 0;i < 10;i ++){
        cout<<"t[i]:"<<t[i]<<"  ";
        if (getNum == 0){
            getNum++;
            r[0] = t[i];
            max = t[i];
        }else{

            if (getNum == 5 && t[i] >= max){
                continue;
            }

            cout<<"r[i]:"<<endl;

            for (int i = 0;i < getNum;i ++){
                cout<<" "<<r[i];
            }
            cout<<endl;
            int trueIndex = getPosition(r,t[i],0,getNum-1);
            cout<<"trueIndex:"<<trueIndex<<"  ";
            if (getNum == 5){
                for (int i = getNum-1;i > trueIndex ;i--){
                    r[i] = r[i-1];
                }
            }else{
                getNum++;
                for (int i = getNum-1;i > trueIndex ;i--){
                    r[i] = r[i-1];
                }
            }

            r[trueIndex] = t[i];
        }
        cout<<endl;
    }
    for (int i = 0;i < 5;i ++){
        cout<<" "<<r[i];
    }
    cout<<endl;
}

pair<int*,int> myNewKNN(MyVector* v,set<MyVector* >* condidateSet, int k){
    set<MyVector *>::iterator it;

    int getNum = 0;
    int* indexArray = new int[k];
    double* distanceArray = new double[k];
    double maxDistanceInArray = 0;

    for (it = condidateSet->begin();it != condidateSet->end();it ++){
        MyVector* tempVector = (*it);
        double tempDistance = getDistanceOfTwoVector(v,tempVector);

        if (getNum == 0){
            getNum++;
            distanceArray[0] = tempDistance;
            indexArray[0] = (*it)->index_;
            maxDistanceInArray = tempDistance;
        }else{
            if (getNum == k && tempDistance >= maxDistanceInArray)
                continue;
            int trueIndex = myNewGetPosition(distanceArray,tempDistance,0,getNum-1);

            if (getNum < k)
                getNum++;

           
            for (int i = getNum-1;i > trueIndex ;i--){
                distanceArray[i] = distanceArray[i-1];
                indexArray[i] = indexArray[i-1];
            }
            

            distanceArray[trueIndex] = tempDistance;
            indexArray[trueIndex] = (*it)->index_;
            maxDistanceInArray = distanceArray[getNum-1];
        }
    }

    if (k > condidateSet->size())
        k = condidateSet->size();

    delete distanceArray;
    return make_pair(indexArray,k);
}

double getDistanceOfTwoVector(MyVector* a, MyVector* b){
    double distance = 0;
    for (int i = 0;i < DIMENSION;i++){
        distance += (a->vector_[i] - b->vector_[i]) * (a->vector_[i] - b->vector_[i]);
    }
    distance = sqrt(distance);
    return distance;
}

//可能最好改成循环的而不是递归的
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

int myNewGetPosition(double* array,double d,int l,int r){
    int mid;
    while (l < r){
        mid = l + (r - l) / 2;
        if (array[mid] == d){
            return mid;
        }else if (array[mid] > d){
            r = mid - 1;
        }else{
            l = mid + 1;
        }
    }


    if (array[l] >= d)
        return l;
    else
        return l + 1;
}

void myQuickSort(double* array, int l,int r){
    if (l >= r)
        return;
    int i = l+1;
    double temp = array[l];
    for (int k = l+1;k <= r;k++){
        if (array[k] < temp){
            double tempAgain = array[i];
            array[i] = array[k];
            array[k] = tempAgain;
            i++;
        }
    }
    array[l] = array[i-1];
    array[i-1] = temp;
    myQuickSort(array,l,i-2);
    myQuickSort(array,i,r);
}

void myNewQuickSort(int* indexArray,double* disArray,int l,int r){
    if (l >= r)
        return;
    int i = l+1;
    double temp = disArray[l];
    int tempI = indexArray[l];
    for (int k = l+1;k <= r;k++){
        if (disArray[k] < temp){
            double tempAgain = disArray[i];
            disArray[i] = disArray[k];
            disArray[k] = tempAgain;
            int tempIndex = indexArray[i];
            indexArray[i] = indexArray[k];
            indexArray[k] = tempIndex;
            i++;

        }
    }
    disArray[l] = disArray[i-1];
    disArray[i-1] = temp;
    indexArray[l] = indexArray[i-1];
    indexArray[i-1] = tempI;
    myNewQuickSort(indexArray,disArray,l,i-2);
    myNewQuickSort(indexArray,disArray,i,r);
}

void testQuickSort(){
    
    double* a = new double[10];
    int i;
    cout<<"a kaishi:"<<endl;
    for (i = 0;i < 10;i ++){
        a[i] = rand() % 20;
        cout<<a[i]<<" ";
    }
    cout<<endl;
    myQuickSort(a,0,9);
    cout<<"test quick sort:"<<endl;
    for (i = 0;i < 10;i++){
        cout<<a[i]<<" ";
    }
    cout<<endl;
}
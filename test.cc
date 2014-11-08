#include "data.h"
#include "hashfunctions.h"

#include <iostream>
#include <fstream>
#include <string>

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
void intToBin(int *i, char *buf);

int main(int argc, char *argv[]){
    char *filename = argv[1];
    Data *data = load_data_from_file(filename);
    Data *test_data = load_data_from_file("./test.bin");

    set<MyVector *> query_set = set<MyVector *>();
    for(int i = 0;i < data->size_;++i){
        query_set.insert(data->get_vector(i));
    }

    char *buf = new char[sizeof(int)];
    
    ofstream result_file;
    if(data->size_ == 10000){
        result_file.open("10000.exa", ios::out|ios::binary);
    }
    else if(data->size_ == 100000){
        result_file.open("100000.exa", ios::out|ios::binary);
    }
    else{
        result_file.open("1000000.exa", ios::out|ios::binary);
    }
    
    for(int i = 0;i < test_data->size_;++i){
        pair<int *, int> query_result = myNewKNN(test_data->get_vector(i), &query_set, 100);
        for(int j = 0;j < 100;++j){
            intToBin(&(query_result.first[j]), buf);
            result_file.write(buf, sizeof(int));
        }
    }
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

void intToBin(int *i, char *buf){
    buf[0] = *((char *)i + 3);
    buf[1] = *((char *)i + 2);
    buf[2] = *((char *)i + 1);
    buf[3] = *((char *)i);
}
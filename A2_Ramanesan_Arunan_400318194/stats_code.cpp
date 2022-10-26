#include <iostream>
#include <math.h>
using namespace std;

//Ramanesan Arunan
//400318194


// Define classes here 
class Stats {
    public:

    Stats(int*arr,int length);
    virtual ~Stats();
    double getmean();
    double getstdev();
    virtual void computeStats();
    virtual void printStats();

    private:

    double mean;
    double stdev;
    
    protected:

    int maxval;
    int*array1;
    int count;
};

Stats::Stats(int*arr,int length){
    array1 = new int[length];
    int curr = 0;
    for(int i = 0; i<length;i++){
        array1[i] = arr[i];
        if(arr[i]>curr){
            curr = arr[i];
        }
        
    }
    count = length;
    maxval = curr;
}

Stats::~Stats(){
    delete [] array1;
    /* cout << "stats destruct \n"; */
}

double Stats::getmean(){
    return mean;
}

double Stats::getstdev(){
    return stdev;
}

void Stats::computeStats(){
    double sum = 0.0; double var = 0.0;
    if(count == 0) { 
        mean = 0; stdev = 0; 
    }
    else{
        for(int i=0;i<count;i++){
            sum += array1[i];
            }
        mean = sum/count;
        for(int i=0;i<count;i++){
            var += pow(array1[i] - mean,2);
            }
        var = var/count;
        stdev = sqrt(var);

    }
    
}

void Stats::printStats(){
    cout << "mean = " << mean << " and standard deviation = " << stdev;
}


class OneVarStats: public Stats{
    
    public:
    OneVarStats(int*array,int length);
    ~OneVarStats();
    void computeStats();
    void printStats();

    private:
    int histLength;
    int*hist;
};

OneVarStats::OneVarStats(int*array,int length)
: Stats(array,length), histLength(maxval + 1), hist(new int[maxval + 1]) { }

OneVarStats::~OneVarStats(){
    delete [] hist;
    /* cout << "onevar destruct \n"; */
    
}

void OneVarStats::computeStats(){
    Stats::computeStats();    
    for(int i=0; i<histLength;i++){
        int counter = 0;
        for(int j = 0; j<count;j++){
            if(array1[j] == i){
                counter += 1;
            }
        }
        hist[i] = counter;
    }
}

void OneVarStats::printStats(){
    Stats::printStats();
    cout << "\n" << "[";
    for(int i = 0 ; i<histLength; i++){
        cout << hist[i] << " ";
    }
    cout << "]";
}



class TwoVarStats: public Stats{

    public:

    TwoVarStats(int*arr1,int*arr2, int length);
    ~TwoVarStats();
    void computeStats();
    void printStats();
  
    private:

    int*array2;
    double correlationCoef;
    double computeCorrCoef();

};

TwoVarStats::TwoVarStats(int*arr1,int*arr2, int length)
:Stats(arr1,length){
    array2 = new int[length];
    int curr = 0;
    for(int i = 0; i<length;i++){
        array2[i] = arr2[i];
    }
}

TwoVarStats::~TwoVarStats(){
    delete [] array2;
    /* cout << "2var destruct \n"; */
   
}


void TwoVarStats::computeStats(){
    Stats::computeStats();
    correlationCoef = computeCorrCoef();
}

void TwoVarStats::printStats(){
    Stats::printStats();
    cout << "\nthe correlation coefficient = " << correlationCoef;
}

// the correlation coefficient function 
double TwoVarStats::computeCorrCoef() {  // works on array1 and array2 and count
    double sumX = 0.0, sumY = 0.0, sumXY = 0.0;  // hold S(x), S(y), S(xy) respectively.
    double sumX2 = 0.0, sumY2 = 0.0;  // hold S(x^2), S(y^2) respectively.
    
    for (int i=0; i< count; i++){
        sumX += array1[i];
        sumY += array2[i];
        sumXY += array1[i] * array2[i];

        sumX2 += array1[i] *  array1[i];
        sumY2 += array2[i] *  array2[i];
    } 
    double corr_coef = (count * sumXY - sumX * sumY)/ (sqrt((count * sumX2 - sumX * sumX) * (count * sumY2 -  sumY * sumY))); 

    return corr_coef;
}


int main( ) {
    int x[] = {2, 4, 7, 11, 5};
    int y[] = {5, 9, 14, 20, 10};
    int z[] = {14, 7, 4, 9, 21};

    int stats_len = 4;
    Stats* pp[stats_len];
    pp[0] = new Stats(x, 5);
    pp[1] = new OneVarStats (x,  5);
    pp[2] = new TwoVarStats (x, y, 5);
    pp[3] = new TwoVarStats (y, z, 5);

    for (int i=0; i < stats_len; i++){
        pp[i]->computeStats();
        cout << "\n";
    }

    for (int i=0; i < stats_len; i++){
        pp[i]->printStats();
        cout << "\n";
    } 
    /* int x[5] = {1,2,3,4,5};
    int y[5] = {1,2,3,4,5};
    Stats t2(x,5);
    OneVarStats t3(x,5);
    TwoVarStats(x,y,5); */
    return EXIT_SUCCESS;
}
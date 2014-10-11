#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <string.h>
#include <fstream>

#define dimension 3
#define n 20
#define iteration 1000
#define probabilityNumber 100

using namespace std;

string iToString(int i){
	string s ="";
	char c = i + '0';
	s.push_back(c);
	return s;
}

int prob(int k){
	int number = rand() % probabilityNumber;
	if (number <= k){
		return 1;
	}
	return 0;
}

vector<int> fillIndexes(int index){
	vector<int> indexes;
	for(int i=dimension-1; i>=0; i--){
		int d = index / (int)pow(n, i);
		indexes.push_back(d);
		index = index - (d*(int)pow(n, i));
	}
	return indexes;
}

int getIndex(vector<int> indexes, int d, int upordown){
	if( (indexes[d] == n-1 && upordown == 1) || (indexes[d] == 0 && upordown == -1)){
		return -1;
	}
	int newIndex = 0;
	indexes[d] = indexes[d] + upordown;
	for(int i=0; i<dimension; i++){
		newIndex += indexes[i] * (int)pow(n, dimension-1-i);
	}
	return newIndex;
}

// does breadth-wise search for path
bool isPathNode (int *a, int index, int *seen){
	vector<int> indexes = fillIndexes(index);
	if(seen[index] == 0 && a[index] == 1){
		// there exists a path from first layer (dimension 0 = 0)
		if(indexes[0] == n-1){
			return true;
		}
		seen[index] = 1;
	}
	else{
		return false;
	}
	for(int d=0; d<dimension; d++){
		int newIndex = getIndex(indexes, d, 1);
		if(newIndex != -1){
			if(isPathNode(a, newIndex, seen)){
				return true;
			}
		}
		newIndex = getIndex(indexes, d, -1);
		if(newIndex != -1){
			if(isPathNode(a, newIndex, seen)){
				return true;
			}
		}
	}
	return false;
}


bool isPath(int *a){
	int *seen = (int *)malloc((int)pow(n, dimension) * sizeof(int));
	// check from all nodes if there is a path
	for(int i=0; i<(int)pow(n, dimension-1); i++){
		memset(seen, 0, (int)pow(n, dimension)*sizeof(int));
		if(isPathNode(a, i, seen)){
			return true;
		}
	}
	free(seen);
	return false;
}

int main(){
	srand(time(NULL));
	double d[probabilityNumber];
	memset(d, 0.0, sizeof(d));

	ofstream file;
	file.open("montecarlo_" + iToString(dimension) +  ".csv");

	for(int k=0; k<probabilityNumber; k++){
		for(int it=0; it<iteration; it++){
			int * a = (int *)malloc((int)pow(n, dimension) * sizeof(int));
			memset(a, 0, (int)pow(n, dimension)* sizeof(int));
			for(int i=0; i<(int)pow(n, dimension); i++){
				a[i] = prob(k);
			}			
			if(isPath(a)){
				d[k]++;
			}
			free(a);
		}
		d[k] = d[k]/iteration;
		file << d[k] << endl;
	}
	file.close();
	return 0;
}

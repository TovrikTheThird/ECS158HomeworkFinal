#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>    // std::find
#include <omp.h>
#include <time.h>

//Compile using g++ -fopenmp -g combn.cpp -o combn.out

using namespace std;

vector<int> values;
vector<vector <int> > allCombinations;

//Prints a singe vector out
void print1d(const vector<int>& v) {
	static int count = 0;
	for(int i = 0; i < v.size(); i++) {
		cout << v[i] << " ";
	}
	cout << endl;
}

//prints a 2D vector out 
void print2d(const vector<vector<int> > &v) {
	for(int i = 0; i < v.size(); i++) {
		for(int j = 0; j < v[i].size(); j++) {
			cout << v[i][j] << " ";
		}
		cout << endl;
	}
}

//pushes a 1D vector onto a 2D vector
void addComb(vector<vector<int> > &v, vector<int> &v2) {
	// Kinda like memoization that you talked about
	// If the combination already exists, don't add it
	if(std::find(v.begin(), v.end(), v2) == v.end()) {
		//cout << "BOO YA UNIQUE!!" << endl;
		v.push_back(v2);
	}
	//else
		//cout << "NOOO IT'S A DUPLICATE!!!" << endl;
}


void findCombsPar(int offset, int k, vector<int> &combination) {
	if (k == 0) {
		#pragma omp critical
		addComb(allCombinations, combination);
		return;
	}
	for(int i = offset; i < values.size(); ++i) {
		combination.push_back(values[i]);
		findCombsPar(i + 1, k - 1, combination);
		combination.pop_back();
	}
}

//
vector<vector<int> > combn(int x, int m) {
	for(int i = 0; i < x; ++i)
		values.push_back(i+1);

	omp_set_num_threads(8); // Use 8 threads for all consecutive parallel regions

	#pragma omp parallel for
	for(int i = 0;  i < x - m; i++) {
		//printf("Thread rank: %d\n", omp_get_thread_num());
		vector<int> combination;
		findCombsPar(i, m, combination);
	}

	#pragma omp barrier
	//cout << "Total combinations: " << allCombinations.size() << endl;
	//print2d(allCombinations);
	vector< vector<int> > ret;
	ret = allCombinations;
	return ret;
}

int main (int argc, char** argv) {
	int x = atoi(argv[1]);
	int m = atoi(argv[2]);
	clock_t time;
	time = clock();
	vector<vector<int> > vals;
  vals = combn(x,m);
  time = clock() - time;
  //printf("Time Taken for %d values and %d combinations was: **%f** \n",x,m,((float)(time)/CLOCKS_PER_SEC));
  printf("%f,", ((float)(time)/CLOCKS_PER_SEC));
  return 0;
}

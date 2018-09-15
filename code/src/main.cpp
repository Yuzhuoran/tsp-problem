/*
 * main.cpp
 *
 *  Created on: Dec 3, 2016
 *      Author: Lenovo
 */

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <cmath>
#include <limits.h>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <map>
#include <list>
#include <set>
#include <time.h>
#include "MST_Approx.h"
#include "bnb.h"
#include "sa.h"
#include "nn.h"
#include "Two_EXrevised.h"

using namespace std;

int main(int argc, char *argv[]){

	if(argc!= 5){
		cout << "error: not enough input arguments";
	}

	string data_resource = argv[1];
	string algorithm = argv[2];
	int cut_off = stoi(argv[3]);
	int seed = stoi(argv[4]);


	char *data = argv[1];
	char *algo = argv[2];
	int cut = stoi(argv[3]);
	int s = stoi(argv[4]);

	if(algorithm == "bnb"){
        bnb(data_resource, algorithm, cut_off, seed);
	}
	else if(algorithm == "mst"){
		MST(data_resource, algorithm, cut_off, seed);
	}
	else if(algorithm == "2EXrevise"){
		Two_EXrevise(data_resource, algorithm, cut_off, seed);
	}
	else if(algorithm == "SA"){
		sa(data_resource, algorithm, cut_off, seed);
	}
	else if(algorithm == "nn"){
		nn(data, algo, cut, s);
	}


}



/*
 * sa.h
 *
 *  Created on: Dec 3, 2016
 *      Author: Lenovo
 */

#ifndef SA_H_
#define SA_H_

//* Debug with Chuanji Zhang *//

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <math.h>
#include <iomanip>
#include <time.h>


using namespace std;

vector<double> x;
vector<double> y;
//int seed;

struct tour{
	vector<int> beginnode;
	vector<int> endnode;
	vector<double> dist;
};

//* Find distance between two nodes
int distancecal(int begin, int end){
	int cost;
	cost = sqrt((x[begin-1]-x[end-1]) * (x[begin-1]-x[end-1]) + (y[begin-1]-y[end-1]) * (y[begin-1]-y[end-1]));
	return cost;
}


int costcal(tour original){
	int cost=0;
	for (unsigned int i =0; i< original.dist.size(); i++){
		cost = cost + original.dist[i];
	}
	return cost;
}

//* Two Opt Swap
tour two_opt_swap(int u, int v, tour original){
	tour cost = original;
	int first = min(u,v);
	int second = max(u,v);
	int temp = original.endnode[first];


	cost.endnode[first] = cost.beginnode[second];
	cost.dist[first] = distancecal(cost.beginnode[first], cost.endnode[first]);

	reverse(cost.beginnode.begin()+first+1, cost.beginnode.begin()+second);
	reverse(cost.endnode.begin()+first+1, cost.endnode.begin()+second);
	reverse(cost.dist.begin()+first+1, cost.dist.begin()+second);

	for (int i = first+1; i< second; i++){
		int tsp;
		tsp = cost.beginnode[i];
		cost.beginnode[i] = cost.endnode[i];
		cost.endnode[i] = tsp;
	}

	cost.beginnode[second] = temp;
	cost.dist[second] = distancecal(cost.beginnode[second], cost.endnode[second]);
	return cost;
}

//* Simulated Annealing *//
void tspcal(tour *original, int cutoff_time, double plg, string alg, string clg, int seed){


	double initialtemp = 100;
	double itertemp = 1;
	double temperature =initialtemp;
	double coefficient = 0.92;
	int size = original->beginnode.size();
	int bestsolution = costcal(*original);
	srand(seed);

	stringstream ss3;
	ss3<<(int)cutoff_time;
	string tlg = ss3.str();


	ofstream output;
	output.open("output//"+alg+"_SA_"+tlg+"_"+clg+".trace");
	ofstream output2;
	output2.open("output//"+alg+"_SA_"+tlg+"_"+clg+".sol");

	cutoff_time=cutoff_time*plg;

	while (clock()<cutoff_time){



		for (int ind = 0; ind<itertemp; ind++){

			int i = rand()%x.size();
			int j = rand()%x.size();
			while (j==i){
				j = rand()%x.size();
			}

			tour newtour=two_opt_swap(i, j, *original);
			int newsolution = costcal(newtour);
			if (newsolution < bestsolution){
				bestsolution = newsolution;
				*original=newtour;
				double time = (double)clock()/(double) CLOCKS_PER_SEC;
				output <<time<<","<<bestsolution<<endl;
			}
			else {
				int p = rand()%100;
				double q = (double)p/100;
				double pr = exp((bestsolution-newsolution)/temperature);
				if (q < pr){
					bestsolution = newsolution;
					*original=newtour;

					double time = (double)clock()/(double) CLOCKS_PER_SEC;
					output <<time<<" "<<bestsolution<<endl;
				}
			}

		}
		temperature = temperature * coefficient;


	}
	output2<< bestsolution<<endl;
	for (unsigned int i=0; i<x.size(); i++){
		output2<<  original->beginnode[i]<<" "<<  original->endnode[i]<<" "<<original->dist[i]<<endl;
	}
	output.close();
	output2.close();

}

int sa(string inputfile, string algorithm, int cuttime, int seed) {



	stringstream ss;
	ss<<seed;
    string clg = ss.str();

	string alg = inputfile;
	inputfile = "data//" + inputfile + ".tsp";

	ifstream input(inputfile);


	string line;
	for (int i =0; i<5; i++){
		getline(input,line);
	}


	string node;
	double x_tsp;
	double y_tsp;
	while(!input.eof()) {
		//cout << "check1" <<endl;
		input >> node;
		if (node.compare("EOF")!=0){
			input >> x_tsp;
			input >> y_tsp;
			x.push_back(x_tsp);
			y.push_back(y_tsp);

		}
		else
		break;
	}

	input.close();

//* Find Initial Solution
	tour initialtour;
	for (unsigned int i = 0; i<x.size(); i++){
		initialtour.beginnode.push_back(i+1);
		if (i != x.size()-1){
			initialtour.endnode.push_back(i+2);
			initialtour.dist.push_back(distancecal(i+1,i+2));
		}
		else{
			initialtour.endnode.push_back(1);
			initialtour.dist.push_back(distancecal(1,i+1));
		}
	}


	clock_t begintime = clock();
	double plg = (double) CLOCKS_PER_SEC + (double)begintime;
	tspcal(&initialtour, cuttime, plg, alg, clg, seed);

    return 0;

}



#endif /* SA_H_ */

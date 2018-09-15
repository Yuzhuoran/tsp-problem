/*
 * MST_Approx.h
 *
 *  Created on: Dec 3, 2016
 *      Author: Lenovo
 */

#ifndef MST_APPROX_H_
#define MST_APPROX_H_

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
#include <sstream>

using namespace std;

#define Max_Num 2000

struct City1{
	int id;
	double x;
	double y;
}city1[Max_Num];

int par[Max_Num];
int size1[Max_Num];
vector<vector<bool> > isMST(Max_Num, vector<bool> (Max_Num, false));
vector<bool> visited(Max_Num, false);
vector<int> P;

int readFile(char *input_file){
	ifstream input(input_file);
	char skip[200];
	char temp[200];
	int N;

	for(int i=0;i<5;i++){
		if(i==2){
			input >> temp >> N;
			//cout << "N is:"<< N <<endl;
		}
		input.getline(skip, sizeof(skip));
	}

	if(input){
		for(int i=0; i<N ;i++){
			input >> city1[i].id >> city1[i].x>> city1[i].y;
			//cout << city1[i].id << " " << city1[i].x << " " << city1[i].y << endl;
		}
		input.close();
		return N;
	}else return 0;

}

double distance(int i, int j){

	double dist = 0;
	dist = sqrt((city1[i].x - city1[j].x)*(city1[i].x - city1[j].x)+
			(city1[i].y - city1[j].y)*(city1[i].y - city1[j].y));
	return dist;
}

vector<vector<double> > init(int N){
	vector<vector<double> > dist_matrix(N);
	for(int i=0;i<N;i++){
		dist_matrix[i].resize(N);
	}
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			if(i==j){
				dist_matrix[i][j] = INT_MAX;
				//cout << "i:" << i << " j:" << j << " " << dist_matrix[i][j] <<endl;
			}else{
				dist_matrix[i][j] = distance(i,j);
				//cout << "i:" << i << " j:" << j << " " << dist_matrix[i][j] <<endl;
			}

		}
	}
	return dist_matrix;
}


struct Edge{
    int from;
    int to;
    double Wgt;

    Edge(int _from, int _to, double _Wgt = INT_MAX) :
        from(_from), to(_to), Wgt(_Wgt) {}

    bool operator <(const Edge & rhs) const {
        return Wgt < rhs.Wgt;
    }
};


list<Edge> Put_list(vector<vector<double> > & m, int N){

	list<Edge> Elist;
	for(int i=0; i<N; i++){
		for(int j=i+1; j<N; j++){
			Elist.push_back(Edge(i,j,m[i][j]));
		}
	}
	Elist.sort();//ascending order

	//print list
	list<Edge>::iterator it;
	//cout << "print List" << endl;
	for(it = Elist.begin(); it!=Elist.end(); it++){
		//cout << it->from << " " << it->to << " " << it->Wgt <<endl;
	}

	return Elist;

}


void init_union(int n){
	for(int i=0; i<n; i++){
		par[i] = i;
		size1[i] = 0;
	}
}

int find(int x){
	if(par[x] == x){
		return x;
	}else{
		par[x] = find(par[x]);
		return par[x];
	}
}

void unite(int x, int y){
	x=find(x);
	y=find(y);
	if(x==y) return;
	if(size1[x] < size1[y]){
		par[x] = y;
	}else{
		par[y] = x;
		if(size1[x] == size1[y]) size1[x]++;
	}
}


bool same(int x, int y){
	return find(x) == find(y);
}

bool find_path(vector<int> &P, int a){
	for(unsigned int i=0; i<P.size(); i++){
		if(P[i] == a){
			return true;
		}
	}
	return false;
}


void Kruskal(list<Edge> & l, int N, vector<vector<double> > & m){

	int count = 0;

	while(count < N-1 && !l.empty()){
		Edge e(l.front());
		l.pop_front();

		//cout << "-----------MST-----------" << endl;
		if(!same(e.from, e.to)){
			unite(e.from, e.to);
			//cout << e.from << e.to << endl;
			count ++;
			isMST[e.from][e.to] = isMST[e.to][e.from] = true;
		}
	}
}


void DFS (int u, int N){
	P.push_back(u);
	visited[u] = true;
	for(int i=0; i<N; i++){
		if(isMST[u][i] && !visited[i]) DFS(i,N);
	}
}


int get_distance(vector<int> & P, vector<vector<double> > & dist_matrix){

	double total_distance = 0;
	int a, b;
	for(unsigned int i=0; i<P.size()-1; i++){
		a = P[i];
		b = P[i+1];
		total_distance = total_distance + dist_matrix[a][b];
		//cout << total_distance <<endl;
	}

	return (int)(total_distance);
}


int MST(string name, string algorithm, int cut_off, int seed){

	clock_t startTime = clock();
	stringstream ss;
	ss<<cut_off;

		string clg = ss.str();

		char *input_file;
		char *output_file;
		char *output_file2;

		string path1 = "data//";
		path1.append(name);
		path1.append(".tsp");
		input_file= (char*)path1.data();

		string path2 = "output//";
		path2.append(name);
		path2.append("_");
		path2.append(algorithm);
		path2.append("_");
		path2.append(clg);
		path2.append(".sol");
		output_file = (char*)path2.data();

		string path3 = "output//";
		path3.append(name);
		path3.append("_");
		path3.append(algorithm);
		path3.append("_");
		path3.append(clg);
		path3.append(".trace");
		output_file2 = (char*)path3.data();

	int N = readFile(input_file);
	init_union(N);
	vector<vector<double> > dist_matrix = init(N);
	list<Edge> Elist = Put_list(dist_matrix, N);
	Kruskal(Elist,N,dist_matrix);
	srand(time(NULL));
	int start = 0;
	//cout << "the random number is : " << start <<endl;
    DFS(start,N);
    P.push_back(start);
    //cout << "-------------Path is-------------------------" <<endl;
    for(unsigned int i=0; i<P.size(); i++){
    	//cout << P[i] << endl;
    }

	//cout << "*************************************" <<endl;
	int total_distance = get_distance(P, dist_matrix);
	//cout <<"**************************************" <<endl;

	clock_t endTime = clock();
	double totalTime = (double)((endTime-startTime))/(double)CLOCKS_PER_SEC;

	ofstream output(output_file);
	output << total_distance << endl;
	for(unsigned int i=0; i<P.size()-1; i++){
		output << P[i] << " " << P[i+1] << " " << dist_matrix[P[i]][P[i+1]] << endl;
	}

	output.close();

	ofstream output2(output_file2);
	output2<< totalTime <<" " << total_distance<<endl;
	output2.close();

	//cout << "The path is: " << endl;
	for(unsigned int i=0; i<P.size()-1; i++){
		//cout << P[i] << " " << P[i+1] << " " << dist_matrix[P[i]][P[i+1]] << endl;
	}
	//cout << "The total distance is: " << total_distance << endl;
	//cout << totalTime << endl;
	return 0;

}


#endif /* MST_APPROX_H_ */

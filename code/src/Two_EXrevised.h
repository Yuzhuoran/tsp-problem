/*
 * Two_EXrevised.h
 *
 *  Created on: Dec 7, 2016
 *      Author: Lenovo
 */

#ifndef TWO_EXREVISED_H_
#define TWO_EXREVISED_H_

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <cmath>
#include <time.h>
#include <string.h>
#include <algorithm>
#include <map>

using namespace std;

#define Max_Num 2000
vector<bool> mark_matrix(500);

struct City{
		int id;
		double x;
		double y;
}city[Max_Num];

int tourlength = 0;
int numofcity = 0;


struct pairCompare{
	bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right){
		return left.second < right.second;
	}
};

vector<pair<int,double> > getNeighorSorted(vector<vector<double> > dist_matrix, int N, int src){

	vector<pair<int,double> > res;
	for(int i=0; i<N; i++){
		if(i == src) continue;
		res.push_back(make_pair(i,dist_matrix[src][i]));
	}
	sort(res.begin(), res.end(), pairCompare());
	return res;
}

vector<vector<pair<int,double> > > getNeighbor_matrix(vector<vector<double> > dist_matrix, int N){

	vector<vector<pair<int,double> > > sort_matrix(N);
	for(int i=0;i<N;i++){
		sort_matrix[i].resize(N);
	}
	for(int i=0; i<N; i++){
		sort_matrix[i] = getNeighorSorted(dist_matrix,N,i);
	}

	return sort_matrix;

}

bool find(int candidate, vector<int> & path){
	for(unsigned int i=0; i<path.size(); i++){
		if(path[i] == candidate){
			return true;
		}
	}
	return false;
}

vector<int> greedy_path(int N, vector<vector<double> > & m, int s){
	int start = s;
	double dist = INT_MAX;
	int temp = 0;
	vector<int> path;
	path.push_back(start);
	while(path.size() < (unsigned) N){
		for(int i=0; i<N;i++){
			if((i==start) || find(i, path)){
			}else{
				int dis_temp = m[start][i];
				if(dis_temp < dist){
					dist = m[start][i];
					temp = i;
				}
			}
		}
		dist = INT_MAX;
		path.push_back(temp);
		start = temp;
		temp = 0;
	}

	/*cout << "Init Random Tour:" <<endl;
	for(int i=0; i<N; i++){
		cout << path[i]<<endl;
	}
*/
	return path;

}

int tourLength(vector<int> & T, vector<vector<double> > & m){
	int l = 0;
	for(unsigned int i=0; i<T.size()-1; i++){
		int a=T[i];
		int b=T[i+1];
		l = l + m[a][b];
	}
	l = l + m[T[T.size()-1]][T[0]];
	return (int)(l);
}

void swap_two(vector<int> & T, int i, int k){
	
	int sizetempvalue = T.size();
	vector<int> temp(sizetempvalue, 0);
	//int temp[T.size()]={0};
	int index = 0;

	for(int x=k; x>=i; x--){
		temp[index]=T[x];
		index++;
	}

	index = 0;
	for(int x=i; x<=k; x++){
		T[x]=temp[index];
		index++;
	}


	//cout << "inside swap path" << endl;
	for(unsigned int i=0; i<T.size(); i++){
		//cout << T[i] <<endl;
	}
}

double distance3(int a, int b, int c, int d, vector<vector<double> > & m,vector<int> T){
	double dist = 0;
	dist = m[T[a]][T[b]] + m[T[c]][T[d]];

	return dist;
}

int get_total_distance(vector<int> T_NEW, vector<vector<double> > & dist_matrix){

	double total_distance = 0;
	for(unsigned int i=0; i<T_NEW.size()-1; i++){
		//cout << T_NEW[i] << " " << T_NEW[i+1] << " " << (int)dist_matrix[T_NEW[i]][T_NEW[i+1]] << endl;
		total_distance = total_distance + dist_matrix[T_NEW[i]][T_NEW[i+1]];
	}
	//cout << T_NEW[T_NEW.size()-1] << " " << T_NEW[0] << " " << (int)dist_matrix[T_NEW[T_NEW.size()-1]][T_NEW[0]] << endl;
	total_distance = total_distance + dist_matrix[T_NEW[T_NEW.size()-1]][T_NEW[0]];
	//cout << total_distance <<endl;
	return (int)total_distance;
}

int get_distancerevise(vector<int> T_NEW, vector<vector<double> > & dist_matrix){

	double total_distance = 0;
	for(unsigned int i=0; i<T_NEW.size()-1; i++){

		total_distance = total_distance + dist_matrix[T_NEW[i]][T_NEW[i+1]];
	}

	total_distance = total_distance + dist_matrix[T_NEW[T_NEW.size()-1]][T_NEW[0]];

	return (int)total_distance;
}

void init_mark(){
	for(int i=0; i<500; i++){
		mark_matrix[i] = false;
	}
}

bool DLB(int i){
	if(mark_matrix[i] == true){
		return true;
	}else{
		return false;
	}
}

void mark(int i){
	mark_matrix[i] = true;
}

void unmark(int i){
	mark_matrix[i] = false;
}

bool checkalldl(){
	int i;
	int cout=0;
	for(i=0;i<10;i++){
		if(mark_matrix[i]==false){
			cout++;
		}
	}
	if(cout!=0){
		return true;
	}else{
		return false;
	}
}

vector<int> algo2opt(vector<vector<pair<int,double> > > nm, vector<int> & T0, vector<vector<double> > & m,
		int cut_off, clock_t startTime, FILE *fp, int total_dist){

	int dist_new, dist_change;
	unsigned int k=0;
	unsigned int cut_number = T0.size();
	vector<int> T = T0;

	init_mark();

	while (((double)(clock()-startTime)/(double)CLOCKS_PER_SEC) <=cut_off && (k<cut_number)){
		int b = rand()%(T.size());
		b = T[b];
		int findb=0;
		int s, bpre, bnext, spre,bl, sl, bprel, snext, snextl, bnextl, sprel;

		if(DLB(b) == false){
			int flag=0;
			int flag2=0;
			for(int x=0; x<5; x++){
				s = nm[b][x].first;
				int finds=0;
				for(unsigned int i=0; i<T0.size(); i++){
					if((findb==1)&&(finds==1)){
						break;
					}
					if(T[i] == b){
						bl = i;
						findb=1;
						if(i==0){
							bpre = T[T.size()-1];
							bprel = T.size()-1;
						}else{
							bpre = T[i-1];
							bprel = i-1;
						}
						if(i == (T.size()-1)){
							bnext = T[0];
							bnextl = 0;
						}else{
							bnext = T[i+1];
							bnextl = i+1;
						}
					}
					if(T[i] == s){
						sl = i;
						finds=1;
						if(i==0){
							spre = T[T.size()-1];
							sprel = T.size()-1;
						}else{
							spre = T[i-1];
							sprel = i-1;
						}
						if(i==(T.size()-1)){
							snext = T[0];
							snextl = 0;
						}else{
							snext = T[i+1];
							snextl = i+1;
						}
					}
				}


				if(m[b][s] < m[b][bpre]){

					int cost = (m[b][s] + m[bpre][spre])-(m[b][bpre] + m[s][spre]);
					dist_change = get_distancerevise(T,m) + cost;
					//cout << dist_change <<endl;
					if((cost<0) ){
						if(DLB(bpre)==true){
							unmark(bpre);
							if(k<1){
								k=0;
							}else{
							    k=k-1;
							}
						}
						if(DLB(spre)==true){
							unmark(spre);
							if(k<1){
								k=0;
							}else{
								k=k-1;
							}
						}
						if(DLB(b)==true){
							unmark(b);
							if(k<1){
								k=0;
							}else{
								k=k-1;
							}
						}
						if(DLB(s)==true){
							unmark(s);
							if(k<1){
								k=0;
							}else{
								k=k-1;
							}
						}


						


						if(bl>sl){
							swap_two(T,sl,bprel);
						}else{
							swap_two(T,bl,sprel);
						}

						dist_new = get_distancerevise(T,m);

						double totalTime = ((double)((clock()-startTime))/(double)CLOCKS_PER_SEC);
						//fprintf(fp,"prev\n");
						if(dist_new < total_dist){
							fprintf(fp,"%lf,%d\n",totalTime,dist_new);
						}
					    flag = 1;
						break;
					}
				}else if(m[b][s] < m[b][bnext]){

					int cost = (m[b][s] + m[bnext][snext])-(m[b][bnext] + m[s][snext]);

					dist_change = get_distancerevise(T,m) + cost;
					if((cost<0)){
						if(DLB(bnext)==true){
							unmark(bnext);
							if(k<1){
								k=0;
							}else{
								k=k-1;
						    }
						}
						if(DLB(snext)==true){
							unmark(snext);
							if(k<1){
							    k=0;
							}else{
							    k=k-1;
							}
						}
						if(DLB(b)==true){
							unmark(b);
							if(k<1){
								k=0;
							}else{
								k=k-1;
							}
						}
						if(DLB(s)==true){
							unmark(s);
							if(k<1){
								k=0;
							}else{
								k=k-1;
							}
						}

						for(int i=0; i<10;i++){
							//cout <<T[i] <<endl;
						}

						if(bl>sl){
							swap_two(T,snextl,bl);
						}else{
							swap_two(T,bnextl,sl);
						}


						dist_new = get_distancerevise(T,m);

						double totalTime = ((double)((clock()-startTime))/(double)CLOCKS_PER_SEC);
						//fprintf(fp,"next\n");
						if(dist_new < total_dist){
							fprintf(fp,"%lf,%d\n",totalTime,dist_new);
						}
						flag = 1;
						break;
					}
				}else{
					mark(b);
					k++;
					flag2=1;
					break;
				}
			}
			if((flag==1)||(flag2==1)){
               //break;
			}else{
			   mark(b);
			   k++;
			}
		}
	}
	//fprintf(fp,"k is: %d\n", k);
	//fprintf(fp,"jump\n");
	return T;
}



int Two_EXrevise(string name, string algorithm, int cut_off, int seed){

	stringstream ss;
	ss<<cut_off;
	string clg = ss.str();
	stringstream ss2;
	ss2<<seed;
	string seednumber = ss2.str();



	char *input_file;
	char *output_file;
	char* result;


	string path1 ="data//";
	path1.append(name);
	path1.append(".tsp");
	input_file= (char*)path1.data();

	string path2 = "output//";
	path2.append(name);
	path2.append("_");
	path2.append(algorithm);
	path2.append("_");
	path2.append(clg);
	path2.append("_");
	path2.append(seednumber);
	path2.append(".sol");
	output_file = (char*)path2.data();

	string path3="output//";
	path3.append(name);
	path3.append("_");
	path3.append(algorithm);
	path3.append("_");
	path3.append(clg);
	path3.append("_");
	path3.append(seednumber);
	path3.append(".trace");
	result = (char*)path3.data();

	FILE *fp=NULL;
	fp= fopen(result,"a");
	int N = readFile(input_file);
	vector<vector<double> > dist_matrix = init(N);
	vector<vector<pair<int,double> > > neighbor_matrix = getNeighbor_matrix(dist_matrix,N);
 	clock_t startTime = clock();
	vector<int>T_NEW;
	int total_distance = INT_MAX;

	srand(seed);

	while(((double)((clock()-startTime))/(double)CLOCKS_PER_SEC) <=cut_off){
	     int s = rand() % N;
	     //cout << "----------" <<endl;
	    // cout << "s: " << s << endl;
	     vector<int> T0 = greedy_path(N, dist_matrix,s);
	     tourLength(T0,dist_matrix);
	     T_NEW = algo2opt(neighbor_matrix,T0, dist_matrix,cut_off, startTime, fp, total_distance);
	     int td=get_total_distance(T_NEW, dist_matrix);
	     if(td<total_distance){
	    	total_distance = td;
	     }
	     //cout <<"td: " << td << " total_distance: " << total_distance << endl;
	     double timenow = ((double)((clock()-startTime))/(double)CLOCKS_PER_SEC);
	    // cout << "now time: " << timenow <<endl;
	}

	T_NEW.push_back(T_NEW[0]);

	ofstream output(output_file);
	output << total_distance << endl;
	int mark;
	for(unsigned int i=0; i<T_NEW.size()-1; i++){
		if(T_NEW[i]==0){
			 mark = i;
		}
	}

	for(unsigned int i=mark; i<T_NEW.size()-1; i++){
		output << T_NEW[i] << " " << T_NEW[i+1] << " " << (int)dist_matrix[T_NEW[i]][T_NEW[i+1]] << endl;
	}
	for(int i=0; i<mark; i++){
		output << T_NEW[i] << " " << T_NEW[i+1] << " " << (int)dist_matrix[T_NEW[i]][T_NEW[i+1]] << endl;
	}


	output.close();
	fclose(fp);

  return 0;

}

#endif /* TWO_EXREVISED_H_ */

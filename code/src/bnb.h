/*
 * bnb.h
 *
 *  Created on: Dec 7, 2016
 *      Author: Lenovo
 */

#ifndef BNB_H_
#define BNB_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include<algorithm>
#include <sstream>
const int Inf = INT_MAX / 100;
using namespace std;
using namespace std::chrono;

class solution {
private:
	struct PrimNode {
		int id;
		int weight;
		int pre;
		int deg;
		bool visit;
		PrimNode(int a, int b, bool v = false, int c = -1, int d = 0) : id(a), weight(b), pre(c), deg(d), visit(v) {}
	};
	struct city {
		int id;
		bool visit;
		city(int a) :id(a), visit(false) {}
	};
	struct point {
		double	x;
		double y;
	};
	int city_size = 0;
	string name;
	vector<point> vectex;// Store the node information
	vector<city> cty;
	int **DistMatrix;// Distance Matrix
public:
	int GetSize() { return city_size; }
	vector<point> GetVectex() { return vectex; }
	vector<city> GetCty() { return cty; }
	int **GetDistMatrix() { return DistMatrix; }
	void FreeDm(){
		for(int i=0;i<city_size;i++)
			delete []DistMatrix[i];
		delete []DistMatrix;
	}
	void Construct_tsp(string input_file_location)// construct TSP node
	{
		ifstream data;
		char temp[50];
		const int length = 100;
		data.open(input_file_location);
		if (!data.is_open())
		{
			cout << "open file error" << endl;
			return;
		}
		data >> temp;
		data >> name;
		data >> temp;
		data >> city_size;
		for (int i = 1; i <= 4; i++)
		{
			data.getline(temp, length);
		}
		int n = 1;
		while (n <= city_size)
		{
			point tmpt;
			data >> temp;
			data >> tmpt.x;
			data >> tmpt.y;
			vectex.push_back(tmpt);
			n++;
		}
	}
	void Construct_DistMatrix() {
		DistMatrix = new int*[city_size];
		for (int i = 0; i < city_size; i++)
		{
			DistMatrix[i] = new int[city_size];
			for (int j = 0; j < city_size; j++)
			{
				if (i == j)
					DistMatrix[i][j] = Inf;
				else
					DistMatrix[i][j] = dist(vectex[i], vectex[j]);
			}
		}
	}
	void Construct_All(string input_file_location) {
		Construct_tsp(input_file_location);
		Construct_DistMatrix();
	}
	int dist(point p1, point p2)
	{
		return int(sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)) + 0.5);
	}
	int MST_index_pop(vector<PrimNode> q)//get minimum index to pop in the prime algorithm for mst problem
	{
		int u = Inf;
		int index = 1;
		for (unsigned int i = 0; i < q.size(); i++)
		{
			if (q[i].visit == true) continue;// has already visit the node in a tree
			if (q[i].weight < u)// get the node with smallest adjacent edge
			{
				u = q[i].weight;
				index = i;
			}
		}
		return index;
	}
	int OneTreeMST(vector<int> &deg, vector<int> &pre, int **DistMatrix, const vector<int> &pi) {
		// Get the MST for construct a 1-Tree Bound, record the information of degree and path
		int weight = 0;
		int itr = 1;
		int s = deg.size();
		vector<PrimNode> q;
		q.push_back(PrimNode(0, Inf, true)); // not visit0
		for (int i = 1; i < city_size; i++)//node 0 is exclude for the MST, start with weight=Inf
		{
			q.push_back(PrimNode(i, Inf));
		}
		while (itr <= q.size() - 1)
		{
			int v = MST_index_pop(q);// index to pop from vector q
			q[v].visit = true;// visit the node
			if (itr != 1)// itr=1, the first node put int
			{
				weight += q[v].weight;
				q[v].deg++;
				q[q[v].pre].deg++;
			}
			//q[v].id is the node to delete; Find its neighboor in the subG
			for (unsigned int i = 1; i < q.size(); i++)
			{
				if (q[i].visit == true) continue;
				if (DistMatrix[v][i] + pi[v] + pi[i] < q[i].weight)// this is a matrix with the additional cost
				{
					q[i].weight = DistMatrix[v][i] + pi[v] + pi[i];
					q[i].pre = v;
				}
			}
			itr++;
		}
		deg[0] = 2;
		// get the information of degree and path, pre[i] is the node j which go to i, pre[i]=j where j->i
		for (unsigned int i = 1; i < q.size(); i++)
		{
			pre[i] = q[i].pre;
			deg[i] = q[i].deg;
		}
		return weight;
	}
	int ConnectFirstVertex(int s, int row, int **DistMatrix, const vector<int> &pi, int first = -1) {
		// find two distinct smallest edge to connected node 0
		int min = Inf;
		int index = 0;
		for (int i = 1; i < s; i++)
		{
			if (DistMatrix[row][i] + pi[row] + pi[i] < min&&first != i)
			{
				min = DistMatrix[row][i] + pi[row] + pi[i];
				index = i;
			}
		}
		return index;
	}
	int OneTreeBound(int K, vector<int> &deg, vector<int>&pre, vector<int> &zeroAdj, int**DistMatrix) {
		int HKbound = 0;
		int piSum = 0;
		int piw = 0;
		int t;// time step
		int s = deg.size();
		double t1;
		vector<int>pi(s, 0);
		// For K iteration, in this part, K is set to be 80
		for (int k = 1; k <= K; k++)
		{
			if (*max_element(deg.begin(), deg.end()) == 2) // if a tour is constructed, return
				break;
			zeroAdj.clear();
			int MstWeight = OneTreeMST(deg, pre, DistMatrix, pi);// return the 1-Tree MST and deg, path;without node 0
			int firstEdge = ConnectFirstVertex(s, 0, DistMatrix, pi);
			int secondEdge = ConnectFirstVertex(s, 0, DistMatrix, pi, firstEdge);
			//L is the 1-Tree Bound in k time step
			int L = MstWeight + DistMatrix[0][secondEdge] + pi[secondEdge] + DistMatrix[0][firstEdge] + pi[firstEdge];
			deg[firstEdge]++;
			deg[secondEdge]++;// connect these two node to node 0
			zeroAdj.push_back(firstEdge);
			zeroAdj.push_back(secondEdge);// these two node are 0 neighbour, record them
			double sum = 0;
			if (k == 1)
			{
				t1 = double(L) / (2 * s);
				t = int(t1);
			}
			else
			{
				t = t1*(k - 1) *(2 * K - 5) / (2 * K - 2) - (k - 2)*t1 + t1 * (k - 1)*(k - 2) / ((K - 1)*(K - 2) * 2);
				if (t < 0) break;
			}
			// check if the lower bound is updated
			piw = L - 2 * piSum;
			if (piw > HKbound)
				HKbound = piw;// update
			piSum = 0;
			//update the pi
			for (int i = 0; i <s; i++)
			{
				pi[i] += t*(deg[i] - 2);// pi(m+1)=pi(m)+t(m)*(deg(m)-2)
				piSum += pi[i];
			}
		}
		//return HKbound, and modify the degree, path information by reference
		return HKbound;
	}
	void BnBHK(int lb, int &optimal, vector<int> &OptPath, vector<int> deg, vector<int>pre, vector<int> zeroAdj, int K, ofstream &TraceOutput, high_resolution_clock::time_point &start, int**DistMatrix, int cutime) {
		if (lb > optimal)
			return;
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> durt = end - start;
		if (durt.count() > cutime){
			return;
		}

		int n = deg.size();
		if (*max_element(deg.begin(), deg.end()) == 2)// check if all degree is 2, then a tour is constructed
		{
			//cout << "solution " << lb << endl;
			if (optimal > lb)
			{
				auto trace = std::chrono::high_resolution_clock::now();// found an improved solution
				std::chrono::duration<double, std::milli> tracetime = trace - start;
				TraceOutput << double(tracetime.count() / 1000) << ", " << lb << endl;
				optimal = lb; // update the upper
							  // trhe following step is to recover the tour of the solution
				vector<int> res1;
				vector<int> res2;
				res1.push_back(zeroAdj[0]);
				int p = zeroAdj[0];
				while (pre[p] != -1)
				{
					res1.push_back(pre[p]);
					p = pre[p];
				}
				reverse(res1.begin(), res1.end());
				res2.push_back(0);
				p = zeroAdj[1];
				while (p != 1 && p != -1)// the last one is 1
				{
					res2.push_back(p);
					p = pre[p];
				}
				res2.insert(res2.end(), res1.begin(), res1.end());// connected two sequence
				OptPath = res2;// store the opt-path
			}
			return;
		}
		else
		{
			int MaxDeg = 0;
			int u;// u is the node to branch
			for (unsigned int i = 0; i < deg.size(); i++)
			{
				if (deg[i] > MaxDeg)
				{
					MaxDeg = deg[i];
					u = i;
				}
			}
			// choose the node with max deg to branch
			vector<int> child;
			// find the node adjacent  the adjacent of node i would be stored in 1.pre[i], or 2.pre[j]=i, or 3.zeroAdj[j]=i
			for (int i = 0; i < n; i++)
			{
				if (pre[i] == u)
					child.push_back(i);
			}
			if (pre[u] != -1)
				child.push_back(pre[u]);
			for (unsigned int i = 0; i < zeroAdj.size(); i++)
			{
				if (zeroAdj[i] == u)
					child.push_back(0);
			}
			// the next is to store the data to branch
			vector<vector<int> > childDeg;
			vector<vector<int> > childPre;
			vector<vector<int> > childAdj;
			vector<int> childLb;
			vector<int> validchild;
			//calculate and store the child lower bound, deg, tour. Then use the best-first branch
			for (unsigned int i = 0; i < child.size(); i++)
			{
				vector<int> cdeg(n, 0);
				vector<int> cpre(n, -1);
				vector<int> cAdj;
				int v = child[i];
				int temp1 = DistMatrix[u][v];
				DistMatrix[u][v] = Inf;
				DistMatrix[v][u] = Inf;
				int newlb = OneTreeBound(K, cdeg, cpre, cAdj, DistMatrix);// derive the lower bound, deg, tour information of the child
				if (newlb < optimal)// if lower bound > optimal, prune the leave
				{
					validchild.push_back(v);// the branch child, other is delete
					childLb.push_back(newlb);// store the lower bound
					childDeg.push_back(cdeg);// store the related degree
					childPre.push_back(cpre);// store the related tour information
					childAdj.push_back(cAdj);// store the node adjacent to node 0
				}
				DistMatrix[u][v] = temp1;
				DistMatrix[v][u] = temp1;
			}
			for (unsigned int i = 0; i < childLb.size(); i++)
			{
				int minlb = Inf;
				int t;// t index is to explore next
				for (unsigned int j = 0; j < childLb.size(); j++)
				{
					if (childLb[j] < minlb)
					{
						minlb = childLb[j];
						t = j;
					}
				}
				// get the t index to explore with largest lower bound
				int v = validchild[t];// e(u,v) to delete
				int temp1 = DistMatrix[u][v];
				DistMatrix[v][u] = Inf;
				DistMatrix[u][v] = Inf;
				// the recursive part
				//cout << "loop " << K - 80 << endl;
				//cout << "Lower bound :" << childLb[t] << endl;
				// add M=M+1
				BnBHK(childLb[t], optimal, OptPath, childDeg[t], childPre[t], childAdj[t], K + 1, TraceOutput, start, DistMatrix, cutime);// the next loop iter increase by 10
																																		  //childLb[t] = -1;// after explore, set the lower bound to min weight
				childLb[t] = 2 * Inf;
				DistMatrix[v][u] = temp1;
				DistMatrix[u][v] = temp1;
			}
			return;
		}

	}
};


int bnb(string name, string al , int cutt, int seed ) {
	solution test;
	ofstream SolOutput;
	ofstream TraceOutput;

	stringstream ss;
	ss << cutt;
	string clg = ss.str();


	SolOutput.open("output/" + name + "_BnB_" + clg + ".sol");
	TraceOutput.open("output//" + name + "_BnB_" + clg + ".trace");
	test.Construct_All("data//" + name + ".tsp");
	cutt*=1e3;
	TraceOutput.precision(2);
	TraceOutput.setf(ios::fixed);
	vector<int> OptPath;
	vector<int> deg(test.GetSize(), 0);
	vector<int> pre(test.GetSize(), -1);
	vector<int> zeroAdj;
	auto start = std::chrono::high_resolution_clock::now();// start time
	int M = 80;// set M=80
	int ub = Inf; // does not need upper bound for initialization
	int lb = test.OneTreeBound(80, deg, pre, zeroAdj, test.GetDistMatrix());// Initialization bound
	test.BnBHK(lb, ub, OptPath, deg, pre, zeroAdj, 80, TraceOutput, start, test.GetDistMatrix(), cutt);// Branch
	if (ub != Inf)
	{
		SolOutput << ub << endl;
		for (unsigned int i = 0; i < OptPath.size() - 1; i++)
		{
			SolOutput << OptPath[i] << " " << OptPath[i + 1] << " " << test.GetDistMatrix()[i][i + 1] << endl;
		}
		if (!OptPath.empty())
			SolOutput << OptPath[OptPath.size() - 1] << " " << OptPath[0] << " " << test.GetDistMatrix()[OptPath.size() - 1][0];
	}
	else {
		cout << "Can't find a solution" << endl;
		//system("pause");
	}
	test.FreeDm();
	SolOutput.close();
	TraceOutput.close();
	return 0;

}



#endif /* BNB_H_ */

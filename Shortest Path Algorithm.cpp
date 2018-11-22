#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <list>

using namespace std;

class Graph {
	int vertices;
	list<pair<int, int>> *unDirectedEdges;
	list<pair<int, int>> *directedEdges;

public:
	Graph(int vertices);
	void addEdge(int u, int v, int weight, bool directed);

	void shortestPath(int s, bool directed);
	void minimumSpanningTree(int s);
};

Graph::Graph(int V)
{
	this->vertices = V;
	unDirectedEdges = new list<pair<int, int>>[vertices];
	directedEdges = new list<pair<int, int>>[vertices];
}

void Graph::addEdge(int u, int v, int weight, bool directed)
{
	u = u - 'A';
	v = v - 'A';
	if (directed)
	{
		directedEdges[u].push_back(make_pair(v, weight));
	}
	else
	{
		unDirectedEdges[u].push_back(make_pair(v, weight));
		unDirectedEdges[v].push_back(make_pair(u, weight));
	}
}

void Graph::shortestPath(int src, bool directed)
{
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> cloud;
	vector<int> dist(vertices, INT_MAX);

	src = src - 'A';
	cloud.push(make_pair(0, src));
	dist[src] = 0;

	while (!cloud.empty())
	{
		int u = cloud.top().second;
		cloud.pop();
		
		if (directed)	//Algorithm for directed graph
		{
			for (auto iter : directedEdges[u])
			{
				int adjVertex = iter.first;
				int weight = iter.second;

				if (dist[adjVertex] > dist[u] + weight)
				{
					dist[adjVertex] = dist[u] + weight;
					cloud.push(make_pair(dist[adjVertex], adjVertex));
				}
			}
		}
		else			//Algorithm for undirected graph
		{
			for (auto iter : unDirectedEdges[u])
			{
				int adjVertex = iter.first;
				int weight = iter.second;

				if (dist[adjVertex] > dist[u] + weight)
				{
					dist[adjVertex] = dist[u] + weight;
					cloud.push(make_pair(dist[adjVertex], adjVertex));
				}
			}
		}
	}
	cout << "Shortest Paths (Dijkstra's Algorithm)\n";
	cout << "Path	    Path Cost \n";
	cout << "-----------------------------\n";
	for (int i = 0; i < vertices; i++)
		cout << "A - " << (char)(i + 'A') << "\t\t" << dist[i] << endl;
}

void Graph::minimumSpanningTree(int src)
{
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> cloud;
	vector<int> dist(vertices, INT_MAX);
	vector<int> parent(vertices, INT_MAX);

	src = src - 'A';
	cloud.push(make_pair(0, src));
	dist[src] = 0;
	parent[src] = 0;
	while (!cloud.empty())
	{
		int u = cloud.top().second;
		cloud.pop();

		for (auto iter : unDirectedEdges[u])
		{
			int adjVertex = iter.first;
			int weight = iter.second;

			if (dist[adjVertex] > weight)
			{
				dist[adjVertex] = weight;
				parent[adjVertex] = u;
				cloud.push(make_pair(dist[adjVertex], adjVertex));
			}
		}
	}

	cout << "\nPrims Algorithm\n";
	cout << "Edges        Distances\n";
	cout << "----------------------\n";
	for (int i = 0; i < vertices; i++)
		cout << "" << (char)(parent[i] + 'A') << " - " << (char)(i + 'A') << "\t\t" << dist[i] << endl;
}

int main()
{
	ifstream file;
	file.open("input.txt");
	char src, dest;
	int weight, numberOfEdges;
	int vertices = 0;
	char directed;

	string line;
	string delimeter = " ";

	file >> vertices >> numberOfEdges >> directed;
	Graph g(vertices);

	char* srcEdge = new char[1];
	char* destEdge = new char[1];
	for (int counter = 0; counter <= numberOfEdges && getline(file, line); counter++)
	{
		vector<string> tokenizer;
		size_t pos = 0;
		while ((pos = line.find(delimeter)) != string::npos)
		{
			tokenizer.push_back(line.substr(0, pos));
			line.erase(0, pos + delimeter.length());
		}
		if (counter) 
		{
			strcpy(srcEdge, tokenizer[0].c_str());
			strcpy(destEdge, tokenizer[1].c_str());
			if(directed == 'D')
				g.addEdge(srcEdge[0], destEdge[0], stoi(line), true);
			else
				g.addEdge(srcEdge[0], destEdge[0], stoi(line), false);
		}
		tokenizer.clear();
	}

	file >> src;
	file.close();

	if (directed == 'U')
	{
		g.shortestPath(src, false);
		g.minimumSpanningTree(src);
	}
	else if(directed == 'D')
		g.shortestPath(src, true);
}

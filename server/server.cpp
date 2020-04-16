//------------------------------------------------------
// Names: Amro Amanuddein and Pushkar Sabharwal
// ID's: 1572498 (Amro) 1588927 (Pushkar)
// CMPUT275, Winter 2020
//
// Assignment 2 Part 2: Driving Route Finder
//------------------------------------------------------

// Including all required files
#include <iostream>
#include <list>
#include <cassert>
#include <sstream>
#include <string>
#include <fstream>
#include "wdigraph.h"
#include "heap.h"
#include "dijkstra.h"
#include "serialport.h"
using namespace std;

// Declaring struct for lat and lon
struct Point {
	long long lat;
	long long lon;
};

// Cost function implementation
long long manhatten(const Point& pt1, const Point& pt2) {
	long long mandist = abs((pt1.lat)-(pt2.lat)) + abs((pt1.lon)-(pt2.lon));
	return mandist;
}

// Readgraph implementation
void readGraph (string filename, WDigraph& graph, unordered_map<int, Point>& points) {
	// Reading the txt file
	ifstream fin;
	fin.open(filename);
	string line;
	string ID_str;
	string lat_str, lon_str;
	string startEdge, stopEdge;
	int ID, l, m;
	double lat_d, lon_d;
	long long lat, lon;
	// Keep reading until the end of file is not reached
	while (fin) {
		getline(fin, line);
		size_t foundV = line.find("V,");
		size_t foundE = line.find("E,");
		// Case in which V is found as the start char
		if (foundV != string::npos){
			startEdge = line.substr(2, line.size()-3);
			int posStart = startEdge.find(",");
			startEdge = startEdge.substr(0, posStart);
			ID_str = startEdge;
			stopEdge = line.substr(posStart+3, line.size()-posStart-2);
			int posStop = stopEdge.find(",");
			lat_str = stopEdge.substr(0,posStop);
			lon_str = stopEdge.substr(stopEdge.find(",")+1, stopEdge.size()-lat_str.size());

			ID = stoi(ID_str); 
			lat_d = stod(lat_str);
			lon_d = stod(lon_str);
			// Conversion to required data type
			lat = static_cast <long long> (lat_d*100000); 
			lon = static_cast <long long> (lon_d*100000);
			// Temporarily stores lat and lon
			Point temp_point;
			temp_point.lat = lat;
			temp_point.lon = lon;
			// Inserts the vertex into the WDigraph instance and unordered_map
			points.insert({ ID, temp_point});	
			graph.addVertex(ID);

		}
		// Case in which E is found as the start char
		else if (foundE != string::npos){
			startEdge = line.substr(2, line.size()-3);
			int posStart = startEdge.find(",");
			startEdge = startEdge.substr(0, posStart);
			stopEdge = line.substr(posStart+3, line.size()-posStart-2);
			int posStop = stopEdge.find(",");
			stopEdge = stopEdge.substr(0, stopEdge.size());
			l = stoi(startEdge);
			m = stoi(stopEdge);
			Point point1, point2;
			point1 = points[l];
			point2 = points[m];
			// Calculating cost of the current edge to connect the WDigraph
			long long cost_of_edge = manhatten(point1, point2);
			graph.addEdge(m, l, cost_of_edge);
		}
	}
	fin.close();
	
}
// Modified version of split from:
// https://stackoverflow.com/questions/11719538/how-to-use-stringstream-to-separate-comma-separated-strings
vector<string> split(string str, char delimiter){
	vector<string> vectorString;
	string token;
	stringstream ss(str);
	while(getline(ss,token,delimiter)){
		vectorString.push_back(token);
	}
	return vectorString;
}

// Function to find the closest Vertex taken from the CMPUT 275 Part 1 solution
int findClosest(const Point& point, const unordered_map<int, Point>& points){
	pair<int, Point> best = *points.begin();
	for (const auto& check : points){
		if (manhatten(point, check.second) < manhatten(point,best.second)){
			best = check;
		}
	}
	return best.first;
}

// Function to send messages from desktop to Serial
void toSerial(const string& line, SerialPort&Serial){
	cout << "Sending to serial... " << line << endl;
	assert(Serial.writeline(line + "\n"));
}

// If we dont recieve an acknowledgement i.e an A then it returns false
bool waitforAcknowledgement(SerialPort& Serial){
	bool A_or_nah = false;
	string Acknowledgement = Serial.readline(1000);
	if (Acknowledgement[0] == 'A'){
		A_or_nah = true;
	}
	return A_or_nah;
}

// This function takes the tree and creates a list of the path's vertices
list<int> pathCreation(unordered_map<int,PIL> &tree, pair <int,int> endPoint){
	list<int> created;
	int endVertex = endPoint.second;
	while (endVertex != endPoint.first){
		created.push_front(endVertex);
		endVertex = tree[endVertex].first;
	}
	// THe start vertex is placed in the front
	created.push_front(endPoint.first);
	return created;
}

// Implementation of Dijkstra's algorithm using pseudocode given on eclass
void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PIL>& tree) {
	BinaryHeap<pair<int, int>, long long> events;
	pair<int, int> startPair;
	startPair.first = startVertex;
	startPair.second = startVertex;
	events.insert(startPair, 0);
	while (events.size() > 0) {
		HeapItem<pair<int, int>, long long> minItem = events.min();
		events.popMin();
		unordered_map<int, PIL>::iterator it = tree.find(minItem.item.second);
		if (it == tree.end()) { 
			tree[minItem.item.second] = make_pair(minItem.item.first, minItem.key);
			for (auto i= graph.neighbours(minItem.item.second); i!=graph.endIterator(minItem.item.second); i++){
				pair<int, int> newPair = make_pair(minItem.item.second, *i);
				events.insert(newPair, minItem.key + graph.getCost(minItem.item.second, *i));
			}
		}
	}
}	

// Function to parse the client request form serial
pair<int,int> processRequest(unordered_map<int, Point>& points, string request){
	Point start;
	Point end;
	vector<string> splitStr = split(request, ' ');
	// Reading in the coordinates
	start.lat = stoll(splitStr[1]);
	start.lon = stoll(splitStr[2]);
	end.lat = stoll(splitStr[3]);
	end.lon = stoll(splitStr[4]);
	pair <int,int> vertex;
	// Map to a nearest vertex from the given coordinates
	vertex.first = findClosest(start, points);
	vertex.second = findClosest(end,points);
	return vertex;

}

// This function communicates with the Arduino from desktop and sends all the waypoints in correct.
void waypointstoSerial(const list<int>& path, unordered_map<int, Point>& points, SerialPort& Serial){
	// If we have a number of waypoints less than 500 or if there is no path at all between the two points
	if (path.size() < 500 || path.empty() != true){
		string waypointCount = "N " + to_string(path.size());
		// send it over to serial yea
		toSerial(waypointCount, Serial);
		if (waitforAcknowledgement(Serial)){
			for (auto iter: path){
				// The latitude and longitude coordinates are converted to strings to be sent over to Serial 
				string latitude = to_string(points[iter].lat);
				string longitude = to_string(points[iter].lon);
				// Forming the string to be sent over
				string waypoint = "W "+latitude+" "+longitude;
				// Send it yea
				toSerial(waypoint,Serial);
				// If there is no 'A' recieved
				if (!waitforAcknowledgement(Serial)){
					return;
				}
			}
			// E for End of communiation
			toSerial("E", Serial);
		}

	}
	else {
		// If there is no path or if there number of waypoints is greater than 500
		// then this will be handled by the client...
		cout << "NO PATH!" << endl;
		toSerial("N 0",Serial);
	}
}

// main function: implements as soon as program is run
int main() {
	WDigraph edmontonGraph;
	unordered_map<int, PIL> searchTree;
	unordered_map<int, Point> points; 
	readGraph("edmonton-roads-2.0.1.txt", edmontonGraph, points);
	SerialPort Serial("/dev/ttyACM0");
	while (true){
		string requests = Serial.readline(1000);
		// If we have a request from client
		if (requests[0] == 'R'){
			list<int> path;
			pair <int,int> vert = processRequest(points,requests);
			// Call dijkstra to obtain an unordered_map with distance to all nearest vertices
			dijkstra(edmontonGraph,vert.first,searchTree);
			path = pathCreation(searchTree,vert);
			// Send over the waypoints to serial
			waypointstoSerial(path,points,Serial);

		}
	}

	return 0;
}


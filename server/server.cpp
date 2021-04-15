// =================================================
// Names: Raj Shreyas Penukonda, Yevhen Kaznovskyi
// IDs: 1623713, 1623127
// CMPUT 275, Winter 2020
// Major Assignment #2 Part 2
// =================================================
#include "wdigraph.h"
#include "heap.h"
#include "dijkstra.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <string>
#include <math.h>
#include "serialport.h"
using namespace std;

//Declaring the structure to contain latitude and longtitude of a point
struct Point {
    long long lat;
    long long lon;
};

// Returns the Manhattan distance between the two given points.
long long manhattan(const Point& pt1, const Point& pt2){
    return abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
}

/*Read the Edmonton map data from the provided file and load it 
into the given WDigraph object.Store vertex coordinates in Point 
struct and mapeach vertex to its corresponding Point struct. */
void readGraph(char filename[], WDigraph &graph, unordered_map<int, Point> &points){

    //opening the file using the passed parameter
    ifstream ip(filename);

    //string will store the data
    string Identifier, s1, s2, s3;

    //while we have something to read
    while(ip.good()) {
        //get line will read from ip, up until the commas
        //or newline and store the string on the variables
        getline(ip, Identifier, ',');
        getline(ip, s1, ',');
        getline(ip, s2, ',');
        getline(ip, s3, '\n');

        //checking what we are reading, if a vertex, add
        //vertex with second var being the ID
        if(Identifier == "V"){
            int vertex = stoi(s1);
            graph.addVertex(stoi(s1));      
            double lat = stod(s2);
            double lon = stod(s3);

            //conver the lat and lon to proper number form
            lat = static_cast<long long> (lat * 100000);
            lon = static_cast<long long> (lon * 100000);

            //in the points map add the vertex with coordinates
            points[vertex];
            points[vertex].lat = lat;
            points[vertex].lon = lon;
            
        //else add an edge with a cost of manhattan dist. between the points
        }else if(Identifier == "E"){
            graph.addEdge(stoi(s1), stoi(s2), manhattan(points[stoi(s1)], points[stoi(s2)]));
        }
    }
    // close the file
    ip.close();
}

//finds the closest vertex to a given point using manhattan dist.
int closestVertex(const Point pt, unordered_map<int, Point> points){
    //the initial point will be the first one to compare others too
    bool first = false; 
    int closest_vertex = 0;
    int min_dist = 0;

    //going through all points with iterator
    for (auto iter = points.begin(); iter != points.end(); iter++){
        //getting the point from iter
        Point temp_point = points[(*iter).first];
        //and calculating the manhattan between iter point and location
        //we are intrested in
        int calc_dist = manhattan(temp_point, pt);
        //if its the first point, we initialize the values with it
        if(first == false){
            closest_vertex = (*iter).first;
            min_dist = calc_dist;
            first = true;
        }
        //if the distance we calculated is better then the previous
        //it means a closer vertex was found, and we reset it
        if(calc_dist <= min_dist){
            closest_vertex = (*iter).first;
            min_dist = calc_dist;
        }
    }
    return closest_vertex;
}


//this function is responsible for checking if a path from the start to end vertex exist
//returning 0 for total count if it doesn not, and counting up the total if there is a path
int countPathLength(int closest_vertex_start, int closest_vertex_end, unordered_map<int, PIL> tree){

    int count = 0;
    //if there is no end vertex in the tree then there is no path
    if(tree.find(closest_vertex_end) == tree.end()){
        return count;
    }

    auto iter_path = tree.find(closest_vertex_end);
    auto iter_finish = tree.find(closest_vertex_start);

    //while the start of the path is not yet reach, we work backwards from the end to count
    while(iter_path != iter_finish){
        //update our location on the path every new step
        iter_path = tree.find(iter_path->second.first);
        count++;
    }
    //must add another count as the start vertex is not accounted for after the loop
    count++;
    return count;
}

//creates the path that is going to be displayed and stores it in a deque by psuhing verts to the front
void makePath(int vertex_search, unordered_map<int, PIL> tree, unordered_map<int, Point> points, deque <Point>& output){

    //while the predecessor is not -1, which indicates reaching the start vertex
    while(vertex_search != -1){
        //find the vertex in the tree, then find the coordinates of the vertex in the points map
        auto found_vert = tree.find(vertex_search);
        auto Point_Q = points.find(found_vert->first);
        //add the point into the deque at the front
        output.push_front(Point_Q->second);
        //now reset the search to the predecessor
        vertex_search = found_vert->second.first;
    }
}

//makes sure that the sent line is the confirmation symbol and displayes one vertex location at a time
void displayPath(deque <Point>& output){
    SerialPort Serial("/dev/ttyACM0");
    int i = 0;
    
    string com;

    //wait for proper ACK from the user after showing "N #", if the ACK is not A, we
    //eat it up and wait again for the correct ACK
    while(true){
        com = Serial.readline(1000);

        //Waiting until ACK is recieved.
        if(com[0] == 'A'){
            if(output.empty()){
                Serial.writeline("E");
                cout << "Communication ended" << endl;
                Serial.writeline("\n");
                Serial.writeline("\r");
                break;    
            }

            //If ACK is received, Send the info on the next vertex
            Serial.writeline("W ");
            Serial.writeline((to_string(output.front().lat)));
            Serial.writeline(" ");
            Serial.writeline((to_string(output.front().lon)));
            Serial.writeline("\n");
            Serial.writeline("\r");

            cout << "W " << output.front().lat << " " << (output.front().lon) << endl;
            output.pop_front();
        }     
    }
}   


int main(){
    SerialPort Serial("/dev/ttyACM0");
    WDigraph graph;
    //points will store all the vertices into a map
    unordered_map<int, Point> points;
    //read the graph from the termial input
    readGraph("edmonton-roads-2.0.1.txt", graph, points);

    string ins;
    int closest_vertex_start;
    int closest_vertex_end; 
    //if one full request was not finished, such as first line not being correct input
    //the program will wait and take in new input until a full request is complete
    struct Point Point_location;
    while(true) {
        // constantly read in terminal commands
        ins = Serial.readline(1000);
        //make sure the first char is R
        if(ins[0] == 'R'){
            cout << "Got Request" << endl;
            //read the requested points and find closest vertices
            cout << ins << endl;

            //read the start point lat/lon and store into a point struct, then runs closestVertex            
            Point_location.lat = stol(ins.substr(2, ins.find(" ", 3)));
            Point_location.lon = stol(ins.substr(ins.find("-"), ins.find(" ", 16)));
            closest_vertex_start = closestVertex(Point_location, points);
            
            
            //read the end point lat/lon and store into a point struct, then runs closestVertex
            Point_location.lat = stol(ins.substr(ins.find(" ", 16)+1, ins.find("-", 16)-1));
            Point_location.lon = stol(ins.substr(ins.find("-", 16)));
            closest_vertex_end = closestVertex(Point_location, points);
            

            //run the dijkestra to find the tree from the start point
            unordered_map<int, PIL> tree;
            dijkstra(graph, closest_vertex_start, tree);

            //get total count of the path and display it
            int count = countPathLength(closest_vertex_start, closest_vertex_end, tree);
            
            //Converting the number of vertices to string to print on serial monitor.
            string scount = to_string(count);
            
            Serial.writeline("N ");
            Serial.writeline(scount);
            Serial.writeline("\n");

            //make the path with the deque and display one at a time with displayPath
            deque <Point> output;

            if(count != 0){
                makePath(closest_vertex_end, tree, points, output);
                displayPath(output);
            } else if(count == 0){
                //If the path length is 0, wait for 'A' client and send 'E' for ending communication.
                while(ins[0] != 'A'){
                    ins = Serial.readline(1000);
                }
                Serial.writeline("E");
                Serial.writeline("\n");
                cout << "Communication ended";
            }
        }
    }
    return 0;
}


/*
 Date:             28 October 2019
 Title:            Program 3 - Graphs
 Semester:         Spring 2018
 Course:           CMPS 3013
 Assignment:       A05
 Author:           Brent Laden
 Email:            Brelad41@gmail.com
 Files:            Geo.hpp, Heap.hpp, json.hpp, graph_builder.cpp, cities.json, output.txt.
 Description:
       This program uses heaps to find the closest cities via 
	   a json file. The program goes through all the cities assigning
	   the latitudes and longitudes as coordinates. After, it makes edges
	   connecting all the cities to each other. The distances are
	   calculated using the Haversine formula. The output for this
	   program will produce the first and last 10 cities and the closest
	   three cities.
*/


#include <iostream>
#include <fstream>
#include <string.h>
#include "Heap.hpp"
#include "JsonFacade.hpp"
#include "json.hpp"
#include "geo.hpp"

using namespace std;

using json = nlohmann::json;

struct City {
	double Priority;
	string Name;
	double Longitude;
	double Latitude;
	//
	City() {
		Name = "";
		Priority = 0;
		Longitude = 0;
		Latitude = 0;
	}
	City(json obj) {
		Name = obj["city"];
		Priority = obj["population"];
		Latitude = obj["latitude"];
		Longitude = obj["longitude"];
	}
};

int main() {
	//prepares output file
	ofstream outfile;
	outfile.open("output.txt");
	//opens jsonfile
	string filename = "cities.json";
	JsonFacade J(filename);

	outfile << "Brent Laden" << endl;

	json obj;
	City** Cities;
	City* temp;


	//allocates memory
	Heap <City> H(1000, false);

	int size = J.getSize();
	Cities = new City *[size];

	// loads array of cities from json file
	for (int i = 0; i < size; i++) {
		obj = J.getNext();
		Cities[i] = new City(obj);
	}
	int count = 0;
	int citynum = 0;

	while (count < size) {
		//makes coordinates of all the cities lons and lats
		Coordinate city1(Cities[count]->Latitude, Cities[count]->Longitude);
		if (count <10 || count>size - 11)
			outfile << count + 1 << ": " << Cities[count]->Name << endl;
		int counter = 0;
		//while loop to make edges with all other cities, and uses distance as new Priority
		while (counter < size) {
			Coordinate city2(Cities[counter]->Latitude, Cities[counter]->Longitude);
			Cities[counter]->Priority = HaversineDistance(city1, city2);
			H.Insert(Cities[counter]);
		}
		//goes through all connected cities
		while (citynum < 1000) {
			temp = H.Extract();
			//only allows first and last 10 cities
			if (count < 10 || count > size - 11)
				//3 closest cities
				if (citynum < 3)
					if (temp->Name != Cities[count]->Name)	//no double prints
						outfile << "/t" << citynum + 1 << ") " << temp->Name << ' ' << temp->Priority << endl;
			citynum++;
			temp = NULL;
		}
		citynum = 0;
	}
	outfile.close();
}
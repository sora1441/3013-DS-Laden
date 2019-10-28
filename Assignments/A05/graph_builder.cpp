/*
 Date:             28 October 2019
 Title:            Program 3 - Graphs
 Semester:         Spring 2018
 Course:           CMPS 3013
 Assignment:       A05
 Author:           Brent Laden
 Email:            Brent Laden
 Files:            Geo.hpp, heap.hpp, json.hpp, JsonFacade.hpp, graph_builder.cpp, cities.json.
 Description:
       This program uses heaps to find the closest cities via 
	   a json file. The distances are calculated using the Haversine
	   formula. The output for this program will produce the first 
	   and last 10 cities an who they are connected to.
*/


#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include "Heap.hpp"
#include "JsonFacade.hpp"
#include "json.hpp"
#include "Geo.hpp"

// fstream -> used for file handling
// iomanip -> used to for spacing between names and priority
#include <fstream> 
#include <iomanip>

using namespace std;

using json = nlohmann::json;

/**
* Structure = City
*
* Description:
*      The blue print for the data that is to be extracted.
*
*/
struct City {
	int rank;
	string Name, state;

	// Variable that will hold the values for latitude, longtitude of 
	// each Cities. The Priority temporarily holds the population size.
	double longitude, latitude, Priority;

	// default constructor
	City() {
		Priority = 0;
		Name = "";
		state = "";
		rank = 0;
		longitude = 0.00f;
		latitude = 0.00f;
	}
	City(json obj) {
		Name = obj["city"];
		Priority = obj["population"];
		state = obj["state"];
		rank = obj["rank"];
		longitude = obj["longitude"];
		latitude = obj["latitude"];
	}
};


//====================================================================//
//                          MAIN PROGRAM                              //
int main() {
	json obj;
	City** Cities;
	City* temp;
	string filename = "cities.json";
	JsonFacade J(filename);
	double distance;

	// Creation of ofstream class object
	// creates a file that holds the output of the program.  
	ofstream f_out;
	f_out.open("karki_vishwas.txt"); // open "file.txt" for writing data

									 // Creates a heap that holds 1000 cities data
	Heap <City> H(1000, false); // change value H.Size()

								// the data of cities such as population, ranks, longitude
								// , latitude, and state is assigned
	int size = J.getSize();
	Cities = new City *[size];
	// loads an array of cities with the json data
	for (int i = 0; i < 1000; i++) {
		obj = J.getNext();
		Cities[i] = new City(obj);
	}

	// Variables created to manipulate the index for print
	// Count -> helps to print only the top 4 closest cities 
	//      of individual city.
	// index_2 -> helps to print the closest cities of the 
	//      the first and last 10 Cities. 
	int count = 0;
	int index_2 = 0;


	/* Nested loop: It is created to get the Haversine distance between each
	//              city with 1000 other cities.
	//
	// Description: Inside the for loop Coordinate objects are created to store
	//              pass it as parameters for finding the distance between two
	//              Cities.
	*/
	for (int i = 0; i<1000; i++) {
		Coordinate T2(Cities[i]->longitude, Cities[i]->latitude);

		for (int j = 0; j<1000; j++) {
			Coordinate T3(Cities[j]->longitude, Cities[j]->latitude);
			// assign the distance between two cities to variable distance
			distance = HaversineDistance(T2, T3);
			// assign the variable distacne to the priority of the city relative
			// to the city coordinate in outer loop.
			Cities[j]->Priority = distance;
			//insert the city in the heap.
			H.Insert(Cities[j]);

		}

		/*
		//  If and Else statement
		//  the use of this statement helps to print only the first 4 cities closest to
		//  the relative city(i).
		*/

		if (index_2 < 10 || (index_2 >= 990 && index_2 < 1000))
			// prints the first 10 and the last 10 cities
		{
			while (count < 1000)
				// removes all the 1000 values stored in the heap using extract
				// prints the top 4 while removing each city.
			{
				// remove a city from the heap and assign to a city pointer temp.
				temp = H.Extract();
				// prints the output to the karki_vishwas.txt file
				if (count < 1)
				{
					f_out << index_2 + 0000 << " : " << temp->Name << endl;
				}
				else if (count < 2)
				{
					f_out << count << ")  " << temp->Name << setw(10) << "  (" << temp->Priority << ") " << endl;
				}
				else if (count < 4)
				{
					f_out << "   " << count << ")  " << temp->Name << setw(10) << "  (" << temp->Priority << ") " << endl;
				}

				count++;
				// point the pointer to NULL to avoid dangling pointer
				temp = NULL;

			}
			f_out << "\n ============================================ \n";
		}
		else if (index_2 >= 10 && index_2 < 990)
			// removes all the 1000 values stored in the heap using extract
		{
			while (count < 1000)
			{
				temp = H.Extract();

				count++;
				temp = NULL;
			}
			// creates a small gap between the first 10 and the last 10 cities
			if (index_2 >= 10 && index_2 < 11)
			{
				f_out << "\n         ...           \n" << endl;
			}
		}

		// counter increment 
		index_2++;
		count = 0;

	} // Nested loops ends.

	  // close the file
	f_out.close();

}
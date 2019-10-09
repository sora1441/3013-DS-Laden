/*
Brent Laden
October 7, 2019
CMPS 3013
A03

The program reads in a json file of animals, including
name, date, log, lat, priority, validation, and version.
They are sorted into arrays of priorty queues. It will
then remove the top 5 animals from each heap and print
them out
*/

#include "json_helper.cpp"
#include <fstream>
#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>

using namespace std;

const static double EarthRadiusKm = 6372.8;

inline double DegreeToRadian(double angle) {
	return M_PI * angle / 180.0;
}

class Coordinate {
public:
	Coordinate(double latitude, double longitude) : myLatitude(latitude), myLongitude(longitude) {}

	double Latitude() const {
		return myLatitude;
	}

	double Longitude() const {
		return myLongitude;
	}

private:
	double myLatitude;
	double myLongitude;
};

double HaversineDistance(const Coordinate &p1, const Coordinate &p2) {
	double latRad1 = DegreeToRadian(p1.Latitude());
	double latRad2 = DegreeToRadian(p2.Latitude());
	double lonRad1 = DegreeToRadian(p1.Longitude());
	double lonRad2 = DegreeToRadian(p2.Longitude());

	double diffLa = latRad2 - latRad1;
	double doffLo = lonRad2 - lonRad1;

	double computation = asin(sqrt(sin(diffLa / 2) * sin(diffLa / 2) + cos(latRad1) * cos(latRad2) * sin(doffLo / 2) * sin(doffLo / 2)));
	return 2 * EarthRadiusKm * computation;
}

struct Animal {
	string animal_name;
	long date;
	double latitude;
	double longitude;
	float adjuster;
	bool validated;
	string version;
	float priority;

	Animal() {
		animal_name = "";
		date = 0;
		latitude = 0.0;
		longitude = 0.0;
		adjuster = 0.0;
		validated = 0;
		version = "";
		priority = 0.0;
	}

	Animal(string name, long _date, double lat, double lon, float adj, bool val, string ver) {
		animal_name = name;
		date = _date;
		latitude = lat;
		longitude = lon;
		adjuster = adj;
		validated = val;
		version = ver;
		priority = 0.0;
	}

	Animal(json j) {
		animal_name = j["animal_name"];
		date = j["date"];
		latitude = j["latitude"];
		longitude = j["longitude"];
		adjuster = j["adjuster"];
		validated = j["validated"];
		version = j["version"];
		priority = 0.0;
	}
};
class Heap {
private:
	vector<Animal> group;
	int counter = 1;

	//switches a parent and child to correct priority after insertion
	void SwapUp(int i) {
		int n = parent(i);
		while (p > 0 && group[i].priority > group[p].priority) {
			swap(i, p);
			i = p;
			p = parent(i);
		}
	}

	//swaps down after removal
	void SwapDown(int i) {
		int c = childFinder(i);

		while (c > 0) {
			if (group[i].priority < group[c].priority) {
				swap(i, c);
				i = c;
				c = childFinder(i);
			}
			else
				c = -1;
		}
	}

	//switches two elements
	void swap(int p, int i)
	{
		Animal temp = group[p];
		group[p] = group[i];
		group[i] = temp;
	}

	//return location of parent
	int parent(int i) {
		return int(i / 2);
	}

	//return location of left child
	int leftChild(int i) {
		return i * 2;
	}

	//return location of right child
	int rightChild(int i) {
		return i * 2 + 1;
	}

	//retun location of child to swap or -1 if none to swap
	int childFinder(int i) {
		if (rightChild(i) >= counter)    
			if (leftChild(i) >= counter)
				return -1;
			else 
				return leftChild(i);
		else 
			if (group[i * 2 + 1].priority > group[i * 2].priority)
				return rightChild(i);
			else
				return leftChild(i);
	}

	//Prints out extracted animal name and priority
	void printAnimal(Animal a) {
		cout << a.animal_name << " " << a.priority << endl;
	}
	//calculates the priority of the animal
	Animal getPriority(Animal &x) {
		double distance;
		Coordinate C1(x.latitude, x.longitude);
		Coordinate C2(33.9137, -98.4934);
		distance = HaversineDistance(C1, C2);
		x.priority = (EarthRadiusKm - distance) * x.adjuster / x.animal_name.length();
		if (x.validated == true)
			x.priority = x.priority * -1;
		return x;
	}

public:
	//create empty heap
	Heap() {
		group.push_back(Animal{});
		counter = 1;
	}
	//inserts animal into heap
	void Insert(Animal &x) {
		group.push_back(x);
		getPriority(x);
		SwapUp(counter);
		counter++;
	}

	//extracts top element from heap
	void Extract() {
		if (group.empty())
			return;
		Animal removed = group[1];
		group[1] = group[--counter];

		if (group.size() > 1) {
			SwapDown(1);
		}
		printAnimal(removed);
	}
};

class AnimalHelper {
private:
	Animal **Animals;
	JsonHelper *J;
	json obj;
	int size;

public:
	AnimalHelper(string filename) {
		J = new JsonHelper(filename);
		size = J->getSize();
		Animals = new Animal *[size];
		for (int i = 0; i < size; i++) {
			obj = J->getNext();
			Animals[i] = new Animal(obj);
		}
	}



	void PrintAnimals() {

		for (int i = 0; i < size; i++) {

			cout << Animals[i]->animal_name << " "
				<< Animals[i]->latitude << " "
				<< Animals[i]->longitude << " "
				<< Animals[i]->date << " "
				<< Animals[i]->adjuster << " "
				<< Animals[i]->validated << " "
				<< Animals[i]->version << " "
				<< Animals[i]->priority << endl;

		}
	}
};


int main(int argc, char **argv) {

	AnimalHelper AH("animals.json");
	char ch;

	AH.PrintAnimals();

	return 0;
}
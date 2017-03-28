#include "utilities.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void loadNodes() {
	//Format: node_id;latitude_in_degrees;longitude_in_degrees;longitude_in_radians;latitude_in_radians
	string line;
	ifstream file;
	file.open("NodeTest.txt");
	if (file.is_open())	{
		while (getline(file, line))	{
			uint64 node_id;
			double latitude_in_degrees, longitude_in_degrees;
			double longitude_in_radians, latitude_in_radians;
			char delimiter;
			istringstream iss(line);
			iss >> node_id >> delimiter
				>> latitude_in_degrees >> delimiter
				>> longitude_in_degrees >> delimiter
				>> longitude_in_radians >> delimiter
				>> latitude_in_radians;
		}
		file.close();
	}
}
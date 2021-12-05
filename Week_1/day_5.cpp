/* 
 * Takes an input txt files with pairs of coordinates, and considers horizontal/vertical lines between the pairs
 * Records number of points hwere multiple lines intersect
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <array>
#include <string>
#include <algorithm>  // Use for min, max
#include <sstream>

using namespace std;

// std::array<std::array<int, 1000>, 1000>  line_writer(int coordinates[4], std::array<std::array<int, 1000>, 1000> vent_array)
// {
//     cout << "Start line writer" << endl;
//     // Vertical line
//     if (coordinates[0] == coordinates[2]){
//         cout << "Adding Horizontal Line" << endl;
//         for (int y = min(coordinates[1], coordinates[3]); y < max(coordinates[1],coordinates[3]) + 1; y++){
//             vent_array[coordinates[0]][y] = 1;
//         }
//     }
//     // Horizontal line
//     if (coordinates[1] == coordinates[3]){
//         cout << "Adding Vertical Line" << endl;
//         for (int x = min(coordinates[0], coordinates[2]); x < max(coordinates[0], coordinates[2]) + 1; x++){
//             vent_array[x][coordinates[1]] = 1;
//         }
//     }
//     cout << "Finish line writer" << endl;
//     return vent_array;
// }

int intersection_counter(std::array<std::array<int, 1000>, 1000> vent_array){
    int counter = 0;
    for (int i=0; i < 1000; i++){
        for (int j=0; j < 1000; j++){
            if (vent_array[i][j] > 1){
                counter++;
            }
        }
    }
    return counter;
}

int main()
{
    // IMPORT ALL TABLES INTO ONE BIG ARRAY
    int index = 0;
    int ARRAY_DIM = 1000;
    std::array<std::array<int, 1000>, 1000> vent_array = {0}; // Record line number at each vertex

    ifstream MyReadFile("Week_1/Inputs/day_5_input.txt"); // Read from the text file
    string line; // Create a text string, which is used to output the text file
    int line_number = 1;

    string delimiter = " -> ";
    string comma = ",";
    int coordinates [4];
    while (getline (MyReadFile, line)) {
        // The cons of this method of data extraction is that it's ugly and non-generalisable
        // The pros are that I could write it myself instead of stealing it off stack exchange

        coordinates[0] = stoi(line.substr(0, line.find(comma)));
        line.erase(0, line.find(comma) + comma.length());
        coordinates[1] = stoi(line.substr(0, line.find(delimiter)));
        line.erase(0, line.find(delimiter) + delimiter.length());
        coordinates[2] = stoi(line.substr(0, line.find(comma)));
        line.erase(0, line.find(comma) + comma.length());
        coordinates[3] = stoi(line);
        cout << coordinates[0] << " & " << coordinates[1] << " & " << coordinates[2] << " & " << coordinates[3] << endl;

        // Ideally these functions would be in a separate function, but get segmentation errors when passing in a big arrauy
        if (coordinates[0] == coordinates[2]){
            cout << "Adding Horizontal Line" << endl;
            for (int y = min(coordinates[1], coordinates[3]); y < max(coordinates[1],coordinates[3]) + 1; y++){
                vent_array[coordinates[0]][y] += 1;
            }
        }
        // Horizontal line
        if (coordinates[1] == coordinates[3]){
            cout << "Adding Vertical Line" << endl;
            for (int x = min(coordinates[0], coordinates[2]); x < max(coordinates[0], coordinates[2]) + 1; x++){
                vent_array[x][coordinates[1]] += 1;
            }
        }
    }
    MyReadFile.close(); // Close the file

    cout << "Final multi-vent count: " << intersection_counter(vent_array) << endl;


    return 0;
}


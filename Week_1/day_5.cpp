/* 
 * Takes an input txt files with pairs of coordinates, and considers horizontal/vertical lines between the pairs
 * Records number of points hwere multiple lines intersect
 */

#include <iostream> 
#include <fstream> 
#include <array>
#include <string>

using namespace std;

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
        // std::cout << coordinates[0] << " & " << coordinates[1] << " & " << coordinates[2] << " & " << coordinates[3] << endl;

        // Ideally these functions would be in a separate function, but get segmentation errors when passing in a big array
        // Vertical line
        if (coordinates[0] == coordinates[2]){
            for (int y = min(coordinates[1], coordinates[3]); y <= max(coordinates[1],coordinates[3]); y++){
                vent_array[coordinates[0]][y] += 1;
            }
        }

        // Horizontal line
        if (coordinates[1] == coordinates[3]){
            for (int x = min(coordinates[0], coordinates[2]); x <= max(coordinates[0], coordinates[2]); x++){
                vent_array[x][coordinates[1]] += 1;
            }
        }

        // Diagonal line (y=x)
        if ((coordinates[3]-coordinates[1]) == (coordinates[2]-coordinates[0])){
            for (int n = 0; n <= abs(coordinates[3]-coordinates[1]); n++){
                vent_array[min(coordinates[0], coordinates[2] + n)][min(coordinates[1], coordinates[3]) + n] += 1;
            }
        }

        // Diagonal line (y=-x)
        if ((coordinates[3]-coordinates[1]) == -(coordinates[2]-coordinates[0])){
            for (int n = 0; n <= abs(coordinates[3]-coordinates[1]); n++){
                if (coordinates[3] > coordinates[1]){ // x is growing
                    vent_array[min(coordinates[0], coordinates[2]) + n][max(coordinates[1], coordinates[3]) - n] += 1;
                }
                else { // x is shrinking
                    vent_array[max(coordinates[0], coordinates[2]) - n][min(coordinates[1], coordinates[3]) + n] += 1;
                }
            }
        }
    }
    MyReadFile.close(); // Close the file

    std::cout << "Final multi-vent count: " << intersection_counter(vent_array) << endl;
    return 0;
}


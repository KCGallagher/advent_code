/* 
 * Takes an input txt files with an array of heights
 * Identifies points lower than all neighbours, and returns a risk total based on their summed value
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>

using namespace std;

void print_2D_vector(vector< vector<int> > data)
{
    for (int i = 0; i < data.size(); i++){
        cout << "Line " << i << " = {";
        for (int j=0; j < data[i].size() - 1; j++){
            cout << data[i][j] << ", ";
        }
        cout << data[i][data[i].size() - 1] << "}\n";
    }
}

int total_risk_level(vector< vector<int> > h){
    // Iterate over interior of arrray, ignoring -1 frame
    int total_risk = 0;
    for (int i = 1; i < h.size() - 1; i++){
        for (int j = 1; j < h[i].size() - 1; j++){
            if ((h[i][j] < h[i+1][j]) && (h[i][j] < h[i-1][j]) && (h[i][j] < h[i][j+1]) && (h[i][j] < h[i][j-1])){
                cout << "Low point at (" << i-1 << ", " << j-1 << ") with value of: " << h[i][j] << endl;
                total_risk += (h[i][j] + 1);
            }
        }
    }
    return total_risk;
}

int main()
{
    // IMPORT ALL HEIGHT VALUES INTO ONE ARRAY
    vector< vector<int> > heights; // Record height at each point in array

    ifstream MyReadFile("Week_2/Inputs/day_9_input.txt"); 
    string line; 
    int line_num = 0;

    // SURROUND REAL ARRAY WITH 10 VALUES TO AVOID BOUNDARY ISSUES LATER 
    // These will always be higher than neighbouring real values, but gives every real value 4 neighbours

    while (getline (MyReadFile, line)) {
        if(line_num == 0){
            int line_length = line.size(); // may be useful later
            vector<int> frame_vector;
            for (int i = 0; i < line.size() + 2; i++){
                frame_vector.push_back(10); // - '0' converts char to int
            }
            heights.push_back(frame_vector);
        }
        vector<int> row_vector;
        row_vector.push_back(10); // For left column of 10
        for (int i = 0; i < line.size(); i++){
            row_vector.push_back(line[i] - '0'); // - '0' converts char to int
        }
        row_vector.push_back(10); // For right column of 10
        heights.push_back(row_vector);

        line_num += 1;
    }
    MyReadFile.close(); // Close the file

    // Append row of -1 on bottom of array too
    vector<int> frame_vector;
    for (int i = 0; i < heights[0].size(); i++){
        frame_vector.push_back(10); // - '0' converts char to int
    }
    heights.push_back(frame_vector);


    print_2D_vector(heights);
    int total_risk = total_risk_level(heights);

    std::cout << "Final summed risk level: " << total_risk << endl;
    return 0;
}


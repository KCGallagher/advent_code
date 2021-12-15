/* 
 * Takes an input txt files with an array of risks
 * Identifies a route through array with minimal risk using Dijkstra's algorithm
 * Returns the summed risk of the minimal risk route
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>

using namespace std;

void print_2D_vector(vector< vector<int> > data)
{
    // Assume frame of width one around array, which will not be printed
    for (int i = 1; i < data.size() - 1; i++){
        cout << "Line " << i << " = {";
        for (int j = 1; j < data[i].size() - 2; j++){
            cout << data[i][j] << ", ";
        }
        cout << data[i][data[i].size() - 2] << "}\n";
    }
}

int update_distance(vector<vector<int>>& data, vector<vector<int>> risks, vector<vector<int>> visited, pair<int, int> index, int value)
{
    if (visited[index.first][index.second]){
        return risks[index.first][index.second];  // Unchanged as visited
    } else{
        int new_risk = value + data[index.first][index.second];
        if (new_risk < risks[index.first][index.second]){
            return new_risk;
        } else{
            return risks[index.first][index.second];  // Unchanged as current route better
        }
    }
}

int shortest_path(vector< vector<int> > data)
{
    // IMPLEMENTATION OF DIJKSTRA'S ALGORITHM

    //Duplicate data array to make other arrays of the same size
    vector< vector<int> > risks = data;  // I.e. the minimum total cost to get to this point
    for (auto &i : risks){
        std::fill(i.begin(), i.end(), std::numeric_limits<int>::max() / 2); 
    }  // fill risks with inf, or the maximum integer value, (to denote unvisited values)
    vector< vector<int> > visited = data;  // I.e. the minimum total cost to get to this point
    for (auto &i : visited){
        std::fill(i.begin(), i.end(), 0); 
    }  // initially all nodes are unvisited

    risks[1][1] = 0; // starting value
    pair<int, int> current = make_pair(1, 1); //current node to consider

    int iter_num = 0;
    while(true){
        iter_num++;
        if(iter_num > (data.size() * data[0].size())){
            cout << "Warning: Early termination of search as max iterations reached" << endl;
            break;  // Safety check to prevent overflow
        }

        int i = current.first;
        int j = current.second;
        int current_value = risks[i][j];

        for (int step = 1; step >= -1; step -= 2){  // So I don't have to write +1 and -1 each time
            risks[i + step][j] = update_distance(data, risks, visited, make_pair(i + step, j), current_value);
            risks[i][j + step] = update_distance(data, risks, visited, make_pair(i, j + step), current_value);
        } // Should have written an iterator for this but didn't

        visited[current.first][current.second] = 1; // Mark current node as visited
        if(visited[data.size() - 2][data[0].size() - 2]){
            break;  // Algorithm is finished when end point is visited
        }

        // Find location of min value, make it the new current
        int min_value = std::numeric_limits<int>::max() / 2;
        for (int i = 1; i < data.size() - 1; i++){
            for (int j = 1; j < data[i].size() - 1; j++){
                if((risks[i][j] < min_value) && (!visited[i][j])){
                    min_value = risks[i][j];
                    current = make_pair(i, j);
                }
            }
        }
        // cout << "Min of " << min_value << " found at (" << current.first << ", " << current.second << ")\n";
    }

    int finish_risk = risks[data.size() - 2][data[0].size() - 2];  // At location of end point
    return finish_risk;
}


int main()
{
    // IMPORT ALL HEIGHT VALUES INTO ONE ARRAY
    vector< vector<int> > risks; // Record height at each point in array

    ifstream MyReadFile("Week_3/Inputs/day_15_input.txt"); 
    string line; 
    int line_num = 0;

    // SURROUND REAL ARRAY WITH LARGE VALUES TO AVOID BOUNDARY ISSUES LATER 
    // These will always be higher than neighbouring real values, but gives every real value 4 neighbours
    // This will ensure these values are never considered for the path, to avoid indexing errors
    // Half the max value of an integer is used - this is larger than any risks in the path, but won't overflow itself

    while (getline (MyReadFile, line)) {
        if(line_num == 0){
            int line_length = line.size(); // may be useful later
            vector<int> frame_vector;
            for (int i = 0; i < line.size() + 2; i++){
                frame_vector.push_back(10); // - '0' converts char to int
            }
            risks.push_back(frame_vector);
        }
        vector<int> row_vector;
        row_vector.push_back(10); // For left column of values
        for (int i = 0; i < line.size(); i++){
            row_vector.push_back(line[i] - '0'); // - '0' converts char to int
        }
        row_vector.push_back(10); // For right column of large values
        risks.push_back(row_vector);

        line_num += 1;
    }
    MyReadFile.close(); // Close the file

    // Append row of large values on bottom of array too
    vector<int> frame_vector;
    for (int i = 0; i < risks[0].size(); i++){
        frame_vector.push_back(10); 
    }
    risks.push_back(frame_vector);

    int total_risk = shortest_path(risks);

    std::cout << "Final summed risk level: " << total_risk << endl;
    return 0;
}

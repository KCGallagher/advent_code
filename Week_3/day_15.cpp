/* 
 * Takes an input txt files with an array of risks
 * Identifies a route through array with minimal risk using Dijkstra's algorithm
 * Returns the summed risk of the minimal risk route (ie the total path length)
 * Also considers and tiled and clock stepped version of the array to find a path for.
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

vector<int> vector_add(vector<int> data, int value){
    // Adds a scalar to a vector element-wise
    for (int n = 0; n < data.size(); n++){
        data[n] += value;
        if (data[n] > 9){
            data[n] -= 9; // wrap back around
        }
    }
    return data;
}

vector<vector<int>> tile_map(vector<vector<int>> data, int n){
    vector<vector<int>> t_data;
    for (int i = 0; i < n * data.size(); i++){
        vector<int> row;
        for (int m = 0; m < n; m++){ // Add line n times, with increment
            // cout << i << " & " << m << endl;
            vector<int> values = vector_add(data[i % data.size()], (i / data.size()) + m);
            row.insert(row.end(), values.begin(), values.end());
        }
        t_data.push_back(row);

    }
    return t_data;
}

vector<vector<int>> pad_map(vector<vector<int>> data, int value){
    vector<vector<int>> p_data;

    // Start with frame row on top 
    vector<int> frame_vector;
    for (int x = 0; x < data[0].size() + 2; x++){ // + 2 for extra column on each side
        frame_vector.push_back(value); 
    }
    p_data.push_back(frame_vector);
    for (int i = 0; i < data.size(); i++){
        vector<int> row_vector;
        row_vector.push_back(value); // For left column of values
        for (int j = 0; j < data[0].size(); j++){
            row_vector.push_back(data[i][j]); 
        }
        row_vector.push_back(value); // For right column of  values
        p_data.push_back(row_vector);
    }
    // Append row of large values on bottom of array too
    p_data.push_back(frame_vector);
    return p_data;
}



int update_distance(vector<vector<int>>& data, vector<vector<int>> risks, vector<vector<int>> visited, pair<int, int> index, int value)
{
    // Passing the whole arrays here is very inefficient, but made the indexing look nicer 
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
    vector<vector<int>> risks; // Record height at each point in array
    const int N_TILE = 5;

    ifstream MyReadFile("Week_3/Inputs/day_15_input.txt"); 
    string line; 
    int line_num = 0;

    // SURROUND REAL ARRAY WITH LARGE VALUES TO AVOID BOUNDARY ISSUES LATER 
    // These will always be higher than neighbouring real values, but gives every real value 4 neighbours
    // This will ensure these values are never considered for the path, to avoid indexing errors
    // Half the max value of an integer is used - this is larger than any risks in the path, but won't overflow itself

    while (getline (MyReadFile, line)) {
        vector<int> row_vector;
        for (int i = 0; i < line.size(); i++){
            row_vector.push_back(line[i] - '0'); // - '0' converts char to int
        }
        risks.push_back(row_vector);
    }
    MyReadFile.close(); // Close the file

    vector<vector<int>> p_risks = pad_map(risks, 10);

    int total_risk_a = shortest_path(p_risks);
    std::cout << "Final summed risk level for partial map: " << total_risk_a << endl;

    vector<vector<int>> full_risks = tile_map(risks, N_TILE); // required for part b
    vector<vector<int>> p_full_risks = pad_map(full_risks, 10);
    int total_risk_b = shortest_path(p_full_risks);
    std::cout << "Final summed risk level for complete map: " << total_risk_b << endl;
    return 0;
}

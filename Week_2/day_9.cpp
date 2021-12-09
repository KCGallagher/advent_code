/* 
 * Takes an input txt files with an array of heights
 * Identifies points lower than all neighbours, and returns a risk total based on their summed value
 * Finds basins surrounding minimal points (of height less than the max) and calculates their areas
 * Returns the product of the areas of the three largest basins
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>
#include <algorithm>

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
                // cout << "Low point at (" << i-1 << ", " << j-1 << ") with value of: " << h[i][j] << endl;
                total_risk += (h[i][j] + 1);
            }
        }
    }
    return total_risk;
}

int count_basin_size(vector< vector<int> > h, vector< vector<int> > g){
    int basin_size = 0;
    for (int i = 1; i < g.size() - 1; i++){
        for (int j = 1; j < g[i].size() - 1; j++){
            if (g[i][j] == 1){  // count number of cells part of basin
                basin_size += 1;
            }
        }
    }
    return basin_size;
}

int find_basin(vector< vector<int> > h, vector< vector<int> > g){
    int max_iteration_num = 30;  // Max number of times to iterate basin expansion
    bool new_regions = true;  // boolean whether new regions have been added (so we should keep searching)
    int count_number = 0;
    while (new_regions){
        new_regions = false; // should be overwritten later if there are new regions
        for (int i = 1; i < g.size() - 1; i++){
            for (int j = 1; j < g[i].size() - 1; j++){
                if (g[i][j] == 1){ 
                    // If part of basin, look whether all neighbouring cells should be too
                    if ((h[i+1][j] < 9) && (g[i+1][j] == 0)){  // unvisited cell part of basin
                        g[i+1][j] = 1;
                        new_regions = true;
                    }
                    if ((h[i-1][j] < 9) && (g[i-1][j] == 0)){
                        g[i-1][j] = 1;
                        new_regions = true;
                    }
                    if ((h[i][j+1] < 9) && (g[i][j+1] == 0)){
                        g[i][j+1] = 1;
                        new_regions = true;
                    }
                    if ((h[i][j-1] < 9) && (g[i][j-1] == 0)){
                        g[i][j-1] = 1;
                        new_regions = true;
                    }
                }
            }
        }
        count_number += 1;
        if (count_number > max_iteration_num){
            new_regions = false; // End search prematurely if over max number of iterations
        }
    }
    int basin_size = count_basin_size(h, g);
    return basin_size;
}


int product_basin_sizes(vector< vector<int> > h){
    // Generate
    vector<int> basin_sizes;
    for (int i = 1; i < h.size() - 1; i++){
        for (int j = 1; j < h[i].size() - 1; j++){
            if ((h[i][j] < h[i+1][j]) && (h[i][j] < h[i-1][j]) && (h[i][j] < h[i][j+1]) && (h[i][j] < h[i][j-1])){
                vector< vector< int> > g = h; // duplicate vector h as g
                for (auto &i : g){
                    std::fill(i.begin(), i.end(), 0); // fill g with zeroes    
                }
                g[i][j] = 1;  // mark as start/centre of basin
                int new_basin_size = find_basin(h, g);
                basin_sizes.push_back(new_basin_size);
                // cout << "Basin centred at (" << i-1 << ", " << j-1 << ") with size of: " << new_basin_size << endl;
            }
        }
    }
    std::sort(basin_sizes.begin(),basin_sizes.end(), std::greater<>()); // sort into descending order
    int size_product = 1;
    for (int n = 0; n < 3; n++){
        size_product *= basin_sizes[n];  // find the product of the largest three values
    }
    return size_product;
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

    // print_2D_vector(heights);
    int total_risk = total_risk_level(heights);
    int basin_product= product_basin_sizes(heights);

    std::cout << "Final summed risk level: " << total_risk << endl;
    std::cout << "Product of three largest basins: " << basin_product << endl;
    return 0;
}

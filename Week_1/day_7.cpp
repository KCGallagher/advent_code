/* 
 * Takes an input txt files with a list of horizontal positions
 * Finds the mean position and the total distance travelled to get there
 * We assume that the 'average' position that minimises the total distance 
 * travelled across all samples is the mean 
 * Return the total distance summed across all values
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

int mean_pos(vector<int> pos_data)
{
    int sum = std::accumulate(pos_data.begin(), pos_data.end(), 0);
    cout << sum << endl;
    float ave = sum / pos_data.size();
    return static_cast<int>(round(ave));
}

int median_pos(vector<int> pos_data)
{
    size_t n = pos_data.size() / 2;
    nth_element(pos_data.begin(), pos_data.begin()+n, pos_data.end());
    return pos_data[n];
}

int summed_distance(vector<int> pos_data, int mean_pos)
{
    int distance_total = 0;
    int crab_number = pos_data.size();
    for (int n = 0; n < pos_data.size(); n++){
        distance_total += abs(pos_data[n] - mean_pos);
    }
    return distance_total;
}

int summed_distance_part2(vector<int> pos_data, int mean_pos)
{
    int distance_total = 0;
    int crab_number = pos_data.size();
    for (int n = 0; n < pos_data.size(); n++){
        int separation = abs(pos_data[n] - mean_pos);
        distance_total += (separation * (separation + 1)) / 2;
    }
    return distance_total;
}


int main()
{
    bool non_linear_weighting = true; // For non linear distance weight in part b

    // IMPORT ALL CRAB POSITIONS INTO ONE LONG VECTOR
    ifstream MyReadFile("Week_1/Inputs/day_7_input.txt");
    string pos_data; // Create a text string, which is used to output the text file
    vector<int> crab_pos; // Records the position of each crab

    while (getline (MyReadFile, pos_data)) {
        stringstream ss(pos_data);
        while(ss.good()){
            string substr;
            getline(ss, substr, ',');
            crab_pos.push_back(stoi(substr));
        }
    }
    MyReadFile.close(); // Close the file


    // FIND TARGET POSITION FOR ALL CRABS, AND DISTANCE THERE
    int total_distance = 0;
    if (non_linear_weighting){ // USE IN PART B
        vector<int> distance_options;
        int min = *min_element(crab_pos.begin(), crab_pos.end());
        int max = *max_element(crab_pos.begin(), crab_pos.end());
        for (int target = min; target < max; target++){
            distance_options.push_back(summed_distance_part2(crab_pos, target));
        }  // Search through all possible target positions to find the best
        total_distance = *min_element(distance_options.begin(), distance_options.end());
    } 
    else {
        int target_pos = median_pos(crab_pos);  // optimal target is always median, no need to search
        cout << "Target position for crabs is: " << target_pos << endl;
        total_distance = summed_distance(crab_pos, target_pos);
    }

    std::cout << "Total distance moved by crabs: " << total_distance << endl;
    return 0;
}

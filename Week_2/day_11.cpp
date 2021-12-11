/* 
 * Takes an input txt files with an array of octopus cycle time
 * Iterates over time, tracking the number of flashes at a point in time
 * Returns the summed number of flashes over a time period
 * Also returns point at which all octopi flash simultaneously
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <assert.h>

using namespace std;

void print_2D_vector(vector< vector<int> > data)
// Assumes a frame of width 1 around the array which will not be printed
{
    for (int i = 1; i < data.size() - 1; i++){
        cout << "   Line " << i << " = {";
        for (int j = 1; j < data[i].size() - 2; j++){
            cout << data[i][j] << ", ";
        }
        cout << data[i][data[i].size() - 2] << "}\n";
    }
    cout << "\n";
}

tuple <vector< vector<int> >, int, bool> next_timestep(vector< vector<int> > energy, bool complete_flash){
    int step_flashes = 0; // number of flashes in this step
    vector< vector<int> > flashed(energy.size(), std::vector<int>(energy[0].size(), 0));  // zeros of the same size as energy
    for (int i = 1; i < energy.size() - 1; i++){
        for (int j =  1; j < energy[i].size() - 1; j++){
            energy[i][j] ++;  // Add one to each value - probably a nicer way to do this
        }
    }
    bool new_flashes = true;
    int iter_num = 0;
    while (new_flashes){
        new_flashes = false;
        for (int i = 1; i < energy.size() - 1; i++){
            for (int j = 1; j < energy[i].size() - 1; j++){
                if ((energy[i][j] > 9) && (flashed[i][j] == 0)){  // Sufficient energy to flash, but hasn't already this turn
                    flashed[i][j] = 1;
                    step_flashes ++;
                    new_flashes = true;
                    // Increase energy of neighbouring cells
                    for (int step = 1; step >= -1; step -= 2){  // So I don't have to write +1 and -1 each time
                        energy[i + step][j] += 1;
                        energy[i][j + step] += 1;
                        energy[i + step][j + step] += 1;
                        energy[i + step][j - step] += 1;
                    }
                }
            }
        }
        iter_num++;
    }
    for (int i = 1; i < energy.size() - 1; i++){
        for (int j = 1; j < energy[i].size() - 1; j++){
            if (flashed[i][j] == 1){  // should be the same as if (energy[i][j] > 9)
                assert (energy[i][j] > 9);
                energy[i][j] = 0;
            }
        }
    }
    // cout << "  Flashes in step: " << step_flashes << ", over " << iter_num << " iterations" << endl;
    complete_flash = (step_flashes == ((energy.size() - 2) * (energy[0].size() - 2)));
    return std::make_tuple(energy, step_flashes, complete_flash);
}


int main()
{
    // IMPORT ALL HEIGHT VALUES INTO ONE ARRAY
    const int N_STEPS = 100;
    int total_flashes = 0;
    bool complete_flash = false; // Stop iterating when this is true and complete flash has occured
    vector< vector<int> > energies; // Record height at each point in array

    ifstream MyReadFile("Week_2/Inputs/day_11_input.txt"); 
    string line; 
    int line_num = 0;

    // SURROUND REAL ARRAY WITH 0 VALUES TO AVOID BOUNDARY ISSUES LATER 
    // These may grow but will never be iterated over

    while (getline (MyReadFile, line)) {
        if(line_num == 0){
            int line_length = line.size(); // may be useful later
            vector<int> frame_vector;
            for (int i = 0; i < line.size() + 2; i++){
                frame_vector.push_back(0); // - '0' converts char to int
            }
            energies.push_back(frame_vector);
        }

        vector<int> row_vector;
        row_vector.push_back(0); // For left column of 0
        for (int i = 0; i < line.size(); i++){
            row_vector.push_back(line[i] - '0'); // - '0' converts char to int
        }
        row_vector.push_back(0); // For right column of 0
        energies.push_back(row_vector);
        line_num += 1;
    }
    MyReadFile.close(); // Close the file

    // Append row of 0 on bottom of array too
    vector<int> frame_vector;
    for (int i = 0; i < energies[0].size(); i++){
        frame_vector.push_back(0);
    }
    energies.push_back(frame_vector);

    int step_counter = 0;
    int final_flashes = 0;
    while (!complete_flash || step_counter < N_STEPS){
        tuple <vector< vector<int> >, int, bool> output = next_timestep(energies, complete_flash); 
        energies = std::get<0>(output);
        total_flashes += std::get<1>(output);
        complete_flash = std::get<2>(output); // update boolean to detect if complete flash has occured
        step_counter++;
        if(step_counter == N_STEPS){
                final_flashes = total_flashes;  // Print number of flashes aftr N Steps
        }
    }

    std::cout << "Total number of flashes: " << final_flashes << endl;
    std::cout << "Complete flash occured on step " << step_counter << endl;
    return 0;
}

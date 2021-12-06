/* 
 * Takes an input txt files with a list of lanternfish ages
 * Evolves fish according to a defined set of rules (fish reproduce every seven days)
 * Return the total number of fish after a given time period (TOTAL_DAYS)
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <numeric>
#include <string>
#include <sstream>

using namespace std;

vector<int> evolve_fish(vector<int> fish_ages)
{
    int initial_size = fish_ages.size(); // avoid iterating over new fish
    for (int n = 0; n < initial_size; n++){
        if (fish_ages[n] == 0){
            fish_ages[n] = 6; // Pre-existent fish must wait 7 days before next reproduction cycle
            fish_ages.push_back(8); // New fish are birthed with 9 day timer before reproduction
        }
        else {
            fish_ages[n] -= 1; // Counter that stores time until next reproduction cycle
        }
    }
    return fish_ages;
}

vector<long long> evolve_predictor(vector<long long> current_pop)
{
    vector<long long> placeholder_population (9,0);
    for (int i = 0; i < 9-1; i++){
        placeholder_population[i] = current_pop[i+1]; 
        // I.e. current F_8 will be the next F_7, and current F_1 will be next F_0
    }
    placeholder_population[6] += current_pop[0]; // For fish restarting reproductive cycle
    placeholder_population[8] += current_pop[0]; // For new births
    return placeholder_population;
}

long long population_predictor(vector<int> fish_ages, int day_number)
{
    vector<long long> current_population (9, 0); 
    // This is a vector of populations in each state F_n, where F_5 is fish 5 days
    // from reproduction, and F_0 is fish ready to reproduce
    // In the form [F_0, F_1, ..., F_7, F_8]
    for (int n = 0; n < fish_ages.size(); n++){
        // Put each initial fish into the right population box
        current_population[fish_ages[n]] += 1;
    }
    for (int day = 0; day < day_number; day++){
        current_population = evolve_predictor(current_population);
    }

    // Return sum of elements in array - loop is more stable than accumulate()
    long long sum_of_elements = 0;
    for (int i = 0; i < 9; i++){
        sum_of_elements += current_population[i];
    }
    return sum_of_elements;
}


int main()
{
    int TOTAL_DAYS = 256; // Total number of days to model
    bool predict_population = true; // For part b, where pop becomes too large to simulate directly
    bool print_population = false;
    bool print_day_number = true;

    // IMPORT ALL FISH AGES INTO ONE LONG VECTOR
    ifstream MyReadFile("Week_1/Inputs/day_6_input.txt");
    string fish_data; // Create a text string, which is used to output the text file
    vector<int> fish_ages; // Records the number of days until each fish reproduces

    while (getline (MyReadFile, fish_data)) {
        stringstream ss(fish_data);
        while(ss.good()){
            string substr;
            getline(ss, substr, ',');
            fish_ages.push_back(stoi(substr));
        }
    }

    MyReadFile.close(); // Close the file

    // EVOLVE FISH POPULATION DAILY
    long long final_population = 0;
    if (predict_population){
        final_population = population_predictor(fish_ages, TOTAL_DAYS);
    }
    else {
        for (int day = 0; day < TOTAL_DAYS; day++){
            if (print_day_number){
                cout << "Simulating day " << day << "/" << TOTAL_DAYS << endl;
            }
            fish_ages = evolve_fish(fish_ages);
        }
        final_population = fish_ages.size();
    }
    

    // OUTPUT FINAL RESULT
    if (print_population){
        for (int n = 0; n < fish_ages.size(); n++){
            cout << fish_ages[n] << ", ";
        }
    }
    
    std::cout << "\n Final number of fish after " << TOTAL_DAYS << " days is: " << final_population << endl;

    return 0;
}


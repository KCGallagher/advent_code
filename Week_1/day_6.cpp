/* 
 * Takes an input txt files with a list of lanternfish ages
 * Evolves fish according to a defined set of rules (fish reproduce every seven days)
 * Return the total number of fish after a given time period (TOTAL_DAYS)
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>
#include <sstream>

using namespace std;

vector<int> evolve_fish(vector<int> fish_ages)
{
    for (int n = 0; n < fish_ages.size(); n++){
        fish_ages[n] += 1;
    }
    fish_ages.push_back(5);
    return fish_ages;
}


int main()
{
    int TOTAL_DAYS = 10; // Total number of days to model

    // IMPORT ALL FISH AGES INTO ONE LONG VECTOR
    ifstream MyReadFile("Week_1/Inputs/day_6_input.txt"); // Read from the text file
    string fish_data; // Create a text string, which is used to output the text file
    vector<int> fish_ages;

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
    for (int day = 0; day < TOTAL_DAYS; day++){
        fish_ages = evolve_fish(fish_ages);
    }

    // OUTPUT FINAL RESULT
    for (int n = 0; n < fish_ages.size(); n++){
        cout << fish_ages[n] << ", ";
    }
    std::cout << "Final number of fish is: " << fish_ages.size() << endl;

    return 0;
}


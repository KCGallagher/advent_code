/* 
 * Inpput a string of the polymer template and a list of pair insertion rules
 * Adds new elements between given character pairs based on insertion rules
 * Scores polymer based on frequency different between most and least common elements
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <assert.h>

using namespace std;

string iterate_polymer(string polymer, vector<pair<string, char>> rules)
{   
    string new_polymer = polymer;
    for (int b = polymer.size() - 2; b >= 0; b --){
        // Iterate over pairs in polymer to compare to rules
        // Start at end (penultimate character) so the first comparison is on the last pair
        // This is so that untouched indices match in new and old strings
        for (int n = 0; n < rules.size(); n++){
            if (polymer.substr(b, 2) == rules[n].first){
                new_polymer.insert(b + 1, string(1, rules[n].second)); // insert new base as string of length 1
            }
        }
    }
    return new_polymer;
}

int score_polymer(string polymer)
{
    // Score is the difference in frequency between most and least common bases
    std::set<char> bases(polymer.begin(), polymer.end());  // set of char in polmyer
    std::set<char>::iterator it = bases.begin(); // iterator pointing to start of set
    vector<int> counts;
    while (it != bases.end())
    {
        int count = 0;
        for (int i = 0; i < polymer.size(); i++){
            if(polymer[i] == (*it)){
                count ++;
            }
        }
        counts.push_back(count);       
        it++;  //Increment the iterator
    }
    int max = *max_element(counts.begin(), counts.end());
    int min = *min_element(counts.begin(), counts.end());
    return (max - min);
}

int main()
{
    const int ITER_NUM = 10; // Number of iterations to apply rules
    string polymer; 
    vector<pair<string, char>> rules;

    ifstream MyReadFile("Week_2/Inputs/day_14_input.txt"); 
    string line; 
    string delimiter = " -> ";

    bool read_template = true;
    while (getline (MyReadFile, line)) {
        if (line.size() == 0){
            read_template = false; // Reached end of template, and start of insertion rules
            continue;  // Skip to next line
        } else if(read_template){
            polymer += line;  // This is the initial template
        } else {
            pair<string, char> line_rule;
            int separation_index = line.find(delimiter);
            line_rule.first = line.substr(0, line.find(delimiter));  
            line.erase(0, line.find(delimiter) + delimiter.length());
            line_rule.second = line[0];
            rules.push_back(line_rule);
        }
    }
    MyReadFile.close(); 

    for (int n = 0; n < ITER_NUM; n++){
        string old_polymer = polymer;
        polymer = iterate_polymer(old_polymer, rules);
    }
    int final_score = score_polymer(polymer);

    // cout << "Sequence after " << ITER_NUM << " iterations is: " << polymer << endl;
    cout << "Score after " << ITER_NUM << " iterations is: " << final_score << endl;

    return 0;
}


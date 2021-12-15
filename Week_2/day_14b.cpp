/* 
 * Input a string of the polymer template and a list of pair insertion rules
 * Generate a map of each pair of bases in the template (to its frequency in the template)
 * Adds new elements between given character pairs based on insertion rules, and update map
 * Scores polymer based on frequency different between most and least common elements.
 * 
 * N.B This script should give the same scores as day_14.ccp, but cannot provide the sequences.
 * It is much more efficient for large numbers of iterations however (i.e. very large polymers).
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <assert.h>

using namespace std;

map<string, long long> generate_map(string polymer)
{
    map<string, long long> pairs;
    for (int b = 0; b < polymer.size() - 1; b++){
        // Iterate over pairs in polymer to add to map
        string base_pair = polymer.substr(b, 2);
        if (pairs.count(base_pair)){  // if pair already in map
            pairs[base_pair] ++; 
        } else{
            pairs[base_pair] = 1;   // first instance
        }
    }
    return pairs;
}

void print_map(map<string, long long> data, string name)
{
    cout << "Map of " << name << " is:" << endl;
    for(map<string, long long>::const_iterator it = data.begin();
        it != data.end(); ++it)
    {
        cout << "   {" << it->first << ": " << it->second << "}\n";
    }
}

void print_map(map<char, long long> data, string name)
{
    // Overloaded function to allow printing of counts map as well
    cout << "Map of " << name << " is:" << endl;
    for(map<char, long long>::const_iterator it = data.begin();
        it != data.end(); ++it)
    {
        cout << "   {" << it->first << ": " << it->second << "}\n";
    }
}

map<string, long long> iterate_polymer(map<string, long long> pairs, vector<pair<string, char>> rules)
{   
    map<string, long long> old_pairs = pairs;
    for (map<string, long long>::iterator p = old_pairs.begin(); p != old_pairs.end(); p++) {
        for (int n = 0; n < rules.size(); n++){
            if (p->first == rules[n].first){  // If rules applies to existing pairs in map
                string new_pairs[2];  //  Array of new pairs formed by split
                new_pairs[0] = std::string() + rules[n].first[0] + rules[n].second;  // std::string() required for char concatenation
                new_pairs[1] = std::string() + rules[n].second + rules[n].first[1];
                for (int i = 0; i < sizeof(new_pairs)/sizeof(new_pairs[0]); i++){
                    if (pairs.count(new_pairs[i])){  // if pair already in map
                        pairs[new_pairs[i]] += old_pairs[rules[n].first]; 
                    } else{
                        pairs[new_pairs[i]] = old_pairs[rules[n].first];   // first instance
                    }
                }
                pairs[rules[n].first] -= old_pairs[rules[n].first]; //  Remove old pair that has now been split
                assert (pairs[rules[n].first] >= 0); // This count should never go negative
            }
        }
    }
    return pairs;
}

long long score_polymer(map<string, long long> pairs)
{
    // Score is the difference in frequency between most and least common bases
    map<char, long long> counts;

    for(map<string, long long>::const_iterator it = pairs.begin();
        it != pairs.end(); ++it)
    {
        string pair = it->first;
        for (int n = 0; n < pair.size(); n++){
            if (counts.count(pair[n])){  // if base already in map
                counts[pair[n]] += it->second;  
            } else{
                counts[pair[n]] = it->second;  // first instance
            }
        }
    }
    print_map(counts, "Counts");

    // FIND MAX AND MIN IN MAP
    pair<long long, long long> min_and_max = make_pair(0, 0);  // record min and max
  
    // Iterate in the map to find the required entries
    for (map<char, long long>::iterator c = counts.begin(); c != counts.end(); c++) {
        if (c == counts.begin()){
            min_and_max.first = c->second;  // First guess for later comparison
            // We don't need to set an initial guess for max here as 0 is fine for that
        }
        if (c->second > min_and_max.second) {
            min_and_max.second = c->second;
        }
        if (c->second < min_and_max.first) {
            min_and_max.first = c->second;
        }
    }

    long long score = (min_and_max.second - min_and_max.first) / 2;  // To account for double counting in pairs
    return score;
}

int main()
{
    const int ITER_NUM = 40; // Number of iterations to apply rules
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

    map<string, long long> pairs = generate_map(polymer);

    for (int n = 0; n < ITER_NUM; n++){
        map<string, long long> old_pairs = pairs;
        pairs = iterate_polymer(old_pairs, rules);
        // print_map(pairs, "Base pairs");
    }

    // Add pair of start/end characters to map before counting - these are the only non-duplicated characters
    string base_pair = string(1, polymer[0]) + polymer[polymer.size() - 1];
    if (pairs.count(base_pair)){ 
        pairs[base_pair] ++;  
    } else{
        pairs[base_pair] = 1;  
    }  
    long long final_score = score_polymer(pairs);  

    cout << "Score after " << ITER_NUM << " iterations is: " << final_score << endl;

    return 0;
}


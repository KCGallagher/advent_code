/* 
 * Takes an input txt files with connections between different regions
 * Major regions are denotes with uppercase, and minor with lowercase
 * Records the number of route that pass from start to end
 * This done without passing through any minor regions multiple times,
 * and also considering routes that cross one minor region twice
 */

#include <iostream> 
#include <fstream> 
#include <map>
#include <string>
#include <set>

using namespace std;

// Define recursive functions for later
int count_paths(string, set<string>, bool, map<string, set<string>>);
int sum_paths(string, set<string>, bool, map<string, set<string>>);

bool minor_region(string region_code)
{
    // Major regions have uppercase codes, minor have lowercase
    // We will only check the first character, and assume others match
    return islower(region_code[0]);
}

void print_map(map<string, set<string>> data, string name)
{
    cout << "Map of " << name << " is:" << endl;
    for(map<string, set<string>>::const_iterator it = data.begin();
        it != data.end(); ++it)
    {
        cout << "   {" << it->first << ": {";
        set<string> values = it->second;
        for (auto it = values.begin(); it != values.end(); ++it)
            cout << *it << ", ";
        cout << "}" << endl;
    }
}

int count_paths(string room, set<string> small_visited, bool revisit, map<string, set<string>> cave)
{
    set<string> visited = small_visited;
    if(room == "end"){
        return 1;
    }
    if(small_visited.count(room)){  //  If room in small_visited
        if(revisit || (room == "start")){
            return 0;
        }
        else{
            revisit = true;
        }
    }
    if(minor_region(room)){
        visited = small_visited;
        visited.insert(room); // Only occurs if room not already visited
    }
    return sum_paths(room, visited, revisit, cave);
}

int sum_paths(string room, set<string> visited, bool revisit, map<string, set<string>> cave)
{
    int summed_paths = 0;
    for(auto neighbour : cave[room]) {
        summed_paths += count_paths(neighbour, visited, revisit, cave);
    }
    return summed_paths;    
}


int main()
{
    // GENERATE MAP OF LINKED CAVES FOR EACH CAVE
    int total_paths = 0;
    map<string, set<string>> cave_paths;

    ifstream MyReadFile("Week_2/Inputs/day_12_input.txt"); // Read from the text file
    string line; // Create a text string, which is used to output the text file
    int line_number = 1;

    string delimiter = "-";
    while (getline (MyReadFile, line)) {
        string line_caves[2];
        line_caves[0] = line.substr(0, line.find(delimiter));
        line.erase(0, line.find(delimiter) + delimiter.length());
        line_caves[1] = line;

        int end_cave_index = 1;
        for (int i = 0; i < 2; i++){
            if (cave_paths.count(line_caves[i]) > 0){  // if start cave already in map
                cave_paths[line_caves[i]].insert(line_caves[end_cave_index]);
            } else{
                cave_paths[line_caves[i]] = {line_caves[end_cave_index]};
            }
            end_cave_index--; //So that we can consider reverse direction of all paths
        }

    }
    MyReadFile.close(); 

    // print_map(cave_paths, "cave paths");

    set<string> empty_set; //For the list of small regions already visited at start of sim
    int total_paths_a = count_paths("start", empty_set, true, cave_paths);
    int total_paths_b = count_paths("start", empty_set, false, cave_paths);

    std::cout << "Number of valid starts: " << cave_paths["start"].size() << endl;
    std::cout << "Number of valid paths without revisiting minor regions: " << total_paths_a << endl;
    std::cout << "Number of valid paths revisiting one minor region: " << total_paths_b << endl;
    return 0;
}


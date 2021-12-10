/* 
 * Takes an input txt files with lines of various bracket characters
 * Identifies mismatching bracket pairs, and reocrds them according to a scoring system
 * Returns the total score from all mismatched bracket pairs
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void print_vector(vector<string> data)
{
    for (int i = 0; i < data.size(); i++){
        cout << "Line " << i << " = " << data[i] << " of length " << data[i].size() << endl;

    }
}

void print_open_clusters(vector<char> data)
{
    cout << "Current form of line: ";
    for (int i = 0; i < data.size() - 1; i++){
        cout << data[i] << ", ";
    }
    cout << data[data.size() - 1] << endl;
}

int corrupt_syntax(string data)
{
    int line_error = 0; 
    
    vector<char> opening_brackets = {'(', '{', '[', '<'}; 
    vector<char> closing_brackets = {')', '}', ']', '>'}; 
    vector<int> cluster_number = {0}; // number of open clusters for each bracket, same order as above
    vector<char> open_clusters = {}; // records all open clusters 
    vector<int> bracket_penalties= {3, 1197, 57, 25137}; // Penalty scores for each corrupt bracket, same order as above

    for (int n = 0; n < data.size(); n++){
        char bracket = data[n];
        if (std::count(opening_brackets.begin(), opening_brackets.end(), bracket)){
            auto iterator = find(opening_brackets.begin(), opening_brackets.end(), bracket);
            int index = iterator - opening_brackets.begin();
            open_clusters.push_back(opening_brackets[index]);
        } else if (std::count(closing_brackets.begin(), closing_brackets.end(), bracket)){
            auto iterator_closer = find(closing_brackets.begin(), closing_brackets.end(), bracket); // index of current closer
            int index_closer = iterator_closer - closing_brackets.begin(); // index of current closer

            auto iterator_last_opener = find(opening_brackets.begin(), opening_brackets.end(), open_clusters.back());
            int index_last_opener = iterator_last_opener - opening_brackets.begin(); // index of last opener

            if (index_closer == index_last_opener){  // If the closer closes the last opening bracket
                open_clusters.pop_back(); // Remove last opener that has now been closed
            }
            else { // invalid closer
                // print_open_clusters(open_clusters);
                // cout << "Next bracket: " << closing_brackets[index_closer] << ", so indices: "  << index_closer << " & " << index_last_opener << endl;
                line_error += bracket_penalties[index_closer];
                break;
            }
        } else {
           throw std::invalid_argument("Unknown character found");
        }
    }
    return line_error;
}




int main()
{
    int error_score = 0;  // Total syntax error score

    // IMPORT ALL HEIGHT VALUES INTO ONE ARRAY
    vector<string> bracket_lines; // Record each string of brackets into an array

    ifstream MyReadFile("Week_2/Inputs/day_10_input.txt"); 
    string line; 
    int line_num = 0;


    while (getline (MyReadFile, line)) {
        bracket_lines.push_back(line);
    }
    MyReadFile.close(); // Close the file

    print_vector(bracket_lines);

    for (int n = 0; n < bracket_lines.size(); n++){
        int line_score = corrupt_syntax(bracket_lines[n]); // defined for printing
        error_score += line_score;
        cout << "Line " << n << " has penalty score " << line_score << endl;
    }

    std::cout << "Total syntax error score: " << error_score << endl;
    return 0;
}

/* 
 * Takes an input txt files with lines of various bracket characters
 * Identifies mismatching bracket pairs, and reocrds them according to a scoring system
 * Returns the total score from all mismatched bracket pairs.
 * Also identifies any incomplete bracket pairs, and returns score based on these too
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

std::pair<int, long long> syntax_scores(string data)
{
    int corrupt_error = 0; 
    long long incomplete_error = 0; 
    
    vector<char> opening_brackets = {'(', '{', '[', '<'}; 
    vector<char> closing_brackets = {')', '}', ']', '>'}; 
    vector<char> open_clusters = {}; // records all open clusters 
    vector<int> corrupt_penalties = {3, 1197, 57, 25137}; // Penalty scores for each corrupt bracket, same order as above
    vector<int> incomplete_penalties= {1, 3, 2, 4}; // Penalty scores for each incomplete bracket, same order as above

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
                corrupt_error += corrupt_penalties[index_closer];
                break;
            }
        } else {
           throw std::invalid_argument("Unknown character found");
        }
    }
    // At this point, open_clusters contains all brackets that have no closing pair.
    if (corrupt_error == 0){
        for (int n = open_clusters.size() - 1; n >= 0; n--){  // Iterate through open clusters backwards
        auto iterator = find(opening_brackets.begin(), opening_brackets.end(), open_clusters[n]);
        int index = iterator - opening_brackets.begin();
        incomplete_error *= 5; // Pointless step to make scoring more complicated
        incomplete_error += incomplete_penalties[index];
        }
    }
    return std::make_pair(corrupt_error, incomplete_error);
}


int main()
{
    int corrupt_score = 0;  // Total corrupt syntax score
    vector<long long> incomplete_scores;  // Total incomplete syntax score

    vector<string> bracket_lines; // Record each string of brackets into an array
    ifstream MyReadFile("Week_2/Inputs/day_10_input.txt"); 
    string line; 

    while (getline (MyReadFile, line)) {
        bracket_lines.push_back(line);
    }
    MyReadFile.close(); 

    // print_vector(bracket_lines);  // Used to check all bracket lines have been imported correctly

    for (int n = 0; n < bracket_lines.size(); n++){
        pair<int, int> line_scores = syntax_scores(bracket_lines[n]); 
        corrupt_score += line_scores.first;
        if (line_scores.second > 0){  // Only record none zero scores (i.e. dont record 0 for corrupt lines)
            incomplete_scores.push_back(line_scores.second); 
            // cout << "    Line " << n << " has penalty score " << line_scores.second << endl;
        }
    }

    // Find median incomplete score to output
    size_t median_index = (incomplete_scores.size() - 1) / 2;
    std::sort(incomplete_scores.begin(), incomplete_scores.end());

    std::cout << "Total corrupt syntax score: " << corrupt_score << endl;
    std::cout << "Median incomplete syntax score: " << incomplete_scores[median_index] << endl;
    return 0;
}

/* 
 * Takes an input txt files with connections between different regions
 * Major regions are denotes with uppercase, and minor with lowercase
 * Records the number of route that pass from start to end
 * This done without passing through any minor regions multiple times,
 * and also considering routes that cross one minor region twice
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>
#include <assert.h>

using namespace std;

vector<vector<int>> make_dots_array(vector<pair<int, int>> coords){
    // Determine size for array - find max values in coords and assume min is (0,0)
    pair<int, int> max_size = make_pair(0, 0);
    for (int n = 0; n < coords.size(); n++){
        if (coords[n].first + 1 > max_size.first){
            max_size.first = coords[n].first + 1;
        }
        if (coords[n].second + 1 > max_size.second){
            max_size.second = coords[n].second + 1;
        }
    }

    // Create empty array of sufficient size
    vector<vector<int>> array;
    for (int i = 0; i < max_size.second; i++){
        vector<int> row_vector(max_size.first, 0);
        array.push_back(row_vector);
    }

    // Fill array
    for (int n = 0; n < coords.size(); n++){
        array[coords[n].second][coords[n].first] = 1;
    }
    return array;
}

vector<vector<int>> fold_dots_array(vector<vector<int>> dots, pair<char, int> fold){
    vector<vector<int>> output;
    if (fold.first == 'x'){
        assert (2 * fold.second + 1 == dots[0].size()); // so fold is halfway across array
        for (int i = 0; i < dots.size(); i++){ //iterate over rows
            vector<int> row_vector;
            for (int j = 0; j < fold.second; j++){
                row_vector.push_back(dots[i][j] || dots[i][2 * fold.second - j]);
            }
            output.push_back(row_vector);
        }
    } else if(fold.first == 'y'){
        assert (2 * fold.second + 1 == dots.size()); // so fold is halfway across array
        for (int i = 0; i < fold.second; i++){ //iterate over rows
            vector<int> row_vector;
            for (int j = 0; j < dots[0].size(); j++){
                row_vector.push_back(dots[i][j] || dots[2 * fold.second - i][j]);
            }
            output.push_back(row_vector);
        }
    } else {
        throw std::invalid_argument("Unknown axis for fold");
    }
    return output;
}

int count_dots(vector< vector<int> > data)
{
   int total_dots = 0;
    for (int i = 0; i < data.size() ; i++){ // for each row
        for (int j = 0; j < data[i].size(); j++){
            total_dots +=data[i][j];
        }
    }
    return total_dots;
}

char print_dot(int dot)
{
    if(dot){
        return '#';
    } else{
        return ' ';
    }
}

void print_dots_array(vector< vector<int> > data)
{
    // Print framed array of dots
    cout << "Print dot array: \n" << "    " + string(data[0].size(), '_') << endl;
    for (int i = 0; i < data.size() ; i++){ // for each row
        cout << "   |"; // Indent printed array with a tab
        for (int j = 0; j < data[i].size(); j++){
            cout << print_dot(data[i][j]);
        }
        cout << "|\n";
    }
    cout << "    " + string(data.back().size(), '-') << "\n\n";  
    // Sadly my compiler doesn't like the overline character so a hyphen will have to do
}

int main()
{
    // GENERATE MAP OF LINKED CAVES FOR EACH CAVE
    int total_paths = 0;
    vector<pair<int, int>> coords;
    vector<pair<char, int>> folds;

    ifstream MyReadFile("Week_2/Inputs/day_13_input.txt"); // Read from the text file
    string line; // Create a text string, which is used to output the text file
    int line_number = 1;

    string delimiter = ",";
    bool read_coords = true;
    while (getline (MyReadFile, line)) {
        if (line.size() == 0){
            read_coords = false; // Reached end of coords, and start of folds
            continue;  // Skip to next line
        } else if(read_coords){
            pair<int, int> line_coords;
            line_coords.first = stoi(line.substr(0, line.find(delimiter)));
            line.erase(0, line.find(delimiter) + delimiter.length());
            line_coords.second = stoi(line);
            coords.push_back(line_coords); // add to overall list of coords
        } else {
            pair<char,int> line_fold;
            int equals_index = line.find("=");
            line_fold.first = line[equals_index - 1];  //  For character before equals
            line_fold.second = stoi(line.substr(equals_index + 1, equals_index + 2));
            folds.push_back(line_fold);
        }
    }
    MyReadFile.close(); 

    vector<vector<int>> dots = make_dots_array(coords);
    int initial_dots = count_dots(dots);
    int first_fold_dots = count_dots(fold_dots_array(dots, folds[0]));

    for (int n = 0; n < folds.size(); n++){
        vector<vector<int>> old_dots = dots;
        dots = fold_dots_array(old_dots, folds[n]);
    }
    int final_fold_dots = count_dots(dots);

    cout << "Number of initial dots: " << initial_dots << endl;
    cout << "Number of dots after first fold: " << first_fold_dots << endl;
    cout << "Number of dots after final fold: " << final_fold_dots << endl;

    print_dots_array(dots);
    return 0;
}


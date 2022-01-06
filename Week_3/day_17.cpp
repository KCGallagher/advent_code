/* 
 * Maps out a target from a range of x and y values
 * Varies the initial velocity of a projectile from the origin,
 * under the contraint that it passes through the target on an integer time step
 * Returns the maximum height attained by the rocket, and the number of valid paths.
 */

#include <iostream> 
#include <fstream> 
#include <string>
#include <vector>
#include <array>

using namespace std;

vector<vector<int>> space_array(array<int, 4> target, vector<int> origin = {0,0})
{
    // Returns an array large enough to contain the target and origin.
    int x_dim = max(target[1], origin[0]) - min(target[0], origin[0]);
    int y_dim = max(target[3], origin[1]) - min(target[2], origin[1]);

    vector<vector<int>> array;
    vector<int> row_vector (y_dim + 1, 0);  // Extra space for 0
    for (int x = 0; x <= x_dim; x++){
        array.push_back(row_vector);
    }
    return array;
}

vector<vector<int>> plot_target(vector<vector<int>> space, array<int, 4> target, vector<int> origin = {0,0})
{
    array<int, 2> offset = {min(target[0], origin[0]), min(target[2], origin[1])};  // diff between index and pos
    for (int x = target[0]; x <= target[1]; x++){
        for (int y = target[2]; y <= target[3]; y++){
            space[x - offset[0]][y - offset[1]] = 1;  // 1 in space represents a target space
        }
    }
    space[origin[0] - offset[0]][origin[1] - offset[1]] = 2;  // 2 in space represents origin
    return space;
}

char print_element(int value)
{
    if(value == 0){
        return ' ';  // For empty space
    } else if(value == 1){
        return 'T';  // For target space
    } else if(value == 2){
        return 'O';  // For origin
    } else if(value == 3){
        return 'X';  // For trajectory
    } else{
        throw invalid_argument("Unrecognised Code in Space");
    }
}

void print_space(const vector< vector<int> > data)
{
    // Print framed array of space 
    cout << "Print space array: \n" << "    " + string(data.size(), '_') << endl;
    for (int y = data[0].size() - 1; y >= 0; y--){ // for each row
        cout << "   |"; // Indent printed array with a tab
        for (int x = 0; x < data.size(); x++){
            cout << print_element(data[x][y]);
        }
        cout << "|\n";
    }
    cout << "    " + string(data.size(), '-') << "\n\n";  
    // Sadly my compiler doesn't like the overline character so a hyphen will have to do
}

bool hit_target(vector<int> pos, array<int, 4> target){
    // Check whether position is in target
    bool x_check = (pos[0] >= target[0]) && (pos[0] <= target[1]);
    bool y_check = (pos[1] >= target[2]) && (pos[1] <= target[3]);
    return x_check && y_check;
}

int val_sign(int val){
    // Returns sign of a value (0 for 0)
    return (val > 0) - (val < 0);
}

vector<vector<int>> plot_trajectory(int vx, int vy, array<int, 4> target, vector<int> origin){
    vector<vector<int>> coordinates;
    coordinates.push_back(origin);
    int step = 0;
    vector<int> current_pos = origin;
    while (true){
        // Update position
        current_pos[0] += vx;
        current_pos[1] += vy;
        coordinates.push_back(current_pos);

        // Update velocity
        vx = (abs(vx) - 1) * (val_sign(vx));
        vy--;

        // Check if trajectory is complete
        if (hit_target(current_pos, target)){
            break;
        }
        if ((current_pos[1] < target[2]) && (vy < 0)){  
            // Break if below target and going down
            vector<int> null_coordinate = {origin[0], origin[1] - 1};  // Just below origin
            vector<vector<int>> null_coordinates;
            null_coordinates.push_back(null_coordinate);
            coordinates = null_coordinates;  // No relevant coords as missed target, so return zeroes
            break;
        }
    }
    return coordinates;
}

int find_height(vector<vector<int>> coordinates, vector<int> origin = {0,0}){
    // Iterate over all coords, return height at peak of trajectory
    int height = origin[1];
    for (int n = 0; n < coordinates.size(); n++){
        if (height < coordinates[n][1]){
            height = coordinates[n][1];
        }
    }
    return height;
}

pair<int, int> max_height(vector<vector<int>> space, array<int, 4> target, vector<int> origin = {0,0})
{
    // Find the most extreme velocities in the direction of the target, that don't overshoot in one step
    int vx_min = min(0, target[0] - origin[0]); 
    int vx_max = max(0, target[1] - origin[0]); 

    pair<int, int> v0 = make_pair(0,0); //Left and downmost velocity that still hits target
    int max_height = origin[0];
    int path_count = 0;

    for (int vx = vx_min; vx <= vx_max; vx++){  // Iterate over all possible v_x values
        int vy =  min(0, target[2] - origin[1]);  // Min velocity without skipping target
        while (true){
            vector<vector<int>> coordinates = plot_trajectory(vx, vy, target, origin);
            int height = find_height(coordinates);
            if (coordinates[0] == origin){
                path_count++;  // Paths that don't reach target have modified origin
            }
            if (height > max_height){
                max_height = height;
            }
            if (vy > 200){
                // Haven't thought of a nice way to break, so currently using preset max value
                break;
            }
            vy++;
        }
    }
    return make_pair(max_height, path_count);
}


int main()
{
    ifstream MyReadFile("Week_3/Inputs/day_17_input.txt"); 
    string line; 

    //  READ IN TARGETS (ONE PER LINE)
    string x_coord = "x=";
    string y_coord = "y=";
    string delimiter = "..";
    array<int, 4> target;  // Form [x_min, x_max, y_min, y_max]

    while (getline (MyReadFile, line)) {
        int x_start = line.find(x_coord) + x_coord.length();
        target[0] = stoi(line.substr(x_start, line.find(delimiter) - x_start));
        int x_stop = line.find(delimiter) + delimiter.length();
        target[1] = stoi(line.substr(x_stop , line.find(",") - x_stop));
        line.erase(0, line.find(","));

        int y_start = line.find(y_coord) + y_coord.length();
        target[2] = stoi(line.substr(y_start, line.find(delimiter) - y_start));
        line.erase(0, line.find(delimiter) + delimiter.length());
        target[3] = stoi(line);
    }
    MyReadFile.close(); // Close the file

    // GENERATE ARRAY OF PHYSICAL SPACE - unnecessary but nice to look at
        // Here, values 0/1/2/3 represent empty space/target/origin/trajectory
    vector<vector<int>> space = space_array(target);
    space = plot_target(space, target);
    // print_space(space);  

    // TRACK TRAJECTORY ACROSS SPACE
    auto [height, count] = max_height(space, target);

    cout << "The maxmimum height reached by the probe is: " << height << endl;
    cout << "The number of avaliable paths to the probe is: " << count << endl;
    return 0;
}
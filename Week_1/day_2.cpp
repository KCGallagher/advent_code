/* 
 * Takes an input txt files with sequential instruction on where to travel
 * Returns the final position attained by following these instructions
 * Note that this is in the form [depth, x position] to define current position
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>
#include <sstream>
using namespace std;

vector<int> update_pos(string command, int magnitude, vector<int> pos, bool use_aim)
{
    string up = "up"; // To avoid comparison to an absolute string
    string down = "down";
    string forward = "forward";
    if (use_aim){
        if (command == up){
            pos[2] = pos[2] - magnitude;
        }
        if (command == down){
            pos[2] = pos[2] + magnitude;
        }
        if (command == forward){
            pos[1] = pos[1] + magnitude;
            pos[0] = pos[0] + magnitude * pos[2];
        }
    }
    else {
        if (command == up){
            pos[0] = pos[0] - magnitude;
        }
        if (command == down){
            pos[0] = pos[0] + magnitude;
        }
        if (command == forward){
            pos[1] = pos[1] + magnitude;
        }
    }

    return pos;
}

int main()
{
    std::vector<int> my_pos = {0, 0, 0}; // 2D array in form [depth, x position, aim] to define current position
    bool USE_AIM = true;

    ifstream MyReadFile("Week_1/day_2_input.txt"); // Read from the text file
    string line; // Create a text string, which is used to output the text file

    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, line)) {
        string buf;                 // Have a buffer string
        stringstream ss(line);       // Insert the string into a stream
        std::vector<std::string> tokens; // Create vector to hold our words
        while (ss >> buf)
            tokens.push_back(buf);

        // First token is commmand, second token is distance/magnitude
        my_pos = update_pos(tokens[0], stoi(tokens[1]), my_pos, USE_AIM);
    }

    MyReadFile.close(); // Close the file

    cout << "Final Position: (" << my_pos[0] << ", " << my_pos[1] << ")" << endl;
    cout << "Prolbem Solution: " << my_pos[0] * my_pos[1] << endl;
    return 0;
}


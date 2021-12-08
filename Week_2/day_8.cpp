/* 
 * Takes an input txt files with binary values on separate lines
 * Finds a binary number corresponding to the mode digit at placeholder (gamma rate)
 * Epsilon rate is complement of this, script also returns the product of the two values
 */

#include <iostream> 
#include <fstream> 
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int count_part_a(vector<string> outputs)
{
    int in_line_count = 0;
    vector<int> vec = {2,3,4,7};  // Unique lengths for each character
    for (int i = 0; i < outputs.size(); i++){ 
        if (std::find(vec.begin(), vec.end(), outputs[i].size()) != vec.end()){
            in_line_count += 1;
        }
    }
    return in_line_count;
}

int sum_part_b(vector<string> outputs)
{
    int in_line_count = 0;
    return in_line_count;
}

int main()
{
    int running_count = 0; // Of digits 1,4,7,8 for part A
    int running_sum = 0;  // Of the overall 4 digit output for part B

    ifstream MyReadFile("Week_2/Inputs/day_8_input.txt"); // Read from the text file
    string line; // Create a text string, which is used to output the text file

    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, line)) {
        string delimiter = " | ";
        string digits_string = line.substr(0, line.find(delimiter));
        string digit_buffer;                 // Have a buffer string
        stringstream ss(digits_string);       // Insert the string into a stream
        std::vector<std::string> digits; // Create vector to hold our words
        while (ss >> digit_buffer)
            digits.push_back(digit_buffer);

        string output_buffer;
        string output_string = line.erase(0, line.find(delimiter) + delimiter.length());
        stringstream ss2(output_string);       // Insert the string into a stream
        std::vector<std::string> outputs; // Create vector to hold our words
        while (ss2 >> output_buffer)
            outputs.push_back(output_buffer);
        
    //     for (int i=0; i < digits.size(); i++){
    //         cout << digits[i] << ", ";
    //     }
    //     cout << " & ";
    //     for (int i=0; i < outputs.size(); i++){
    //         cout << outputs[i] << ", ";
    //     }
    //     cout << "\n";

        running_count += count_part_a(outputs);
        running_sum += sum_part_b(outputs);

    }

    MyReadFile.close(); // Close the file

    cout << "There are " << running_count << " occurances of digits 1, 4, 7, 8 in the sample." << endl;
    cout << "The total sum of all four digit outputs (once decoded) is: " << running_sum << endl;
    return 0;
}


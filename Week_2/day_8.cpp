/* 
 * Takes an input txt files with all digit and target output signal patterns
 * Counts the number of specific patterns for part a
 * Decodes the signal patterns to sum all decoded output numbers for part b
 */

#include <iostream> 
#include <fstream> 
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <assert.h>

using namespace std;

void print_char_vector(vector<char> vector, string name)
{
    cout << name << " = {";
    for (int i=0; i < vector.size() - 1; i++){
        cout << vector[i] << ", ";
    }
    cout << vector[vector.size() - 1] << "}\n";
}

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

int sum_part_b(vector<string> digits, vector<string> outputs)
{
    // Sort elements in digits vector by length
    std::sort(digits.begin(), digits.end(), []
    (const std::string& first, const std::string& second){
        return first.size() < second.size();
    });

    // for (int i=0; i < digits.size(); i++){
    //     cout << digits[i] << ", ";
    // }
    // cout << "\n";

    // Create map from signal characters (lowercase) to true characters (Uppercase)
    vector<char> unknown_char = {'a','b','c','d','e','f','g'};
    vector<char> map = {'X','X','X','X','X','X','X'};  // a will map to map[0] etc
    
    for (int n = 0; n < unknown_char.size(); n++){
        char x = unknown_char[n];
        if (!(digits[9].find(x) < 7)){  // Sanity check - all characters should be in this string
            cout << "Unknown character " << x << " not in longest digit code " << digits[9] << endl;
            break;
        }
        if ((digits[1].find(x)) < 3 && !(digits[0].find(x) < 2)){ // A
            map[n] = 'A';  // A is the character in the length 3 string (value = 7), but not the length 2 (value = 1)
            continue;
        }
        if (digits[0].find(x) < 2){ // C or F
            if ((digits[6].find(x) < 6) && (digits[7].find(x) < 6) && (digits[8].find(x) < 6)){
                map[n] = 'F';  // F is in all three strings of length 6 (values 0,6,9)
                continue;
            }
            else {
                map[n] = 'C';  // C is only in two strings of length 6 (values 0,9)
                continue;
            }
        }
        if (digits[2].find(x) < 4){ // B or D (as options C and F are gone)
            if ((digits[3].find(x) < 5) && (digits[4].find(x) < 5) && (digits[5].find(x) < 5)){
                map[n] = 'D';  // D is in all three strings of length 5 (values 2,3,5)
                continue;
            }
            else {
                map[n] = 'B';  // B is only in one string of length 5 (value 5)
                continue;
            }
        }
        // Only E and G left now!
        if ((digits[3].find(x) < 5) && (digits[4].find(x) < 5) && (digits[5].find(x) < 5)){
            map[n] = 'G';  // G is in all three strings of length 5 (values 2,3,5)
            continue;
        }
        else {
            map[n] = 'E';  // B is only in one string of length 5 (value 2)
            continue;
        }
    }

    // Check for duplicates in map (otherwise continue)
    vector<char> sorted_map = map;  // Create copy of map to sort
    std::sort(sorted_map.begin(), sorted_map.end());
    bool hasDuplicates = std::adjacent_find(sorted_map.begin(), sorted_map.end()) != sorted_map.end();
    if (hasDuplicates){
        cout << "Warning: Duplicates found in map object but mapping must be one-to-one" << endl;
        print_char_vector(map, "Map");

    }

    // Use map to decode output, and convert true letters to numbers!
    vector<int> decoded_outputs = {};
    vector<string> map_to_digits_outputs = {"ABCEFG", "CF", "ACDEG", "ACDFG", "BCDF", "ABDFG", "ABDEFG", "ACF", "ABCDEFG", "ABCDFG"};
    for (int i = 0; i < outputs.size(); i++){ // Iterate over output numbers
        std::string mapped_output = "";
        for (int j = 0; j < outputs[i].size(); j++){ //Iterate over characters in output
            auto iterator = find(unknown_char.begin(), unknown_char.end(), outputs[i][j]);
            int index = iterator - unknown_char.begin();
            char y = map[index]; // Translate back to true letters
            mapped_output += y;
        } 
        // Find the (sorted) output in map_to_digits_output - the index is the corresponding number!
        sort(mapped_output.begin(), mapped_output.end());  // Sort for comparison with map_to_digits_outputs
        auto iterator_2 = find(map_to_digits_outputs.begin(), map_to_digits_outputs.end(), mapped_output); 
        int digit = iterator_2 - map_to_digits_outputs.begin();
        decoded_outputs.push_back(digit);
    }

    // Convert output vector of digits into a number
    int output_number = 0;
    assert (decoded_outputs.size() == outputs.size());
    for (int n = 0; n < outputs.size(); n++){
        output_number += decoded_outputs[n] * pow(10, (outputs.size() - n - 1));
    }
    // cout << "Output number: " << output_number << endl;
    return output_number;
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

        running_count += count_part_a(outputs);
        running_sum += sum_part_b(digits, outputs);

    }

    MyReadFile.close(); // Close the file

    cout << "There are " << running_count << " occurances of digits 1, 4, 7, 8 in the sample." << endl;
    cout << "The total sum of all four digit outputs (once decoded) is: " << running_sum << endl;
    return 0;
}

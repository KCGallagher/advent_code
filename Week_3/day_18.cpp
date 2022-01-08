/* 
 * Takes an list of pair based `snailfish' numbers
 * Evaluates the sum of these numbers according to a set of predefined rules
 * Returns overall sum, and max sum from addition of a pair of numbers
 * 
 * Might be nice to refactor so that snailnumbers are a new data type, with
 * operations such as addition overloaded for this type.
 * Also some concerns over behaviour when three digit numbers appear - can
 * give unexplained behaviour in certain cases
 * 
 */

#include <iostream> 
#include <fstream> 
#include <string>
#include <vector>
#include <algorithm>
#include <math.h> 

using namespace std;


bool is_number(const string& s)
{
    return all_of(s.begin(), s.end(),
                  [](char c){ return isdigit(c) != 0; });
}

string add_numbers(string x, string y)
{
    return ("[" + x + "," + y + "]");
}

vector<string> extract_values(string line)
{
    // Returns a vector of all numbers in the line (as strings)
    vector<string> numbers;
    int n = 0;
    while(n < line.length()){
        if (isdigit(line[n])){  // Start of number
            string number;
            for (int m = 0; m < line.length() - n; m++){
                if (!isdigit(line[n+m])){
                    numbers.push_back(number);  // Found complete number
                    n += m;  // Update extent of search
                    break;
                }
                number += line[n+m];
            }
        }
        n++;
    }
    return numbers;
}

string to_split(string line){
    // Returns first value over 10 to target in split
    vector<string> numbers = extract_values(line);
    for (int n = 0; n < numbers.size(); n++){
        if (numbers[n].length() > 1){
            return numbers[n]; 
        }
    }
    return "";  // Empty string if no values required to split
}

string split_num(string num)
{  
    int split_val = stoi(to_split(num));
    int value_1 = (int)floor(split_val / 2.0);
    int value_2 = (int)ceil(split_val / 2.0);
    string pair = ("[" + to_string(value_1) + "," + to_string(value_2) + "]");

    int split_loc = num.find(to_string(split_val));  
    int value_length = min(num.find("]", split_loc), num.find(",", split_loc)) - split_loc;
    num.replace(split_loc, value_length, pair); //  Replace pair with zero
    return num;
}

int to_explode(string num)
{
    // Finds 5th order nested pair to explode - returns first index of pair
    int order = 0;
    for (int n = 0; n < num.length(); n++){
        if (num[n] == '['){
            order++;
        }
        if (num[n] == ']'){  // First closer
            order--;
        }
        if (order > 4){
            return n;
        }
    }
    return -1;  // If no pair to explode found
}


string explode_num(string num, int n)
{
    // Input number, and index of '[' in pair to explode
    string pair = num.substr(n, num.find("]", n) - n + 1);
    string value_1 = pair.substr(1, pair.find(",") - 1);  // Offset of 1 for "["
    string value_2 = pair.substr((pair.find(",") + 1), (pair.length() - pair.find(",") - 2));  // Offset of 1 for "]"

    int later_target = num.find_first_of("0123456789", n + pair.length());
    if(later_target != string::npos){  // I.e if a number before the target pair was found
        int later_length = min(num.find("]", later_target), num.find(",", later_target)) - later_target;
        string later_i_value = num.substr(later_target, later_length); // Initival value after pair
        int later_f_value = (stoi(later_i_value) + stoi(value_2));  // Value after pair post explode
        num.replace(later_target, later_length, to_string(later_f_value));  //  Change first value after pair
    }
      
    num.replace(n, pair.length(), "0"); //  Replace pair with zero

    int earlier_target = num.find_last_of("0123456789", n - 1);
    if(earlier_target != string::npos){
        int earlier_length = min(num.find("]", earlier_target), num.find(",", earlier_target)) - earlier_target;
        string earlier_i_value = num.substr(earlier_target, earlier_length);
        int earlier_f_value = (stoi(earlier_i_value) + stoi(value_1));
        num.replace(earlier_target, earlier_length, to_string(earlier_f_value));  // Change first value before pair
    }
    return num;
}

string reduce_num(string num)
{
    while(true){
        int explode_loc = to_explode(num);  // Returns -1 if no value to explode
        string split_target = to_split(num);  // Returns empty string if no value to split
        if (explode_loc >= 0){ 
            num = explode_num(num, explode_loc);
            continue;
        } else if(split_target.length()){
            num = split_num(num);
            continue;
        }
        else{
            break;  // Number is fully reduced
        }
    }
    return num;
}

int pair_magnitude(string line_sec){
    string value_1 = line_sec.substr(1, line_sec.find(",") - 1);  // Offset of 1 for "["
    string value_2 = line_sec.substr((line_sec.find(",") + 1), (line_sec.length() - line_sec.find(",") - 2)); 
    return ((3 * stoi(value_1)) + (2 * stoi(value_2)));  // -'0' to convert char to int
}

string eval_fundamental_pair(string line){
    // Evaluate magnitude of highest order (deepest) pair
    int last_opener = 0;
    for (int n = 0; n < line.length(); n++){
        if (line[n] == '['){
            last_opener = n;
        }
        if (line[n] == ']'){  // First closer
            string pair = line.substr(last_opener, (n - last_opener + 1));
            int mag = pair_magnitude(pair);
            line.replace(last_opener, (n - last_opener + 1), to_string(mag));
            return line;
            }
    }
    throw invalid_argument("No Fundamental Pair in Line");
}

int line_magnitude(string line){
    // Sequentially evaluate the innermost pair in each line
    while (!is_number(line)){  // Until line reduced to single value
        line = eval_fundamental_pair(line);
    }
    return stoi(line);
}

int main()
{
    ifstream MyReadFile("Week_3/Inputs/day_18_input.txt");
    string line;
    vector<string> lines;

    int line_count = 0;
    string current_sum;
    string new_sum;

    while (getline (MyReadFile, line)) {
        line_count++;
        lines.push_back(line);
        if(line_count == 1){
            current_sum = line;
            continue;
        }
        new_sum = add_numbers(current_sum, line);
        current_sum = reduce_num(new_sum);  // Update running total
    }
    MyReadFile.close(); 

    int total_sum = line_magnitude(current_sum);  // Find magnitude of final line
    cout << "Cumulative sum of snailfish numbers: " << total_sum << endl;

    int largest_pair = 0;
    for (int i = 0; i < lines.size(); i++){
        for (int j = 0; j < lines.size(); j++){
            if (i == j){
                continue;  // Don't consider adding number to itself
            }
            string sum = add_numbers(lines[i], lines[j]);
            sum = reduce_num(sum);
            int mag = line_magnitude(sum);
            if (mag > largest_pair){
                largest_pair = mag;
            }
        }
    }
    cout << "Largest sum from a pair of snailfish numbers: " << largest_pair << endl;

    return 0;
}
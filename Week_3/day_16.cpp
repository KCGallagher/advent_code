/* 
 * Takes an hexadecimal string and converts it to a binary string
 * Unpacks the package version numbers from the operator packet
 * Returns the sum of version numbers
 */

#include <iostream> 
#include <fstream> 
#include <string>
#include <bitset>


using namespace std;

string hd_to_bin(string hd_string)
{
    // Converts hexadecimal string to binary, character wise
    string bin_string = "";
    for (const char &x :hd_string){
        int value = std::stoi(string(1, x), 0, 16);
        bitset<4> b(value);
        bin_string += b.to_string();
    }
    return bin_string;
}


pair<int, string> literal_value(string data)
{
    int group_start = 6;
    string string_value = "";
    while (true) {
        string_value += data.substr(group_start + 1, 4) ;  
        // Just records literal value for now
        if (data[group_start] == '0'){
            break;
        }
        group_start += 5;  // Jump to next group
    }
    // SIDE EFFECT - removes current packet from string
    // This might not be good practice, but it is easiest to do here
    data = data.substr(group_start + 5);

    int total_value = stol(string_value, 0, 2);
    cout << "      Literal Value: " << total_value << endl;
    return make_pair(total_value, data);
}

pair<int, string> count_version(string data)
{
    int version_num = stoi(data.substr(0, 3), 0, 2);
    if(data.substr(3, 3) == "100"){  // For Literal Value Packet
        cout << "   Lit Value Packet: " << version_num << endl;
        pair<int, string> output = literal_value(data);
        int lit_value = output.first;
        data = output.second;  // Wanted to use structured bindings but wouldn't compile
    } 
    else {  // Operator packet
        if(data[6] == '0'){  // 15 bits give length of subpackages
            cout << "   Operator Packet (0) " << version_num << endl;
            int len = stoi(data.substr(7, 15), 0, 2);
            string sub_data = data.substr(7 + 15, len);
            while (sub_data.length()){  // Iterate through sub packages
                pair<int, string> output = count_version(sub_data);
                version_num += output.first;
                sub_data = output.second;
            }
            data = data.substr(7 + 15 + len);  // Remove all packages considered above

        } else {  // 11 bits give number of subpackages
            cout << "   Operator Packet (1): " << version_num << endl;
            int num = stoi(data.substr(7, 11), 0, 2);
            string sub_data = data.substr(7 + 11);
            for (int n = 0; n < num; n++){  // Iterate through sub packages
                pair<int, string> output = count_version(sub_data);
                version_num += output.first;
                sub_data = output.second;
            }
            data = sub_data;
        }
    }
    return make_pair(version_num, data);  
}

int count_versions(string data, int version_count)
{
    while (data.length()){  // Stop when data is empty
        if (data.length() < 10){  // Avoid checking very large numbers
            if (stol(data, 0, 2) == 0){
                break;  // Stop when you reach the padding zeroes
            }
        }
        
        pair<int, string> output = count_version(data);
        version_count += output.first;
        data = output.second;  // Wanted to use structured bindings but wouldn't compile
    }
    return version_count;
}


int main()
{
    ifstream MyReadFile("Week_3/Inputs/day_16_input.txt");
    string line;
    string hd_data = ""; // String of hexadecimal data

    while (getline (MyReadFile, line)) {
        hd_data += line;  // Accounts for multiple lines of data by concatenating
    }
    MyReadFile.close(); 

    string bin_data = hd_to_bin(hd_data);  // Convert to binary form

    int version_count = 0;
    int score = count_versions(bin_data, version_count);
 
    // std::cout << hd_data << endl;
    // std::cout << bin_data << endl;

    cout << "Cumulative version number of packages: " << score << endl;

    return 0;
}


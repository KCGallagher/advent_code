/* 
 * Takes an hexadecimal string and converts it to a binary string
 * Unpacks the package version numbers from the operator packet and returns sum
 * Operates on all literal values as er predefined rules to give final value
 * 
 * Note that this file has structured bindings which requires c++17
 */

#include <iostream> 
#include <fstream> 
#include <string>
#include <bitset>
#include <tuple> 
#include <vector>
#include <algorithm>
#include <numeric>


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


tuple<string, long long> literal_value(string data)
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
    data = data.substr(group_start + 5);

    long long total_value = stoll(string_value, 0, 2);
    return make_tuple(data, total_value);
}

long long packet_value(vector<long long> values, int type_id)
{
    if (type_id == 0){  // Sum
        return std::accumulate(values.begin(), values.end(), 0);
    } else if (type_id == 1){  // Product
        long long product = 1;
        for (auto& n : values)
            product *= n;
        return product;
    } else if (type_id == 2){  // Minimum
        return *min_element(values.begin(), values.end());
    } else if (type_id == 3){  // Maximum
        return *max_element(values.begin(), values.end());
    } else if (type_id == 5){  // Greater than
        return int(values[0] > values[1]);
    } else if (type_id == 6){  // Less than
        return int(values[0] < values[1]);
    } else if (type_id == 7){  // Equal to
        return int(values[0] == values[1]);
    } else {
        throw invalid_argument("Unrecognised Type_ID");
    }
}

tuple <string, int, long long> open_package(string data)
{
    long long value = 0;
    int version_num = stoi(data.substr(0, 3), 0, 2);
    if(data.substr(3, 3) == "100"){  // For Literal Value Packet
        auto [new_data, new_value] = literal_value(data);
        data = new_data;
        value = new_value;
    } 
    else {  // Operator packet
        if(data[6] == '0'){  // 15 bits give length of subpackages
            int len = stoi(data.substr(7, 15), 0, 2);
            string sub_data = data.substr(7 + 15, len);
            vector<long long> values;
            while (sub_data.length()){  // Iterate through sub packages
                auto [new_data, version, new_value] = open_package(sub_data);
                sub_data = new_data;
                version_num += version;
                values.push_back(new_value);
            }
            value = packet_value(values, stoi(data.substr(3, 3), 0, 2));
            data = data.substr(7 + 15 + len);  // Remove all packages considered above

        } else {  // 11 bits give number of subpackages
            int num = stoi(data.substr(7, 11), 0, 2);
            string sub_data = data.substr(7 + 11);
            vector<long long> values;
            for (int n = 0; n < num; n++){  // Iterate through sub packages
                auto [new_data, version, new_value] = open_package(sub_data);
                sub_data = new_data;
                version_num += version;
                values.push_back(new_value);
            }
            value = packet_value(values, stoi(data.substr(3, 3), 0, 2));
            data = sub_data;
        }
    }
    return make_tuple(data, version_num, value);  
}

pair<int, long long> open_packages(string data, int version_count)
{
    long long value = 0;
    while (data.length()){  // Stop when data is empty
        if (data.length() < 10){  // Avoid checking very large numbers
            if (stoi(data, 0, 2) == 0){
                break;  // Stop when you reach the padding zeroes
            }
        }
        
        auto [new_data, version, new_value] = open_package(data);
        data = new_data;
        version_count += version;
        value = new_value;
    }
    return make_pair(version_count, value);
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
    pair<int, long long> scores = open_packages(bin_data, version_count);
 
    // std::cout << hd_data << endl;
    // std::cout << bin_data << endl;

    cout << "Cumulative version number of packages: " << scores.first << endl;
    cout << "Final score of packages: " << scores.second << endl;

    return 0;
}


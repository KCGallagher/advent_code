/* 
 * Takes an input txt files with binary values on separate lines
 * Finds a binary number corresponding to the mode digit at placeholder (gamma rate)
 * Epsilon rate is complement of this, script also returns the product of the two values
 */

#include <iostream> 
#include <fstream> 
#include <cmath>
#include <vector>
#include <string>

using namespace std;

vector<int> find_gamma(vector<int> total, int sample_num)
{
    // Returns mode digits based on total of each placeholder
    vector<int> gamma(total.size(), 0); // Vector of zeros
    for (int i = 0; i < total.size(); i++)
        if (2 * total[i] > sample_num){
            gamma[i] = 1;
        }
    return gamma;
}

vector<int> find_epsilon(vector<int> gamma)
{
    // Returns complementary binary number
    vector<int> epsilon(gamma.size(), 0); // Vector of zeros
    for (int i = 0; i < gamma.size(); i++){
        epsilon[i] = 1 - gamma[i];
    }
    return epsilon;
}

int bin_to_dec(vector<int> number)
{
    // Converts binary array to decminal number
    int decimal = 0;
    for (int i = 0; i < number.size(); i++){
        decimal += number[i] * pow(2, number.size() - i - 1);
        }
    return decimal;
}


int main()
{
    int index = 0;
    std::vector<int> total(12, 0); // Running total of binary values

    ifstream MyReadFile("Week_1/Inputs/day_3_input.txt"); // Read from the text file
    string line; // Create a text string, which is used to output the text file

    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, line)) {
        for (int i = 0; i < line.size(); ++i){
            char digit = line[i];
            int value = stoi(string(1, digit));
            total[i] = total[i] + value;

        }
        index ++;
    }

    MyReadFile.close(); // Close the file

    std::vector<int> gamma = find_gamma(total, index);
    cout << "gamma = {";
    for (int n : gamma) {
        std::cout << n << ", ";
    }
    std::cout << "}; \n";
    cout << "In decmical form, gamma = " << bin_to_dec(gamma) << endl;
    int product = bin_to_dec(gamma) * bin_to_dec(find_epsilon(gamma));
    cout << "In decmical form, product = " << product << endl;

    return 0;
}


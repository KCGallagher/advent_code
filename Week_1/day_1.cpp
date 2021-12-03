/* 
 * Takes an input txt files with sequential numerical values on separate lines
 * Returns the number of values that are numerically greater than the previous value
 * Build in VS code under terminal menu, then open a new bash cript to run executable generated
 */

#include <iostream> 
#include <fstream> 
using namespace std;

int increase_counter(int num_array[], int len_array)
{
    int count = 0;
    for (int x=1; x<len_array; x++){
        if (num_array[x] > num_array[x-1]){
            count ++;
        }
    }
    return count;
}

int main()
{
    int index = 0;
    int DATA_SIZE = 2000;
    int array[DATA_SIZE]; // Create array of known size to store numbers read into code

    ifstream MyReadFile("Week_1/day_1_input.txt"); // Read from the text file
    string line; // Create a text string, which is used to output the text file

    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, line)) {
        array[index] = std::stoi(line); //  Convert string to int to store in array
        index ++;
    }

    MyReadFile.close(); // Close the file

    int count = increase_counter(array, DATA_SIZE);
    cout << "Number of Increases: " << count << endl;
    return 0;
}


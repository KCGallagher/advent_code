/* 
 * Takes an input txt files with sequential numerical values on separate lines
 * Returns the number of values that are numerically greater than the previous value
 * Build in VS code under terminal menu, then open a new bash cript to run executable generated
 */

#include <iostream> 
#include <fstream> 
using namespace std;

int increase_counter(int array[], int len_array)
{
    // Counts number of elements in array that are greater than previous element
    int count = 0;
    for (int x=1; x<len_array; x++){
        if (array[x] > array[x-1]){
            count ++;
        }
    }
    return count;
}

int increase_counter_window(int array[], int len_array)
{
    // Counts number of windows (width 3) in array that are greater than prev window
    int count = 0;
    for (int x=3; x<len_array; x++){
        int prev_window = array[x-3] + array[x-2] + array[x-1];
        int curr_window = array[x-2] + array[x-1] + array[x];
        if (curr_window > prev_window){
            count ++;
        }
    }
    return count;
}

int main()
{
    int index = 0;
    int DATA_SIZE = 2000;
    int num_array[DATA_SIZE]; // Create array of known size to store numbers read into code

    ifstream MyReadFile("Week_1/Inputs/day_1_input.txt"); // Read from the text file
    string line; // Create a text string, which is used to output the text file

    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, line)) {
        num_array[index] = std::stoi(line); //  Convert string to int to store in array
        index ++;
    }

    MyReadFile.close(); // Close the file

    int count = increase_counter(num_array, DATA_SIZE);
    cout << "Number of Increases: " << count << endl;

    int window_count = increase_counter_window(num_array, DATA_SIZE);
    cout << "Number of Increases (Window): " << window_count << endl;
    return 0;
}


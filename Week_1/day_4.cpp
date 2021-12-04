/* 
 * Takes an input txt files with multiple bingo tables, and a separate call sheet
 * Finds the winning table, and returns product of final call, and sun of remaining numbers
 */

#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int call_bingo(int bool_tables [5][5][100], int table_num)
{
    // Return completed table number, else return -1 for no completed tables
    // Not ideal that table num must be specified above, but can't find workaround
    int bingo_table = -1;
    for (int n=0; n < table_num; n++){
        for (int i=0; i<5; i++){
            int row_tot = 0;
            int col_tot = 0;
            for (int j=0; j<5; j++){
                row_tot += bool_tables[i][j][n];
                col_tot += bool_tables[j][i][n];
            }
            if (row_tot == 5 || col_tot == 5){
                std::cout << "Congrats: We have bingo! \n";
                bingo_table = n;
                break;
            }
        }
    }
    return bingo_table;
}

int main()
{
    // IMPORT ALL TABLES INTO ONE BIG ARRAY
    int index = 0;
    int TABLE_NUMBER = 100;
    int tables [5][5][TABLE_NUMBER];
    int bool_tables [5][5][100] = {0};  // Record when each number is called

    ifstream MyReadFile("Week_1/day_4_input_squares.txt"); // Read from the text file
    string line; // Create a text string, which is used to output the text file
    int line_number = 1;

    while (getline (MyReadFile, line)) {
        string buf;                 // Have a buffer string
        stringstream ss(line);       // Insert the string into a stream
        std::vector<std::string> tokens; // Create vector to hold our words
        while (ss >> buf)
            tokens.push_back(buf);

        int table_num = line_number / 6;
        int row_num = (line_number % 6) - 1;
        for (int i=0; i < tokens.size(); i++){
            int value = stoi(tokens[i]);
            tables[row_num][i][table_num] = value;
        }
        line_number ++;
    }

    MyReadFile.close(); // Close the file

    // IMPORT ALL BINGO CALLS INTO ONE LONG VECTOR
    ifstream MyCallFile("Week_1/day_4_input.txt"); // Read from the text file
    string call_data; // Create a text string, which is used to output the text file
    vector<int> calls;

    while (getline (MyCallFile, call_data)) {
        stringstream ss(call_data);
        while(ss.good()){
            string substr;
            getline(ss, substr, ',');
            calls.push_back(stoi(substr));
        }
    }

    MyCallFile.close(); // Close the file

    // ITERATE OVER CALLS UNTIL FOUND THE WINNER
    int winning_table = -1;
    int number = 0; // Most recent call
    for (int call = 0; call < calls.size(); call++){
        number = calls[call];
        // Search for number in tables
        for (int n = 0; n < TABLE_NUMBER; n++){
            for (int i=0; i<5; i++){
                for (int j=0; j<5; j++){
                    if (tables[i][j][n] == number){
                        bool_tables[i][j][n] = 1;
                    }
                }
            }
        }
        winning_table = call_bingo(bool_tables, TABLE_NUMBER);
        if(winning_table != -1){
            break;
        }
    }

    // Determine final score from winning table
    int uncalled_scores = 0;
    for (int i=0; i<5; i++){
        for (int j=0; j<5; j++){
            if (bool_tables[i][j][winning_table] == 0){
                uncalled_scores += tables[i][j][winning_table];
            }
        }
    }
    std::cout << "Winning table is Number " << winning_table << endl;
    std::cout << "Final score is: " << uncalled_scores * number << endl;


    // FINAL PRINT STATEMENTS TO TEST INPUT/OUTPUT
    bool test_outputs = false;
    if (test_outputs){
        cout << "Calls = {";
        for (int n : calls) {
            std::cout << n << ", ";
        }
        std::cout << "}; \n";

        // Print final table
        cout << "final table = {" << endl;
        for (int i = 0; i<5; i++){
            for (int j = 0; j<5; j++){
                std::cout << tables[i][j][TABLE_NUMBER-1] << ", ";
            }
            std::cout << "\n";
        }
        std::cout << "}; \n";
    }

    return 0;
}


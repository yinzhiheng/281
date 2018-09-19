// EECS 281, Project 0
// Learn about:
//   1) Command-line processing with getopt_long()
//   2) The vector member functions .resize() and .reserve()
//   3) The proper way to read data until end-of-file


#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


// Print help for the user when requested.
// argv[0] is the name of the currently executing program
void printHelp(char *argv[]) {
    cout << "Usage: " << argv[0] << " [-m resize|reserve|nosize]|-h" << endl;
    cout << "This program is to help you learn command-line processing," << endl;
    cout << "reading data into a vector, the difference between resize and reserve," << endl;
    cout << "and how to properly read until end-of-file." << endl;
} // printHelp()


// TODO: Finish this function, look for the individual TODO comments internally.
// Process the command line; the only thing we need to return is the mode
// when the user specifies the -m/--mode option.
string getMode(int argc, char * argv[]) {
    bool modeSpecified = false;
    string mode;

    // These are used with getopt_long()
    opterr = true; // Give us help with errors
    int choice;
    int option_index = 0;
    option long_options[] = {
        // TODO: Fill in two lines, for the "mode" ('m') and
        // the "help" ('h') options.
		{"mode", required_argument, nullptr, 'm'},
	    {"help",no_argument, nullptr, 'h'},
        { nullptr, 0,                 nullptr, '\0' }
    };

    // TODO: Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "m:h", long_options, &option_index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(argv);
            exit(0);

        case 'm':
            mode = optarg;
            if (mode != "resize" && mode != "reserve" && mode != "nosize") {
                cerr << "Error: invalid mode " << mode << endl;
                exit(1);
            } // if
            modeSpecified = true;
            break;

        default:
            cerr << "Error: invalid option" << endl;
            exit(1);
        } // switch
    } // while

    if (!modeSpecified) {
        cerr << "Error: no mode specified" << endl;
        exit(1);
    } // if

    return mode;
} // getMode()


// TODO: Write this function.  It should do the following:
//   1) Read in the size of the data to be read (use type size_t)
//   2) Use data.resize() to set the current size of the vector
//   3) Use a for loop to read in the specified number of values,
//      storing them into the vector using a subscript
void readWithResize(vector<double> &data) {
	size_t current_size;
	current_size = data.size();
	cout << "The current size is: " << current_size << endl;
	cout << "The elements of the vector are: " << endl;
	for (int i = 0; i < current_size; i++)
	{
		cout << data[i] << endl;
	}
	cout << endl;
	data.resize(10);
	cout << "The current size is: " << current_size << endl;
	cout << "The elements of the vector are: " << endl;
	for (int i = 0; i < current_size; i++)
	{
		cout << data[i] << endl;
	}

} // readWithResize()


// TODO: Write this function.  It should do the following:
//   1) Read in the size of the data to be read (use type size_t)
//   2) Use data.reserve() to set the maximum size of the vector
//   3) Use a for loop to read in the specified number of values,
//      storing them into the vector using data.push_back()
void readWithReserve(vector<double> &data) {
	size_t current_size;
	current_size = data.size();
	cout << "The current size is: " << current_size << endl;
	cout << "The elements of the vector are: " << endl;
	for (int i = 0; i < current_size; i++)
	{
		cout << data[i] << endl;
	}
	cout << endl;
	data.reserve(10);
	cout << "The current size is: " << current_size << endl;
	cout << "The elements of the vector are: " << endl;
	for (int i = 0; i < current_size; i++)
	{
		cout << data[i] << endl;
	}

} // readWithReserve()


// TODO: Write this function.  It should do the following:
//   Use a while loop to read in the values, storing them
//   into the vector using data.push_back()
void readWithNoSize(vector<double> &data) {
    // TODO: DELETE the following line of code when you write
    // this function!  It is only here so that the file compiles.
    cout << data.size() << endl;

} // readWithNoSize()


// This function is already done.
double getAverage(const vector<double> &data) {
    double average, sum = 0;

    for (size_t i = 0; i < data.size(); ++i)
        sum += data[i];

    average = sum / static_cast<double>(data.size());
    return average;
} // getAverage()


// This function is already done.  Note that we have to sort the vector
// before we can find the median.
double getMedian(vector<double> &data) {
    // Sort the data
    sort(data.begin(), data.end());

    // Figure out if vector size is even or odd
    if (data.size() % 2 == 1)
        return data[data.size() / 2];

    // Even size, average 2 middle values
    auto mid = data.size() / 2;
    double v1 = data[mid];
    double v2 = data[mid - 1];
    return (v1 + v2) / 2;
} // getMedian()


// This function is already done.
int main(int argc, char *argv[]) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);

    string mode;
    vector<double> data;

    // Set two digits of precision on output
    cout << fixed << showpoint << setprecision(2);

    mode = getMode(argc, argv);

    if (mode == "resize")
        readWithResize(data);
    else if (mode == "reserve")
        readWithReserve(data);
    else if (mode == "nosize")
        readWithNoSize(data);
    else {
        cerr << "Invalid mode \"" << mode << "\" specified, should not be possible to reach this point!" << endl;
        return 1;
    } // if

    if (data.size() == 0)
        cout << "No data => no statistics!" << '\n';
    else {
        cout << "Average: " << getAverage(data) << '\n';
        cout << " Median: " << getMedian(data)  << '\n';
    } // else

    return 0;
} // main()

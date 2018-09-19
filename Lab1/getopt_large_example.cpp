// This is an EXAMPLE! You (the student) don't have to write any code!

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <getopt.h>

// PROBLEM DESCRIPTION
// ===================
// A sequence of numbers of the form (a + b*i) for constants 'a' and 'b' is
// called an arithmetic progression.
//
// We can specify a (finite) arithmetic progression by specifying its start, its
// end, and its 'step', or the difference between consecutive terms.
//
// For example, the sequence [3, 13, 23, 43] has a start of 3, an end of 43, and
// a step of 10. Note that the step may be negative, such as in [25, 20, 15, 10]
// but we require that the sequence contain at least two elements, and that they
// are not equal (so that the step value is well-defined).
//
// YOUR PROGRAM will receive AS COMMAND LINE ARGUMENTS a description of an
//  arithmetic progression, along with a 'power'. You'll raise each term in the
// sequence to that power and compute its sum. If the power isn't specified, you
// should assume that it should be 1.
//
// The solution is not designed primarily for efficiency. It is written for both
// CORRECTNESS and ERROR CHECKING. It will detect all kinds of invalid inputs
// WITHOUT CRASHING or producing garbage answers. It will instead detect the
// issue and print a relevant error message.
//
// You should consider using this code (or code like it) as a sort of template
// for your projects, although you don't have to. There are several errors that
// are checked in this file that you don't have to check in the projects!

// INPUT SPECIFICATION
// ===================
// Your program should support the following command-line arguments:
// --begin [n] or -b [n]
//         Specifies the beginning of the sequence. 'n' should be an integer.
// --end [n] or -e [n]
//         Specifies the end of the sequence. 'n' should be an integer.
// --difference [n] or -d [n]
//         Specifies the difference between terms of the arithmetic sequence.
//         'n' should be a nonzero integer.
// --power [n] or -p [n]
//         Specifies the "power" to raise the terms in the sequence. 'n' should
//         be a positive integer.
// --square or -s
//         Exists as a shorthand for --power 2 or -p 2. It should be treated
//         identically as the corresponding power flag by your program.
// --cube or -c
//         Exists as a shorthand for --power 3 or -p 3. It should be treated
//         identically as the corresponding power flag by your program.
// --help or -h
//         Prints this input specification.

// No flag should be specified more than once (or in the case of power, square,
// and cube, no two of the three should be specified).
// --begin and --end should both be specified.
// --begin and --end should have different numbers as arguments.
// --difference is optional (it does not have to be present) but if it is, its
// value should have the correct 'sign' (it should be positive if end > begin
// and negative otherwise), and should have a value which divides their
// difference. For example, -b 10 -e 20 -d 3 is invalid since (20-10) is not
// divisible by 3, and -b 20 -e 10 -d 5 is invalid since 5 is positive.

// OUTPUT SPECIFICATION
// ====================
// If any error occurs, output "invalid" followed by a newline to std::cout and
// (optionally) an error message describing the problem to std::cerr, and main()
// should return 1.
// Otherwise, print out "the sum is [s]" followed by a newline to std::cout, and
// main() should return 0.
// For example, the error output could be written in C++ as
//     std::cout << "invalid" << std::endl;
//     std::cerr << "the --begin flag was specified twice" << std::endl;
//     return 1; // in main()
// And the success case can be written as
//     std::cout << "the sum is " << sum << std::endl; // sum is a number
//     return 0; // in main()

// ERROR CHECKING SPECIFICATION
// ============================
// In the input specification, the word "should" indicates what the USER should
// do. If your program receives input that the user "should" not have provided,
// then the program must detect this and print an error message. In particular,
// it may NOT assume that arguments -b, -e, -s, -d are actually followed by
// something, nor may it assume that they are valid integers.

// If an invalid (i.e. unrecognized) option is entered, the program should also
// consider this a user error.

// It MAY assume that all calculations can be done with 'int'. (Overflow is not
// checked).

// BELOW THIS POINT IS THE INSTRUCTOR SOLUTION

// helper function
int raise_power(int value, int power) {
    assert(power >= 1); // should NEVER fail (protect against bad user input!)
    int result = 1;
    for (int i = 0; i < power; i++) {
        result *= value;
    }
    return result;
}

// helper function
int sequence_sum(int begin, int end, int difference, int power) {
    assert(begin != end); // should NEVER fail (protect against bad user input!)
    int sum = 0;
    for (int i = begin; i != end; i += difference) {
        sum += raise_power(i, power);
    }
    sum += raise_power(end, power);
    return sum;
}


// The long_options variable contains a description of the options.
static option long_options[] = {
    // LONG        ARGUMENT USED?     (ignore) RETURN VALUE
    {"begin",      required_argument, nullptr, 'b'},
    {"end",        required_argument, nullptr, 'e'},
    {"difference", required_argument, nullptr, 'd'},
    {"power",      required_argument, nullptr, 'p'},
    {"square",     no_argument,       nullptr, 's'},
    {"cube",       no_argument,       nullptr, 'c'},
    {"help",       no_argument,       nullptr, 'h'},
    // this one tells getopt_long that there are no more options:
    {nullptr,      0,                 nullptr,  0}
};

// When a flag is given an argument, it is stored in a global variable with type
// char* that is called "optarg".

// This helper function tries to interpret the optarg value as an integer and
// returns it. If it cannot be interpreted as an int (for example, if it is
// missing or otherwise incorrect) then it will throw an exception.

// Note: exceptions are a mechanism for error-handling provided by C++.
// They cause the stack to be "unwound", backing up until it can find a point
// that the exception can be "caught" after being thrown.

// For example:
//////////////////////////////////////////////////////////////////////////////////////////
//  void print_sqrt(double number) {                                                    //
//      if (number < 0) {                                                               //
//          throw "sqrt of negative"; // throw an exception of type const char*         //
//      }                                                                               //
//      std::cout << "the sqrt of " << number << " is " << sqrt(number) << std::endl;   //
//  }                                                                                   //
//  try {                                                                               //
//      // exceptions can be thrown in here                                             //
//      std::cout << "Hello!" << std::endl;                                             //
//      print_sqrt(-1.0);                                                               //
//      // the exception will stop code from ever getting here!                         //
//      std::cout << "Goodbye!" << std::endl;                                           //
//  } catch (const char* msg) { // catch an exception of type std::string               //
//      // this part only runs if the exception happens!                                //
//      std::cout << "An error happened!" << std::endl;                                 //
//  }                                                                                   //
//  std::cout << "Done." << std::endl;                                                  //
//////////////////////////////////////////////////////////////////////////////////////////
int get_optarg_argument_as_int() {
    if (optarg == nullptr) {
        throw "required argument is missing";
    }
    std::string str(optarg); // convert from const char* to std::string.

    try {
        return stoi(str); // stoi throws if it's not an integer
    } catch (...) {
        // catch the exception;
        // but now re-throw a new exception of type const char*
        throw "argument is not an int";
    }
}

// an alternative implementation that uses stringstreams
int get_optarg_argument_as_int_alternate() {
    if (optarg == nullptr) {
        throw "required argument is missing";
    }
    std::stringstream stream(optarg); // put it into a stream
    int n;
    stream >> n; // extract into n (removes leading whitespace; but there won't be any)
    if (!stream || !stream.eof()) {
        // !stream means that it failed (e.g. in "blah" or "blah3" extracting into int)
        // !stream.eof() means there's input left over (for example, in "123apple") NOTE: stoi doesn't actually check this
        throw "invalid int argument";
    }
    // n was successfully extracted, so it is safe to return
    return n;
}

int main(int argc, char** argv) {
    try { // Almost all of main() is inside this try{} block!

        // begin, end, diff, power are the values of the parameters.
        int begin = 0;
        int end = 0;
        int diff = 0;
        int power = 1;
        // we remember whether we've already set them in these variables:
        bool begin_was_set = false;
        bool end_was_set = false;
        bool diff_was_set = false;
        bool power_was_set = false;

        // 'choice' is the value returned by getopt_long.
        int choice = 0;
        while ((choice = getopt_long(argc, argv, "b:e:d:p:sch", long_options, nullptr)) != -1) {
            // "b:e:d:p:sc" is the short-form description.
            // "b:" indicates that 'b' is a short flag which has a required argument.
            // NOTE: the above condition assigns choice in addition to checking its value against -1.
            // When there are no more arguments, getopt_long returns -1.
            // NOTE: this code runs in the order that the user passed the arguments,
            // not the order that they're listed in the source.
            switch (choice) {
            case 'b': // begin
                if (begin_was_set) {
                    throw "begin already set";
                }
                begin = get_optarg_argument_as_int(); // load from argument
                begin_was_set = true; // remember that we've set it
                break;
            case 'e':
                if (end_was_set) {
                    throw "end already set";
                }
                end = get_optarg_argument_as_int(); // load from argument
                end_was_set = true; // remember that we've set it
                break;
            case 'd':
                if (diff_was_set) {
                    throw "diff already set";
                }
                diff = get_optarg_argument_as_int(); // load from argument
                diff_was_set = true; // remember that we've set it
                if (diff == 0) {
                    throw "diff cannot be 0";
                }
                break;
            case 'p':
                if (power_was_set) {
                    throw "power cannot be specified twice";
                }
                power = get_optarg_argument_as_int(); // load from argument
                power_was_set = true; // remember that we've set it
                break;
            case 's':
                if (power_was_set) {
                    throw "power cannot be specified twice";
                }
                power = 2;
                power_was_set = true; // remember that we've set it
                break;
            case 'c':
                if (power_was_set) {
                    throw "power cannot be specified twice";
                }
                power = 3;
                power_was_set = true; // remember that we've set it
                break;
            case 'h':
                std::cout <<
                " --begin [n] or -b [n]\n"
                "         Specifies the beginning of the sequence. 'n' should be an integer.\n"
                " --end [n] or -e [n]\n"
                "         Specifies the end of the sequence. 'n' should be an integer.\n"
                " --difference [n] or -d [n]\n"
                "         Specifies the difference between terms of the arithmetic sequence.\n"
                "         'n' should be a nonzero integer.\n"
                " --power [n] or -p [n]\n"
                "         Specifies the 'power' to raise the terms in the sequence. 'n' should\n"
                "         be a positive integer.\n"
                " --square or -s\n"
                "         Exists as a shorthand for --power 2 or -p 2. It should be treated\n"
                "         identically as the corresponding power flag by your program.\n"
                " --cube or -c\n"
                "         Exists as a shorthand for --power 3 or -p 3. It should be treated\n"
                "         identically as the corresponding power flag by your program.\n"
                " --help or -h\n"
                "         Prints this input specification.\n";
                return 0; // return from main with success
            default:
                // unrecognized option
                throw "unrecognized option";
            }
        }

        if (!begin_was_set) {
            throw "no begin set";
        }

        if (!end_was_set) {
            throw "no end set";
        }

        if (begin == end) {
            throw "start == end";
        }

        if (!diff_was_set) {
            // here, we assign a default value to diff
            diff = end > begin ? 1 : -1;
        }

        if (diff == 0) {
            throw "diff cannot be zero";
        }

        if ((end - begin > 0) != (diff > 0)) {
            throw "diff is in the wrong direction";
        }

        if ((end - begin) % diff != 0) {
            throw "the provided difference is invalid for begin/end";
        }

        // all error checking has passed!

        // compute the actual sum:
        std::cout << "the sum is " << sequence_sum(begin, end, diff, power);
        return 0; // returning 0 from main indicates that the program completed successfully
    } catch (const char* err) {
        // some error happened:
        std::cerr << err << std::endl; // print the error message to std::cerr
        std::cout << "invalid" << std::endl; // print 'invalid' to std::cout
        return 1; // returning 1 from main indicates that an error occurred
    }
}



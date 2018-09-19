#include <iostream>
#include <getopt.h>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include<sstream>

using namespace std;

struct D_word
{
	string name;
	bool was_searched;
	string prev_word;
};

bool is_word_alike(D_word& target_w, D_word* & cand_w, string method_switch_c, string method_switch_l, string method_switch_p);
//bool is_search_over(string end_word, const deque<D_word*>& candidate, const D_word& Cur_word, string &suc_fail);

int main(int argc, char** argv)
{
	int option_index = 0, option = 0;
	int num_word;
	string routing_scheme, output_mode, method_switch_c, method_switch_p, method_switch_l, type_D;
	string line,the_first_word;
	istringstream isstring;
	vector<D_word*> Dictionary;
	vector<D_word*>::iterator Dictionary_iter;
	D_word begin_word, end_word;

	begin_word.name = "not_initialized";
	end_word.name = "not_initialized";
	// use getopt to find command line options
	struct option longOpts[] = { { "stack", no_argument, nullptr, 's' },
								 {"queue",no_argument, nullptr, 'q'},
								 {"change",no_argument, nullptr, 'c'},
								 {"swap",no_argument, nullptr, 'p'},
								 { "length", no_argument, nullptr, 'l' },
								 {"output", required_argument, nullptr,'o'},
								 {"begin", required_argument, nullptr,'b'},
								 {"end", required_argument, nullptr,'e'},
								 {"help", no_argument, nullptr, 'h'},
								 { nullptr, 0, nullptr, '\0' }
	};

	try
	{
		while ((option = getopt_long(argc, argv, "sqcplo:b:e:h", longOpts, &option_index)) != -1) {
			switch (option) {
			case 's':
				if (routing_scheme == "queue")
					throw "More than one --stack\ --queue one command line.\n";
				else
				    routing_scheme = "stack";
				break;
			case 'q':
				if (routing_scheme == "stack")
					throw "More than one --stack\ --queue one command line.\n";
				else
					routing_scheme = "queue";
				break;
			case 'c':
				method_switch_c = "change";
				break;
			case 'p':
				method_switch_p = "swap";
				break;
			case 'l':
				method_switch_l = "length";
				break;
			case 'o':
				if (optarg == "M")
					output_mode = "modification";
				else if (optarg == "W")
					output_mode = "word";
				else
				{
					throw "No output_mode is input from the command lines.\n";
				}
			case 'h':
				std::cout << "This program reads a CSV file that contains song names,\n";
				exit(0);
			}
		}
		/*Now we read all the commands from the command lines*/
		/*Let's check for the error*/
		/* 1. More (See the previous) or less than one --stack/-s or --queue/-q on the command line.*/
		if (routing_scheme != "stack" && routing_scheme != "queue")
			throw "Less than one --stack\ --queue one command line.\n";

		/* 2. No --change/-c, --length/-l, or --swap/-p on the command line.*/
		if (method_switch_c != "change" && method_switch_l != "length" && method_switch_p != "swap")
			throw "No --change/-c, --length/-l, or --swap/-p on the command line.\n";
		
		/* 3. The --output/-o flag is followed by an invalid character.*/
		if (output_mode != "word" && output_mode != "modification")
			throw "The --output/-o flag is followed by an invalid character.\n";

		/* 4.  Either the start or end word is not specified, or does not exist in the dictionary.(later)*/
		if (begin_word.name == "not_initialized" || end_word.name == "not_initialized")
			throw "Either the start or end word is not specified.\n";

		/* 5. The --change/-c and/or --swap/-p flags are specified, but --length/-l is not, and the start/end words do not match in length. (later)*/
	}
	catch (string err)
	{
		cerr << err;
		exit(1);
	}


	/* Read 1st line */
	getline(cin, line);
	if (line == "C")
		type_D = "complex";
	else if (line == "S")
		type_D = "simple";
	/*Read 2nd line*/
	getline(cin, line);
	isstring.str(line);
	isstring >> num_word;
	isstring.clear();
	isstring.str("");
	Dictionary.reserve(num_word);

	/*Let's initialize the dictionary*/
	if (type_D == "simple")
	{
		while (getline(cin,line))
		{
			isstring.str(line);
			isstring >> the_first_word;
			isstring.clear();
			isstring.str("");
			if (the_first_word == "//")
				continue;
			else
			{
				D_word *candi = new D_word;
				candi->name = the_first_word;
				candi->prev_word = "not_in_trace";
				candi->was_searched = false;
				Dictionary.push_back(candi);
			}
		}
	}
	else if (type_D == "complex")
	{
		Dictionary.reserve(2 * num_word);
		while (getline(cin, line))
		{
			isstring.str(line);
			isstring >> the_first_word;
			isstring.clear();
			isstring.str("");
			if (the_first_word == "//")
				continue;
			else
			{
				// TO DO
			}
		}
	}

	/* Error Checking */
   /* begin_word, end_word is not specified in the dictionary*/
	bool is_beginw_find = false, is_endw_find = false;
	D_word* begin_ptr, *end_ptr;

	for (size_t i = 0; i < Dictionary.size(); i++)
	{
		if (Dictionary[i]->name == begin_word.name)
		{
			is_beginw_find = true;
			Dictionary[i]->was_searched = true;
			begin_ptr = Dictionary[i];
		}
		if (Dictionary[i]->name == end_word.name)
		{
			is_endw_find = true;
			Dictionary[i]->was_searched = true;
			end_ptr = Dictionary[i];
		}
		if (is_beginw_find == true && is_endw_find == true)
			break;
	}
	if (is_beginw_find == false || is_endw_find == false)
	{
		cerr << "begin_word or end_word is not specified in the dictionary.\n";
		exit(1);
	}

	deque<D_word> Candidate, Candidate2;
	D_word Cur_word;
	Cur_word = begin_word;

	bool is_fail = false, is_success = false;
	int count_word = 0;

	if (output_mode == "word")
	{
		while (is_fail == true || is_success == true)
		{
			count_word++;
			/* Let's first find all the words alike the Cur_word*/
			for (size_t i = 0; i < Dictionary.size(); i++)
			{
				if (is_word_alike(Cur_word, Dictionary[i], method_switch_c, method_switch_l, method_switch_p) == true)
				{
					D_word temp;
					Dictionary[i]->prev_word = Cur_word.name;
					Dictionary[i]->was_searched = true;
					temp = *Dictionary[i];
					Candidate.push_back(temp);
					Candidate2.push_back(temp); // Save time, when tracing back
				}
			}
			/* Let's Check whether it's over. if not, pop_back, and turn such word into Cur_word*/
			/* We cannot find a solution*/
			if (Candidate.size() == 0)
			{
				is_fail = true;
				break;
			}

			/* We successfully find the target word*/
			for (size_t i = 0; i < Candidate.size(); i++)
			{
				if (Candidate[i].name == end_word.name)
				{
					end_ptr->prev_word = Candidate[i].name;
					is_success = true;
					break;
				}
			}

			/* If not over, back to loop*/
			if (is_fail == false && is_success == false)
			{
				if (routing_scheme == "stack")
				{
					Cur_word = Candidate.back();
					Candidate.pop_back();
				}
				else if (routing_scheme == "queue")
				{
					Cur_word = Candidate.front();
					Candidate.pop_front();
				}
			}
		}
		/*If over, tracback and print*/
		if (is_fail = true)
		{
			cout << "No solution, " << count_word << " words checked.\n";
		}
		else if (is_success == true)
		{
			cout << "Words in morph: " << count_word << "\n";
			while (begin_ptr->name != end_word.name)
			{
				cout << begin_ptr->name << "\n";
				for (size_t i = 0; i < Candidate2.size(); i++)
				{
					if (Candidate2[i].prev_word == begin_ptr->name)
					{
						begin_ptr = &Candidate2[i];
					}
				}
			}
			cout << end_word.name << "\n";
		}
		
		/*Delete the newed things*/
		for (Dictionary_iter = Dictionary.begin(); Dictionary_iter != Dictionary.end(); Dictionary_iter++)
		{
			delete *Dictionary_iter;
		}
		exit(0);
	}
	else if (output_mode == "modification")
	{
		/*TO DO*/
	}



}


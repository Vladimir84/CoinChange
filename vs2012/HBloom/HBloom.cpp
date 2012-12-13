#include <cstddef>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <stdexcept>


std::vector<std::vector<int> > work(const std::vector<int>& denominations, std::size_t total_sum=100){
// Returns table of all valid combinations 
	int ncurrency_types = denominations.size();
	std::vector < std::vector <std::vector<int> > > table(total_sum+1); // if Value > max.size() -> exception
	if (ncurrency_types>0){
		std::vector<int> zer0_record(ncurrency_types,0);
		table[0].push_back(zer0_record);
		for(int coin_id=0; coin_id<ncurrency_types; ++coin_id)
			for(std::size_t j=denominations[coin_id];j<=total_sum;++j){
				auto entry(table[j-denominations[coin_id]]);
				if (entry.empty()) continue;
				std::for_each(entry.begin(), entry.end(), [&](std::vector<int>& combination){
					auto tmp(combination);
					tmp[coin_id]+=1;
					table[j].push_back(tmp);
				});
			}
	}
	auto combinations=table[total_sum];
	return combinations;
}


void print_combinations(const std::vector<std::string>& denominations, const std::vector<std::vector<int>>& combinations){
	using namespace std;
	
	// Determine max field width 
	int max_l=-1;
	for (auto &s:denominations){
		int sz=s.size();
		if (sz>max_l) max_l=sz;
	}
	int field_width=max_l+2;

	// Print out currency names
	for (size_t i=0;i<denominations.size();i++){
		cout<<setw(field_width)<<denominations[i];
	}
	cout<<endl;

	std::for_each(combinations.begin(), combinations.end(), [=](vector<int> combination){
		for (size_t i = 0; i < combination.size(); ++i){
			cout<<setw(field_width)<< to_string(combination[i]);
		}
		cout<<"\n";
	});
	cout<<"\nCount: "<<combinations.size()<<endl;
}


bool isDouble(std::string argument, double &number){
// Check if string represents a number
// e.g. for strings "10","+10","10.0","1E+1 " result is <true>
// for "abc","12abc", "ab12" result is <false>
	char * endptr;
	number = strtod(argument.c_str(), &endptr);
	bool _isdouble(false);
	if (errno==0){
		if (*endptr=='\0') _isdouble=true; 
	} 
	return _isdouble;
}


void isValidCoinValue(const double number) throw (const char *){
// Check if number can be converted to a valid coind value
	if (number<0) throw "There a coin with negative value";
	if (number==0) throw "There a coin with zero value";
	if (number<1) throw "There a coin with value greater then target sum";
}


std::size_t processInput(std::vector<std::string> arguments,std::vector<std::string>& coins, std::vector<double>& coins_values){
	if (arguments.size() % 2 != 0) throw "Incorrect argument. Perhaps coin name or its value is missing?";
	double target_sum=-1;

	std::vector<std::string> _coinsLocal;
	std::vector<double> _coins_valuesLocal;

	double coin_value;
	for (auto arg=arguments.begin();arg!=arguments.end();){
		if (isDouble(*arg, coin_value)) throw "Expected a coin name, found a number";
		_coinsLocal.push_back(*(arg++));
		if (!isDouble(*(arg++),coin_value)) throw "Expected a number, found a string. Maybe some coin has no value specified";
		try  {
			isValidCoinValue(coin_value);
		}catch (const char *) { throw;}
		_coins_valuesLocal.push_back(coin_value);
		if (coin_value>target_sum) target_sum=coin_value;
	}

	// Pospone modification of input until after possible exceptions
	coins.assign(_coinsLocal.begin(), _coinsLocal.end());
	coins_values.assign(_coins_valuesLocal.begin(), _coins_valuesLocal.end());
	return static_cast<std::size_t>(target_sum);
}


std::vector<std::string> TokenizeInputArguments(const std::string& input, char delim){
// Split string into non-empty comma-separated words
    std::stringstream inputss(input);
    std::vector<std::string> arguments;
	std::string token;

    while(std::getline(inputss, token, delim)) {
		if (!token.empty())
			arguments.push_back(token);
    }
    return arguments;
}


int main(int argc, char* argv[])
{
	std::vector<int> values;
	std::vector<std::string> denominations;
	int target_sum;
	// Set up parameters
	if (argc<2){
		// Default case, the number of ways pennies, nickels, dimes, and quarters
		// can be combined to sum exactly $1

		int _dnmns[] = {25,10,5,1};
		std::vector<int> _dz (_dnmns, _dnmns + sizeof(_dnmns) / sizeof(int) );
		values.assign(_dz.begin(),_dz.end());

		const char *args[] = {"Quarter", "Dime", "Nickel", "Penny"};
		std::vector<std::string> coins(args, std::end(args));
		denominations.assign(coins.begin(), coins.end());

		target_sum=100;
	} else {
		// Get command line parameter in the following format:
		// "Quarter,4,Dime,10,Nickel,20,Penny,100"
		// The argument is a comma delimited list which specifies the name of each
		// denomination along with the number required of that denomination to
		// reach our target sum

		std::string args(argv[1]);
		if (args.empty()) {
			std::cout<<"Empty argument string\n";
			return 1;
		}
		std::vector<std::string> arguments;
		// List of coma separated words
		arguments=TokenizeInputArguments(args, ',');

		std::vector<double> numbers;
		std::vector<std::string> coins;
		// Get denomination names, corresponding numbers, 
		// and target sum.
		// For "Coin,1.5,Arrowhead,3,Button,150" target sum is 150
		try{
			target_sum = processInput(arguments, coins, numbers);
		}catch (const char* exception) { 
			std::cout << exception << std::endl; 
			return 1;
		}
		// convert the number required of each denomination to
		// reach our target sum to the value. 
		// For "Coin,1.5,Arrowhead,3,Button,150" corresponding values are 100, 50 and 1.
		for (double number : numbers){
			values.push_back(static_cast<int>(target_sum/number));
		}
		denominations.assign(coins.begin(), coins.end());
	}

	// Find all the valid combinations
	std::vector<std::vector<int> > combinations;
	try{ combinations = work(values,target_sum);
	} catch (std::length_error& le) {
		std::cout<<le.what()<<std::endl;
		return 1;
	}
	// Output the combinations and total count
	print_combinations(denominations,combinations);

	return 0;
}
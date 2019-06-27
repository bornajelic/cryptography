// Crypto.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <set>
#include "Frequency.h"



//calculating index of coincidence (probability of two randomly selected letters being equal)
double index_of_Coincidence(std::string coset) {
	
	//std::cout << "calculating IC for - >" << coset << "\n";
	double rez = 0;
	double f = 0;
	double n = (double)coset.length();
	std::set<char> charSet;
	char tmp = coset[0];
	for (int i = 0; i < coset.length(); ++i) {
		

		if (!(charSet.find(coset[i]) != charSet.end())) {
			charSet.insert(coset[i]);
			f = std::count(coset.begin(), coset.end(), coset[i]);
			rez += f*(f - 1);
		}
		else {
			continue;
		}
		
	}

	return (1.0 / (n*(n - 1))) * rez;
}

//splitting string and creating cosets, coset contains every lth letter in cryptogram
std::vector<std::string> split_Function(std::string &s, int l) {
	
	int i = 0;
	int start_pos = 0;
	std::vector<std::string> vec;
	
	while (i < l) {
		std::string tmp = "";
		for (size_t j = start_pos; j < s.length(); j+=l) {
			tmp += s.at(j);
		}
		
		vec.push_back(tmp);
		++start_pos;
		++i;
	}

	return vec;
}
//average value of IC
double calculate_Average(std::vector<std::string> &vec) {
	double average = 0.0;
	for (int i = 0; i < vec.size(); ++i) {
		average += index_of_Coincidence(vec[i]);
	}
	return average/(double)vec.size();
}


//KOLIKI JE FINAL KEY, TOLIKO COSETOVA MORAS IMATI I TE COSETOVE KORISTITI.. NEKI VEC ISKORISTI di ces spremiti sve i uzmi one cosetove za koji je kljuc finalni

//functon creates a map with keySize as KEY, and average value as VALUE
std::map<int, double> createTable(size_t &k1, size_t &k2, std::string &text, size_t &key_Size,std::vector<std::string> &vec) {

	std::map<int, std::vector<std::string>> tmp;
	std::map<int, double> map;
	double average = 0;
	
	while (k1 <= k2) {
		vec.clear();
		vec = split_Function(text, k1); //stored cosets
		tmp.insert({ k1,vec });

		average = calculate_Average(vec);
		map.insert(std::pair<int, double>(k1, average));
		++k1;
	}
	
	using pair_type = decltype(map)::value_type;
	auto max_avg = std::max_element(std::begin(map), std::end(map), [](const pair_type &p1, const pair_type &p2) {
		return p1.second < p2.second;
	});
	
	key_Size = max_avg->first;
	vec.clear();
	for (auto i = tmp.begin(); i != tmp.end(); ++i) {
	
		if (i->first == key_Size) {
			std::cout << i->first << " = " << key_Size << std::endl;
			vec = i->second;
		}
	}

	
	return map;
}
//helper function
double letter_Count(char &c, std::string &text) {
	int count = 0;
	for (int i = 0; i < text.length(); ++i) {
		if (text[i] == c) {
			count++;
		}
	}
	return (double)count;
}

//calculating X^2 (goodnes of fit)
double calculate_Xsquared(std::string &text) { //N I B F O P D V W T Z

	std::map<char, std::pair<double, double>> table; //slovo,frequency in text, frequency in english
	std::map<char, double> map_of_letters_in_text;

	double X = 0.0;
	//prvo count word occurence
	for (std::string::iterator it = text.begin(); it != text.end(); ++it) {

		if (!(map_of_letters_in_text.find(*it) != map_of_letters_in_text.end())) {
			double d = letter_Count(*it, text) / text.size();
			map_of_letters_in_text.insert({ *it,d}); //imam slovo i count,sada ide frekvencija
		}
	}

	for (auto i = map_of_letters_in_text.begin(); i != map_of_letters_in_text.end(); ++i) {
		for (auto j = english_freq.begin(); j != english_freq.end(); ++j) {
			if (i->first == j->first) {
				table.insert({ j->first,{ i->second,j->second.first } });
				break;
			}
		}
	}

	for (auto k = table.begin(); k != table.end(); ++k) {
		X += std::pow(((k->second.first - k->second.second)), 2) / (k->second.second);	
	}
	return X;

}

//function takes a letter "c" as arugment, and finds a letter before "c" in alphabet (circular) 
char index_to_the_left(char c) {

	
	int position;
	for (auto j = english_freq.begin(); j != english_freq.end(); ++j) {
		if (j->first == c) {
			//shift letter
			if (j->second.second == 0) {
				position = 25;
				for (auto i = english_freq.begin(); i != english_freq.end(); ++i) {
					if (i->second.second == 25) {
						c = i->first;
					}
				}
			}
			else {
				auto x = j;
				--x;
				c = x->first;
			}
		}
		else {
			continue;
		}
	}
	return c;
}

std::pair<int, double> minimum_Pair(std::vector<std::pair<int, double>> &table) {
	std::pair<int, double> min = {1000,1000}; //dummy value
	for (const std::pair<int, double> &pair : table) {
		if (min.second > pair.second) {
			min = pair;
		}
	}
	return min;
}

void shift(std::string &text, int alfabethLenght, std::vector<std::pair<int, double>> &table) {
	
	if (alfabethLenght >= 26) {
		return;
	}
	else {
		std::string tmp = "";
		for (std::string::iterator it = text.begin(); it != text.end(); ++it) {
			if (alfabethLenght == 0) {
				tmp = text;
				break;
			}
			else {
				tmp += index_to_the_left(*it);
			}
			
		} //izracunali smo rijec
		table.push_back({ alfabethLenght,calculate_Xsquared(tmp) }); //treba ti samo max element iz table-a  FASWGHUNOLR
		shift(tmp, alfabethLenght+1, table);
	}
	

}



//function generates a keyword, argument of a function is vector of cosets(type string)
std::string getKeyWord(std::vector<std::string> &vec) {
	//krivo tu mapu moram poslati
	std::vector<std::pair<int, double>> end;
	std::vector<std::pair<int,double>> table;

	for (int i = 0; i < vec.size(); ++i) {	
		std::cout << "vec[" << i << "] = " << vec[i] << std::endl;
		shift(vec[i], 0, table);
		end.push_back(minimum_Pair(table));
		table.clear();
	}

	//dohvati iz abecede slovo iz vektora
	std::string result = "";
	for (const std::pair<int, double> &pair : end) {
		for (auto i = english_freq.begin(); i != english_freq.end(); ++i) {
			if (i->second.second == pair.first) {
				result += i->first;
				break;
			}
		}
	}

	return result;
}




 
int main()
{
	unsigned int key_Size;
	size_t k1, k2;
	std::vector<std::string> cosets;

	//std::string text = "VVQGYTVVVKALURWFHQAC MMVLE HUCAT WFHHI PLXHV UWSCI GINCMUHNHQ RMSUI MHWZO DXTNA EKVVQ GYTVV QPHXI NWCAB ASYYM TKSZRCXWRP RFWYH XYGFI PSBWK QAMZY BXJQQ ABJEM TCHQS NAEKV VQGYTVVPCA QPBSL URQUC VMVPQ UTMML VHWDH NFIKJ CPXMY EIOCD TXBJWKQGAN";
	//std::string text = "WQXYM REOBP VWHTH QYEQV EDEXR BGSIZ SILGR TAJFZ OAMAV VXGRFQGKCP IOZIJ BCBLU WYRWS TUGVQ PSUDI UWOES FMTBT ANCYZ TKTYBVFDKD ERSIB JECAQ DWPDE RIEKG PRAQF BGTHQ KVVGR AXAVT HARQE ELUEC GVVBJ EBXIJ AKNGE SWTKB EDXPB QOUDW VTXES MRUWW RPAWKMTITK HFWTD AURRV FESFE STKSH FLZAE ONEXZ BWTIA RWWTT HQYEQVEDEX RBGSO REDMT ICM";
	std::string text = "NWAIWEBB RFQFOCJPUGDOJ VBGWSPTWRZ";
	text.erase(std::remove_if(text.begin(), text.end(), ::isspace),text.end());
	std::cout << "Obradjujemo sifrat : " << text << "\n\n\n";
	std::string yy = "WWBQCUOBSW";
	
	
	std::cout << "\nodaberite raspon kljuca izmedju 1 - 15(neda mi se pisati provjeru):\n";
	std::cin >> k1;
	std::cin >> k2;
	
	
	std::map<int, double> map2 = createTable(k1, k2, text,key_Size,cosets);
	for (std::map<int, double>::const_iterator it = map2.begin(); it != map2.end(); ++it) {
		std::cout << "Lenght : " << it->first << " Average IC : " << it->second << "\n";
	}

	std::cout << "COSET(vector) size\n";
	std::cout << cosets.size() << std::endl;
	for (int i = 0; i < cosets.size(); i++) {
		std::cout << cosets[i] << "\n";
	}
	std::cout << "Max key  =  " << key_Size << "\n";
	std::cout << "Keyword = " << getKeyWord(cosets) << "\n\n";

	

	






}


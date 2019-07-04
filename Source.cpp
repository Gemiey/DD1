#include <iostream> 
#include <string>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <bitset>
#include <map>
#include <set>
using namespace std;

vector<int> minterms;
vector<int> dontcares;
vector < vector < vector <string> > > matrix;
set<string> PI;
set<string> EPI;
map<int, string> newTable;
char equi[16];//an array to store variable equivalent to each digit
map<int, bool> checked;

int v, m, d;

void Translate(string original, string s) { 
	// the function replaces all underscores with all possible combinations of 0s and 1s and stores them in the string variables s1 and s2. 
	//The original variable, s is converted to its decimal equivalent. 
	// Then, the newTable map is accessed, checked if it contains any data. The newTable map contains all binary string representations of each value.
	std::size_t found = s.find('_');
	if (found != std::string::npos) {
		string s1 = s;
		string s2 = s;
		s1[found] = '0';
		s2[found] = '1';
		Translate(original, s1);
		Translate(original, s2);
	}
	else {
		int i = std::stoi(s, nullptr, 2);
		if (newTable[i].size() == 0) {
			newTable[i] = original;
		}
		else if (newTable[i].find(original) == std::string::npos) {
			newTable[i] += original;
		}
	}
}
void GetMinterms(string s) {
	// The string has all underscores replaced with 0s and 1s to reach the original binary equivalent. It is then converted to decimal.
	// The decimal value of the minterm is output.
	std::size_t found = s.find('_');
	if (found != std::string::npos) {
		string s1 = s;
		string s2 = s;
		s1[found] = '0';
		s2[found] = '1';
		GetMinterms(s1);
		GetMinterms(s2);
	}
	else {
		int i = std::stoi(s, nullptr, 2);
		cout << i << ", ";
	}
}
void EPIcovered(string s) {
	// The string s has all underscores replaced with 1s and 0s to reach the binary equivalent. Then it is converted to decimal.
	// The vector checked marks its index as true, indicating it is a prime implicant.
	std::size_t found = s.find('_');
	if (found != std::string::npos) {
		string s1 = s;
		string s2 = s;
		s1[found] = '0';
		s2[found] = '1';
		EPIcovered(s1);
		EPIcovered(s2);
	}
	else {
		int i = std::stoi(s, nullptr, 2);
		checked[i] = true;
	}
}
void GetBinaryString(int n) {
	// The integer n is converted to a 16 digit binary value stored in the string variable s.
	// It is then reduced to a size of v, for the number of inputs in the program.
	// The string is then checked for the number of 1s inside it.
	// At the end of the function, s is stored in the matrix according to the number of 1s in its binary value.

	string s = std::bitset<16>(n).to_string();//This value of 16 cannot be changed
	s = s.substr(s.size() - v, s.size());
	int counter = 0;
	for (int i = 0; i<s.size(); i++) {
		if (s[i] == '1') {
			counter++;
		}
	}
	matrix[0][counter].push_back(s);
}
bool Compare(int n, int m, string& s1, string& s2) {//n is the index of the column before the column of insertion and m is the number of ones
	// The function compares the strings s1 and s2, and checks for any differences. The differences are replaced with an underscore.
	// If there is only one difference and the underscore equivalent of the binary string does not already exist in the following column, it is placed in the column and the strings are marked with 'T'.
	int difference = 0;
	string temp = s1.substr(0, v);
	for (int i = 0; i<v; i++) {
		if (s1[i] != s2[i]) {
			difference++;
			temp[i] = '_';
		}
	}
	if (difference == 1) {
	//	bool test=false;
		bool flag =std::any_of(temp.begin(), temp.end(), ::isdigit);
		if(!flag) return true;
		if (!binary_search(matrix[n + 1][m].begin(), matrix[n + 1][m].end(), temp)) matrix[n + 1][m].push_back(temp);
		s1 += 'T';
		s2 += 'T';
	}
	return false;
}
string ToBoolean(string s) {
	// The string is converted to the boolean equivalent. This is achieved by using the equi array, which stores all possible letters.
	string boolexp = "";
	for (int i = 0; i<s.size(); i++) {
		if (s[i] == '1') boolexp += equi[i];
		else if (s[i] == '0') boolexp = boolexp + equi[i] + "'";
	}
	return boolexp;
}
int main() {
	equi[0] = 'A';
	equi[1] = 'B';
	equi[2] = 'C';
	equi[3] = 'D';
	equi[4] = 'E';
	equi[5] = 'F';
	equi[6] = 'G';
	equi[7] = 'H';
	equi[8] = 'I';
	equi[9] = 'J';
	equi[10] = 'K';
	equi[11] = 'L';
	equi[12] = 'M';
	equi[13] = 'N';
	equi[14] = 'O';
	equi[15] = 'P';
	equi[16] = 'Q';

	// file is opened and read from. 
	
	ifstream myfile("example.txt", ios::in);
	string tempstring, tempstring1, tempstring2;
	if (!myfile.is_open()) {
		cout << "File cannot open." << endl;
	}
	vector<string> str;
	while (std::getline(myfile, tempstring)) {
		str.push_back(tempstring);
	}
	for (int i = 0; i<str.size(); i++) cout << str[i] << endl;

	myfile.close();
	int m1, d1;
	int i = 0, j = 0;
	bool flag=false;
	if(!std::any_of(str[1].begin(), str[1].end(), ::isdigit)) { //This validation check is made especially to check if there are no minterms provided.
		cout<<"Function is always false.\n";
		return 0;}
    if(str[1].size()!= 0) flag= true;
	while (flag) {
		minterms.push_back(stoi(str[1].substr(0, str[1].find(','))));
		if (str[1].find(',') == string::npos) flag = false;
		str[1] = str[1].substr(str[1].find(',') + 1);

	}
	flag=false;
    if(std::any_of(str[2].begin(), str[2].end(), ::isdigit))	flag = true;
	while (flag) {
		dontcares.push_back(stoi(str[2].substr(0, str[2].find(','))));
		if (str[2].find(',') == string::npos) flag = false;
		str[2] = str[2].substr(str[2].find(',') + 1);

	}
	v = stoi(str[0]);
	m = minterms.size();
	d = dontcares.size();




	matrix.resize(v);

	// validations are placed for the size of the minterms and don't cares and to avoid duplicates
	for (int i = 0; i<v; i++) matrix[i].resize(v + 1);
	for (int i = 0; i<m; i++) {
		if (minterms[i]>pow(2, v) - 1) {
			cout << "No minterm can exceed " << pow(2, v) - 1 << endl;
			return 0;
		}
		if (binary_search(dontcares.begin(), dontcares.end(), minterms[i])) {
			cout << "A minterm can not be re entered as a don't care." << endl;
			return 0;
		}
		for (int j = i + 1; j<m; j++) {
			if (minterms[i] == minterms[j]) {
				cout << "A minterm cannot be entered twice\n";
			}
		}
		GetBinaryString(minterms[i]);
		checked[minterms[i]] = false;
	}
	for (int i = 0; i<d; i++) {
		if (dontcares[i]>pow(2, v) - 1) {
			cout << "No dont care can exceed " << pow(2, v) - 1 << endl;
			return 0;
		}
		for (int j = i + 1; j<m; j++) {
			if (dontcares[i] == dontcares[j]) {
				cout << "A don't care cannot be entered twice\n";
			}
		}
		GetBinaryString(dontcares[i]);
	}
	bool f;
	for (int g = 0; g<v; g++) {
		for (int i = 0; i<v; i++) {
			for (int j = 0; j<matrix[g][i].size(); j++) {
				for (int k = 0; k<matrix[g][i + 1].size(); k++) {
					if(Compare(g, i, matrix[g][i][j], matrix[g][i + 1][k])){
						cout<<"There are no PIs or EPIs, the function is always true.\n";//if all possible terms are entered as minterms and dont' cares
						return 0;
					};
				}
			}
		}
	}

	// PIs are added

	for (int g = 0; g<v; g++) {
		for (int i = 0; i<v; i++) {
			for (int j = 0; j<matrix[g][i].size(); j++) {
				if (matrix[g][i][j].at(matrix[g][i][j].size() - 1) != 'T') {
					PI.insert(matrix[g][i][j]);
				}
			}
		}
	}

	// PIs are displayed in all formats

	cout << "These are the prime implicants \n";
	for (set<std::string>::iterator it = PI.begin();it != PI.end(); it++) {
		Translate((*it), (*it));
		cout << (*it) << "               " << ToBoolean((*it)) << "       (";
		GetMinterms((*it));
		cout << ")" << endl;
	}
	for (int i = 0; i<m; i++) {
		if (newTable[minterms[i]].size() == v) {
			EPI.insert(newTable[minterms[i]]);
		}
	}

	// EPIs are added and displayed.

	cout << "These are the essential prime implicants\n";
	for (set<std::string>::iterator it = EPI.begin();it != EPI.end(); it++) {
		cout << (*it) << "               " << ToBoolean((*it)) << endl;
		EPIcovered((*it));
	}

	// Non essential PIs are displayed.

	cout << "Those are the minterms that are not covered by essential prime implicants.\n";
	for (int i = 0; i<m; i++) {
		if (!checked[minterms[i]]) cout << minterms[i] << endl;
	}
}


#include <iostream>
#include<string>
#include <queue>
#include <map>
#include <climits> // for CHAR_BIT
#include <iterator>
#include <algorithm>
#include <fstream>
#include <cstdlib>
using namespace std;
string data_encode;
char asc[100];
string codeing[100];
class tree{
public:
	tree();
	~tree();


	unsigned char data;
	tree* right;
	tree* left;
};

tree::tree(){
	right = NULL;
	left = NULL;
	//data = NULL;
}
tree::~tree(){

}

int decode()
{
	ofstream fout;
	fout.open("output.txt");
	int i = 0;
	string check;
	while (data_encode[i] == '0' || data_encode[i] == '1'){
		check.push_back(data_encode[i]);
		for (int x = 0; x < 100; x++){
			if (check == codeing[x]){
				fout << asc[x];
				check.clear();
				break;
			}
		}
		i++;
	}
	return 0;
}





typedef vector<bool> HuffCode;
typedef map<char, HuffCode> HuffCodeMap;

class INode
{
public:
	const int f;

	virtual ~INode() {}

protected:
	INode(int f) : f(f) {}
};

class InternalNode : public INode
{
public:
	INode *const left;
	INode *const right;

	InternalNode(INode* c0, INode* c1) : INode(c0->f + c1->f), left(c0), right(c1) {}
	~InternalNode()
	{
		delete left;
		delete right;
	}
};

class LeafNode : public INode
{
public:
	const unsigned char c;

	LeafNode(int f, unsigned char c) : INode(f), c(c) {}
};

struct NodeCmp
{
	bool operator()(const INode* lhs, const INode* rhs) const { return lhs->f > rhs->f; }
};

INode* BuildTree(const int(&frequencies)[256])
{
	priority_queue<INode*, std::vector<INode*>, NodeCmp> trees;

	for (int i = 0; i < 256; ++i)
	{
		if (frequencies[i] != 0)
		{
			trees.push(new LeafNode(frequencies[i], (unsigned char)i));
		}
	}
	while (trees.size() > 1)
	{
		INode* childR = trees.top();
		trees.pop();

		INode* childL = trees.top();
		trees.pop();

		INode* parent = new InternalNode(childR, childL);
		trees.push(parent);
	}
	return trees.top();
}

void GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes)
{
	if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node))
	{
		outCodes[lf->c] = prefix;
	}
	else if (const InternalNode* in = dynamic_cast<const InternalNode*>(node))
	{
		HuffCode leftPrefix = prefix;
		leftPrefix.push_back(false);
		GenerateCodes(in->left, leftPrefix, outCodes);

		HuffCode rightPrefix = prefix;
		rightPrefix.push_back(true);
		GenerateCodes(in->right, rightPrefix, outCodes);
	}
}





int encode()
{
	ifstream fin;
	fin.open("input.txt");
	string SampleString;
	getline(fin, SampleString);
	// Build frequency table
	int frequencies[256] = { 0 }; //inital value=0
	const char* ptr = SampleString.c_str();
	const char* outptr = SampleString.c_str();
	while (*ptr != '\0')
		++frequencies[*ptr++];

	INode* root = BuildTree(frequencies);

	HuffCodeMap codes;
	GenerateCodes(root, HuffCode(), codes);
	delete root;
	int num = 0;
	for (HuffCodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it)
	{
		HuffCode::const_iterator x;
		asc[num] = it->first;
		for (x = it->second.begin(); x != it->second.end(); x++){
			if (*x == true){
				codeing[num].push_back('1');
			}
			else if (*x == false){
				codeing[num].push_back('0');
			}
		}
		num++;
	}
	fin.seekg(0,ios::beg);
	char get;
	while (fin.get(get)){
		for (int i = 0; i < num; i++){
			if (get == asc[i]){
				data_encode.append(codeing[i]);
			}
		}
	}
	fin.close();
	return 0;
}



int main()
{
	encode();
	decode();
	return 0;
}
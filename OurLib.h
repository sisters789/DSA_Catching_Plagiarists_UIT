#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <string>
#include <fstream>
#include <errno.h>
#include <io.h>
#include <map>  // map
#include <utility> // pair 

#define N_MAX 3
#define NOT_FOUND -1
using namespace std;

struct ngrams
{
    int TanSo;
    string n_gram;
};
struct ListNGramsOfFile
{
    string tenFile;
    vector<vector<ngrams> *> NGrams_List; // List 1-grmas, List 2-grams, List 3-grams

};

struct DoSoKhop
{
    string file1;
    string file2;
    int n;
};

struct node
{
    ngrams data;
    node* pLeft;
    node* pRight;
};
typedef struct node* Tree;
struct BSTGramsOfFile
{
    string tenFile;
    vector<Tree *> NGrams_List; // List 1-grmas, List 2-grams, List 3-grams
};
struct HashGramsOfFile
{
    string tenFile;
    map<string, int> NGrams_List; // List 1-grmas, List 2-grams, List 3-grams
};


int mymin(int a, int b);
//
void _parse_argument_main(int argc, char* argv[], string &folderName, string& ctdl, int &n, int &m, int &k);

// Vector
void _get_ngrams_from_file(const string& dir, vector<vector<ngrams>*> &NGramsList, int n, int m);
void _get_ngrams_from_line(const string& line, vector<vector<ngrams>*> &NGramsList, vector<string> gramsQueue[], int n, int m);
int _search(vector<ngrams> &list, const string& key);
void _get_ngrams_from_folder(const string& dir, string ctdl, int n, int m, vector<ListNGramsOfFile>& folderListt);
DoSoKhop _compare_two_file(ListNGramsOfFile& file1, ListNGramsOfFile &file2);
void _compare_all_file(const vector<ListNGramsOfFile>& folderList, vector<DoSoKhop>& result);
bool compare(const DoSoKhop& a, const DoSoKhop& b);


// BST
node* _createNode(const ngrams& data);
void _insert(Tree& tree, const ngrams& data);
node* _search(const Tree& tree, const string& key);
void _traverse(const Tree& t1, const Tree& t2, DoSoKhop& comp);

void _get_ngrams_from_line(const string& line, vector<Tree*> &NGramsList, vector<string> gramsQueue[], int n, int m);
void _get_ngrams_from_file(const string& dir, vector<Tree*> &NGramsList, int n, int m);
void _get_ngrams_from_folder(const string& dir, string ctdl, int n, int m, vector<BSTGramsOfFile>& folderListt);
DoSoKhop _compare_two_file(const BSTGramsOfFile& file1, const BSTGramsOfFile &file2);
void _compare_all_file(const vector<BSTGramsOfFile>& folderList, vector<DoSoKhop>& result);
void _destroy(Tree& tree);
void _destroy(vector<BSTGramsOfFile>& folderList);

// map
void _get_ngrams_from_line(const string& line, map<string, int> &NGramsList, vector<string> gramsQueue[], int n, int m);
void _get_ngrams_from_file(const string& dir, map<string, int> &NGramsList, int n, int m);
void _get_ngrams_from_folder(const string& dir, string ctdl, int n, int m, vector<HashGramsOfFile>& folderListt);
DoSoKhop _compare_two_file(const HashGramsOfFile& file1, HashGramsOfFile &file2);
void _compare_all_file(vector<HashGramsOfFile>& folderList, vector<DoSoKhop>& result);

// map_from_scratch
#define M_TABLE 32768
struct Table
{
    int size;
    string tenFile;
    vector<ngrams> NGrams_List[M_TABLE]; // List 1-grmas, List 2-grams, List 3-grams 
};
void _initTable(Table& t, string tenFile);
unsigned long _hashing(string str);
void _insertTable(Table& table, const string& data);
bool _getTable(const Table& table, string key, ngrams& result);

void _get_ngrams_from_line(const string& line, Table& hTable, vector<string> gramsQueue[], int n, int m);
void _get_ngrams_from_file(const string& dir, Table& hTable, int n, int m);
void _get_ngrams_from_folder(const string& dir, string ctdl, int n, int m, vector<Table>& folderList);
DoSoKhop _compare_two_file(const Table& file1, Table &file2);
void _compare_all_file(vector<Table>& folderList, vector<DoSoKhop>& result);

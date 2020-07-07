#include "OurLib.h"
int mymin(int a, int b)
{
    return (a<b)? a:b;
}
void _parse_argument_main(int argc, char* argv[], string &folderName, string& ctdl, int &n, int &m, int &k)
{
    for(int i=0; i<argc-1; i++)
    {
        if (strcmp(argv[i], "-d")==0)
        {
            folderName = argv[i+1];
        }
        else if (strcmp(argv[i], "-a")==0)
        {
            ctdl = argv[i+1];
        }
        else if (strcmp(argv[i], "-n")==0)
        {
            string t = argv[i+1];
            n = argv[i+1][0] - '0';
            m = argv[i+1][t.size()-1] - '0';
        }
        else if (strcmp(argv[i], "-k")==0)
        {
            if (strcmp(argv[i+1], "-1")==0)
                k=-1;
            else
            {
                //k = argv[i+1][0] - '0';
				k = atoi(argv[i+1]);
            }
            
        }
    }
}

void _get_ngrams_from_file(const string& dir, vector<vector<ngrams>*> &NGramsList, int n, int m)
{
    vector<string> gramsQueue[N_MAX];
    //cout << "dir in ngrams from file: " << dir << endl;
    ifstream file;
    string line = "";
    file.open(dir, ios::in);
    while(!file.eof())
    {
        getline(file, line);
        _get_ngrams_from_line(line, NGramsList, gramsQueue, n, m);
    }
    file.close();
}

void _get_ngrams_from_line(const string& line, vector<vector<ngrams>*> &NGramsList, vector<string>  gramsQueue[], int n, int m)
{
    char * str = new char[line.size()+1];
    strcpy(str, line.c_str());
    string t_word;
    string t_gram;
    ngrams temp;
    int i;
    // " .,:;`'/\\+-(){}[]<>*&^%$#@!?~|_=0123456789\t\n\""
    char* word = strtok(str, " .,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890");
    while(word!=NULL)
    {
        i = 0;
        while(word[i]!='\0')
        {
            if (word[i] >= 'A' && word[i] <= 'Z')
                word[i] +=32;
            i++;
        }
        //
        t_word = word;
        for (int numberOfGram=n; numberOfGram<=m; numberOfGram++)
        {
            gramsQueue[numberOfGram-1].push_back(t_word);
            if (gramsQueue[numberOfGram-1].size()==numberOfGram)
            {
                t_gram = "";
                for (int k=0;k<numberOfGram;k++)
                {
                    t_gram.append((gramsQueue[numberOfGram-1])[k]);
                    if (k != numberOfGram - 1)
                        t_gram += " ";
                }
                
                //int pos = _search(*NGramsList[numberOfGram-1], t_gram);
                int pos = _search(*NGramsList[numberOfGram-n], t_gram);
                if ( pos == NOT_FOUND)
                {
                    temp.n_gram = t_gram;
                    temp.TanSo=1;
                    //NGramsList[numberOfGram-1]->push_back(temp);
                    NGramsList[numberOfGram-n]->push_back(temp);
                }
                else
                {
                    //(*NGramsList[numberOfGram-1])[pos].TanSo+=1;
                    (*NGramsList[numberOfGram-n])[pos].TanSo+=1;
                }
                
                gramsQueue[numberOfGram-1].erase(gramsQueue[numberOfGram-1].begin());
            }
        }
        word = strtok(NULL, " .,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890");
    }
    delete[]str;
}

void _get_ngrams_from_folder(const string& dir, string ctdl, int n, int m, vector<ListNGramsOfFile>& folderList)
{
    string fileDir = dir;
    string t_dir;
    //fileDir.erase(fileDir.find_first_of("*"));
    fileDir.erase(fileDir.find_last_of("\\")+1);
    //cout << "fileDir" << fileDir << "\n";
    string src= fileDir;
    t_dir = src+"*.txt";
    //cout << "t_dir: " << t_dir.c_str() << endl;
    intptr_t handle;
    struct _finddata_t fileinfo;
    handle = _findfirst(t_dir.c_str(), &fileinfo);
    if (handle == -1) {
        cout << strerror(errno);
        exit(1);
    }
    
    //cout << fileinfo.name << endl;
    ListNGramsOfFile temp;
    temp.tenFile = fileinfo.name;
    cout << "Dang xu ly file " << temp.tenFile << "\n";
    for(int i=n; i<=m; i++)
    {
        temp.NGrams_List.push_back(new vector<ngrams>);
    }
    //cout << "fileDir: " << fileDir + temp.tenFile << endl;
    _get_ngrams_from_file(fileDir.append(fileinfo.name), temp.NGrams_List, n, m);
    folderList.push_back(temp);
    while (_findnext(handle, &fileinfo) == 0) {
        //cout << fileinfo.name << endl;
        fileDir = src;
        ListNGramsOfFile temp;
        temp.tenFile = fileinfo.name;
        cout << "Dang xu ly file " << temp.tenFile << "\n";
        for(int i=n; i<=m; i++)
        {
            temp.NGrams_List.push_back(new vector<ngrams>);
        }
        _get_ngrams_from_file(fileDir.append(fileinfo.name), temp.NGrams_List, n, m);
        folderList.push_back(temp);
    }

    _findclose(handle);
}

int _search(vector<ngrams> &list, const string& key)
{
    for(int i=0;i<list.size();i++)
    {
        if (key==list[i].n_gram)
            return i;
    }
    return NOT_FOUND;
}

DoSoKhop _compare_two_file(const ListNGramsOfFile& file1, const ListNGramsOfFile &file2)
{
    DoSoKhop result;
    ngrams *f1 = NULL, *f2 = NULL;
    result.file1 = file1.tenFile;
    result.file2 = file2.tenFile;
    result.n = 0;

    for (int i=0; i<file1.NGrams_List.size(); i++) // duyet het cac danh sach don vi
    {
        for (int j=0; j<(file1.NGrams_List[i])->size(); j++)
        {
            f1 = &(*(file1.NGrams_List[i]))[j];
            for (int k=0; k<(file2.NGrams_List[i])->size(); k++)
            {
                f2 = &(*(file2.NGrams_List[i]))[k];
                if (f1->n_gram == f2->n_gram)
                {
                    result.n += (f1->TanSo < f2->TanSo)? f1->TanSo : f2->TanSo;
                    break;
                }
            }
        }
    }

    return result;
}
void _compare_all_file(const vector<ListNGramsOfFile>& folderList, vector<DoSoKhop>& result)
{
    DoSoKhop temp;
    for (int i=0; i<folderList.size()-1; i++)
    {
        for (int j=i+1; j<folderList.size(); j++)
        {
            cout << "So sanh: " << folderList[i].tenFile << " " << folderList[j].tenFile << "\n";
            temp = _compare_two_file(folderList[i], folderList[j]);
            result.push_back(temp);
        }
    }
}

bool compare(const DoSoKhop& a, const DoSoKhop& b)
{
    return a.n > b.n;
}

void _get_ngrams_from_line_v2(const string& line, vector<vector<ngrams>*> &NGramsList, vector<string>  gramsQueue[], int n, int m)
{
    char a[] = " .,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890";
    string t_word = "";
    string t_gram = "";
    ngrams temp;
    int i;
    char c;
    // " .,:;`'/\\+-(){}[]<>*&^%$#@!?~|_=0123456789\t\n\""
    for (i=0; i<line.size(); i++)
    {
        c = line[i];
        if (c >= 32 && c<= 126)
        {
            if ((c >= 'a' && c <= 'z')  || c=='"')
            {
                t_word+= c;
            }
            else if (c >= 'A' && c <= 'Z')
            {
                t_word += tolower(c);
            }
            else
            {
                for (int numberOfGram=n; numberOfGram<=m; numberOfGram++)
                {
                    gramsQueue[numberOfGram-1].push_back(t_word);
            
                    if (gramsQueue[numberOfGram-1].size()==numberOfGram)
                    {
                        t_gram = "";
                        for (int k=0;k<numberOfGram;k++)
                        {
                            t_gram.append((gramsQueue[numberOfGram-1])[k]);
                            if (k != numberOfGram - 1)
                                t_gram += " ";
                        }

                        //int pos = _search(*NGramsList[numberOfGram-1], t_gram);
                        int pos = _search(*NGramsList[numberOfGram-n], t_gram);
                        if ( pos == NOT_FOUND)
                        {
                            temp.n_gram = t_gram;
                            temp.TanSo=1;
                            //NGramsList[numberOfGram-1]->push_back(temp);
                            NGramsList[numberOfGram-n]->push_back(temp);
                        }
                        else
                        {
                            //(*NGramsList[numberOfGram-1])[pos].TanSo+=1;
                            (*NGramsList[numberOfGram-n])[pos].TanSo+=1;
                        }
            
                        gramsQueue[numberOfGram-1].erase(gramsQueue[numberOfGram-1].begin());
                    }
                }
                t_word = "";
            }
            
        }
        //
        else if (c=='\t'||c=='\r'||c=='\n')
        {
            for (int numberOfGram=n; numberOfGram<=m; numberOfGram++)
            {
                gramsQueue[numberOfGram-1].push_back(t_word);

                if (gramsQueue[numberOfGram-1].size()==numberOfGram)
                {
                    t_gram = "";
                    for (int k=0;k<numberOfGram;k++)
                    {
                        t_gram.append((gramsQueue[numberOfGram-1])[k]);
                        if (k != numberOfGram - 1)
                            t_gram += " ";
                    }

                    //int pos = _search(*NGramsList[numberOfGram-1], t_gram);
                    int pos = _search(*NGramsList[numberOfGram-n], t_gram);
                    if ( pos == NOT_FOUND)
                    {
                        temp.n_gram = t_gram;
                        temp.TanSo=1;
                        //NGramsList[numberOfGram-1]->push_back(temp);
                        NGramsList[numberOfGram-n]->push_back(temp);
                    }
                    else
                    {
                        //(*NGramsList[numberOfGram-1])[pos].TanSo+=1;
                        (*NGramsList[numberOfGram-n])[pos].TanSo+=1;
                    }
            
                    gramsQueue[numberOfGram-1].erase(gramsQueue[numberOfGram-1].begin());
                }
            }

            t_word = "";
        }
        else
        {
            t_word+=c;
        }
        
    }
}


// BST
////////////////////////////////////////////////////////////////////////////
node* _createNode(const string& data)
{
    node* newNode = new node;
    newNode->data.TanSo=1;
    newNode->data.n_gram = data;
    newNode->pLeft = newNode->pRight = NULL;
    
    return newNode;
}
void _insert(Tree& tree, const string& data)
{
    if (tree == NULL)
    {
        tree = _createNode(data);
        return;
    }

    if (data.compare(tree->data.n_gram) <0)  // data < tree->data
    {
        _insert(tree->pLeft, data);
    }
    else if (data.compare(tree->data.n_gram) >0)
    {
        _insert(tree->pRight, data);
    }
    else
    {
        tree->data.TanSo++;
    }
    
}
node* _search(const Tree& tree, const string& key)
{
    if (tree==NULL)
        return NULL;
    
    if (key.compare(tree->data.n_gram) <0)  // data < tree->data
    {
        return _search(tree->pLeft, key);
    }
    else if (key.compare(tree->data.n_gram) >0)
    {
        return _search(tree->pRight, key);
    }
    else
    {
        return tree;
    }
}

void _get_ngrams_from_line(const string& line, vector<Tree*> &NGramsList, vector<string> gramsQueue[], int n, int m)
{
    char * str = new char[line.size()+1];
    strcpy(str, line.c_str());
    string t_word;
    string t_gram;
    ngrams temp;
    int i;
    // " .,:;`'/\\+-(){}[]<>*&^%$#@!?~|_=0123456789\t\n\""
    char* word = strtok(str, " .,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890");
    while(word!=NULL)
    {
        i = 0;
        while(word[i]!='\0')
        {
            if (word[i] >= 'A' && word[i] <= 'Z')
                word[i] +=32;
            i++;
        }
        //
        t_word = word;
        for (int numberOfGram=n; numberOfGram<=m; numberOfGram++)
        {
            gramsQueue[numberOfGram-1].push_back(t_word);
            if (gramsQueue[numberOfGram-1].size()==numberOfGram)
            {
                t_gram = "";
                for (int k=0;k<numberOfGram;k++)
                {
                    t_gram.append((gramsQueue[numberOfGram-1])[k]);
                    if (k != numberOfGram - 1)
                        t_gram += " ";
                }
                
                //int pos = _search(*NGramsList[numberOfGram-1], t_gram);
                _insert(*NGramsList[numberOfGram-n], t_gram);
                
                gramsQueue[numberOfGram-1].erase(gramsQueue[numberOfGram-1].begin());
            }
        }
        word = strtok(NULL, " .,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890");
    }
    delete[]str;
}
void _get_ngrams_from_file(const string& dir, vector<Tree*> &NGramsList, int n, int m)
{
    vector<string> gramsQueue[N_MAX];
    //cout << "dir in ngrams from file: " << dir << endl;
    ifstream file;
    string line = "";
    file.open(dir, ios::in);
    while(!file.eof())
    {
        getline(file, line);
        _get_ngrams_from_line(line, NGramsList, gramsQueue, n, m);
    }
    file.close();
}
void _get_ngrams_from_folder(const string& dir, string ctdl, int n, int m, vector<BSTGramsOfFile>& folderList)
{
    string fileDir = dir;
    string t_dir;
    //fileDir.erase(fileDir.find_first_of("*"));
    fileDir.erase(fileDir.find_last_of("\\")+1);
    //cout << "fileDir" << fileDir << "\n";
    string src= fileDir;
    t_dir = src+"*.txt";
    //cout << "t_dir: " << t_dir.c_str() << endl;
    intptr_t handle;
    struct _finddata_t fileinfo;
    handle = _findfirst(t_dir.c_str(), &fileinfo);
    if (handle == -1) {
        cout << strerror(errno);
        exit(1);
    }
    
    //cout << fileinfo.name << endl;
    BSTGramsOfFile temp;
    temp.tenFile = fileinfo.name;
    cout << "Dang xu ly file " << temp.tenFile << "\n";
    for(int i=n; i<=m; i++)
    {
        temp.NGrams_List.push_back(new Tree);
        *temp.NGrams_List[i-n] = NULL;
    }
    //cout << "fileDir: " << fileDir + temp.tenFile << endl;
    _get_ngrams_from_file(fileDir.append(fileinfo.name), temp.NGrams_List, n, m);
    folderList.push_back(temp);
    while (_findnext(handle, &fileinfo) == 0) {
        //cout << fileinfo.name << endl;
        fileDir = src;
        BSTGramsOfFile temp;
        temp.tenFile = fileinfo.name;
        cout << "Dang xu ly file " << temp.tenFile << "\n";
        for(int i=n; i<=m; i++)
        {
            temp.NGrams_List.push_back(new Tree);
            *temp.NGrams_List[i-n] = NULL;
        }
        _get_ngrams_from_file(fileDir.append(fileinfo.name), temp.NGrams_List, n, m);
        folderList.push_back(temp);
    }

    _findclose(handle);
}

void _traverse(const Tree& t1, const Tree& t2, DoSoKhop& comp)
{
    if (t1==NULL)
        return;

    _traverse(t1->pLeft, t2, comp);
    _traverse(t1->pRight, t2, comp);

    node* ret = _search(t2, t1->data.n_gram);
    if (ret!=NULL)
    {
        comp.n += ((ret->data.TanSo < t1->data.TanSo)? ret->data.TanSo : t1->data.TanSo);
    }
}
DoSoKhop _compare_two_file(const BSTGramsOfFile& file1, const BSTGramsOfFile &file2)
{
    DoSoKhop res;
    res.file1 = file1.tenFile;
    res.file2 = file2.tenFile;
    res.n = 0;
    //cout << "\n";
    for (int i=0; i<file1.NGrams_List.size(); i++)
    {
        _traverse(*file1.NGrams_List[i], *file2.NGrams_List[i], res);
    }

    return res;
}

void _compare_all_file(const vector<BSTGramsOfFile>& folderList, vector<DoSoKhop>& result)
{
    DoSoKhop temp;
    for (int i=0; i<folderList.size()-1; i++)
    {
        for (int j=i+1; j<folderList.size(); j++)
        {
            cout << "So sanh: " << folderList[i].tenFile << " " << folderList[j].tenFile << "\n";
            temp = _compare_two_file(folderList[i], folderList[j]);
            result.push_back(temp);
        }
    }
}

void _destroy(Tree& tree)
{
    if (tree==NULL)
        return;
    
    _destroy(tree->pLeft);
    _destroy(tree->pRight);

    delete tree;
    tree=NULL;
}
void _destroy(vector<BSTGramsOfFile>& folderList)
{
    for(int i=0; i<folderList.size(); i++)
    {
        for (int j=0; j<folderList[i].NGrams_List.size(); j++)
        {
            _destroy(*(folderList[i].NGrams_List[j]));
            delete folderList[i].NGrams_List[j];
        }
    }
}

////////////////////////////////////////////////////////////////////////////

// map
void _get_ngrams_from_line(const string& line, map<string, int>& hTable, vector<string> gramsQueue[], int n, int m)
{
    char * str = new char[line.size()+1];
    strcpy(str, line.c_str());
    string t_word;
    string t_gram;
    ngrams temp;
    int i;
    // " .,:;`'/\\+-(){}[]<>*&^%$#@!?~|_=0123456789\t\n\""
    char* word = strtok(str, " .,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890");
    while(word!=NULL)
    {
        i = 0;
        while(word[i]!='\0')
        {
            if (word[i] >= 'A' && word[i] <= 'Z')
                word[i] +=32;
            i++;
        }
        //
        t_word = word;
        for (int numberOfGram=n; numberOfGram<=m; numberOfGram++)
        {
            gramsQueue[numberOfGram-1].push_back(t_word);
            if (gramsQueue[numberOfGram-1].size()==numberOfGram)
            {
                t_gram = "";
                for (int k=0;k<numberOfGram;k++)
                {
                    t_gram.append((gramsQueue[numberOfGram-1])[k]);
                    if (k != numberOfGram - 1)
                        t_gram += " ";
                }
                
                //int pos = _search(*NGramsList[numberOfGram-1], t_gram);
                hTable[t_gram]++;
                
                gramsQueue[numberOfGram-1].erase(gramsQueue[numberOfGram-1].begin());
            }
        }
        word = strtok(NULL, " .,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890");
    }
    delete[]str;
}
void _get_ngrams_from_file(const string& dir, map<string,int> &hTable, int n, int m)
{
    vector<string> gramsQueue[N_MAX];
    //cout << "dir in ngrams from file: " << dir << endl;
    ifstream file;
    string line = "";
    file.open(dir, ios::in);
    while(!file.eof())
    {
        getline(file, line);
        _get_ngrams_from_line(line, hTable, gramsQueue, n, m);
    }
    file.close();
}
void _get_ngrams_from_folder(const string& dir, string ctdl, int n, int m, vector<HashGramsOfFile>& folderList)
{
    string fileDir = dir;
    string t_dir;
    //fileDir.erase(fileDir. find_first_of("*"));
    fileDir.erase(fileDir.find_last_of("\\")+1);
    //cout << "fileDir" << fileDir << "\n";
    string src= fileDir;
    t_dir = src+"*.txt";
    //cout << "t_dir: " << t_dir.c_str() << endl;
    intptr_t handle;
    struct _finddata_t fileinfo;
    handle = _findfirst(t_dir.c_str(), &fileinfo);
    if (handle == -1) {
        cout << strerror(errno);
        exit(1);
    }
    
    //cout << fileinfo.name << endl;
    HashGramsOfFile temp;
    temp.tenFile = fileinfo.name;
    cout << "Dang xu ly file " << temp.tenFile << "\n";
    
    //cout << "fileDir: " << fileDir + temp.tenFile << endl;
    _get_ngrams_from_file(fileDir.append(fileinfo.name), temp.NGrams_List, n, m);
    folderList.push_back(temp);
    while (_findnext(handle, &fileinfo) == 0) {
        //cout << fileinfo.name << endl;
        fileDir = src;
        HashGramsOfFile temp;
        temp.tenFile = fileinfo.name;
        cout << "Dang xu ly file " << temp.tenFile << "\n";
        
        _get_ngrams_from_file(fileDir.append(fileinfo.name), temp.NGrams_List, n, m);
        folderList.push_back(temp);
    }

    _findclose(handle);
}

DoSoKhop _compare_two_file(const HashGramsOfFile& file1, HashGramsOfFile &file2)
{
    DoSoKhop res;
    int freq1, freq2; 
    string ngram;
    res.file1 = file1.tenFile;
    res.file2 = file2.tenFile;
    res.n = 0;

    for(auto i:file1.NGrams_List)
    {
        ngram = i.first;
        freq1 = i.second;
        freq2 = file2.NGrams_List[ngram];
        res.n += ((freq1<freq2)? freq1 : freq2);
    }
    //cout << "\n";
    
    return res;
}

void _compare_all_file(vector<HashGramsOfFile>& folderList, vector<DoSoKhop>& result)
{
    DoSoKhop temp;
    for (int i=0; i<folderList.size()-1; i++)
    {
        for (int j=i+1; j<folderList.size(); j++)
        {
            cout << "So sanh: " << folderList[i].tenFile << " " << folderList[j].tenFile << "\n";
            temp = _compare_two_file(folderList[i], folderList[j]);
            result.push_back(temp);
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////

// map_from_scratch
//////////////////////////////////////////////////////////////////////////////////////////

void _initTable(Table& t, string tenFile)
{
    t.size=0;
    t.tenFile = tenFile;
}
unsigned long _hashing(string str)
{
    unsigned long hash = 5381;
    int c;
    for (int i=0; i<str.size(); i++)
    {
        c=str[i];
        hash = (hash<<5 + hash) + c;
    }

    return hash%M_TABLE;
}
void _insertCell(vector<ngrams> &cell, const string &data)
{
    int i;
    for (i=0;i<cell.size();i++)
    {
        if (cell[i].n_gram==data)
        {
            cell[i].TanSo++;
            return;
        }
    }
    
    ngrams temp;
    temp.n_gram = data;
    temp.TanSo = 1;
    cell.push_back(temp);
}
void _insertTable(Table& table, const string& data)
{
    unsigned long index = _hashing(data);
    if (table.NGrams_List[index].size()==0)
    {
        table.size++;
    }
    _insertCell(table.NGrams_List[index], data);
}

bool _getTable(const Table& table, string key, ngrams& result)
{
    unsigned long index = _hashing(key);
    int i;
    for (i=0; i<table.NGrams_List[index].size(); i++)
    {
        if (key == table.NGrams_List[index][i].n_gram)
        {
            result = table.NGrams_List[index][i];
            return 1;
        }
    }

    return 0;
}

void _get_ngrams_from_line(const string& line, Table& hTable, vector<string> gramsQueue[], int n, int m)
{
    char * str = new char[line.size()+1];
    strcpy(str, line.c_str());
    string t_word;
    string t_gram;
    ngrams temp;
    int i;
    // " .,:;`'/\\+-(){}[]<>*&^%$#@!?~|_=0123456789\t\n\""
    char* word = strtok(str, " .,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890");
    while(word!=NULL)
    {
        i = 0;
        while(word[i]!='\0')
        {
            if (word[i] >= 'A' && word[i] <= 'Z')
                word[i] +=32;
            i++;
        }
        //
        t_word = word;
        for (int numberOfGram=n; numberOfGram<=m; numberOfGram++)
        {
            gramsQueue[numberOfGram-1].push_back(t_word);
            if (gramsQueue[numberOfGram-1].size()==numberOfGram)
            {
                t_gram = "";
                for (int k=0;k<numberOfGram;k++)
                {
                    t_gram.append((gramsQueue[numberOfGram-1])[k]);
                    if (k != numberOfGram - 1)
                        t_gram += " ";
                }
                
                //int pos = _search(*NGramsList[numberOfGram-1], t_gram);
                _insertTable(hTable, t_gram);
                
                
                gramsQueue[numberOfGram-1].erase(gramsQueue[numberOfGram-1].begin());
            }
        }
        word = strtok(NULL, " .,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890");
    }
    delete[]str;
}
void _get_ngrams_from_file(const string& dir, Table& hTable, int n, int m)
{
    vector<string> gramsQueue[N_MAX];
    //cout << "dir in ngrams from file: " << dir << endl;
    ifstream file;
    string line = "";
    file.open(dir, ios::in);
    while(!file.eof())
    {
        getline(file, line);
        //cout << "line: " << line << "\n";
        _get_ngrams_from_line(line, hTable, gramsQueue, n, m);
    }
    file.close();
}
void _get_ngrams_from_folder(const string& dir, string ctdl, int n, int m, vector<Table>& folderList)
{
    string fileDir = dir;
    string t_dir;
    //fileDir.erase(fileDir. find_first_of("*"));
    fileDir.erase(fileDir.find_last_of("\\")+1);
    //cout << "fileDir" << fileDir << "\n";
    string src= fileDir;
    t_dir = src+"*.txt";
    //cout << "t_dir: " << t_dir.c_str() << endl;
    intptr_t handle;
    struct _finddata_t fileinfo;
    handle = _findfirst(t_dir.c_str(), &fileinfo);
    if (handle == -1) {
        cout << strerror(errno);
        exit(1);
    }
    
    //cout << fileinfo.name << endl;
    Table temp;
    _initTable(temp, fileinfo.name);
    cout << "Dang xu ly file " << temp.tenFile << "\n";
    
    //cout << "fileDir: " << fileDir + temp.tenFile << endl;
    _get_ngrams_from_file(fileDir.append(fileinfo.name), temp, n, m);
    folderList.push_back(temp);
    while (_findnext(handle, &fileinfo) == 0) {
        //cout << fileinfo.name << endl;
        fileDir = src;
        Table temp;
        _initTable(temp, fileinfo.name);
        //temp.tenFile = fileinfo.name;
        cout << "Dang xu ly file " << temp.tenFile << "\n";
        
        _get_ngrams_from_file(fileDir.append(fileinfo.name), temp, n, m);
        folderList.push_back(temp);
    }

    _findclose(handle);
}

DoSoKhop _compare_two_file(const Table& file1, Table &file2)
{
    DoSoKhop res;
    int freq1, freq2, i, j; 
    ngrams ngram1, ngram2;
    bool ret=0;
	int dem=0;
    const vector<ngrams>* cell;
    res.file1 = file1.tenFile;
    res.file2 = file2.tenFile;
    res.n = 0;

    for (i=0; i<M_TABLE;i++)
    {
		
        cell = &file1.NGrams_List[i];
		if (cell->size() != 0)
		{
			for (j=0;j<cell->size();j++)
			{
				ngram1 = (*cell)[j];
				if (_getTable(file2, ngram1.n_gram, ngram2)==1)
				{
					res.n += mymin(ngram1.TanSo, ngram2.TanSo);
				}
			}
			dem++;
			if (dem==file1.size)
				break;
		}
    }
    //cout << "\n";
    
    return res;
}

void _compare_all_file(vector<Table>& folderList, vector<DoSoKhop>& result)
{
    DoSoKhop temp;
    for (int i=0; i<folderList.size()-1; i++)
    {
        for (int j=i+1; j<folderList.size(); j++)
        {
            cout << "So sanh: " << folderList[i].tenFile << " " << folderList[j].tenFile << "\n";
            temp = _compare_two_file(folderList[i], folderList[j]);
            result.push_back(temp);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
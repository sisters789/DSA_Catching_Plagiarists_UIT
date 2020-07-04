#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <string.h>
#include <vector>
#include <chrono>
#include "OurLib.h"
#include <map>
#include <utility>

using namespace std::chrono;
using namespace std;

int main(int argc, char* argv[])
{
    auto start = high_resolution_clock::now();
    string dir, ctdl="";
    int n, m, k=-1;
    vector<DoSoKhop> result;
    vector<BSTGramsOfFile> folderListBST;
    vector<ListNGramsOfFile> folderListVector;
    vector<HashGramsOfFile> folderListMap;
    
    ////////////////////////////////////////////////////////////////////////////////////
    n=1; m=3;
    dir = "D:\\Workspace\\VisualCode\\CTDL\\ASSIGNMENT\\sm_doc_set\\*.txt";
    // cout << "dir: " << dir << endl;
    //dir = argv[2];
    
    _parse_argument_main(argc, argv, dir, ctdl, n, m, k);
    // cout << argv[2] << "\n";
    // cout << dir << "\n" << ctdl << "\n" << n << " " << m << " " << k << "\n";
    if (ctdl=="bst")
    {
        _get_ngrams_from_folder(dir, "", n, m, folderListBST);
        _compare_all_file(folderListBST, result);
    }
    else if (ctdl=="vector")
    {
        _get_ngrams_from_folder(dir, "", n, m, folderListVector);
        _compare_all_file(folderListVector, result);
    }
    else if (ctdl == "map")
    {
        _get_ngrams_from_folder(dir, "", n, m, folderListMap);  
        _compare_all_file(folderListMap, result);
    }
    
    // for (int i=0; i< folderList.size();i++)
    // {
    //     cout << folderList[i].tenFile << "\n";
    // }

    

    sort(result.begin(), result.end(), compare);
    /////////////////////////////////////////////////////////////////////////////////////
    int kt = (k==-1)? result.size() : k;
    kt = result.size()<k? result.size():k;
    cout << "Ket qua so sanh:\n";
    for (int i=0; i< kt; i++)
    {
        cout << result[i].file1 << " " << result[i].file2 << " " << result[i].n << "\n";
    }

    cout << "size of result[]: " << result.size() << "\n";
    // /////////////////////////////////////////////////////////////////////////////////////

    // delete vùng nhớ động đã cấp phát
    if (ctdl=="bst")
        _destroy(folderListBST);
    else if (ctdl == "vector")
        for(int i=0;i<folderListVector.size();i++)
        {
            for (int j=0; j<=m-n; j++)
            {
                delete (folderListVector[i].NGrams_List)[j];
            }
        }
    auto end = high_resolution_clock::now();
    ////////////////////////////////////////////////////////////////////////////////////

    
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Thoi gian chay: " << duration.count() << "ms\n";


    // for (int i=0;i<N_MAX;i++)
    // {
    //     delete NGramsList[i];
    // }
    return 0;
}
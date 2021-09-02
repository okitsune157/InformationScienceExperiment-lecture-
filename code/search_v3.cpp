#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <errno.h>

using namespace std;

vector<string> split(string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    vector<string> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

int main(int argc, char *argv[])
{   
    bool flag = false;

    string tag = argv[1];
    tag += ".csv";
    ifstream ifs_tag(tag);
    //perror("tag perror  ");
    //printf("\n");
    //cout << tag;
    //printf("a");
    ifstream ifs("geotag_v10.csv");
    //ifstream ifs("test_v1.csv");
    //perror("'test_v1.csv' perror  ");
    if(!ifs_tag){
        flag = true;
    }
    //perror("print error string by perror  ");
    int count = 0;
    while(flag){
        ofstream ofs(tag);
        //perror("new file ");
        //perror("print error string by perror  ");
        string line;
        while (getline(ifs, line)) {
            vector<string> strvec = split(line, ',');
            stringstream ss;
            ss << strvec[5];
            string tag;
            ss >> quoted(tag);
            if(tag.compare(argv[1]) == 0){
                stringstream ssu;
                ssu << strvec[4];
                string url;
                ssu >> quoted(url);
                for(int i = 0;i < 4;i++){
                    cout << strvec[i] << "," ;
                }
                cout << url << endl;
                ofs << strvec[0] << "," << strvec[1] << "," << strvec[2] << "," << strvec[3] << "," << strvec[4] << endl;
                //perror("ofs  ");
                count++;

                while (getline(ifs, line)) {
                    vector<string> strvec2 = split(line, ',');
                    stringstream ss;
                    ss << strvec2[5];
                    string tag;
                    ss >> quoted(tag);

                    if((tag.compare(argv[1])) != 0  || !(count < 100)){
                        return 0;
                    }
                    stringstream ssu;
                    ssu << strvec2[4];
                    string url;
                    ssu >> quoted(url);
                    for(int i = 0;i < 4;i++){
                        cout << strvec2[i] << "," ;
                    }
                    cout << url <<  endl;

                    ofs << strvec2[0] << "," << strvec2[1] << "," << strvec2[2] << "," << strvec2[3] << "," << strvec2[4] << endl;
                    count++;
                }
            }
        }
        return 0;
    }
    string line;
    while (getline(ifs_tag, line)) {
        vector<string> strvec = split(line, ',');
        stringstream ssu;
        ssu << strvec[4];
        string url;
        ssu >> quoted(url);
        for(int i = 0;i < 4;i++){
            cout << strvec[i] << ",";
        }
        cout << url << endl; 
    }
    return 0;
} 
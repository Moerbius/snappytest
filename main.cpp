
#include <fstream>
#include "snappy.h"

using namespace std;

int main(int argc, char *argv[])
{
    string input;
    string output;

    ifstream in("logo.bmp", ios::in | ios::binary);
    if (in)
    {
        string contents;
        in.seekg(0, ios::end);
        contents.resize(in.tellg());
        in.seekg(0, ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        input = contents;
    }
    else
        return 1;


    snappy::Compress(input.data(), input.size(), &output);

    ofstream f("house.zpy", ios::binary | ios::out);
    f << output;



ifstream in2("house.zpy", ios::in | ios::binary);
    if (in)
    {
        string contents;
        in2.seekg(0, ios::end);
        contents.resize(in2.tellg());
        in2.seekg(0, ios::beg);
        in2.read(&contents[0], contents.size());
        in2.close();
        input = contents;
    }

    snappy::Uncompress(input.data(), input.size(), &output);

    ofstream f2("logo2.bmp", ios::binary | ios::out);
    f2 << output;

    return 0;
}

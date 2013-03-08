#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include "snappy.h"

using namespace std;

void compress(string filename);
void decompress(string filename);
void showHelp();

#define VERSION "1.0"

const string FILEEXTENSION = "zpy";

int main(int argc, char *argv[])
{
    int c;

    if(argc == 1)
        showHelp();

    while ((c = getopt(argc, argv, "hc:x:")) != -1)
    {
        switch (c)
        {
        case 'h':
            showHelp();
            break;
        case 'c':
            compress(optarg);
            break;
        case 'x':
            decompress(optarg);
            break;
        case '?':
            if (optopt == ':')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            exit(1);
        default:
            showHelp();
            break;
        }
    }

    for (int index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);

    return 0;
}

void compress(string fn)
{
    string input;
    string output;

    string filename = fn.substr(0, fn.length() - 4);

    string filecompressed = "";

    //Put the filename into the begining of the data
    input = fn;

    ifstream in(fn.c_str(), ios::in | ios::binary);
    if (in)
    {
        string contents;
        in.seekg(0, ios::end);
        contents.resize(in.tellg());
        in.seekg(0, ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        //Append the uncompressed data to the variable containing the file name
        input.append(contents);
    }
    else
        exit(1);

    //Compress the stream containing the filename and the data
    snappy::Compress(input.data(), input.size(), &output);

    filecompressed.append(filename).append(".").append(FILEEXTENSION);

    //Save the compressed data to the disk
    ofstream f(filecompressed.c_str(), ios::binary | ios::out);
    f << output;
}

void decompress(string fn)
{
    string input;
    string output;
    string fileoriginal = "";

    ifstream in(fn.c_str(), ios::in | ios::binary);
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

    snappy::Uncompress(input.data(), input.size(), &output);

    //Remove the filename from the uncompressed data
    fileoriginal = output.substr(0, output.find(".") + 4);
    output.erase(0, fileoriginal.size());

    //Save the uncompressed data to disk
    ofstream f2(fileoriginal.c_str(), ios::binary | ios::out);
    f2 << output;
}

void showHelp()
{
    printf("Snappy Test %s\n\n", VERSION);
    printf("Usage: snappytest <options> <filename>\n\n");
    printf("Options:\n");
    printf("     -c  Compress a file\n");
    printf("     -x  Extract a file\n\n");
    printf("Example:\n");
    printf("  snappytest -c logo.bmp\n");
    exit(0);
}

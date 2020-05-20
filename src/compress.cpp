/**
 * TODO: file header
 *
 * Author:
 */
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

/* TODO: add pseudo compression with ascii encoding and naive header
 * (checkpoint) */
void pseudoCompression(const string& inFileName, const string& outFileName) {
    ifstream myfile;
    ofstream myOutputFile;
    char inputArray;
    myfile.open (inFileName, ios::binary);
    vector <unsigned int> freqs;

    while(myfile.get(inputArray)){
        freqs[inputArray]++;
    }
    HCTree newTree;
    newTree.build(freqs);
    myOutputFile.open(outFileName);

    for(int i = 0; i < 256; i++){
        myOutputFile << freqs[i] << endl;
    }
    myfile.close();
    myfile.open(inFileName, ios::binary);

    while(myfile.get(inputArray)){
        newTree.encode(inputArray, myOutputFile);
    }
    myfile.close();
    myOutputFile.close();

    //array should be the vector for the freqs
    // go thoruhg file again, call encode to each character using the tree built before



    //decompress
     //from 0 to 256 read inFIlename, fill array according to the header
    //pass thisarray tree.build()
    // while loop to call decode on each character, use the tree
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(const string& inFileName, const string& outFileName) {}

/* Main program that runs the compression */
int main(int argc, char* argv[]) {
    cxxopts::Options options(argv[0],
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        return 0;
    }

    // if original file is empty, output empty file
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName, ios::out);
        outFile.close();
        return 0;
    }

    if (isAsciiOutput) {
        pseudoCompression(inFileName, outFileName);
    } else {
        trueCompression(inFileName, outFileName);
    }

    return 0;
}
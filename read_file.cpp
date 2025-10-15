#include<iostream>
#include<string>
#include<fstream>

using namespace std;

main(int argc, char* argv[]){
    if (argc != 2){
        cerr << "You need to include the name of the file that you want to read!" << endl;
        return 1;
    }
    string file_name = string(argv[1]);
    std::ifstream inputFile("recording.bin", std::ios::in | std::ios::binary);
    if(inputFile){
        inputFile.seekg(0, std::ios::end); // Move to end of file
        long fileSize = inputFile.tellg(); // Get current position (file size in bytes)
        inputFile.seekg(0, std::ios::beg);
        unsigned char read_size;
        inputFile.read(reinterpret_cast<char*>(&read_size), sizeof(unsigned char));
        cout << "Read size: " << static_cast<int>(read_size) << endl;
        if(read_size != sizeof(double)){
            cerr << "WARNING! The size of double on this system is " << sizeof(double) << " bytes, but the expected value is: " << read_size << endl;
            return 1;
        }
        double refresh_rate;
        inputFile.read(reinterpret_cast<char*>(&refresh_rate), sizeof(double));
        cout << "Refresh Rate: " << refresh_rate << endl;
    } else {
        cerr << "There was an error opening the file." << endl;
    }
}
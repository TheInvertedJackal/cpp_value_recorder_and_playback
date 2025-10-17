#include<iostream>
#include<string>
#include<fstream>

#include "../bin_struct/playback_bin.hpp"

using namespace std;

/*
* This is a simple read program, not ment for production, but a useful tool to help read the output of the recording files to ensure that we're getting the right data.
* */

void read_and_print(long buff_size, std::ifstream* file){
    if(buff_size % sizeof(double) != 0){
        cerr << "You need to update the buffer size so that it's a mulitple of the size of doubles: " << sizeof(double) << endl;
        return;
    }
    long doubles_in_buff = buff_size / sizeof(double);
    double* buffer = new double[doubles_in_buff];
    file->read(reinterpret_cast<char*>(buffer), buff_size);
    for (int i = 0; i < doubles_in_buff; i++){
        cout << buffer[i] << endl;
    }
    delete[] buffer;
}

main(int argc, char* argv[]){
    if (argc != 2){
        cerr << "You need to include the name of the file that you want to read!" << endl;
        return 1;
    }
    string file_name = string(argv[1]);
    std::ifstream inputFile(file_name.c_str(), std::ios::in | std::ios::binary);
    if(inputFile){
        inputFile.seekg(0, std::ios::end); // Move to end of file
        long fileSize = inputFile.tellg(); // Get current position (file size in bytes)
        inputFile.seekg(0, std::ios::beg);
        playback_header header;
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

        // Read file in chunks, not all at once
        long left_to_read = fileSize - sizeof(double) - sizeof(unsigned char);
        long default_buffer_size = 16384;
        while(default_buffer_size < left_to_read){
            read_and_print(default_buffer_size, &inputFile);
            left_to_read -= default_buffer_size;
        }
        read_and_print(left_to_read, &inputFile);
        inputFile.close();
    } else {
        cerr << "There was an error opening the file." << endl;
    }
}
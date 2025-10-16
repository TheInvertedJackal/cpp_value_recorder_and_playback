#include "playback_value.hpp"

#include <fstream>
#include <iostream>

using namespace CPP_Value_Manipulation;
using namespace std;

constexpr int meta_bytes = sizeof(double) + sizeof(unsigned char); // Meta data in file to be skipped over.

/*
* Simple linear transformatin to help with smoothing values out if needed.
* Notes about use:
* You'll likely want to not take it to the final step, as you'll start the next number vlaues.
* */
double easing(double from, double to, int step, int total_steps){
    double till_comp = (total_steps * 1.0 - step) / total_steps; //Starts at 1 wehn step == 0 and ends at 0 when step == total_steps
    return from * (till_comp) + to * (1-till_comp);
}

void read_and_print(double* buffer, int count, std::ifstream* file){
    file->read(reinterpret_cast<char*>(buffer), count * sizeof(double));
}

void thread_function(thread_data* data){
    data->file_to_read->seekg(meta_bytes); // Set back to start
    int doubles_in_buff = 1024;
    double* buffer = new double[doubles_in_buff];
    while(*(data->playing) && !*(data->finished_reading)){

    }
}

PlaybackValue::PlaybackValue(double* value, std::string record_file, int easing_steps = 0){
    this->value_to_manipulate = value;
    this->is_playing = false;
    this->easing_steps = easing_steps;
    load_file(record_file);
}

PlaybackValue::~PlaybackValue(){
    stop_playback();
    delete file_to_read;
}

void PlaybackValue::load_file(std::string file_loc){
    // Load file in
    file_to_read = new std::ifstream(file_loc.c_str(), std::ios::in | std::ios::binary);
    if(!file_to_read) throw std::exception("There was an error opeing the input file!");

    // Get total file size
    file_to_read->seekg(0, std::ios::end); // Move to end of file
    file_size = file_to_read->tellg(); // Get current position (file size in bytes)
    file_to_read->seekg(0, std::ios::beg);
    if(file_size <= meta_bytes) throw std::exception("Cannot work with empty file!");

    // Ensure that doubles are the right size (Likely)
    unsigned char read_size;
    file_to_read->read(reinterpret_cast<char*>(&read_size), sizeof(unsigned char));
    if(read_size != sizeof(double)) throw std::exception("The expected double size and the system arch double size don't align!");

    file_to_read->read(reinterpret_cast<char*>(&refresh_rate), sizeof(double));
}

void PlaybackValue::start_playback(){
    if(is_playing) return;
    is_playing = true;
    finished_reading = false;
    work_thread = thread(thread_function);
}



void PlaybackValue::stop_playback(){
    if(!is_playing) return;
    is_playing = false;
    work_thread.join();
}

bool PlaybackValue::is_finished(){
    return finished_reading;
}
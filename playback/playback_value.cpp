#include "playback_value.hpp"
#include "../bin_struct/playback_bin.hpp"
#include "../time_utils/nano_current.hpp"

#include <fstream>
#include <iostream>

using namespace CPP_Value_Manipulation;
using namespace std;

constexpr int meta_bytes = sizeof(playback_header); // Meta data in file to be skipped over.

inline int calc_read(double* buffer, int file_size, int already_read, int buff_size, std::ifstream* file){
    if(file_size - (already_read + buff_size) < 0) buff_size = file_size % buff_size; // If the next read will go over the eof, set the read to the end of the file
    file->read(reinterpret_cast<char*>(buffer), buff_size * sizeof(double));
    return buff_size;
}

void thread_function(thread_data* data){
    data->file_to_read->seekg(meta_bytes); // Set back to start
    int doubles_in_buff = 1024;
    double* buffer = new double[doubles_in_buff]; // Create buffer

    int total_doubles = (data->file_size - sizeof(playback_header)) / sizeof(double);
    int read_doubles = 0;
    int played_doubles = 0;

    while(total_doubles > read_doubles && *(data->playing)){
        long long start_time = timeutils::get_current_nano_time();
        read_doubles += calc_read(buffer, total_doubles, read_doubles, doubles_in_buff, data->file_to_read);
        long long end_time = timeutils::get_current_nano_time();
        long long rest_time_ns = static_cast<long long>(data->refresh_rate * timeutils::ns_in_ms) - (end_time - start_time);
        int double_index = 0;
        while(played_doubles > read_doubles && *(data->playing)){
            if(rest_time_ns > 0) std::this_thread::sleep_for(std::chrono::nanoseconds(rest_time_ns));
            *(data->value_to_change) = buffer[double_index];
            double_index += 1;
            read_doubles += 1;
            rest_time_ns = data->refresh_rate * timeutils::ns_in_ms;
            cout << "Changed!" << endl;
        }
        cout << "Bath Read!" << endl;
    }
    if(*(data->playing)) *(data->finished_reading) = true;
    delete[] buffer;
    delete data;
    *(data->playing) = false;
}

PlaybackValue::PlaybackValue(double* value, std::string record_file){
    this->value_to_manipulate = value;
    this->is_playing = false;
    load_file(record_file);
}

PlaybackValue::~PlaybackValue(){
    stop_playback();
    delete file_to_read;
}

void PlaybackValue::load_file(std::string file_loc){
    // Load file in
    file_to_read = new std::ifstream(file_loc.c_str(), std::ios::in | std::ios::binary);
    if(!file_to_read) throw std::runtime_error("There was an error opeing the input file!");

    // Get total file size
    file_to_read->seekg(0, std::ios::end); // Move to end of file
    file_size = file_to_read->tellg(); // Get current position (file size in bytes)
    file_to_read->seekg(0, std::ios::beg);
    if(file_size <= meta_bytes) throw std::runtime_error("Cannot work with empty file!");

    // Load header data
    playback_header header;
    file_to_read->read(reinterpret_cast<char*>(&header), sizeof(playback_header));
    if(header.double_size != sizeof(double)) throw std::runtime_error("The expected double size and the system arch double size don't align!");
    refresh_rate = header.refresh_rate;
}

void PlaybackValue::start_playback(){
    if(is_playing) return;
    is_playing = true;
    finished_reading = false;
    work_thread = thread(thread_function, get_thread_package());
}

thread_data* PlaybackValue::get_thread_package(){
    thread_data* ret_val = new thread_data;
    ret_val->value_to_change = value_to_manipulate;
    ret_val->file_to_read = file_to_read;
    ret_val->file_size = file_size;
    ret_val->refresh_rate = refresh_rate;
    ret_val->playing = &is_playing;
    ret_val->finished_reading = &finished_reading;
    return ret_val;
}

void PlaybackValue::stop_playback(){
    if(!is_playing) return;
    is_playing = false;
    work_thread.join();
}

void PlaybackValue::restart_playback(){
    stop_playback();
    start_playback();
}

bool PlaybackValue::is_finished(){
    return finished_reading;
}
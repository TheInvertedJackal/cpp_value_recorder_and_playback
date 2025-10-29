#include "record_value.hpp"
#include "../time_utils/nano_current.hpp"
#include "../bin_struct/playback_bin.hpp"

#include <fstream>
#include <iostream>

using namespace CPP_Value_Manipulation;
using namespace std;

void thread_function(std::ofstream* outputFile, double sample_rate, bool* is_recording, std::atomic<double>* address_to_record){
    if (outputFile->is_open()){
        const char double_size = sizeof(double);
        playback_header header;
        header.double_size = double_size;
        header.refresh_rate = sample_rate;
        outputFile->write(reinterpret_cast<const char*>(&header), sizeof(playback_header));
        outputFile->flush();
        int flush_every_this = static_cast<int>(500 / sample_rate); // Flush every half a second.
        long long count = 0;
        while(is_recording){
            // record value
            double value = *(address_to_record);

            // take current time
            long long rec_time = timeutils::get_current_nano_time();

            // print value (and flush buffer every so often)
            outputFile->write(reinterpret_cast<const char*>(&value), sizeof(double));
            count++;
            if (count % flush_every_this == 0) outputFile->flush();

            // Sleep for remaining time if needed
            long long post_write = timeutils::get_current_nano_time();
            long long work_time = post_write - rec_time;
            long long sleep_time = static_cast<long long>(sample_rate * timeutils::ns_in_ms) - work_time;
            if(sleep_time > 0) std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_time));
        }
        outputFile->flush();
        outputFile->close();
    } else {
        cerr << "There was an error opening the file to record values too." << endl;
    }
}

RecordValue::RecordValue(std::atomic<double>* address_to_record, double record_interval, std::string record_file){
    if(address_to_record == nullptr) throw std::runtime_error("Cannot read from a null pointer!");
    if(record_interval < 0) throw std::runtime_error("Rate cannot be less than 0!");
    this->address_to_record = address_to_record;
    this->sample_rate = record_interval;
    this->file_to_write_to = new std::ofstream(record_file, std::ios::out | std::ios::binary);
    this->is_recording = false;
}

RecordValue::~RecordValue(){
    stop_recording();
}

void RecordValue::start_recording(){
    if(is_recording) return;
    is_recording = true;
    thread work_thread = thread(thread_function, file_to_write_to, sample_rate, &is_recording, address_to_record);
    work_thread.detach();
}

void RecordValue::stop_recording(){
    if(!is_recording) return;
    is_recording = false;
    delete file_to_write_to;
}
#include "record_value.hpp"
#include "../time_utils/nano_current.hpp"

#include <fstream>
#include <iostream>

using namespace CPP_Value_Manipulation;
using namespace std;

const double ns_in_ms = 1000000;

void thread_function(string file_path, double sample_rate, bool* is_recording, double* address_to_record){
    std::ofstream outputFile(file_path, std::ios::out | std::ios::binary);
    if (outputFile.is_open()){
        const char double_size = sizeof(double);
        outputFile.write(reinterpret_cast<const char*>(&double_size), sizeof(const char));
        outputFile.write(reinterpret_cast<const char*>(&sample_rate), sizeof(double));
        outputFile.flush();
        int flush_every_this = 100;
        long long count = 0;
        while(is_recording){
            // record value
            double value = *(address_to_record);
            // take current time
            long long rec_time = timeutils::get_current_nano_time();
            // print value (and flush buffer every so often)
            outputFile.write(reinterpret_cast<const char*>(&value), sizeof(double));
            count++;
            if (count % flush_every_this == 0) outputFile.flush();

            // Sleep for remaining time if needed
            long long post_write = timeutils::get_current_nano_time();
            long long work_time = post_write - rec_time;
            long long sleep_time = static_cast<long long>(sample_rate * ns_in_ms) - work_time;
            if(sleep_time > 0) std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        }
        outputFile.close();
    } else {
        cerr << "There was an error opening the file to record values too." << endl;
    }
}

RecordValue::RecordValue(double* address_to_record, double record_interval, std::string record_file){
    if(address_to_record == nullptr) throw std::runtime_error("Cannot read from a null pointer!");
    if(record_interval < 0) throw std::runtime_error("Rate cannot be less than 0!");
    this->address_to_record = address_to_record;
    this->sample_rate = sample_rate;
    this->file_to_write_to = record_file;
    this->is_recording = false;
}

RecordValue::~RecordValue(){
    stop_recording();
}

void RecordValue::start_recording(){
    if(is_recording) return;
    is_recording = true;
    work_thread = thread(thread_function, file_to_write_to, sample_rate, &is_recording, address_to_record);
}

void RecordValue::stop_recording(){
    if(!is_recording) return;
    is_recording = false;
    work_thread.join();
}
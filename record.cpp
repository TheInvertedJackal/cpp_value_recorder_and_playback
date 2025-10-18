#include "record/record_value.hpp"
#include "time_utils/nano_current.hpp"

#include <iostream>

using namespace CPP_Value_Manipulation;
using namespace std;

int main(){
    double value = 0;
    RecordValue recorder = RecordValue(&value, 10, "./recording.bin");
    recorder.start_recording();
    long long start_time = timeutils::get_current_nano_time();
    while(value < 100000000){
        value += .1;
    }
    recorder.stop_recording();
    long long end_time = timeutils::get_current_nano_time();
    double total_time = (end_time - start_time * 1.0) / timeutils::ns_in_ms / 1000;
    cout << "Run took: " << total_time << "s" << endl;
    return 0;
}
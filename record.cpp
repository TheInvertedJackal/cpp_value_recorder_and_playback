#include "record/record_value.hpp"
#include "time_utils/nano_current.hpp"

#include <iostream>

using namespace CPP_Value_Manipulation;
using namespace std;


int main(int argc, char* argv[]){
    double up_to_value = -1;
    double playback_rate = 10;
    string file_to_read;
    if(argc >= 2){
        file_to_read = string(argv[1]);
    } else {
        cerr << "You need to include the file name to record to!" << endl;
        cerr << "record <file>" << endl;
        return -1;
    }
    if(argc >= 3){
        up_to_value = stod(argv[2]);
        if(up_to_value <= 0){
            cerr << "Value cannot be 0 or less! Leave blank for infinity." << endl;
            cerr << "record <file> <to_value>" << endl;
            return -1;
        }
    }
    if(argc >= 4){
        playback_rate = stod(argv[3]);
        if(playback_rate <= 0){
            cerr << "Playback cannot be 0 or less! Leave blank for 10ms." << endl;
            cerr << "record <file> <to_value> <playback_rate>" << endl;
            return -1;
        }
    }
    double true_value = 0;
    std::atomic<double> value = 0;
    RecordValue recorder = RecordValue(&value, playback_rate, file_to_read);
    recorder.start_recording();
    long long start_time = timeutils::get_current_nano_time();
    while(value < up_to_value || up_to_value == -1){
        true_value += .1;
        value = true_value;
    }
    recorder.stop_recording();
    long long end_time = timeutils::get_current_nano_time();
    double total_time = (end_time - start_time * 1.0) / timeutils::ns_in_ms / 1000;
    if(up_to_value != -1) cout << "Run took: " << total_time << "s" << endl;
    return 0;
}
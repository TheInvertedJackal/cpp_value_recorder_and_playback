#include "playback/playback_value.hpp"
#include "time_utils/nano_current.hpp"
#include <iostream>

using namespace CPP_Value_Manipulation;
using namespace std;

int main(){
    double value = 0;
    PlaybackValue player = PlaybackValue(&value, "./recording.bin");
    bool playing = true;
    player.start_playback();
    long long start_time = timeutils::get_current_nano_time();
    while(playing){
        // Testing if value is trulely stopped once the stop method is called.
        // if(value > 50000000){
        //     player.stop_playback();
        //     int temp = 0;
        //     while(temp < 10000){
        //         temp += 1;
        //         cout << "Stopped: " << value << endl; 
        //     }
        //     break;
        // }
        if(!player.is_finished()){
            //cout << "Looking at: " << value << endl;
        } else {
            long long end_time = timeutils::get_current_nano_time();
            double total_time = (end_time - start_time * 1.0) / timeutils::ns_in_ms / 1000;
            cout << "Run took: " << total_time << "s" << endl;
            string response = "N";
            cout << "Finished playback, go again? (Type 'Y' if Yes)" << endl;
            cin >> response;
            if(response == "Y"){
                player.restart_playback();
            } else {
                playing = false;
            }
        }
    }
    player.stop_playback();
    return 0;
}
#include "playback/playback_value.hpp"
#include "time_utils/nano_current.hpp"
#include <iostream>

using namespace CPP_Value_Manipulation;
using namespace std;

enum Test {STOP, TIME, NORMAL};

int main(int argc, char* argv[]){
    Test choice = NORMAL;
    string file_to_read;
    double stop_at = 0;
    if(argc >= 2){
        file_to_read = string(argv[1]);
    } else {
        cerr << "You need to put the file in that is being played back!" << endl;
        cerr << "playback <file> <option (time/stop)>" << endl;
        return -1;
    }
    if(argc >= 3){
        string arg_choice = string(argv[2]);
        if(arg_choice == "stop") {
            choice = STOP;
            if(argc >= 4){
                stop_at = stod(argv[3]);
            } else {
                cerr << "You need to include the value to stop at!" << endl;
                cerr << "playback <file> stop <stop_at>" << endl;
                return -1;
            }
        }
        if(arg_choice == "time") choice = TIME;
    }
    double value = 0;
    PlaybackValue player = PlaybackValue(&value, file_to_read);
    bool playing = true;
    player.start_playback();
    long long start_time = timeutils::get_current_nano_time();
    while(playing){
        if(choice == STOP){
            if(value > stop_at){
                player.stop_playback();
                int temp = 0;
                while(temp < 10000){
                    temp += 1;
                    cout << "Stopped: " << value << endl; 
                }
                break; // End the playback
            }
        } else if (choice == TIME){
            if(player.is_finished()){
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
        } else {
            if(!player.is_finished()){
                cout << "Looking at: " << value << endl;
            } else {
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

    }
    player.stop_playback();
    return 0;
}
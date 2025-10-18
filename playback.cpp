#include "playback/playback_value.hpp"
#include <iostream>

using namespace CPP_Value_Manipulation;
using namespace std;

int main(){
    double value = 0;
    PlaybackValue player = PlaybackValue(&value, "./recording.bin");
    bool playing = true;
    player.start_playback();
    while(playing){
        if(!player.is_finished()){
            cout << "Looking at: " << value << endl;
        } else {
            string response = "N";
            cout << "Finished playback, go again?" << endl;
            cin >> response;
            if(response == "Y"){
                player.restart_playback();
            } else {
                playing = false;
            }
        }
    }
}
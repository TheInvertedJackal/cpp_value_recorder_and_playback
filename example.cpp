#include "../record/record_value.hpp"
#include "../playback/playback_value.hpp"

using namespace CPP_Value_Manipulation;

int main(){
    // 
    double value = 0;
    double sample_rate = 10;
    RecordValue record_value = RecordValue(&value, sample_rate, "./example.bin");
    record_value.start_recording();
    // Do work
    record_value.stop_recording();

    PlaybackValue playback_value = PlaybackValue(&value, "./example.bin");
    playback_value.start_playback();
    bool stop_playback = false;
    bool restart = true;
    while(true){
        // Do work

        // If we've reached the end of the recording...
        if(playback_value.is_finished()){
            // Should we restart?
            if(restart) playback_value.restart_playback();
        }

        // If we should stop manipulating the value...
        if(stop_playback){
            playback_value.stop_playback();
        }
    }
}
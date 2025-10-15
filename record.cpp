#include "record/record_value.hpp"

using namespace CPP_Value_Manipulation;

int main(){
    double value = 0;
    RecordValue recorder = RecordValue(&value, 1, "./recording.bin");
    recorder.start_recording();
    while(true){
        value += .1;
    }
}
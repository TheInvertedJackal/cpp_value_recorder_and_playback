#ifndef RECORD_CPP_VALUE
#define RECORD_CPP_VALUE

#include <string>
#include <thread>

namespace CPP_Value_Manipulation{
    class RecordValue{
        private:
            double* address_to_record;
            double sample_rate;
            std::string file_to_write_to;
            std::thread work_thread;
            bool is_recording;
        public:
            // The Address of the value to record, the sample rate in milliseconds and the file to record the values too
            RecordValue(double* address_to_record, double sample_rate, std::string record_file);
            ~RecordValue();
            // Starts recoding the value and writing it to the desired file
            void start_recording();
            // Stops the recording to the file, not needed to be used.
            void stop_recording();
    };
}

#endif
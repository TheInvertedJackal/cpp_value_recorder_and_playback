#ifndef PLAYBACK_CPP_VALUE
#define PLAYBACK_CPP_VALUE

#include <string>
#include <thread>

namespace CPP_Value_Manipulation{
    class PlaybackValue{
        private:
            double* address_to_manipulate;
            std::thread work_thread;
        public:
            PlaybackValue(double* address_to_manipulate, std::string record_file);
            // Starts the manipulation on a given address
            void start_manipulation();
            // Ends the manipulation on a given valuable.
            void stop_manipulation();
    };
}

#endif
#ifndef PLAYBACK_CPP_VALUE
#define PLAYBACK_CPP_VALUE

#include <string>
#include <thread>

namespace CPP_Value_Manipulation{

    struct thread_data {
        double* value_to_change;
        std::ifstream* file_to_read;
        double refresh_rate;
        int total_steps;
        bool* playing;
        bool* finished_reading;
        long file_size;
    };
    
    class PlaybackValue{
        private:
            double* value_to_manipulate;
            std::thread work_thread;
            std::ifstream* file_to_read;
            int easing_steps;
            bool is_playing;
            bool finished_reading;
            long file_size;
            double refresh_rate;
            void load_file(std::string file_loc);
            thread_data* get_thread_package();
        public:
            PlaybackValue(double* value, std::string record_file, int easing_steps = 0);
            ~PlaybackValue();
            // Starts the manipulation on a given address
            void start_playback();
            // Ends the manipulation on a given valuable.
            void stop_playback();
            // Restarts the playback, even if not finished
            void restart_playback();
            bool is_finished();
    };
}

#endif
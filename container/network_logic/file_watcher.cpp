#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <string>
#include <cassert>

namespace fs=std::filesystem;

class FileWatcher{
    private:
        std::string file_path;
    public:
    bool checker=false; // this will alert who use him that the file have been changed and need to be manualy replace to false after
    //constructor
        FileWatcher(std::string path){
            if(!fs::exists(path)){
                assert(false&& "File not found");
                file_path=path;
            }
        }
        int TrackFile(void){
            auto last_write_time = fs::last_write_time(file_path);

            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(1));  // wait one second between each check
                auto current_write_time = fs::last_write_time(file_path);
                // i check the difference between each verification
                if (current_write_time != last_write_time) {
                    checker=true;
                    last_write_time = current_write_time;
                }
    }
        }
};
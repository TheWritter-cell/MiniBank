#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <cassert>
#include <vector>
#include <filesystem>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>




int main() {
    for (int i = 0; i < 5; ++i) {
        std::string id = generateID();
        std::cout << "ID généré : " << id << std::endl;
    }
    return 0;
}


class DataManager{
    private:
        // db sqlite
        SQLite::Database db;
    public:
        // contains the path of the database
        std::filesystem::path DataBasePath;

        //constructor
        DataManager(std::filesystem::path filepath){
            try{
                db(filepath.string(),SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
                return 0;
            }catch(std::exception& e){
                std::cerr << "Something went wrong :(  " << "\" " << e.what() << " \"" << std::endl;
                assert(false && "Problem raised , solve it ");
            }
        }
        

        int CreateTable(st::string type_of_table){
            std::string Admin="admin";
            std::string Users="Users";
            if(type==Admin){
                db.exc("CREATE TABLE IF NOT EXISTS ADMIN (id TEXT PRIMARY KEY NOT NULL,NAME TEXT NOT NULL, PASSWORD TEXT NOT NULL)");
            }else if(type==Users){
                db.exc("CREATE TABLE IF NOT EXISTS USER (id TEXT PRIMARY KEY NOT NULL,NAME TEXT NOT NULL,BALANCE NUMERIC, PASSWORD NOT NULL)");
            }else{
                assert(false && "Something went wrong with table create retry please")
            }
        }
        std::string generateID() {
            // Take the current time as seed
            auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            std::mt19937_64 rng(now); // randmon 64 bits generator
            std::uniform_int_distribution<uint64_t> dist;

            std::stringstream ss;
            ss << std::hex << std::setfill('0');

            // Generate 4 blocs of 4 octets (as an UUID in a simple version)
            for (int i = 0; i < 4; ++i) {
                ss << std::setw(16) << dist(rng);
                if (i != 3) ss << "-";
            }

            return ss.str();
}

}








int main(){
    std::ifstream fichier_2{"tr.txt"};
    std::string mot;

    while(std::getline(fichier_2,mot)){
        std::cout<<"Ligne lue : "<<mot<<std::endl;
    }
    return 0;
}

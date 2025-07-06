#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <cassert>
#include <vector>
#include <filesystem>


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
            std::string Admin="admin"
            if(type==Admin){
                db.exc("CREATE TABLE IF NOT EXISTS ADMIN (id PRIMARY KEY NOT NULL,NAME TEXT NOT NULL, PASSWORD)")
            }
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

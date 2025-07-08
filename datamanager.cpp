#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <cassert>
#include <vector>
#include <filesystem>
#include <random>
#include <chrono>
#include <sstream>
#include <cstdio>
#include <iomanip>
#include <array>

// Ik know there are too much include but i'm not a genius i use what i find in the internet chatgpt not really helpful :(

// This function convert everything into a string i'm proud i made it by myself :)
template<typename T>
std::string to_str(const T& val){
    std::ostringstream oss;
    oss << val;
    return oss.str();
}

// this function will collect argument and return it in string
// i didn't made it i found it on github
template<typename... Args>
std::vector<std::string> collect_args(Args&&... args){
    return {to_str(std::forward<Args>(args))...};
}
/** This function have been made by me after 3days of research so if you found how to ameliorate it
Please do it because it's realy hard to maintain*/ 
template<typename... Args>
std::string execPythonScript(Args&&... args) {
    std::vector<std::string> arglist = collect_args(std::forward<Args>(args)...);
    std::size_t nb_args = sizeof...(args);
    if (nb_args > 3) {
        assert(false && "Too few arguments; you can only send 3 ");
    }

    std::array<char, 128> buffer;
    std::string result;
    std::ostringstream oss;
    std::string command = "";

    if (arglist[0] == "crypt" && nb_args == 2) {
        oss << "python python_utilities/python_tools.py " << arglist[0] << " " << arglist[1];
        command = oss.str();
    } else if (arglist[0] == "check" && nb_args == 3) {
        oss << "python python_utilities/python_tools.py " << arglist[0] << " " << arglist[1] << " " << arglist[2];
        command = oss.str();
    } else {
        return "NONE";
    }

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Error during the python execution");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}

// Compile with on MinGW64 terminal i'm on windows for linux i don't know
// g++ datamanager.cpp -o db -I "/c/Program Files (x86)/SQLiteCpp/include" -L "/c/Program Files (x86)/SQLiteCpp/lib" -lSQLiteCpp -lsqlite3 

class DataManager {
private:
    // SQLite database object
    SQLite::Database db;

public:
    // Path to the database file
    std::filesystem::path DataBasePath;

    // Constructor: opens or creates the database
    DataManager(std::filesystem::path filepath)
        : db(filepath.string(), SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
          DataBasePath(filepath) {
        // All initialization done in the initializer list
    }

    // Method to create tables based on the type
    int CreateTable(const std::string& type_of_table) {
        std::string Admin = "admin";
        std::string Users = "Users";

        // Create the ADMIN table
        if (type_of_table == Admin) {
            db.exec("CREATE TABLE IF NOT EXISTS ADMIN ("
                    "id TEXT PRIMARY KEY NOT NULL, "
                    "NAME TEXT NOT NULL, "
                    "PASSWORD TEXT NOT NULL)");
            return 0;

        // Create the USER table
        } else if (type_of_table == Users) {
            db.exec("CREATE TABLE IF NOT EXISTS USER ("
                    "id TEXT PRIMARY KEY NOT NULL, "
                    "NAME TEXT NOT NULL, "
                    "BALANCE NUMERIC, "
                    "PASSWORD TEXT NOT NULL)");
            return 0;

        // If type is unknown, raise an error
        } else {
            assert(false && "Invalid table type. Please retry with a correct type.");
        }

        return -1; // Should never reach here
    }

    // Generate a unique hexadecimal ID (UUID-style)
    std::string generateID() {
        auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937_64 rng(now);  // 64-bit random number generator
        std::uniform_int_distribution<uint64_t> dist;

        std::stringstream ss;
        ss << std::hex << std::setfill('0');

        // Generate 4 blocks of 16 hex digits each
        for (int i = 0; i < 4; ++i) {
            ss << std::setw(16) << dist(rng);
            if (i != 3) ss << "-";
        }

        return ss.str();
    }
    // Code For add new user in the table
    int InsertUser(std::string name,const char* password ,long long int balance=0){
        SQLite::Statement insert(db,"INSERT INTO USER (id,NAME , BALANCE,PASSWORD) VALUES (?,?,?,?)");
        try{
            std::string id=generateID();
            std::string hash_psw=execPythonScript("crypt",password);
            insert.bind(1,id);
            insert.bind(2,name);
            insert.bind(3,balance);
            insert.bind(4,hash_psw);
            insert.exec();
            return 0;
        }catch(std::exception& e){
            assert(false && e.what());
        }
    }
};

int main() {
    try {
        // Create the DataManager object and open the database
        DataManager db("database.db");

        // Create the Users table
        db.InsertUser("pai","ijio",450);

    } catch (const std::exception& e) {
        // Handle any exceptions thrown by SQLite
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

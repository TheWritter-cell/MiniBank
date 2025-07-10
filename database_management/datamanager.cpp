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

#define pass do {} while(0) // i just wanted a pass like python :)
// Ik know there are too much include but i'm not a genius i use what i find in the internet chatgpt not really helpful :(

// Compile with on MinGW64 terminal i'm on windows for linux i don't know
// g++ datamanager.cpp -o db -I "/c/Program Files (x86)/SQLiteCpp/include" -L "/c/Program Files (x86)/SQLiteCpp/lib" -lSQLiteCpp -lsqlite3 
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
        oss << "python python_tools.py " << arglist[0] << " " << arglist[1];
        command = oss.str();
    } else if (arglist[0] == "check" && nb_args == 3) {
        oss << "python python_tools.py " << arglist[0] << " " << arglist[1] << " " << arglist[2];
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


// This will be the only code who will be allowed to change database 
class DataManager {
private:
    // SQLite database object
    SQLite::Database db;
    std::vector<std::string> db_field_USER{"id","NAME","PASSWORD","BALANCE"};
    std::vector<std::string> possible_transaction{"withdraw","deposit","transfer"};
    bool checker=false;
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
        //take the time as seed for the id
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
            if(hash_psw=="0" || hash_psw=="NONE"){
                assert(false && "Something went wrong during hashing password please fix and retry");
                return 1;// You should never arrive there i think but it's just for safety
            }
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
    int InsertAdmin(std::string name,const char* password){
        SQLite::Statement insert(db,"INSERT INTO ADMIN(id,NAME,PASSWORD) VALUES(?,?,?)");
        try{
            std::string id=generateID();
            std::string hash_psw=execPythonScript("crypt",password);
            insert.bind(1,id);
            insert.bind(2,name);
            insert.bind(3,hash_psw);
            insert.exec();
            return 0;
        }catch(std::exception& e){
            assert(false && e.what());
        }
    }
    int Update_Data(std::string Admin_id,std::string Admin_psw , 
        std::string User_id,std::string field,std::string new_field){
        bool table_checker=false;
        
        for(auto const element : db_field_USER){
            if(field==element){
                table_checker=true;
                break;
            }
        }

        if(table_checker){
            pass;
        }else{
            assert(false&& "This Field doesn't exist");
            return 1;
        }
        SQLite::Statement query(db, "SELECT PASSWORD FROM ADMIN WHERE id = ?");
        query.bind(1, Admin_id);
        std::string psw;
        if (query.executeStep()){
            psw=query.getColumn(0).getString();
        }else{
            assert(false && "No Admin found with this ID");
            return 1;
        }
        std::string checked=execPythonScript("check",psw,Admin_psw);
        if(checked=="0"){
            pass;
        }else{
            assert(false&& "Admin Password doesn't correspond please retry with the right password");
            return 1;
        }
        std::string query_str = "UPDATE USER SET " + field + " = ? WHERE id = ?";
        SQLite::Statement update(db, query_str);
        update.bind(1,new_field);
        update.bind(2,User_id);
        update.exec();
        return 0;
    }
    int Update_Balance(std::string Admin_id, std::string Admin_psw,
                   std::string User_id, std::string transaction,
                   std::string amount_str, std::string possible_receiver = "") {
    // Vérifier que l'opération est reconnue
    bool valid_operation = false;
    for (const auto& element : possible_transaction) {
        if (element == transaction) {
            valid_operation = true;
            break;
        }
    }
    if (!valid_operation) {
        assert(false && "Unknown operation");
        return 1;
    }

    // Vérifier le mot de passe admin
    SQLite::Statement query(db, "SELECT PASSWORD FROM ADMIN WHERE id = ?");
    query.bind(1, Admin_id);
    std::string psw;
    if (query.executeStep()) {
        psw = query.getColumn(0).getString();
    } else {
        assert(false && "No Admin found with this ID");
        return 1;
    }
    std::string checked = execPythonScript("check", psw, Admin_psw);
    if (checked != "0") {
        assert(false && "Admin Password doesn't correspond please retry with the right password");
        return 1;
    }

    // Convertir montant
    long long amount = std::stoll(amount_str);

    // Récupérer le solde actuel de l'utilisateur
    SQLite::Statement user_query(db, "SELECT BALANCE FROM USER WHERE id = ?");
    user_query.bind(1, User_id);
    long long user_balance = 0;
    if (user_query.executeStep()) {
        user_balance = user_query.getColumn(0).getInt64();
    } else {
        assert(false && "User not found");
        return 1;
    }

    if (transaction == "deposit") {
        user_balance += amount;
        SQLite::Statement update(db, "UPDATE USER SET BALANCE = ? WHERE id = ?");
        update.bind(1, user_balance);
        update.bind(2, User_id);
        update.exec();
    }

    else if (transaction == "withdraw") {
        if (amount > user_balance) {
            assert(false && "Insufficient balance");
            return 1;
        }
        user_balance -= amount;
        SQLite::Statement update(db, "UPDATE USER SET BALANCE = ? WHERE id = ?");
        update.bind(1, user_balance);
        update.bind(2, User_id);
        update.exec();
    }

    else if (transaction == "transfer" and amount>0) {
        // Vérifier que le receiver existe
        SQLite::Statement receiver_query(db, "SELECT BALANCE FROM USER WHERE id = ?");
        receiver_query.bind(1, possible_receiver);
        long long receiver_balance = 0;
        if (receiver_query.executeStep()) {
            receiver_balance = receiver_query.getColumn(0).getInt64();
        } else {
            assert(false && "Receiver not found");
            return 1;
        }

        if (amount > user_balance) {
            assert(false && "Insufficient balance for transfer");
            return 1;
        }

        // Mettre à jour le solde du sender
        user_balance -= amount;
        SQLite::Statement update_sender(db, "UPDATE USER SET BALANCE = ? WHERE id = ?");
        update_sender.bind(1, user_balance);
        update_sender.bind(2, User_id);
        update_sender.exec();

        // Mettre à jour le solde du receiver
        receiver_balance += amount;
        SQLite::Statement update_receiver(db, "UPDATE USER SET BALANCE = ? WHERE id = ?");
        update_receiver.bind(1, receiver_balance);
        update_receiver.bind(2, possible_receiver);
        update_receiver.exec();
    }
        return 0;
    }
    std::vector<std::string> check_user_data(std::string Admin_id, std::string Admin_psw, std::string User_id) {
        std::vector<std::string> container;
        SQLite::Statement query(db, "SELECT PASSWORD FROM ADMIN WHERE id = ?");
        query.bind(1, Admin_id);
        std::string psw;
        if (query.executeStep()) {
            psw = query.getColumn(0).getString();
        } else {
            assert(false && "No Admin found with this ID");
            return {};  // renvoyer un vecteur vide
        }

        std::string checked = execPythonScript("check", psw, Admin_psw);
        if (checked != "0") {
            assert(false && "Admin Password doesn't correspond please retry with the right password");
            return {};
        }

        SQLite::Statement get(db, "SELECT NAME, BALANCE FROM USER WHERE id = ?");
        get.bind(1, User_id);
        try {
            while (get.executeStep()) {
                std::string name = get.getColumn(0).getString();
                std::string balance = to_str(get.getColumn(1).getInt());
                container.push_back(name);
                container.push_back(balance);
            }
            return container;
        } catch (std::exception& e) {
            assert(false && e.what());
            return {};
        }
    }

};








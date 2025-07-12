#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <string>
#include <vector>
#include <filesystem>
#include <SQLiteCpp/SQLiteCpp.h>

class DataManager {
private:
    SQLite::Database db;
    std::vector<std::string> db_field_USER;
    std::vector<std::string> possible_transaction;

public:
    std::filesystem::path DataBasePath;

    DataManager(std::filesystem::path filepath);

    int CreateTable(const std::string& type_of_table);
    std::string generateID();
    int InsertUser(std::string name, const char* password, long long int balance = 0);
    int InsertAdmin(std::string name, const char* password);
    int Update_Data(std::string Admin_id, std::string Admin_psw,
                    std::string User_id, std::string field, std::string new_field);
    int Update_Balance(std::string Admin_id, std::string Admin_psw,
                       std::string User_id, std::string transaction,
                       std::string amount_str, std::string possible_receiver = "");
    std::vector<std::string> check_user_data(std::string Admin_id, std::string Admin_psw, std::string User_id);
};

#endif // DATAMANAGER_HPP

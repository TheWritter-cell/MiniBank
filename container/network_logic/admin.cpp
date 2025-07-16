#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <string>
#include <cassert>
#include <fstream>
#include "json.hpp"
#include "../database_management/datamanager.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

class Admin {
private:
    json admin_data;
    std::string file_path;
    DataManager db("database.db")

public:
    // Constructeur
    Admin(const std::string& path) : file_path(path) {
        if (!fs::exists(file_path)) {
            std::cerr << "Fichier non trouvé: " << file_path << std::endl;
            assert(false && "Fichier introuvable !");
        }

        std::ifstream file{file_path};
        if (file.is_open()) {
            file >> admin_data;
            file.close();
        } else {
            std::cerr << "Impossible d’ouvrir le fichier: " << file_path << std::endl;
        }
    }

    int ExecUserDemand(json data){
        json result;
        if(data["action"]=="deposit"){
            try{
                db.Update_Balance(
                    admin_data["id"],admin_data["password"],data["id"],data["action"],data["amount"]
                );
                result["status"]="OK";
                result["details"]="Deposit successful in your account";
            }catch(const AppError& e){
                result["status"] = "error";
        result["message"] = e.what();
        } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = std::string("Erreur inattendue: ") + e.what();
        }
            
            }
    }

    // Méthode pour afficher les données (pour test)
    void afficher() const {
        std::cout << admin_data.dump(4) << std::endl;
    }
};

int main(){
    Admin add("admin_data_1.json");
    add.afficher();
}
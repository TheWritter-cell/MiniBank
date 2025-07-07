#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>


template<typename T,typename... Args>
std::string execPythonScript(T first,Args... others){
    std::array<char,128> buffer;
    std::string result;
    std::unique_ptr<FILE,decltype(&pclose)> pipe(popen("python ../python_utilities/python_tools.py","r"),pclose);
    if(!pipe){
        throw std::runtime_error("Error during the python execution code");
    }
    while (fgets(buffer.data(),buffer.size(),pipe.get())!=nullptr){
        result+=buffer.data();
    }
    if(!result.empty() && result.back()=='\n'){
        result.pop_back();
    }
    return result;
}

int main(){
    try{
        std::string code=execPythonScript();
        std::cout << "Code recu de python: "<< code <<std::endl;
    }catch(const std::exception& e){
        std::cerr << "Erreur: " << e.what() << std::endl;
    }
    return 0;
}


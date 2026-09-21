#include <iostream>
#include <string>
#include <sstream>
#include <fstream> // librería por excelencia para leer y escribir archivos
#include <cstdlib> // librería para usar Errores

using namespace std;

string rollingHashComparison(const std::string& transmission1, const std::string& transmission2, const std::string& mcode1, const std::string& mcode2, const std::string& mcode3) {
    

    return ""; // Ejemplo de retorno
}


// funcion auxiliar para cargar archivos a un string
string readFileToString(const std::string& filePath){
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "CRITICAL ERROR: No se pudo abrir " << filePath << "\n";
        std::exit(EXIT_FAILURE);
    }
    //Este metodo es mucho mas eficiente pues parsea todo el archivo a velocidad de hardware
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}



int main(){

    //Paso 1: Apertura de archivos en strings con validación estricta
    string transmission1 = readFileToString("transmission1.txt");
    string transmission2 = readFileToString("transmission2.txt");
    string mcode1 = readFileToString("mcode1.txt");
    string mcode2 = readFileToString("mcode2.txt");
    string mcode3 = readFileToString("mcode3.txt");



    //Paso 2: llamadas a funciones (Aqui llamen a sus respecyivas funciones y regresen sus resultados a variables string)
    std::string resultado1 = rollingHashComparison(transmission1,transmission2,mcode1,mcode2,mcode3);
    std::string resultado2;
    std::string resultado3;



    return 0; 
}
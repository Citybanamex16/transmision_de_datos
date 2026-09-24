#include <iostream>
#include <string>
#include <sstream>
#include <fstream> // librería por excelencia para leer y escribir archivos
#include <cstdlib> // librería para usar Errores
#include <vector>
#include <filesystem> // librería para obtener la ruta de ejecución del programa
#include <cstdint>

using namespace std;



// ==== Configuracion de directorio (Realizado con Inteligencia Artificial) ====
// Detectar el sistema operativo para incluir las librerías nativas correctas
#if defined(_WIN32)
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#else
    #include <unistd.h> // Para Linux
#endif

// Función inteligente que reubica el programa en su propia carpeta física
void autoLocalizarDirectorio() {
    try {
        std::filesystem::path ruta_binario;

        #if defined(_WIN32)
            wchar_t path[MAX_PATH];
            GetModuleFileNameW(NULL, path, MAX_PATH);
            ruta_binario = std::filesystem::path(path);
        #elif defined(__APPLE__)
            char path[1024];
            uint32_t size = sizeof(path);
            if (_NSGetExecutablePath(path, &size) == 0) {
                ruta_binario = std::filesystem::path(path);
            }
        #else // Linux
            char path[1024];
            ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
            if (len != -1) {
                path[len] = '\0';
                ruta_binario = std::filesystem::path(path);
            }
        #endif

        // Si logramos rastrear el binario, cambiamos el "CWD" a su carpeta contenedora
        if (!ruta_binario.empty()) {
            std::filesystem::current_path(ruta_binario.parent_path());
        }
    } catch (...) {
        // Alerta silenciosa: Si falla la autolocalización, dejamos que el IDE decida
        std::cerr << "[Aviso] No se pudo forzar la ruta local. Usando ruta del sistema.\n";
    }
}

// === Fin Configuracion de directorio (IA) ====


// === Funciones Rolling Hash ===
// funcion para buscar un patron en un texto usando el algoritmo de Rabin-Karp
int searchPattern(const std::string& text, const std::string& pattern){
    int n = text.length();
    int m = pattern.length();
    
    //Caso base: si el patrón es más largo que el texto, imposible una coincidencia
    if (m > n || m == 0) {
        return 0;
    }

    //constante sugeridas para Hash
    long long p = 31; // Base para el hash
    long long mod = 1e9 + 7; // Módulo para evitar desbordamiento

    //1.Precalculamos p^(m-1) % mod para usarlo en la eliminación del primer carácter del hash
    long long p_pow = 1;
    for (int i = 0; i < m - 1; i++){
        p_pow = (p_pow * p) % mod; // Esta linea calcula la potencia mas alta: p^(m-1)
    }

    // 2. Calcular el hash del patron y de los primeros m caracteres del texto
    long long pattern_hash = 0;
    long long text_hash = 0;
    for (int i = 0; i < m; i++){
        pattern_hash = (pattern_hash * p + pattern[i]) % mod; // Hash/Ventana del patrón: [C,D,E]
        text_hash = (text_hash * p + text[i]) % mod; // Primera Hash/ventana del texto: [A,B,C],D,E,F
    }
    // 3. CICLO PRINCIPAL: Deslizar la ventana por todo el texto, buscando el patron
    // Logica: ¿Hash("ABC") == Hash("CDE")?
    for (int i = 0; i <= n - m; i++) {
        
        // A. ¿Coinciden los Hashes?
        if (text_hash == pattern_hash) {
            // Confirmación carácter por carácter para evitar falsos positivos por colisión
            if (text.substr(i, m) == pattern) {
                return i + 1; // Retornamos la posición basada en 1
            }
        }

        // B. Si no estamos al final, recalculamos el hash para la SIGUIENTE ventana (i + 1)
        if (i < n - m) {
            // 1. Restamos el carácter saliente (text[i])
            //¡Ojo! En programación las restas seguidas de modulo pueden dar resultados negativos, por eso sumamos mod antes de aplicar el módulo nuevamente
            long long term_to_remove = (text[i] * p_pow) % mod;
            text_hash = (text_hash - term_to_remove + mod) % mod;
            // 2. Multiplicamos por la base (esto empuja todos los digitos una pos a la izquierda) 
            //y sumamos el carácter entrante (text[i + m])
            text_hash = (text_hash * p + text[i + m]) % mod;
        }
    }

    return 0;
}

//funcion de limpieza de cadenas (\n y \r y espacios)
string cleanString(const string& str) {
    string cleaned = "";
    for (char c : str) {
        // Conservar SOLO caracteres hexadecimales (0-9, A-F, a-f)
        if ((c >= '0' && c <= '9') || 
            (c >= 'A' && c <= 'F') || 
            (c >= 'a' && c <= 'f')) {
            cleaned += c;
        }
    }
    return cleaned;
}


// Función auxiliar para formatear la respuesta individual de cada búsqueda
string checkMatch(const string& transmission, const string& mcode) {
    int pos = searchPattern(transmission, mcode);
    if (pos > 0) {
        return "true " + to_string(pos) + "\n";
    }
    return "false\n";
}

string rollingHashComparison(const std::string& transmission1, const std::string& transmission2, const std::string& mcode1, const std::string& mcode2, const std::string& mcode3) {

    // 1. Limpieza de caracteres de control (\n, \r, espacios)
    string t1 = cleanString(transmission1);
    string t2 = cleanString(transmission2);
    string m1 = cleanString(mcode1);
    string m2 = cleanString(mcode2);
    string m3 = cleanString(mcode3);

    string resultado = "";

    //2. Declaración de vectores para iterar sobre transmisiones y códigos maliciosos
    vector<string> transmissions = {t1, t2};
    vector<string> mcodes = {m1, m2, m3};

    //3. Ciclo for de iteración para buscar coincidencias entre transmisiones y códigos maliciosos (Estilo for each)
    for(const string& transmission : transmissions) {
        for(const string& mcode : mcodes) {
            resultado += checkMatch(transmission, mcode);
        }
    }

    return resultado;

}

// === Fin Funciones Rolling Hash ===


// === Funciones Longest Common Substring ===
// funcion para encontrar el substring comun mas largo entre ambas transmisiones usando programacion dinamica
// Regresa "inicio fin" (base 1) de la posicion en transmission1 y la subcadena encontrada
string longestCommonSubstring(const std::string& transmission1, const std::string& transmission2) {
    // Limpieza de caracteres de control (\n, \r, espacios)
    string t1 = cleanString(transmission1);
    string t2 = cleanString(transmission2);
    int n = t1.length();
    int m = t2.length();

    // Solo guardamos dos filas de la matriz dp: la anterior (prev) y la actual (cur)
    // cur[j] = longitud del substring comun que TERMINA en t1[i-1] y t2[j-1]
    vector<int> prev(m + 1, 0), cur(m + 1, 0);
    int maxLen = 0; // longitud del substring comun mas largo encontrado
    int endPos = 0; // posicion (base 1) en t1 donde termina ese substring

    // Llenado de la tabla fila por fila
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (t1[i - 1] == t2[j - 1]) {
                // Coinciden: extendemos la racha de la diagonal (arriba-izquierda)
                cur[j] = prev[j - 1] + 1;
                // Actulizar MaxLen y endPos si encontramos un subtring comun mas largo
                if (cur[j] > maxLen) {
                    maxLen = cur[j];
                    endPos = i;
                }
            } else {
                cur[j] = 0; // No coinciden: la racha se rompe
            }
        }
        // La fila actual pasa a ser la anterior para la siguiente iteracion
        swap(prev, cur);
    }

    if (maxLen == 0) {
        return "No se encontro subcadena comun\n";
    }

    // El inicio se deduce restando la longitud al final
    int startPos = endPos - maxLen + 1;
    // Extraemos la subcadena comun mas larga de t1
    string substringComun = t1.substr(endPos - maxLen, maxLen);
    return  "Posicion Incial: "+ to_string(startPos) + " Posicion Final: " + to_string(endPos) + "\n"
          + "Subcadena mas larga: " + substringComun + " (longitud: " + to_string(maxLen) + ")\n";
}


// funcion auxiliar para cargar archivos a un string
string readFileToString(const std::string& filePath){
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "CRITICAL ERROR: No se pudo abrir " << filePath << "\n";
        std::cerr << "Directorio actual de busqueda: " << std::filesystem::current_path() << "\n";
        std::exit(EXIT_FAILURE);
    }
    //Este metodo es mucho mas eficiente pues parsea todo el archivo a velocidad de hardware
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}


// Función para imprimir el reporte final de forma ordenada y limpia (Realizada con Inteligencia Artificial)
void printProjectReport(const string& res1, const string& res2, const string& res3) {
    cout << "========================================" << endl;
    cout << "         REPORTE DE RESULTADOS          " << endl;
    cout << "========================================" << endl;

    // Parte 1
    cout << "--- PARTE 1: Deteccion de Codigo Malicioso ---" << endl;
    if (res1.empty()) {
        cout << "[Sin resultados registrados]" << endl;
    } else {
        cout << res1; // res1 ya trae sus saltos de línea integrados
    }

    // Parte 2
    cout << "\n--- PARTE 2: Codigo Espejeado (Palindromos) ---" << endl;
    if (res2.empty()) {
        cout << "[Sin resultados registrados]" << endl;
    } else {
        cout << res2;
    }

    // Parte 3
    cout << "\n--- PARTE 3: Subcadena Comun Mas Larga ---" << endl;
    if (res3.empty()) {
        cout << "[Sin resultados registrados]" << endl;
    } else {
        cout << res3;
    }

    cout << "========================================" << endl;
}

int main(){

    autoLocalizarDirectorio(); // Llamada a la función de autolocalización

    //Paso 1: Apertura de archivos en strings con validación estricta
    string transmission1 = readFileToString("transmission01.txt");
    string transmission2 = readFileToString("transmission02.txt");
    string mcode1 = readFileToString("mcode01.txt");
    string mcode2 = readFileToString("mcode02.txt");
    string mcode3 = readFileToString("mcode03.txt");

    //Paso 2: llamadas a funciones (Aqui llamen a sus respecivas funciones y regresen sus resultados a variables string)
    std::string resultado1 = rollingHashComparison(transmission1,transmission2,mcode1,mcode2,mcode3);
    std::string resultado2;
    std::string resultado3 = longestCommonSubstring(transmission1, transmission2);



    printProjectReport(resultado1, resultado2, resultado3);

    return 0; 
}
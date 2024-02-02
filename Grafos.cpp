#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

struct Bitacora {
    string mes;
    int dia;
    string hora;
    string ipOrigen;
    int puerto;
    string razonFalla;
    int fanOut;
};

void leerArchivo(const string& bitacora, map<string, vector<Bitacora>>& bitacoraMap) {
    ifstream archivo(bitacora);

    if (!archivo) {
        cerr << "No se pudo abrir el archivo." << endl;
        return;
    }

    map<string, int> meses = {{"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4}, {"May", 5}, {"Jun", 6}, {"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}};

    string mes, hora, ip, razonFalla;
    int dia, puerto;

    while (archivo >> mes >> dia >> hora >> ip >> puerto >> razonFalla) {
        Bitacora b;
        b.mes = meses[mes];
        b.dia = dia;
        b.hora = hora;
        b.ipOrigen = ip;
        b.puerto = puerto;
        b.razonFalla = razonFalla;
        b.fanOut = 0;

        bitacoraMap[ip].push_back(b);
    }

    archivo.close();
}

void calcularFanOut(map<string, vector<Bitacora>>& bitacoraMap) {
    for (auto& entry : bitacoraMap) {
        for (auto& bitacora : entry.second) {
            bitacora.fanOut = entry.second.size();
        }
    }
}

pair<string, int> maximoFanOut(map<string, vector<Bitacora>>& bitacoraMap) {
    pair<string, int> maxPair;

    for (auto& entry : bitacoraMap) {
        for (auto& bitacora : entry.second) {
            if (bitacora.fanOut > maxPair.second) {
                maxPair.first = bitacora.ipOrigen;
                maxPair.second = bitacora.fanOut;
            }
        }
    }

    return maxPair;
}

string bootMaster(map<string, vector<Bitacora>>& bitacoraMap) {
    for (auto& entry : bitacoraMap) {
        for (auto& bitacora : entry.second) {
            if (bitacora.fanOut > 1 && bitacora.razonFalla.find("boot") != string::npos) {
                return bitacora.ipOrigen;
            }
        }
    }

    return "";
}

int main() {
    map<string, vector<Bitacora>> bitacoraMap;
    leerArchivo("bitacora.txt", bitacoraMap);
    calcularFanOut(bitacoraMap);

    auto maxFanOutPair = maximoFanOut(bitacoraMap);
    cout << "Nodo con mayor fan-out: " << maxFanOutPair.first << ", Fan-out: " << maxFanOutPair.second << endl;

    string bootMasterIP = bootMaster(bitacoraMap);
    cout << "Dirección IP presumible del boot master: " << bootMasterIP << endl;

    return 0;
}

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <string>
#include <sstream>

// Función para leer el uso de CPU del sistema
float getCPUUsage() {
    std::ifstream statFile("/proc/stat");
    std::string line, cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    if (statFile.is_open()) {
        std::getline(statFile, line);
        std::istringstream ss(line);
        ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
        statFile.close();
        long idleTime = idle + iowait;
        long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;
        return (1.0 - (float(idleTime) / totalTime)) * 100.0;
    }
    return 0.0; // Retorna 0 si no se pudo leer
}

// Función para leer el uso de memoria del sistema
float getMemoryUsage() {
    std::ifstream meminfoFile("/proc/meminfo");
    std::string line, key;
    long totalMemory = 0, freeMemory = 0;
    if (meminfoFile.is_open()) {
        while (std::getline(meminfoFile, line)) {
            std::istringstream ss(line);
            ss >> key;
            if (key == "MemTotal:") {
                ss >> totalMemory;
            } else if (key == "MemAvailable:") {
                ss >> freeMemory;
                break;
            }
        }
        meminfoFile.close();
        return (1.0 - (float(freeMemory) / totalMemory)) * 100.0;
    }
    return 0.0; // Retorna 0 si no se pudo leer
}

int main() {
    // Comando a ejecutar
    std::string command = "python3 pqc_algorithm.py";

    // Variables para monitoreo
    float totalCPUUsage = 0.0;
    float totalMemoryUsage = 0.0;
    int samples = 0;

    // Inicio del temporizador
    auto start = std::chrono::high_resolution_clock::now();

    // Ejecutar el comando en un hilo separado
    std::thread commandThread([&]() {
        system(command.c_str());
    });

    // Monitoreo durante la ejecución
    while (commandThread.joinable()) {
        float cpuUsage = getCPUUsage();
        float memoryUsage = getMemoryUsage();

        totalCPUUsage += cpuUsage;
        totalMemoryUsage += memoryUsage;
        samples++;

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Tomar muestra cada segundo
    }

    // Final del temporizador
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> executionTime = end - start;

    // Esperar a que termine el hilo
    commandThread.join();

    // Cálculo de promedios
    float avgCPUUsage = totalCPUUsage / samples;
    float avgMemoryUsage = totalMemoryUsage / samples;

    // Imprimir resultados
    std::cout << "Tiempo de ejecución: " << executionTime.count() << " segundos\n";
    std::cout << "Uso promedio de CPU: " << avgCPUUsage << "%\n";
    std::cout << "Uso promedio de memoria: " << avgMemoryUsage << "%\n";

    // Guardar resultados en un archivo
    std::ofstream outputFile("usage_metrics.txt");
    outputFile << "Tiempo de ejecución: " << executionTime.count() << " segundos\n";
    outputFile << "Uso promedio de CPU: " << avgCPUUsage << "%\n";
    outputFile << "Uso promedio de memoria: " << avgMemoryUsage << "%\n";
    outputFile.close();

    return 0;
}
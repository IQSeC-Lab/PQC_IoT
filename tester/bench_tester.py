import psutil
import time
import os

def monitor_usage(duration):
    """Monitorea el uso de CPU y memoria cada segundo durante la ejecución."""
    cpu_usage = []
    memory_usage = []

    for _ in range(duration):
        cpu_usage.append(psutil.cpu_percent(interval=1))
        memory_usage.append(psutil.virtual_memory().percent)

    return cpu_usage, memory_usage

def execute_and_monitor(command):
    """Ejecuta un comando y monitorea el uso del sistema."""
    start_time = time.time()

    # Inicia el proceso
    for i in range(1,30):
        process = os.system(command)

    # Monitoreo en paralelo
    duration = int(time.time() - start_time)
    cpu_usage, memory_usage = monitor_usage(duration)

    end_time = time.time()
    execution_time = end_time - start_time

    return execution_time, cpu_usage, memory_usage

    

def main():
    # Comando para ejecutar el algoritmo
    command = "python3 bike-t.py"

    # Ejecutar y monitorear
    execution_time, cpu_usage, memory_usage = execute_and_monitor(command)

    # Resultados
    print(f"Tiempo de ejecución: {execution_time:.2f} segundos")
    print(f"Uso promedio de CPU: {sum(cpu_usage) / len(cpu_usage):.2f}%")
    print(f"Uso promedio de memoria: {sum(memory_usage) / len(memory_usage):.2f}%")


if __name__=="__main__":
    main()
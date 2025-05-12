# PQC Docker Lab Environment

This lab environment sets up two Docker containers — a `pqc-client` and a `pqc-server` — pre-installed with:

- [liboqs](https://github.com/open-quantum-safe/liboqs)
- [mbedTLS](https://github.com/Mbed-TLS/mbedtls)
- Vim and networking tools

The containers share a common `/app` directory where all `.c` source files are located. Students can modify, compile, and run programs from either container.

---

## 📁 Folder Structure
```
.
├── base/ # Builds the pqc-base image (with libraries)
│ └── Dockerfile
├── client/ # Minimal Dockerfile for the client
├── server/ # Minimal Dockerfile for the server
├── pi-talking/ # C source files shared between both containers
├── docker-compose.yml
```

---

## 🧰 Requirements

- Docker
- Docker Compose plugin (v2 or newer)

---

## 🚀 Setup Instructions
```Bash
// This is to build the base images of client and server.
sudo docker build -t pqc-base ./base
// This is to build each image individually.
sudo docker compose up -d --build
```

## 🖥️ Using the Environment
1. Open two terminals, then run:

Terminal 1 **Server**
```Bash
sudo docker exec -it pqc-server bash
```

Terminal 2 **Client**
```Bash
sudo docker exec -it pqc-client bash
```

## Stop the enviroment
```Bash
sudo docker compose down
```

---
## 📫 Questions?
Contact :
- Contact your instructor
- Jesus Lopez jlopez126@miners.utep.edu

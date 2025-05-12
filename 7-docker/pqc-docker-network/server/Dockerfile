FROM gcc:latest
COPY . /app
WORKDIR /app
RUN gcc server.c -o server
CMD ["./server"]


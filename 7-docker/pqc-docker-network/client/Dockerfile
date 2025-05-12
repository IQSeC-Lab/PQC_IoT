FROM gcc:latest
COPY . /app
WORKDIR /app
RUN gcc client.c -o client
CMD ["./client"]


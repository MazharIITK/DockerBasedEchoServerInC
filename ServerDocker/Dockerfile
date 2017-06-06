FROM ubuntu:latest

RUN apt-get update && apt-get install gcc -y

COPY server.c server.c

RUN gcc -o server server.c -lpthread

EXPOSE 5010

ENTRYPOINT ["./server"]
CMD ["5010"]


FROM ubuntu

RUN apt-get update && apt-get install gcc -y

COPY proxy.c proxy.c

RUN gcc -o proxy proxy.c -lpthread

EXPOSE 5000

ENTRYPOINT ["./proxy"]


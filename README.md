echo-server with docker-compose
=============

This is the initial step towards writing a _Honeypot_. This repository contains an echo-server as *server.c*, a client as 
*client.c* and a proxy as *proxy.c* . There are dockerfiles in ServerDocker and ProxyDocker. 

The *server.c* and *proxy.c* files are run in containers using the **docker-compose.yml** .This is a simple model
which gives an idea of how a proxy can log all the informations that are passed from a client to server as it acts as
an intermediate between client and proxy. The _Honeypot_ will work in the same manner.

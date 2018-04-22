FROM ubuntu:16.04

MAINTAINER KCherkasov

RUN apt-get update && apt-get install -y gcc g++

ADD . root

CMD ls

RUN g++ -o tp_highload_server root/*.cpp

EXPOSE 80

CMD root/tp_highload_server
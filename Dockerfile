FROM ubuntu:16.04

MAINTAINER KCherkasov

RUN apt-get update && apt-get install -y gcc g++

ADD . root

RUN RUN g++ -o tp_highload_server *.cpp

EXPOSE 80

CMD ./tp_highload_server
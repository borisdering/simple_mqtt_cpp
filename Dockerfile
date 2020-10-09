FROM ubuntu:18.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y
RUN apt-get install -y nodejs npm gcc build-essential mosquitto

RUN apt-get install -y less vim 
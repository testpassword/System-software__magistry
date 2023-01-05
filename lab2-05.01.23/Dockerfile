FROM ubuntu:18.04
WORKDIR /lab1
COPY . .
RUN apt update && \
    apt install -y clang && \
    clang main.cpp && \
    chmod +x a.out
CMD ["./a.out"]

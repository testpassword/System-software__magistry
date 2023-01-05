FROM ubuntu:18.04
WORKDIR /lab2
COPY . .
RUN apt update && \
    apt install -y clang && \
    clang main.c && \
    chmod +x a.out
CMD ["./a.out", "2", "3000", "-debug"]

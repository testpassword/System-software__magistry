FROM ubuntu:18.04
LABEL name="posix"
RUN apt update && \
    apt install -y --allow-unauthenticated \
    clang \
    g++ \
    clang-tidy \
    clang-format-7 \
    make
WORKDIR "/posix"
COPY . .
RUN ln -s /usr/bin/clang-format-7 /usr/bin/clang-format && \
    mkdir build && \
    clang++ -pthread -g ./*.cpp -o build/posix && \
    chmod +x build/posix
RUN make build_tests
RUN make run_tests
CMD ["./build/posix", "2", "3000", "-debug"]

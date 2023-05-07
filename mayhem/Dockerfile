FROM debian:bookworm as build 
RUN apt update && apt install build-essential clang -y
COPY . /zsv
WORKDIR /zsv/fuzz
RUN make

FROM debian:bookworm
COPY --from=build /zsv/fuzz/zsv-fuzz .


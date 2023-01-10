LINUX_OUT=./foo

all:build_so build_go


update:
	cd "./core/build/" make clean && make
	go build .

run_dev:all run

run:
	./main

build_so:
	rm -rf ./core/build/* && cd "./core/build/" && cmake .. && make clean && make

build_go:
	go build .

export_h:
	go tool cgo -exportheader main.h golang.go



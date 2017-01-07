####
## Makefile for openpst/streaming-dload
####

all: default

default:
	if [ ! -d "./lib/libopenpst/include" ] || [ ! -d "./lib/gui-common/include" ]; then git submodule init && git submodule update;  fi
	if [ ! -d "./lib/libopenpst/lib/serial/include" ]; then cd ./lib/libopenpst/ && git submodule init && git submodule update;  fi
	qmake -makefile -o ./build/Makefile streaming_dload.pro
	$(MAKE) -C build

clean:
	rm -rf build/*
	rm -rf lib/libopenpst/build/*
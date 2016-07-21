all: trx_com

trx_communicate: trx_com.cpp
	g++ -lwiringPi trx_com.cpp -o trx_com
	g++ -lwiringPi ex.cpp -o ex

run: trx_com
	./trx_com
clean:
	rm -rf ./trx_com
	rm -rf ./ex

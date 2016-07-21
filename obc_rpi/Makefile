all: trx_com ex_com master

trx_com: trx_com.c
	gcc trx_com.c -o trx_com

ex_com: ex_com.c
	gcc ex_com.c -o ex_com

master: master.c
	gcc master.c -o master

clean:
	rm -rf ./trx_com
	rm -rf ./ex_com
	rm -rf ./master

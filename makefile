coin: coinminer.cpp
	g++ -g -Ofast -Wall -Werror -lcrypto -std=c++11 coinminer.cpp -o coinminer

packages:
	sudo apt-get install libssl-dev
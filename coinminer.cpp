#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <thread>
#include <string>
#include <string.h>
#include <vector>
#include <math.h>
#include <openssl/sha.h>

class ParameterParser {

};

class Token {

    public:
        void setNonce(std::string nonce);
        std::string getToken();
        int tokenSize = 60;
        Token(const std::string triOwn) {
            this->triOwn = triOwn;
            time = timestampToString(std::time(0));
        }
        std::string nonce;
        std::string triOwn;
        const std::string proto = "CC1.0";
        std::string time;
        const std::string reserved = "0f0f0f";
        const std::string timestampToString(const time_t &timestamp);
};

void Token::setNonce(std::string nonce) {
    this->nonce = nonce;
}

std::string Token::getToken() {
    return nonce + "-" + triOwn + "-" + proto + "-" + time + "-" + reserved;
}

const std::string Token::timestampToString(const time_t &timestamp) {
    std::stringstream stringstream;
    stringstream << timestamp;
    return stringstream.str();;
}

class Miner {

    public:
        virtual std::string coinHash() = 0;

    private:
        Token token;
        virtual void displayValidToken() = 0;
        
};

class CoinCoinMiner : public Miner {

    public:
        std::string coinHash() { return "";}

    private:
        int target;
};

class BenchmarkMiner : public Miner {

};

char getRandomChar() {
    int number = rand() % 95;
    return number + 32;
}

std::string getRandomNonce() {
    std::string randomNonce;
    for (int i = 0 ; i < 32 ; i++) {
        randomNonce += getRandomChar();
    }

    return randomNonce;
}

std::string sha1(Token *token) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha1;
    SHA1_Init(&sha1);
    SHA1_Update(&sha1, token->getToken().c_str(), token->getToken().size());
    SHA1_Final(hash, &sha1);

    std::stringstream stringStream;
    for (int i = 0 ; i < SHA_DIGEST_LENGTH ; i++) {
        stringStream << std::hex << std::setw(2) << std::setfill('0') << (int) hash[i];
    }

    return stringStream.str();
}

bool isHashValid(std::string hash, int strength) {
    for (int i = 0 ; i < strength ; i++) {
        if (hash[i] != 'c') {
            return false;
        }
    }
    return true;
}


void displayToken(Token *token) {

    std::cout << token->nonce << " => size = " << token->nonce.size() << std::endl;
    std::cout << token->triOwn << " => size = " << token->triOwn.size() << std::endl;
    std::cout << token->proto << " => size = " << token->proto.size() << std::endl;
    std::cout << token->time << " => size = " << token->time.size() << std::endl;
    std::cout << token->reserved << " => size = " << token->reserved.size() << std::endl;
    std::cout << token->getToken() << std::endl;
    std::cout << "total size = " << token->getToken().size() << std::endl;
    std::cout << std::endl;
}


int main(int argc, char const *argv[])
{
    auto startTime = std::chrono::high_resolution_clock::now();

    const std::vector<std::string> parameters(argv, argv + argc);

    std::srand((unsigned) std::time(0));

    //const auto processor_count = std::thread::hardware_concurrency();

    Token *token = new Token("MLO");

    //bool bug = false;

    for (int i = 0 ; i < 1000000000 ; i++) {
        token->setNonce(getRandomNonce());
        //displayToken(token);
        //std::cout << sha1Test("toto") << std::endl;
        //std::cout << token->getToken() << " = " << sha1(token) << std::endl;
        if (isHashValid(sha1(token), 6)) {
            std::cout << token->getToken() << std::endl;

            auto stopTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(stopTime - startTime);
            std::cout << "subcoin (6) = " << duration.count() << std::endl;
            std::cout << "coin (7) = " << duration.count() * 16 << std::endl;
            std::cout << "hexcoin (8) = " << ((duration.count() * pow(16, 2)) % 60) << "," <<  ((duration.count() * pow(16, 2)) % 60) << std::endl;
            std::cout << "arkhenstone (9) = " << duration.count() * pow(16, 3) << std::endl;
            std::cout << "blackstar (10) = " << duration.count() * pow(16, 4) << std::endl;
            std::cout << "grand cross (11) = " << duration.count() * pow(16, 5) << std::endl;

            break;
        }
    }

    //std::cout << bug << std::endl;



    return 0;
}


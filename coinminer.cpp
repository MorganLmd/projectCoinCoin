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

enum MinerEnum
{
    CoinCoinMinerEnum,
    BenchmarkMinerEnum
};

class ParamParser
{
private:
    const std::vector<std::string> argv;
    const int argc;

public:
    ParamParser(std::vector<std::string> argv, int argc) : argv(argv), argc(argc) {}
    void displayParameters();
    MinerEnum isCommandValid();
    bool isArgNumberOk() { return (argc == 2 || argc == 5); }
};

void ParamParser::displayParameters()
{
    for (std::string param : argv)
    {
        std::cout << param << std::endl;
    }

    std::cout << "count = " << argc << std::endl;
}

MinerEnum ParamParser::isCommandValid()
{
    if (argc == 2 && argv[1] == "-z")
    {
        return BenchmarkMinerEnum;
    }
    else if (argc == 5 && argv[1] == "-t" && argv[3] == "-m")
    {
        if (argv[2].size() == 3 && std::stoi(argv[4]) > 0 && std::stoi(argv[4]) < 12)
        {
            return CoinCoinMinerEnum;
        }
        else
        {
            throw std::invalid_argument("invalid command");
        }
    }
    else
    {
        throw std::invalid_argument("invalid command");
    }
}

class Token
{

public:
    void setNonce(std::string nonce);
    std::string getToken();
    const static int TOKEN_SIZE = 60;
    Token(const std::string triOwn) : triOwn(triOwn) { time = timestampToString(std::time(0)); }

private:
    std::string nonce;
    std::string triOwn;
    std::string time;
    const std::string proto = "CC1.0";
    const std::string reserved = "0f0f0f";

    const std::string timestampToString(const time_t &timestamp);
};

void Token::setNonce(std::string nonce)
{
    this->nonce = nonce;
}

std::string Token::getToken()
{
    return nonce + "-" + triOwn + "-" + proto + "-" + time + "-" + reserved;
}

const std::string Token::timestampToString(const time_t &timestamp)
{
    std::stringstream stringstream;
    stringstream << timestamp;
    return stringstream.str();
    ;
}

class Miner
{
public:
    virtual void coinHash() = 0;
    Miner() : strength(5) { token = new Token("XXX"); }
    Miner(int strength, std::string triOwn) : strength(strength) { token = new Token(triOwn); }

protected:
    Token *token;
    int strength;
    inline char getRandomChar();
    template <typename T>
    inline T getRandomNonce();
    inline std::string sha1();
    inline bool isHashValid(std::string hash);
};

char Miner::getRandomChar()
{
    int number = rand() % 95;
    return number + 32;
}

template <typename T>
T Miner::getRandomNonce()
{
    T randomNonce;
    for (int i = 0; i < 32; i++)
    {
        randomNonce += getRandomChar();
    }

    return randomNonce;
}

std::string Miner::sha1()
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha1;
    SHA1_Init(&sha1);
    SHA1_Update(&sha1, token->getToken().c_str(), Token::TOKEN_SIZE);
    SHA1_Final(hash, &sha1);

    std::stringstream stringStream;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        stringStream << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return stringStream.str();
}

bool Miner::isHashValid(std::string hash)
{
    for (int i = 0; i < strength; i++)
    {
        if (hash[i] != 'c')
        {
            return false;
        }
    }
    return true;
}

class CoinCoinMiner : public Miner
{

public:
    CoinCoinMiner(int strength, std::string triOwn) : Miner(strength, triOwn) {}
    void coinHash();
    int calculateHashValue(std::string hash);
};

void CoinCoinMiner::coinHash()
{
    while (true)
    {
        token->setNonce(getRandomNonce<std::string>());
        if (isHashValid(sha1()))
        {
            std::cout << token->getToken() << " => " << calculateHashValue(sha1()) << std::endl;
        }
    }
    delete token;
}

int CoinCoinMiner::calculateHashValue(std::string hash)
{
    int counter = 0;
    for (char c : hash)
    {
        if (c == 'c')
        {
            counter++;
        }
        else
        {
            break;
        }
    }
    return counter;
}

class BenchmarkMiner : public Miner
{
public:
    void coinHash();
    std::string displayReadableDuration(const long duration);
};

void BenchmarkMiner::coinHash()
{
    bool isRunning = true;
    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout << "Starting benchmark..." << std::endl;
    while (isRunning)
    {
        token->setNonce(getRandomNonce<std::string>());
        if (isHashValid(sha1()))
        {
            std::cout << "miette (5) calculée => " << token->getToken() << " => " << sha1() << std::endl;
            auto stopTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
            std::cout << "miette (5) => " << displayReadableDuration(duration.count()) << std::endl;
            std::cout << "subcoin (6) => " << displayReadableDuration(duration.count() * 16) << std::endl;
            std::cout << "coin (7) => " << displayReadableDuration(duration.count() * pow(16, 2)) << std::endl;
            std::cout << "hexcoin (8) => " << displayReadableDuration(duration.count() * pow(16, 3)) << std::endl;
            std::cout << "arkhenstone (9) => " << displayReadableDuration(duration.count() * pow(16, 4)) << std::endl;
            std::cout << "blackstar (10) => " << displayReadableDuration(duration.count() * pow(16, 5)) << std::endl;
            std::cout << "grand cross (11) => " << displayReadableDuration(duration.count() * pow(16, 6)) << std::endl;

            isRunning = false;
        }
    }
    delete token;
}

std::string BenchmarkMiner::displayReadableDuration(const long duration)
{
    auto days = duration / (24 * 60 * 60 * 1000);
    auto rest = duration % (24 * 60 * 60 * 1000);
    auto hours = rest / (60 * 60 * 1000);
    rest = rest % (60 * 60 * 1000);
    auto minutes = rest / (60 * 1000);
    rest = rest % (60 * 1000);
    auto seconds = rest / 1000;
    auto milliseconds = rest % 1000;
    std::stringstream time;
    time << days << " days(s), " << hours << " hour(s), " << minutes << " minute(s), " << seconds << " second(s), " << milliseconds << " millisecond(s)";
    return time.str();
}

int main(const int argc, char const *argv[])
{
    ParamParser parameterParser(std::vector<std::string>(argv, argv + argc), argc);
    std::srand((unsigned)std::time(0));
    //const auto processor_count = std::thread::hardware_concurrency();

    Miner *miner;
    try
    {
        MinerEnum mode = parameterParser.isCommandValid();
        switch (mode)
        {
        case CoinCoinMinerEnum:
            miner = new CoinCoinMiner(std::stoi(argv[4]), argv[2]);
            break;
        case BenchmarkMinerEnum:
            miner = new BenchmarkMiner();
            break;

        default:
            miner = nullptr;
            throw std::invalid_argument("invalid command");
            return 0;
        }

        if (miner != nullptr)
        {
            miner->coinHash();
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "fatal error : " << e.what() << std::endl;
    }

    return 0;
}

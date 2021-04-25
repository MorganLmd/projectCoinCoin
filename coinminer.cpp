#include <iostream>
#include <string>
#include <string.h>
#include <openssl/sha.h>

class ParameterParser {

};

class Token {

    private:
        std::string nonce;
        const std::string triOwn;
        const std::string proto;
        const std::string time;
        const std::string reserved;
};

class Hasher {

    public:
        virtual std::string coinHash() = 0;

    private:
        Token token;
        virtual void displayValidToken() = 0;
        
};

class CoinCoinHasher : public Hasher {

    public:
        std::string coinHash() { return "";}

    private:
        int target;
};


int main(int argc, char const *argv[])
{
    std::cout << "hello world" << std::endl;

    unsigned char digest[SHA_DIGEST_LENGTH];
    const char* string = "hello world"; 
 
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, string, strlen(string));
    SHA1_Final(digest, &ctx);
 
    char mdString[SHA_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
 
    printf("SHA1 digest: %s\n", mdString);


    return 0;
}


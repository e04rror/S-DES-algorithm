#include <bitset>
#include <iostream>
#include <vector>
#include "permutation.hpp"
#include "functions.cpp"

//a need a function that transforms bits into symbol
void transformFunction(const std::vector<std::bitset<8>>& input){
  for(const auto& bits: input) {
    unsigned long ulongValue = bits.to_ulong();
    char character = static_cast<char>(ulongValue);
    std::cout<<character;
  }
  std::cout<<std::endl;
}

std::vector<std::bitset<8>> decryption(std::vector<std::bitset<8>> &binaryTxt,
                                 const std::bitset<8> &key1,
                                 const std::bitset<8> &key2) {
  std::vector<std::bitset<8>> result;
  int count = 1;
  for (auto &seqBit : binaryTxt) {
    std::bitset<8> bitRes;
    std::cout << count << " block: " << seqBit << std::endl;
    // IP
    std::bitset<8> initPermut = permutation(seqBit, IP);
    std::cout << "IP: " << initPermut << std::endl;
    std::cout<<"First Feistel: "<<std::endl;
    auto firstFeist = feistel(initPermut, key2);
    std::cout<<std::endl;
    std::cout<<"Switch: ";
    
    sW(firstFeist);
    
    std::cout<<firstFeist<<std::endl;

    std::cout<<"Second Feistel: "<<std::endl;
    auto secondFeist = feistel(firstFeist, key1);

    std::cout<<std::endl;
    std::bitset<8> inverseIp = permutation(secondFeist, IP_1);
    std::cout<<"Block of cipher text: "<<inverseIp<<std::endl;
    result.push_back(inverseIp);
    std::cout << std::endl;
    count++;
  }
  return result;
}

int main(int argc, char *argv[]) {
  // Take the data(encrypted text) from the file
  std::vector<std::bitset<8>> encryptedText;
  readBitsFromFile(encryptedText, filename);
  
  // Defines keys
  std::bitset<8> K1, K2;
  
  // Generate key1 and key2
  generateKey(K1, K2);

  // Perform decryption
  auto decrypt = decryption(encryptedText, K1, K2);

  // Display the full decrypted text
  std::cout << "Full decrypted text: ";
  ShowVector(decrypt);

  std::cout<<"Symbols: ";
  transformFunction(decrypt);
  
  return 0;
}

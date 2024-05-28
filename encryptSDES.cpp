#include <bitset>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "permutation.hpp"
#include "functions.cpp"

// transform string(text) to bits
std::vector<std::bitset<8>> charToBits(std::string &text) {

  std::vector<std::bitset<8>> result;
  for (std::size_t i = 0; i < text.size(); ++i) {
    result.push_back(std::bitset<8>(text.c_str()[i]));
  }

  return result;
}

std::vector<std::bitset<8>> encryption(std::vector<std::bitset<8>> &binaryTxt,
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
    std::cout << "First Feistel: " << std::endl;
    auto firstFeist = feistel(initPermut, key1);
    std::cout << std::endl;
    std::cout << "Switch: ";

    sW(firstFeist);

    std::cout << firstFeist << std::endl;

    std::cout << "Second FEistel: " << std::endl;
    auto secondFeist = feistel(firstFeist, key2);

    std::cout << std::endl;
    std::bitset<8> inverseIp = permutation(secondFeist, IP_1);
    std::cout << "Block of cipher text: " << inverseIp << std::endl;
    result.push_back(inverseIp);
    std::cout << std::endl;
    count++;
  }
  return result;
}

int main(int argc, char *argv[]) {
  // input
  std::string text = "trench";
  std::vector<std::bitset<8>> binaryText = charToBits(text);
  std::cout << "Trench in the binary form: ";
  ShowVector(binaryText);
  std::bitset<8> K1, K2;
  generateKey(K1, K2);
  auto encryp = encryption(binaryText, K1, K2);

  std::cout << "Full encrypted text: ";
  ShowVector(encryp);
  writeBitsToFile(encryp, "test.txt");

  return 0;
}

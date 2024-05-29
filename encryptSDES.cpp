#include <bitset>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "permutation.hpp"
#include "functions.cpp"

// transform string(text) to a vector of 8-bit bitsets
std::vector<std::bitset<8>> charToBits(std::string &text) {

  std::vector<std::bitset<8>> result;
  for (std::size_t i = 0; i < text.size(); ++i) {
    result.push_back(std::bitset<8>(text.c_str()[i]));
  }

  return result;
}

//the basic steps of the encryption 
std::vector<std::bitset<8>> encryption(std::vector<std::bitset<8>> &binaryTxt,
                                 const std::bitset<8> &key1,
                                 const std::bitset<8> &key2) {
  std::vector<std::bitset<8>> result;
  int count = 1; // for printing which block we use 
  for (auto &seqBit : binaryTxt) {
    std::bitset<8> bitRes;
    // Print sequence of bits(block)
    std::cout << count << " block: " << seqBit << std::endl;
    
    // Perform initial permutation
    std::bitset<8> initPermut = permutation(seqBit, IP);
    std::cout << "IP: " << initPermut << std::endl;
    
    // Permorm first Feistel function with key1
    std::cout << "First Feistel: " << std::endl;
    auto firstFeist = feistel(initPermut, key1);
    std::cout << std::endl;

    // Switch left and right sides after Feistel function
    std::cout << "Switch: "; 
    sW(firstFeist);
    std::cout << firstFeist << std::endl;
    
    // Perform second Feistel function with key2
    std::cout << "Second FEistel: " << std::endl; 
    auto secondFeist = feistel(firstFeist, key2);
    std::cout << std::endl;

    // Perform inverse permutation to the output
    std::bitset<8> inverseIp = permutation(secondFeist, IP_1);
    std::cout << "Block of cipher text: " << inverseIp << std::endl;
    // Add cipher block to the 
    result.push_back(inverseIp);
    std::cout << std::endl;
    count++;
  }
  return result;
}

int main(int argc, char *argv[]) {
  // Input text
  std::string text = "trench";
  
  // Transform characters to binary form
  std::vector<std::bitset<8>> binaryText = charToBits(text);
  std::cout << "Trench in the binary form: ";
  ShowVector(binaryText);
  
  // Define 8-bit keys
  std::bitset<8> K1, K2;
  
  // Generate key1 and key2
  generateKey(K1, K2);

  // Perform encryption
  auto encryp = encryption(binaryText, K1, K2);

  // Display the full encrypted text
  std::cout << "Full encrypted text: ";
  ShowVector(encryp);
  // Write the cipher text to a file
  writeBitsToFile(encryp, filename);

  return 0;
}

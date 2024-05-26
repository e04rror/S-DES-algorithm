#include <bitset>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "permutation.hpp"


// transform string(text) to bits
std::vector<std::bitset<8>> charToBits(std::string &text) {

  std::vector<std::bitset<8>> result;
  for (std::size_t i = 0; i < text.size(); ++i) {
    result.push_back(std::bitset<8>(text.c_str()[i]));
  }

  return result;
}

template <size_t T>
std::bitset<T> combineIntoOne(const std::bitset<T / 2> &left,
                              const std::bitset<T / 2> &right) {
  std::bitset<T> result;
  for (size_t index = 0; index < (T / 2); ++index) {
    result[index] = right[index];
    result[index + (T / 2)] = left[index];
  }
  return result;
}

// shift to left side
std::bitset<5> leftShift(std::bitset<5> key, int &shift) {
  return (key << shift) | (key >> (5 - shift));
}

// divide function
template <size_t T1, size_t T2>
void divideIntoTwo(std::bitset<T1> &left, std::bitset<T1> &right,
                   std::bitset<T2> &original) {
  for (size_t index = 0; index < T1; ++index) {
    right[index] = original[index];
    left[index] = original[index + T1];
  }
}

// permutation
template <size_t T>
std::bitset<T> permutation(const std::bitset<T> &input, const int *arrTab) {
  std::bitset<T> output;
  for (int i = 0; i < T; i++) {
    output[T - i - 1] = input[T - arrTab[i]];
  }
  return output;
}

// permutation for P8
template <size_t T1, size_t T2>
std::bitset<T1> permutWithDiffSize(const std::bitset<T2> &input,
                                   const int *arrTab) {
  std::bitset<T1> output;
  for (int i = 0; i < output.size(); i++) {
    output[output.size() - i - 1] = input[input.size() - arrTab[i]];
  }
  return output;
}

template <size_t T>
std::bitset<T> XOR(const std::bitset<T> &binary_half_block,
                   const std::bitset<T> &round_key) {
  std::bitset<T> result;
  for (size_t index = 0; index < result.size(); ++index) {
    result[index] = round_key[index] ^ binary_half_block[index];
  }
  return result;
}

// generate P8
std::bitset<8> generateP8(std::bitset<10> &input, int shift = 1) {
  std::bitset<10 / 2> left;
  std::bitset<10 / 2> right;

  divideIntoTwo(left, right, input);
  std::cout << "Left: " << left << std::endl;
  std::cout << "Right: " << right << std::endl;

  left = leftShift(left, shift);
  right = leftShift(right, shift);

  std::cout << "LeftS: " << left << std::endl;
  std::cout << "RightS: " << right << std::endl;
  // I change to input because I want to show the basic steps of generating keys
  input = combineIntoOne<10>(left, right);
  std::cout << "PreResult: " << input << std::endl;

  std::bitset<8> result = permutWithDiffSize<8, 10>(input, P8);

  return result;
}

// find the number in the S box
std::bitset<2> takesNumberFromSBoxe(std::bitset<4> &input,
                                    const int SBOX[SIZE][SIZE]) {
  std::bitset<2> row;
  row[0] = input[0];
  row[1] = input[3];
  std::bitset<2> column;
  column[0] = input[1];
  column[1] = input[2];
  std::cout << "Row: " << row << std::endl;
  std::cout << "Column: " << column << std::endl;
  // transform binary into decimal
  unsigned long dRow = row.to_ulong();
  unsigned long dColumn = column.to_ulong();
  std::cout << "Decimal row: " << dRow << std::endl;
  std::cout << "Decimal column: " << dColumn << std::endl;
  int SBox = SBOX[dRow][dColumn];
  std::cout << "Sbox: " << SBox << std::endl;
  std::bitset<2> result(SBox);
  std::cout << std::endl;
  return result;
}

std::bitset<8> feistel(std::bitset<8> &ip, const std::bitset<8> &key) {
  std::bitset<8 / 2> left, right;
  divideIntoTwo(left, right, ip);
  std::cout << "LEFT SIDE: " << left << std::endl;
  std::cout << "RIGHT SIDE: " << right << std::endl;

  auto expandPermFirst = permutWithDiffSize<8>(right, EP);

  std::cout << "Expand Permutation: " << expandPermFirst << std::endl;

  auto xorWithExpFirst = XOR(expandPermFirst, key);

  std::cout << "After operation XOR: " << xorWithExpFirst << std::endl;

  std::bitset<4> leftXor, rightXor;
  divideIntoTwo(leftXor, rightXor, xorWithExpFirst);

  std::cout << "Left, after xor: " << leftXor << std::endl;
  std::cout << "Right, after xor: " << rightXor << std::endl;

  auto sBox0 = takesNumberFromSBoxe(leftXor, S0);
  auto sBox1 = takesNumberFromSBoxe(rightXor, S1);

  std::bitset<4> combineBoxes = combineIntoOne<4>(sBox0, sBox1);

  std::cout << "After combining boxes: " << combineBoxes << std::endl;

  // perform P4
  combineBoxes = permutation(combineBoxes, P4);
  std::cout << "After P4: " << combineBoxes << std::endl;
  // XOR binary digits from boxes to left side of IP_1

  std::bitset<4> leftXorCombB = XOR(left, combineBoxes);

  std::cout << "After left xor output P4: " << leftXorCombB << std::endl;

  std::bitset<8> result = combineIntoOne<8>(leftXorCombB, right);
  std::cout << "Final: " << result << std::endl;
  return result;
}

// switch
void sW(std::bitset<8> &afterXor) {
  std::bitset<8 / 2> left, right;
  divideIntoTwo(left, right, afterXor);

  afterXor = combineIntoOne<8>(right, left);
}

// algorithm
std::vector<std::bitset<8>> sDes(std::vector<std::bitset<8>> &binaryTxt,
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

// print vector
template <typename T> void ShowVector(const std::vector<T> &cont) {
  for (auto element = cont.begin(); element != cont.end(); ++element) {
    std::cout << *element << ' ';
  }
  std::cout << std::endl;
}

int main(int argc, char *argv[]) {
  // input
  std::string text = "trench";
  std::vector<std::bitset<8>> binaryText = charToBits(text);
  std::cout << "Trench in the binary form: ";
  ShowVector(binaryText);

  std::cout << "KEY: " << KEY << std::endl;
  // transform key into P10
  std::bitset<10> peKey = permutation(KEY, P10);
  // transform key into p8

  std::cout << "P10:" << peKey << std::endl;

  const std::bitset<8> K1 = generateP8(peKey);

  std::cout << "K1:" << K1 << std::endl;

  std::cout << std::endl;
  // change this
  const std::bitset<8> K2 = generateP8(peKey, 2);

  std::cout << "K2:" << K2 << std::endl;

  std::cout << "\n\n";

  auto encryp = sDes(binaryText, K1, K2);

  std::cout << "Full encrypted text: ";
  ShowVector(encryp);

  return 0;
}

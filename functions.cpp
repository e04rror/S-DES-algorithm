#include <bitset>
#include <vector>
#include <iostream>
#include "permutation.hpp"

template <size_t T1, size_t T2>
void divideIntoTwo(std::bitset<T1> &left, std::bitset<T1> &right,
                   std::bitset<T2> &original) {
  for (size_t index = 0; index < T1; ++index) {
    right[index] = original[index];
    left[index] = original[index + T1];
  }
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

template <size_t T>
std::bitset<T> permutation(const std::bitset<T> &input, const int *arrTab) {
  std::bitset<T> output;
  for (int i = 0; i < T; i++) {
    output[T - i - 1] = input[T - arrTab[i]];
  }
  return output;
}

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

std::bitset<5> leftShift(std::bitset<5> key, int &shift) {
  return (key << shift) | (key >> (5 - shift));
}


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
  input = combineIntoOne<10>(left, right);
  std::cout << "PreResult: " << input << std::endl;

  std::bitset<8> result = permutWithDiffSize<8, 10>(input, P8);

  return result;
}

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


std::bitset<8> feistel(std::bitset<8>& ip, const std::bitset<8>& key ) {
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
  std::cout<<"Final: "<<result<<std::endl;
  return result;
}

void sW(std::bitset<8> &afterXor) {
  std::bitset<8 / 2> left, right;
  divideIntoTwo(left, right, afterXor);

  afterXor = combineIntoOne<8>(right, left);
}

template <typename T> void ShowVector(const std::vector<T> &cont) {
  for (auto element = cont.begin(); element != cont.end(); ++element) {
    std::cout << *element << ' ';
  }
  std::cout << std::endl;
}


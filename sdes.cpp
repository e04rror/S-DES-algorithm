#include <bitset>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// Наш ключ
const std::bitset<10> KEY("0011011101");
// перестановка IP
const int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};
// інверсовани IP
const int IP_1[] = {4, 1, 3, 5, 7, 2, 8, 6};
// перстановка ключа для P10
const int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
// перестановка ключа для P8
const int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};
//

// перетворення тексту у двійковий формат
std::vector<std::bitset<8>> charToBits(std::string &text) {

  std::vector<std::bitset<8>> result;
  for (std::size_t i = 0; i < text.size(); ++i) {
    result.push_back(std::bitset<8>(text.c_str()[i]));
  }

  return result;
}
// зміщення уліво
std::bitset<5> leftShift(std::bitset<5> key, int &shift) {
  return (key << shift) | (key >> (5 - shift));
}

// функція поділу надвоє
template <size_t T1, size_t T2>
void divideIntoTwo(std::bitset<T1> &left, std::bitset<T1> &right,
                   std::bitset<T2> &original) {
  for (size_t index = 0; index < T1; ++index) {
    right[index] = original[index];
    left[index] = original[index + T1];
  }
}

// функція перестановки
template <size_t T>
std::bitset<T> permutation(const std::bitset<T> &input, const int *arrTab) {
  std::bitset<T> output;
  for (int i = 0; i < T; i++) {
    output[T - i - 1] = input[T - arrTab[i]];
  }
  return output;
}

// функція для перестановки P8
std::bitset<8> permutationP8(const std::bitset<10> &input, const int *arrTab) {
  std::bitset<8> output;
  for (int i = 0; i < output.size(); i++) {
    output[output.size() - i - 1] = input[input.size() - arrTab[i]];
  }
  return output;
}

// фестель F функція(XOR ключа і половинки блоку)
std::bitset<4> feistelFunc(const std::bitset<4> &binary_half_block,
                           const std::bitset<8> &round_key) {
  std::bitset<4> result;
  for (size_t index = 0; index < result.size(); ++index) {
    result[index] = binary_half_block[index] ^ round_key[index];
  }
  return result;
}

// генеруємо P8
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

  std::bitset<10> preResult;
  for (size_t index = 0; index < (10 / 2); index++) {
    preResult[index] = left[index];
    preResult[index + 5] = right[index];
  }
  std::cout << "PreResult: " << preResult << std::endl;

  std::bitset<8> result = permutationP8(preResult, P8);

  return result;
}

// переміщуємо місцями
std::bitset<8> sW(std::bitset<8> afterXor) {
  std::bitset<8 / 2> left, right;
  divideIntoTwo(left, right, afterXor);

  std::bitset<8> result;

  for (size_t i = 0; i < (8 / 2); i++) {
    result[i] = left[i];
    result[i + (8 / 2)] = right[i];
  }
  return result;
}

// головні кроки алгоритму
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

    std::bitset<8 / 2> left, right;
    divideIntoTwo(left, right, initPermut);
    std::cout << "LEFT SIDE: " << left << std::endl;
    std::cout << "RIGHT SIDE: " << right << std::endl;

    auto firstPart = feistelFunc(left, key1);
    std::cout << "Feistel first left: " << firstPart << std::endl;
    for (size_t index = 0; index < firstPart.size(); ++index) {
      bitRes[index] = right[index];
      bitRes[index + firstPart.size()] = firstPart[index];
    }
    std::cout << "Add left side: " << bitRes << std::endl;
    auto swit = sW(bitRes);
    std::cout << "SW: " << swit << std::endl;

    std::bitset<4> leftAfSw, rightAfSw;

    divideIntoTwo(leftAfSw, rightAfSw, swit);

    std::cout << "Leftside: " << leftAfSw << std::endl;
    std::cout << "Rightside: " << rightAfSw << std::endl;

    auto secondPart = feistelFunc(leftAfSw, key2);

    std::cout << "Feistel second part: " << secondPart << std::endl;

    for (size_t index = 0; index < secondPart.size(); ++index) {
      bitRes[index] = secondPart[index];
    }

    std::cout << "Combined block: " << bitRes << std::endl;

    bitRes = permutation(bitRes, IP_1);

    std::cout << "Inverse function IP: " << bitRes << std::endl;
    count++;
    result.push_back(bitRes);
    std::cout << std::endl;
  }
  return result;
}

// вивід вектора значень
template <typename T> void ShowVector(const std::vector<T> &cont) {
  for (auto element = cont.begin(); element != cont.end(); ++element) {
    std::cout << *element << ' ';
  }
  std::cout << std::endl;
}

int main(int argc, char *argv[]) {
  // вхідні даних
  std::string text = "trench";
  std::vector<std::bitset<8>> binaryText = charToBits(text);
  std::cout << "Trench in the binary form: ";
  ShowVector(binaryText);

  std::cout << "KEY: " << KEY << std::endl;
  // перетворення ключа в P10
  std::bitset<10> peKey = permutation(KEY, P10);
  // перетворення ключа в p8

  std::cout << "P10:" << peKey << std::endl;

  std::bitset<8> k1 = generateP8(peKey);

  std::cout << "K1:" << k1 << std::endl;

  std::cout << std::endl;

  std::bitset<8> k2 = generateP8(peKey, 3);

  std::cout << "K2:" << k2 << std::endl;

  std::cout << "\n\n";

  auto encryp = sDes(binaryText, k1, k2);
  
  std::cout<<"Full encrypted text: ";
  ShowVector(encryp);

  return 0;
}

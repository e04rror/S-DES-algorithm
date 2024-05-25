#include <bitset>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// Our keys
const std::bitset<10> KEY("0011011101");
// initial permutation
const int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};
// inverse IP
const int IP_1[] = {4, 1, 3, 5, 7, 2, 8, 6};
// P10
const int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
// P8
const int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};
// expand permutation
const int EP[] = {4, 1, 2, 3, 2, 3, 4, 1};
// P4
const int P4[] = {2, 4, 3, 1}; 

const int SIZE = 4;// i know name is bad
// box S0
const int S0[SIZE][SIZE] = {{1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 3, 2}};
// box S1
const int S1[SIZE][SIZE] = {{0, 1, 2, 3}, {2, 0, 1, 3}, {3, 0, 1, 0}, {2, 1, 0, 3}};

// transform string(text) to bits
std::vector<std::bitset<8>> charToBits(std::string &text) {

  std::vector<std::bitset<8>> result;
  for (std::size_t i = 0; i < text.size(); ++i) {
    result.push_back(std::bitset<8>(text.c_str()[i]));
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
  for (size_t index = 0; index < (10 / 2); index++) {
    input[index] = left[index];
    input[index + 5] = right[index];
  }
  std::cout << "PreResult: " << input << std::endl;

  std::bitset<8> result = permutWithDiffSize<8, 10>(input, P8);
  
  return result;
}

//find the number in the S box
std::bitset<2> takesNumberFromSBoxe(std::bitset<4>& input, const int SBOX[SIZE][SIZE]){
  std::bitset<2> row;
  row[0] = input[0];
  row[1] = input[3];
  std::bitset<2> column;
  column[0] = input[1];
  column[1] = input[2];
  std::cout<<"Row: "<<row<<std::endl;
  std::cout<<"Column: "<<column<<std::endl;
  //transform binary into decimal
  unsigned long dRow = row.to_ulong();
  unsigned long dColumn = column.to_ulong();
  std::cout<<"Decimal row: "<<dRow<<std::endl;
  std::cout<<"Decimal column: "<<dColumn<<std::endl;
  int SBox = SBOX[dRow][dColumn];
  std::cout<<"Sbox: "<<SBox<<std::endl;
  std::bitset<2> result(SBox);
  std::cout<<std::endl;
  return result;
}

// switch
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

    std::bitset<8 / 2> left, right;
    divideIntoTwo(left, right, initPermut);
    std::cout << "LEFT SIDE: " << left << std::endl;
    std::cout << "RIGHT SIDE: " << right << std::endl;

    auto expandPermFirst = permutWithDiffSize<8>(right, EP);

    std::cout << "Expand Permutation: " << expandPermFirst << std::endl;

    auto xorWithExpFirst = XOR(expandPermFirst, key1);

    std::cout << "After operation XOR: " << xorWithExpFirst << std::endl;
    
    std::bitset<4> leftXor, rightXor;
    divideIntoTwo(leftXor, rightXor, xorWithExpFirst);

    std::cout<<"Left, after xor: "<<leftXor<<std::endl;
    std::cout<<"Right, after xor: "<<rightXor<<std::endl;
    
    auto sBox0 = takesNumberFromSBoxe(leftXor, S0);
    auto sBox1 = takesNumberFromSBoxe(rightXor, S1);
    
    std::bitset<4> combineBoxes; 

    for(int index = 0; index < 2; ++index ) {
      combineBoxes[index] = sBox1[index];
      combineBoxes[index + 2] = sBox0[index]; 
    }

    std::cout<<"After combining boxes: "<<combineBoxes<<std::endl;
    
    //perform P4
    combineBoxes = permutation(combineBoxes, P4);
    std::cout<<"After P4: "<<combineBoxes<<std::endl;
    //XOR binary digits from boxes to left side of IP_1
    
    std::bitset<4> leftXorCombB = XOR(left, combineBoxes);

    std::cout<<"After left xor output P4: "<<leftXorCombB<<std::endl;

    std::cout<<std::endl;
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

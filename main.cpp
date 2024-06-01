#include <iostream>
#include <thread>
#include <cstdlib>

void encrypt() {
  system("./encrypt");
}

void decrypt() {
  system("./decrypt");
}

int main(){
  std::thread t1(encrypt);
  std::thread t2(decrypt);

  
  t1.join();
  t2.join();

  return 0;
}

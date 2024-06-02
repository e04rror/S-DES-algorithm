#include <iostream>
#include <thread>
#include <cstdlib>
#include <future>

void encrypt(std::promise<void> &obj) {
  system("./encrypt");

  std::cout<<"THIS IS DECRYPT!"<<std::endl;
  obj.set_value();
}

void decrypt(std::future<void> &obj) {
  obj.wait();
  system("./decrypt");
}

int main(){
  std::promise<void> forEncryptionEnd;
  std::future<void> waitForEncrypt = forEncryptionEnd.get_future();

  std::thread t1(encrypt, std::ref(forEncryptionEnd));
  std::thread t2(decrypt, std::ref(waitForEncrypt));

  
  t1.join();
  t2.join();

  return 0;
}

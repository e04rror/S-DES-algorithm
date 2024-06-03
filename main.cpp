#include <iostream>
#include <thread>
#include <cstdlib>
#include <future>

// Perform encryption and notify once it's done
void encrypt(std::promise<void> &encryptionPromise) {
    // Execute the encryption command
    system("./encrypt");

    // For understanding where is end of encryption
    std::cout << "THIS IS DECRYPT!" << std::endl;

    // Notify that encryption is complete
    encryptionPromise.set_value();
}

// Wait for encryption to complete and then start decryption
void decrypt(std::future<void> &encryptionFuture) {
    // Wait for the signal that encryption is done
    encryptionFuture.wait();

    // Execute the decryption command
    system("./decrypt");
}

int main(){
  // Create a promise to signal the end of encryption
  std::promise<void> encryptionPromise;
  // Obtain a future to wait for the promise to be fulfilled
  std::future<void> encryptionFuture = encryptionPromise.get_future();

  // Create thread for each function and pass them by the refference
  std::thread encryptionThread(encrypt, std::ref(encryptionPromise));
  std::thread decryptionThread(decrypt, std::ref(encryptionFuture));

  
  encryptionThread.join();
  decryptionThread.join();

  return 0;
}

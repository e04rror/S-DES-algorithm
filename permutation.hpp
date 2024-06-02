#ifndef PERMUTATION_HPP
#define PERMUTATION_HPP

#include <bitset>

// Input key
const std::bitset<10> KEY("0011011101");

// The name of the file in which we save the data(encrypted text)
const std::string filename = "test.txt";

// Initial permutation
const int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};

// Inverse IP
const int IP_1[] = {4, 1, 3, 5, 7, 2, 8, 6};

// P10
const int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};

// P8
const int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};

// expand permutation
const int EP[] = {4, 1, 2, 3, 2, 3, 4, 1};

// P4
const int P4[] = {2, 4, 3, 1};

const int SIZE_OF_HALF_BLOCK_EIGHT = 4; 

// The box S0
const int S0[SIZE_OF_HALF_BLOCK_EIGHT][SIZE_OF_HALF_BLOCK_EIGHT] = {
    {1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 3, 2}};

// The box S1
const int S1[SIZE_OF_HALF_BLOCK_EIGHT][SIZE_OF_HALF_BLOCK_EIGHT] = {
    {0, 1, 2, 3}, {2, 0, 1, 3}, {3, 0, 1, 0}, {2, 1, 0, 3}};

#endif 

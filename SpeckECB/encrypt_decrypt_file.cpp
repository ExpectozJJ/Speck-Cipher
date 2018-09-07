/*
   In this code, we define two functions:

   1) function encrypt_file() is used to encrypt a file using SPECK cipher
   2) function decrypt_file() is used to decrypt a file using SPECK cipher
*/

#include <iostream>
#include <fstream>

using namespace std;

void speck_keyschedule(unsigned long long key[], unsigned long long subkey[]);
void speck_encryption(unsigned long long plaintext[], unsigned long long ciphertext[], unsigned long long subkey[]);
void speck_decryption(unsigned long long plaintext[], unsigned long long ciphertext[], unsigned long long subkey[]);

//encrypt a file
//three inputs to this function: the name of the file to be encrypted
//                               the name of the encrypted file,
//                               the key provided by the user
void encrypt_file(string input_file, string output_file, unsigned long long key[])
{
    unsigned long long subkey[32];
    unsigned long long plaintext[2];
    unsigned long long ciphertext[2];
    char block[16];           //each time we will read at most 16 byte data into memblock from the file
    unsigned long long input_file_size;

    speck_keyschedule(key, subkey);

    //read the file in binary mode
    ifstream fin;
    fin.open(input_file.c_str(), ios::binary);
    //write the file in binary mode
    ofstream fout;
    fout.open(output_file.c_str(), ios::binary);

    //get the input file size
    fin.seekg (0, ios::end);
    input_file_size = fin.tellg();

    //each iteration, read 16 bytes, encrypt it, then write the ciphertext into the output file.
    fin.seekg (0, ios::beg); // start from the beginning of the file
    for (unsigned long long i = 0; i+16 <= input_file_size; i+=16)
    {
        //read 16 bytes into the array block
        fin.read(block,16);

        //convert those 16 byes into two 64-bit words
        //if you are not familiar with pointer, you may use an alternative way for conversion:
        //  plaintext[0] = block[7];
        //  for (int j = 6; j >= 0; j--)  plaintext[0] = (plaintext[0] << 8) | block[j];
        //  plaintext[1] = block[15];
        //  for (int j = 14; j >= 8; j--) plaintext[1] = (plaintext[1] << 8) | block[j];
        plaintext[0] = ((unsigned long long*)(block))[0];
        plaintext[1] = ((unsigned long long*)(block))[1];

        //perform encryption of one block
        speck_encryption(plaintext, ciphertext, subkey);

        //convert those two ciphertext words into 16 bytes
        //if you are not familiar with pointer, you may use an alternative way for conversion:
        //  for (int j = 0; j < 8; j++) block[j] = (ciphertext[0] >> 8*j) & 0xff;
        //  for (int j = 0; j < 8; j++) block[j+8] = (ciphertext[1] >> 8*j) & 0xff;
        ((unsigned long long*)(block))[0] = ciphertext[0];
        ((unsigned long long*)(block))[1] = ciphertext[1];

        //write the ciphertext block into the output file
        fout.write(block,16);
    }

    //encrypt the last block
    unsigned int t = input_file_size & 0xf; // it means: t = input_file_size % 16;
    fin.read(block,t);
    for (int i = 0; i < 16-t; i++)
    {
        block[t+i] = 16-t; //append the value(s) 16-t into the block so that we get a full block
    }

    //encrypt those 16 byes in block
    plaintext[0] = ((unsigned long long*)(block))[0];
    plaintext[1] = ((unsigned long long*)(block))[1];

    //perform encryption of one block
    speck_encryption(plaintext, ciphertext, subkey);

    ((unsigned long long*)(block))[0] = ciphertext[0];
    ((unsigned long long*)(block))[1] = ciphertext[1];

    fout.write(block,16);

    fin.close();
    fout.close();
}

//decrypt a file
//three inputs to this function: the name of the file to be decrypted
//                               the name of the decrypted file,
//                               the key provided by the user
void decrypt_file(string input_file, string output_file, unsigned long long key[])
{
    unsigned long long subkey[32];
    unsigned long long plaintext[2];
    unsigned long long ciphertext[2];

    char block[16];           //each time we will read at most 16 byte data into memblock from the file
    unsigned long long input_file_size;

    speck_keyschedule(key, subkey);

    //read the file in binary mode
    ifstream fin;
    fin.open(input_file.c_str(), ios::binary);
    //write the file in binary mode
    ofstream fout;
    fout.open(output_file.c_str(), ios::binary);

    //get the input file size
    fin.seekg (0, ios::end);
    input_file_size = fin.tellg();

    //each iteration, read 16 bytes, encrypt it, then write the ciphertext into the output file.
    fin.seekg (0, ios::beg); // start from the beginning of the file
    for (unsigned long long i = 0; i+16 <= input_file_size; i+=16)
    {
        //read 16 bytes into the array block
        fin.read(block,16);

        //convert those 16 byes into two 64-bit words
        ciphertext[0] = ((unsigned long long*)(block))[0];
        ciphertext[1] = ((unsigned long long*)(block))[1];

        //perform decryption of one block
        speck_decryption(plaintext, ciphertext, subkey);

        //convert those two words into 16 bytes
        ((unsigned long long*)(block))[0] = plaintext[0];
        ((unsigned long long*)(block))[1] = plaintext[1];

        //write the plaintext block into the output file
        if (i + 16 < input_file_size)
        {
            fout.write(block,16);  //not the last block
        }
        else if (i + 16 == input_file_size)
        {
            fout.write( block, 16 - block[15]);   //the last block
        }

    }

    fin.close();
    fout.close();
}

/*
   In this code, we define two functions:

   1) function encrypt_file() is used to encrypt a file using SPECK cipher
   2) function decrypt_file() is used to decrypt a file using SPECK cipher
*/

#include <iostream>
#include <fstream>

using namespace std;

unsigned long long IV();
void speck_keyschedule(unsigned long long key[], unsigned long long subkey[]);
void speck_encryption(unsigned long long plaintext[], unsigned long long ciphertext[], unsigned long long subkey[],unsigned long long IntVec[]);
void speck_decryption(unsigned long long plaintext[], unsigned long long ciphertext[], unsigned long long subkey[],unsigned long long IntVec[]);

//encrypt a file
//three inputs to this function: the name of the file to be encrypted
//                               the name of the encrypted file,
//                               the key provided by the user
void encrypt_file(string input_file, string output_file, unsigned long long key[])
{
    unsigned long long subkey[32];
    unsigned long long plaintext[2];
    unsigned long long ciphertext[2];
    unsigned long long IntVec[2];
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

    //obtain IV by using sub function to generate random numbers (32-bit integer)
    IntVec[0] = IV();
    IntVec[1] = IV();

    //write the Initialization vector to the front of the file
    ((unsigned long long*)(block))[0] = IntVec[0];
    ((unsigned long long*)(block))[1] = IntVec[1];

    //write IV as first block
    fout.write(block,16);

    for (unsigned long long i = 0; i+16 <= input_file_size; i+=16)
    {
        //read 16 bytes into the array block
        fin.read(block,16);

        //convert 16 bytes (equivalent to 128 bits) into two 64-bit words using explicit unsigned long long typecast
        plaintext[0] = ((unsigned long long*)(block))[0];
        plaintext[1] = ((unsigned long long*)(block))[1];

        //perform encryption using IV key
        speck_encryption(plaintext, ciphertext, subkey, IntVec);

        //rejoin back the two encrypted ciphertext words back into 16-byte block
        ((unsigned long long*)(block))[0] = ciphertext[0];
        ((unsigned long long*)(block))[1] = ciphertext[1];

        //write the ciphertext block into the output file
        fout.write(block,16);
    }

    //encrypt the last block
    unsigned int t = input_file_size & 0xf; // it means: t = input_file_size % 16;
    fin.read(block,t);

    //encrypt those 16 bytes in block
    plaintext[0] = ((unsigned long long*)(block))[0];
    plaintext[1] = ((unsigned long long*)(block))[1];

    //perform encryption of one block
    speck_encryption(plaintext, ciphertext, subkey,IntVec);

    //join both ciphertext into a single block
    ((unsigned long long*)(block))[0] = ciphertext[0];
    ((unsigned long long*)(block))[1] = ciphertext[1];

    //write out block
    fout.write(block,t);

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
    unsigned long long IntVec[2];

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

    //each iteration, read 16 bytes, decrypt it, then write the plaintext into the output file.
    fin.seekg (0, ios::beg); // start from the beginning of the file

    //Code copied until here, check and edit the rest of the code below! :)
    for (unsigned long long i = 0; i+16 <= input_file_size; i+=16)
    {

        //read 16 bytes into the array block
        fin.read(block,16);
        if(i==0)
        {
            // Read Initialization Vector from first 16 bytes
            IntVec[0] = ((unsigned long long*)(block))[0];
            IntVec[1] = ((unsigned long long*)(block))[1];

        }
        else
        {
            //convert 16 bytes (equivalent to 128 bits) into two 64-bit words using explicit unsigned long long typecast
            ciphertext[0] = ((unsigned long long*)(block))[0];
            ciphertext[1] = ((unsigned long long*)(block))[1];

            //perform decryption of one block
            speck_decryption( plaintext, ciphertext, subkey,IntVec);

            //rejoin back the two decrypted plaintext words back into 16-byte block
            ((unsigned long long*)(block))[0] = plaintext[0];
            ((unsigned long long*)(block))[1] = plaintext[1];

            //write the plaintext block into the output file
            fout.write(block,16);

        }
    }

    //decrypt the last block
    unsigned int t = input_file_size & 0xf; // it means: t = input_file_size % 16;
    fin.read(block,t);

    //decrypt those 16 bytes in block
    ciphertext[0] = ((unsigned long long*)(block))[0];
    ciphertext[1] = ((unsigned long long*)(block))[1];

    //perform decryption of one block
    speck_decryption( plaintext, ciphertext, subkey, IntVec);

    //join both plaintext into a single block
    ((unsigned long long*)(block))[0] = plaintext[0];
    ((unsigned long long*)(block))[1] = plaintext[1];

    //write out block
    fout.write(block,t);

    fin.close();
    fout.close();
}

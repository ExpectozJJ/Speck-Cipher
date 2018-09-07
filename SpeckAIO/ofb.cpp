/*
  This file defines three functions of SPECK
  1) speck_keyschedule()
     generate the round keys from the secret key)
  2) speck_encryption
     encrypt one plaintext block
  3) speck_decryption
     speck_decrypt
*/

using namespace std;

#define LCS(x,n) ( (x << n) | (x >> (64-n)) ) // right-rotate a 64-bit word x by n-bit positions
#define RCS(x,n) ( (x >> n) | (x << (64-n)) ) // right-rotate a 64-bit word x by n-bit positions
#define R(x,y,k) (x=RCS(x,8), x+=y, x^=k, y=LCS(y,3), y^=x)
#define InverseR(x,y,k)  ( y ^=x, y=RCS(y,3), x^=k, x-=y, x=LCS(x,8) )



// encrypt one block of message
// inputs: round keys is an array with 32 64-bit words
//         a plaintext block is an array with 2 64-bit words
// output: a ciphertext block (an array with 2 64-bit words)
void speck_encryption_ofb(unsigned long long plaintext[], unsigned long long ciphertext[], unsigned long long subkey[], unsigned long long vec[])
{
    unsigned long long i;
    ciphertext[0]=vec[0];
    ciphertext[1]=vec[1];

    for(i=0; i<32; i++)
    {
        R(ciphertext[1], ciphertext[0], subkey[i]);
        // cout << endl << ciphertext[0] << "  " << subkey[i] << endl;
    }

    vec[0]=ciphertext[0];
    vec[1]=ciphertext[1];

    ciphertext[0]^=plaintext[0];
    ciphertext[1]^=plaintext[1];

}

// decrypt one block of message
// inputs: round keys is an array with 32 64-bit words
//         a ciphertext block is an array with 2 64-bit words
// output: a plaintext block (an array with 2 64-bit words)
void speck_decryption_ofb(unsigned long long plaintext[], unsigned long long ciphertext[], unsigned long long subkey[], unsigned long long vec[])
{
    unsigned long long i;
    plaintext[0]=vec[0];
    plaintext[1]=vec[1];

    for(i=0; i<32; i++)
    {
        R(plaintext[1], plaintext[0], subkey[i]);
        // cout << endl << ciphertext[0] << "  " << subkey[i] << endl;
    }

    vec[0]=plaintext[0];
    vec[1]=plaintext[1];

    plaintext[0]^=ciphertext[0];
    plaintext[1]^=ciphertext[1];
}

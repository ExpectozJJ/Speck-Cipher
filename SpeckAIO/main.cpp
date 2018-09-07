/*
    This program encrypts file using SPECK-128-128
    It asks the user to choose between encryption and decryption,
    asks the user to enter the secret key,
    then ask the user to provide the name of the file to be encrypted (or to be decrypted),
    and provide the name of the file after encryption (or after decryption),
    then a function is called to encrypt (or decrypt) the file.
*/

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

void encrypt_file(string input_file, string output_file, unsigned long long key[], int mode);
void decrypt_file(string input_file, string output_file, unsigned long long key[], int mode);

int main()
{
    char encryption_or_decryption;
    int mode;
    string input_file, output_file;
    unsigned long long key[2];

    do
    {
        cout << "Please type e for encryption, type d for decryption, \nor type x to terminate the program: " ;
        cin >> encryption_or_decryption;
        if (encryption_or_decryption == 'e')
        {
            //encrypt a file
            cout << "please enter the name of the file to be encrypted: ";
            cin >> input_file;
            cout << "please enter the name of the encrypted file: ";
            cin >> output_file;

            //key[0] = 0x0706050403020100;
            //key[1] = 0x0f0e0d0c0b0a0908;

            cout << "please enter the first 64 bits of your key in hexadecimal format: ";
            cin >> hex >> key[0];
            cout << "please enter the second 64 bits of your key in hexadecimal format: ";
            cin >> hex >> key[1];

            do
            {
                cout<<"Please select your encryption mode: \n1) ECB \n2) CBC \n3) OFB"<<endl;
                cout<<"Encryption Mode: ";
                cin>>mode;
            }while (mode!= 1 && mode != 2 && mode !=3);

            encrypt_file(input_file, output_file, key, mode);
        }
        else if (encryption_or_decryption == 'd')
        {
            //decrypt a file
            cout << "please enter the name of the file to be decrypted: ";
            cin >> input_file;
            cout << "please enter the name of the decrypted file: ";
            cin >> output_file;

            //key[0] = 0x0706050403020100;
            //key[1] = 0x0f0e0d0c0b0a0908;

            cout << "please enter the first 64 bits of your key in hexadecimal format: ";
            cin >> hex >> key[0];
            cout << "please enter the second 64 bits of your key in hexadecimal format: ";
            cin >> hex >> key[1];

            do
            {
                cout<<"Please select your decryption mode: \n1) ECB \n2) CBC \n3) OFB"<<endl;
                cout<<"Decryption Mode: ";
                cin>>mode;
            }while (mode!= 1 && mode != 2 && mode !=3);

            decrypt_file(input_file, output_file, key,mode);

        }
        else if (encryption_or_decryption == 'x')
        {
            exit(0);
        }
    }
    while (encryption_or_decryption != 'e' && encryption_or_decryption != 'd' && encryption_or_decryption != 'x');

    return 0;
}




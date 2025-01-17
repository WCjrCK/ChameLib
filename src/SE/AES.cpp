#include <SE/AES.h>

AES::AES() {}

/**
 * @brief AES KGen
 *
 * @param key
 * @param k
 */
void AES::KGen(element_t key, int k)
{
    // TODO
    if (k != 256)
    {
        throw std::invalid_argument("AES::KGen(): k must be 256");
    }
    this->k = k;
    element_random(key);
}

/**
 * @brief AES KGen
 * 
 * @param key 
 */
void AES::KGen(element_t key)
{
    element_random(key);
}

/**
 * @brief AES Enc
 * 
 * @param ciphertext
 * @param key
 * @param plaintext
 */
void AES::Enc(mpz_t ciphertext, element_t key, mpz_t plaintext)
{
    unsigned char aes_key[element_length_in_bytes(key)];
    element_to_bytes(aes_key, key);

    size_t plaintext_size = (mpz_sizeinbase(plaintext, 2) + 7) / 8;
    unsigned char *plaintext_bytes = new unsigned char[plaintext_size];
    memset(plaintext_bytes, 0, sizeof(plaintext_bytes));
    mpz_export(plaintext_bytes, nullptr, 1, sizeof(plaintext_bytes[0]), 0, 0, plaintext);

    unsigned char ciphertext_bytes[256];
    int ciphertext_len;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, NULL); //  AES-256-CBC mode
    ciphertext_len = 0;
    int len;
    // encrypt
    EVP_EncryptUpdate(ctx, ciphertext_bytes, &len, plaintext_bytes, plaintext_size);
    ciphertext_len += len;
    EVP_EncryptFinal_ex(ctx, ciphertext_bytes + ciphertext_len, &len);
    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);

    // ciphertext_bytes -> ciphertext
    mpz_import(ciphertext, ciphertext_len, 1, sizeof(ciphertext_bytes[0]), 0, 0, ciphertext_bytes);
}

/**
 * @brief AES Dec
 * 
 * @param decrypted_plaintext
 * @param key
 * @param ciphertext
 */
void AES::Dec(mpz_t decrypted_plaintext, element_t key, mpz_t ciphertext)
{
    unsigned char aes_key[element_length_in_bytes(key)];
    element_to_bytes(aes_key, key);

    unsigned char ciphertext_bytes[256];
    memset(ciphertext_bytes, 0, sizeof(ciphertext_bytes));
    size_t ciphertext_size;
    mpz_export(ciphertext_bytes, &ciphertext_size, 1, sizeof(ciphertext_bytes[0]), 0, 0, ciphertext);

    unsigned char decrypted_bytes[256];
    int decrypted_len;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, NULL);
    decrypted_len = 0;
    int len;
    // decrypt
    EVP_DecryptUpdate(ctx, decrypted_bytes, &len, ciphertext_bytes, ciphertext_size);
    decrypted_len += len;
    EVP_DecryptFinal_ex(ctx, decrypted_bytes + decrypted_len, &len);
    decrypted_len += len;
    decrypted_bytes[decrypted_len] = '\0'; // add terminator
    EVP_CIPHER_CTX_free(ctx);

    mpz_import(decrypted_plaintext, decrypted_len, 1, sizeof(decrypted_bytes[0]), 0, 0, decrypted_bytes);
}

AES::~AES()
{
}
#ifndef dhaes_h
#define dhaes_h

#include "cryptlib.h"
#include "hmac.h"

//! Abstract base class for DHIES key derivation
class DHIES_KeyDerive
{
  virtual void DeriveKey(byte * out, int outSize, byte const * sharedSecret, int sharedSecretSize, byte const * publicKey, int publicKeySize) = 0
};

//! DHIES key derivation using P1363 KDF2 algorithm, paramterized by hash function
template<class H>
class DHIES_P1363_KDF2 : public DHIES_KeyDerive
{
  P1363_KDF2<H> kdf;

  void DeriveKey(byte * out, int outSize, byte const * sharedSecret, int sharedSecretSize, byte const * publicKey, int publicKeySize)
  {
    kdf.DeriveKey(out, outSize, sharedSecret, sharedSecretSize);
  }
};

//! DHIES key derivation freenet-style
class DHIES_Freenet_KeyDerive : public DHIES_KeyDerive
{
  void DeriveKey(byte * out, int outSize, byte const * sharedSecret, int sharedSecretSize, byte const * publicKey, int publicKeySize)
  {
    assert(sharedSecretSize == 128);
    assert(sharedSecretSize == 128);
    assert(outSize == 256);
    SHA256 h;
    HashFilter hf(h, new ArraySink(out, outSize));
    Integer U(publicKey, publicKeySize);
    Integer X(sharedSecret, sharedSecretSize);
    U.OpenPGPEncode(hf);
    X.OpenPGPEncode(hf);
    hf.MessageEnd();
    h.Final(out);
  }
}

//! DHIES key derivation function which is compatible with beecrypt. Parameterized by hash function.
template<class H>
class DHIES_BeeCrypt_KeyDerive : public DHIES_KeyDerive
{
  void DeriveKey(byte * out, int outSize, byte const * sharedSecret, int sharedSecretSize, byte const * publicKey, int publicKeySize)
  {
    assert(outSize == h.DigestSize());
    H h;
    h.Update(publicKey, publicKeySize);
    h.Update(sharedSecret, sharedSecretSize);
    h.Final(out);
  }
}

template<class KEY_GENERATOR, class SECRET_DERIVER>
class DHIES_EncryptorBase
{
public:
  
  KEY_GENERATOR keyGenerator;

  DHIES_EncryptorBase(Verifier & const verifier, KEY_GENERATOR const & keyGenerator, SECRET_DERIVER const & secretDeriver)
  : verifier_(verifier), secretDeriver_(secretDeriver), keyGenerator_(keyGenerator);
  {}

  DHIES_EncryptorBase(GDSAVerifier & publicKey)
  {
    GDSAVerifier & publicKey
  }

  void Encrypt(RandomNumberGenerator & rng, byte * out, int outSize)
  {
    int publicSize = akad->EphemeralPublicKeyLength();
    int privateSize = akad->EphemeralPrivateKeyLength();
    int agreedSize = akad->AgreedValueLength();

    SecBlock<byte> ephermal(privateSize + publicSize + agreedSize);

    byte * privatePtr = ephermal.ptr;
    byte * publicPtr = privatePtr + privateSize;
    byte * agreedPtr = publicPtr + publicSize;

    skad->GenerateKeyPair(rng, privatePtr, publicPtr)
    bool r = skad->Agree(agreedPtr, privatePtr, publicKey, false);
    secretDeriver.DeriveKey(out, outSize, agreedPtr, aggreedSize, publicptr, publicSize);
  }

protected:
  PK_SimpleKeyAgreementDomain const & skad;
  DHIES_KeyDerive & kdf;
  SecBlock<byte> publicKey;
}

template<class KEY_GENERATOR, class SECRET_DERIVER>
class DHIES_DecryptorBase
{
  void Decrypt(byte const * ephermalPublicKey, byte * out, int outSize)
  {
    assert(akad->EphemeralPrivateKeyLength() == privateKey.size());
    SecBlock<byte> agreed(akad->AgreedValueLength());
    skad->Agree(agreedPtr, privateKey.ptr, ephermalPublicKey, false);
    secretDeriver.DeriveKey(out, outSize, agreed.Ptr, aggreedSize, ephermalPublicKey, akad->EphemeralPublicKeyLength());
  }

protected:
  PK_SimpleKeyAgreementDomain const & skad;
  DHIES_KeyDerive & kdf;
  SecBlock<byte> publicKey;
  SecBlock<byte> privateKey;
};


template<class KEY_GENERATOR, class SECRET_DERIVER, class ENCRYPTOR, class AUTHENTICATOR>
class DHIES_Encryptor : public DHIES_EncryptorBase<KEY_GENERATOR, SECRET_DERIVER>
{
  DHIES_Encryptor(Verifier & const verifier, 
    KEY_GENERATOR const & keyGenerator, SECRET_DERIVER const & secretDeriver,
    ENCRYPTOR const & encryptor, AUTHENTICATOR const & authenticator)
  : DHIES_EncryptorBase(verifier, keyGenerator, secretDeriver), 
    encryptor_(encryptor), authenticator_(authenticator)
  {}

  Encrypt(RandomNumberGenerator & rng, byte const * plainText. int size, byte * out)
  {
    Raw
  }

protected:
  ENCRYPTOR encryptor_;
  AUTHENTICATOR authenticator_;
}
  
  void RawEncypt
  
  void Encrypt(RandomNumberGenerator &rng, const byte *plainText, unsigned int plainTextLength, Integer & U, Integer & tag, Integer & encM);
  {

  }
  void encrypt(byte const * M, unsigned int m_len, )
  {
    // plainText = M
    Integer U; // paper's gu, freenet's C[0]
    Integer tag; // macRes, freenet's C[1]
    Integer encM; // freenet's C[2]

    unsigned byte hash[H::DIGESTSIZE]; // paper's hv
    
    Integer u(rng, m_y.BitCount()); // XXX: is m_y.Bitcount the correct number of random bits?

    RawEncrypt(u, U, tag, X);

  }

  RawEncrypt(Integer const & u, const byte * M, const byte Mlen, Integer & U, Integer & tag, Integer & encM) const
  {
    // m_y is paper's pk (public key)


    Integer X; // freenet's guv

    // XXX: could this be sped up by precomputation as in el gamal?
    // yes, GDSADigestSigner::GDSADigestSigner(RandomNumberGenerator &rng, const Integer &pIn, const Integer &qIn, const Integer &gIn)

    U = a_exp_b_mod_c(m_g, u, m_p);
    X = a_exp_b_mod_c(m_y, u, m_p);

    unsigned byte macKey[H::MLEN];
    unsigned byte encKey[H::ELEN];
    h(U, X, macKey, encKey);

      
    mac(encKey, M, Mlen, tag);
    syme(macKey, M, Mlen, encM);
  }
};



typedef DHIES_Generator<GDSADigestSigner> DHIES_DSA_Generator;
typedef DHIES_Generator<ECSigner> DHIES_EC_Generator;


template<class Verifier, class Signer>
class DHIES_Signer
{
  void operator()(ECVerifier & publicKey, ECSigner & privateKey, byte * output, int outSize)
  {
  }
};

typedef DHIES_Secret<GDSADigestVerifier, GDSADigestSigner> DHIES_DSA_Secret;
typedef DHIES_Secret<ECVerifier, ECSigner> DHIES_EC_Secret;



template<class H>
class DHIES_EC_Secret
{
public:
  void operator()()
  {
    
  }
};

template<class H>
class DHIES_DSA_Secret
{
public:
  void operator()(GDSADigestVerifier & publicKey, GDSADigestSigner & privateKey, byte * output, int outSize)
  {
    
  }
};

template<class H>
class DHIES_Freenet_Secret
{
public:
  void operator()(GDSADigestVerifier & publicKey, GDSADigestSigner & privateKey, byte * output, int outSize)
  {

  }
}


//! This is 
template<class H>
class DHIES_DSA_Secret
{
public:
  //! Length of MAC key in bytes
  enum {MACKEYSIZE = 16};
  
  //! Length of symmetric encryption key in bytes
  enum {ENCKEYSIZE = 16};
  
  DhiesHash(H & h) : h_(h)
  {
    assert(MACKEYSIZE + ENCKEYSIZE == H::DIGESTSIZE); // xxx: compile time assertion
  }

  void Calculate(Integer const & u, Integer const & v)
  {
    

  }

private:
  H & h_;
}

private:
}

class DhiesFreenetHash
{
  //! Length of MAC key in bytes
  enum { MLEN = 16 };

  //! Length of symmetric encryption key in bytes
  enum { ELEN = 16 };
  void operator()(Integer const & a, Integer const & b, unsigned byte * macKey, unsigned byte * encKey)
  {

    // freenet uses last half of the hash bits for macKey unlike
    // the original paper which uses the first half

    // freenet uses first half of the hash bits for encKey unlike 
    // the original paper which uses the last half
    
    byte out[MLEN + ELEN];
    SHA256 sha;
    sha.Update(U);
    sha.Update(X);
    sha.Final(out);
    memcopy(out, macKey); //XXX:
    memcopy(out, encKey);
  }
}



template<class H, class SYMD, class MAC, class GYP, class KEY>
DhiesDecryptor
{
  bool decrypt(KEY const & key, Integer const & U, Integer const & tag, Integer const & encM, unsigned int M_len, Integer & m)
  {
    ps(P, S);
    
    Integer X = U ** sk;

    unsigned byte hash[H::DIGESTSIZE];
    h(U, X, hash);

    if (!macVer(hash, encM, tag))
      return false;
    
    symd(hash, encM, M_len, M);
    return true;
  }
};

class Freenet_SYMD
{
  enum { BLOCKSIZE = 16 } ;

  void operator()(unsigned byte const * hash, unsigned byte const * data, unsigned int size, Integer & out)
  {
    // freenet uses first half of the hash bits for encKey unlike 
    // the original paper which uses the last half
    unsigned byte * encKey = hv;
    RijndaelDncryption re(encKey, Freenet_H::DIGESTSIZE / 2);
    unsigned byte iv[BLOCKSIZE];
    memset(iv, 0, sizeof(iv));
    PGP_CFBDncryption pe(re, iv);
    vector<unsigned byte> bout(size);
    pe.ProcessString(&bout[0], size);
    out.Decode(&data[0], size);
  }
}

class Freenet_MAC_ver
{
  enum { DIGESTSIZE = 16 };
  
  bool operator()(byte const * hash, byte const * data, unsigned int size, Integer & tag)
  {
    // freenet uses last half of the hash bits for macKey unlike
    // the original paper which uses the first half
    unsigned byte * macKey = hv + Freenet_H::DIGESTSIZE / 2;
    HMAC<SHA> mac(macKey, Freenet_H::DIGESTSIZE / 2);
    mac.Update(M, m_len);
    unsigned byte bout[DIGESTSIZE];
    MAC.Final(bout);
    Integer out;
    tag.Decode(out, DIGESTSIZE);
    return out == tag;
  }
};

#endif
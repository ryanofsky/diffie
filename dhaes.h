#ifndef dhaes_h
#define dhaes_h

#include "cryptlib.h"
#include "hmac.h"

//! ElGamal Encryptor
class ElGamalEncryptor : public PK_WithPrecomputation<PK_FixedLengthEncryptor>
{
public:
	ElGamalEncryptor(const Integer &p, const Integer &g, const Integer &y);
	ElGamalEncryptor(BufferedTransformation &bt);

	void DEREncode(BufferedTransformation &bt) const;

	void Precompute(unsigned int precomputationStorage=16);
	void LoadPrecomputation(BufferedTransformation &storedPrecomputation);
	void SavePrecomputation(BufferedTransformation &storedPrecomputation) const;

	void Encrypt(RandomNumberGenerator &rng, const byte *plainText, unsigned int plainTextLength, byte *cipherText);

	unsigned int MaxPlainTextLength() const {return STDMIN(255U, m_p.ByteCount()-3);}
	unsigned int CipherTextLength() const {return 2*m_p.ByteCount();}

	void RawEncrypt(const Integer &k, const Integer &m, Integer &a, Integer &b) const;

	const Integer & GetModulus() const {return m_p;}
	const Integer & GetGenerator() const {return m_g;}
	const Integer & GetPublicResidue() const {return m_y;}

	const ModExpPrecomputation & GetGPC() const {return m_gpc;}
	const ModExpPrecomputation & GetYPC() const {return m_ypc;}

protected:
	ElGamalEncryptor() {}
	unsigned int ExponentBitLength() const;

	Integer m_p, m_g, m_y;
	ModExpPrecomputation m_gpc, m_ypc;
};

//! ElGamal Decryptor
class ElGamalDecryptor : public ElGamalEncryptor, public PK_FixedLengthDecryptor
{
public:
	ElGamalDecryptor(const Integer &p, const Integer &g, const Integer &y, const Integer &x);
	ElGamalDecryptor(RandomNumberGenerator &rng, unsigned int pbits);
	// generate a random private key, given p and g
	ElGamalDecryptor(RandomNumberGenerator &rng, const Integer &p, const Integer &g);

	ElGamalDecryptor(BufferedTransformation &bt);
	void DEREncode(BufferedTransformation &bt) const;

	unsigned int Decrypt(const byte *cipherText, byte *plainText);

	void RawDecrypt(const Integer &a, const Integer &b, Integer &m) const;

	const Integer & GetPrivateExponent() const {return m_x;}

protected:
	Integer m_x;
};



class Freenet_H
{
  enum { DIGESTSIZE = 32 };
  void operator()(Integer const & a, Integer const & b, unsigned byte * out)
  {
    SHA256 sha;
    sha.Update(U);
    sha.Update(X);
    sha.Final(out);
  }
}

class Freenet_MAC
{
  enum { DIGESTSIZE = 16 };
  
  void operator()(byte const * hash, byte const * data, unsigned int size, Integer & out)
  {
    // freenet uses last half of the hash bits for macKey unlike
    // the original paper which uses the first half
    unsigned byte * macKey = hv + Freenet_H::DIGESTSIZE / 2;
    HMAC<SHA> mac(macKey, Freenet_H::DIGESTSIZE / 2);
    mac.Update(M, m_len);
    unsigned byte bout[DIGESTSIZE];
    MAC.Final(bout);
    tag.Decode(out, DIGESTSIZE);
  }
}

class Freenet_SYME
{
  enum { BLOCKSIZE = 16 } ;

  void operator()(unsigned byte const * hash, unsigned byte const * data, unsigned int size, Integer & out)
  {
    // freenet uses first half of the hash bits for encKey unlike 
    // the original paper which uses the last half
    unsigned byte * encKey = hv;
    RijndaelEncryption re(encKey, Freenet_H::DIGESTSIZE / 2);
    unsigned byte iv[BLOCKSIZE];
    memset(iv, 0, sizeof(iv));
    PGP_CFBEncryption pe(re, iv);
    vector<unsigned byte> bout(size);
    pe.ProcessString(&bout[0], size);
    out.Decode(&data[0], size);
  }
}

class Freenet_GYP
{
  operator()(DSAPublicKey const & k, Integer const * G, Integer const * Y, Integer const * P)
  {
    G = &k.GetGenerator();
    Y = &k.GetPublicResidue();
    P = &k.GetModulus();    
  }
}

template<class H, class SYME, class MAC, class GYP, class KEY>
class Dhaes_Encryption
{
public:
    enum { sym_keylen = 16, sym_blocklen = 16 };
    
    Dhaes_Encryption(H h_ = H(), SYME syme_ = SYME(), MAC mac_ = MC(), GYP gyp_ = GYP())
    : h(h_), syme(syme_), mac(mac_), gyp(gyp_)
    {
    }

    // U = gu = C[0]
    // X = guv
    // hash = hv
    // tag = macRes = C[1]
    // encM = C[2]
    void encrypt(KEY const & k, byte const * M, unsigned int m_len, Integer & U, Integer & tag, Integer & encM)
    {
        const Integer *G, *Y, *P;
        gyp(k, G, Y, P);

        Integer u(r, pub_y.BitCount());
        Integer U(a_exp_b_mod_c(*G, u, *P));
        Integer X(a_exp_b_mod_c(*Y, u, *P));
        
        unsigned byte hash[H::DIGESTSIZE];
        h(U, X, hash);
        mac(hash, M, M_len, tag);
        syme(hash, M, m_len, encM);
    }
}

template<class H, class SYMD, class MAC, class GYP, class KEY>
Dheas_Decryption
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
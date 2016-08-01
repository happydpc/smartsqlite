/*
 * Codec class for SQLite3 encryption codec.
 * (C) 2010 Olivier de Gaalon
 * (C) 2016 Daniel Seither (Kullo GmbH)
 *
 * Distributed under the terms of the Botan license
 */

#include <cassert>

#include "codec.h"

Codec::Codec(void *db)
    : m_db(db)
{
    try
    {
        m_encipherFilter = get_cipher(BLOCK_CIPHER_STR, Botan::ENCRYPTION);
        assert(m_encipherFilter);
        m_decipherFilter = get_cipher(BLOCK_CIPHER_STR, Botan::DECRYPTION);
        assert(m_decipherFilter);
        m_cmac = new Botan::MAC_Filter(MAC_STR);
        m_encipherPipe.append(m_encipherFilter);
        m_decipherPipe.append(m_decipherFilter);
        m_macPipe.append(m_cmac);
    }
    catch(Botan::Exception e)
    {
        m_botanErrorMsg.reset(new std::string(e.what()));
    }
}

Codec::Codec(const Codec *other, void *db)
    : Codec(db)
{
    assert(other);
    m_hasReadKey = other->m_hasReadKey;
    m_hasWriteKey = other->m_hasWriteKey;
    m_readKey = other->m_readKey;
    m_ivReadKey = other->m_ivReadKey;
    m_writeKey = other->m_writeKey;
    m_ivWriteKey = other->m_ivWriteKey;
}

void Codec::generateWriteKey(const char *userPassword, int passwordLength)
{
    assert(userPassword);
    assert(passwordLength >= 0);
    auto pwLength = static_cast<std::string::size_type>(passwordLength);

    try
    {
        auto pbkdf = Botan::PBKDF::create(PBKDF_STR);
        Botan::SymmetricKey masterKey = pbkdf->derive_key(
                    KEY_SIZE + IV_DERIVATION_KEY_SIZE,
                    std::string(userPassword, pwLength),
                    reinterpret_cast<const Botan::byte*>(SALT_STR.c_str()),
                    SALT_SIZE,
                    PBKDF_ITERATIONS);
        m_writeKey = Botan::SymmetricKey(masterKey.bits_of().data(), KEY_SIZE);
        m_ivWriteKey = Botan::SymmetricKey(masterKey.bits_of().data() + KEY_SIZE, IV_DERIVATION_KEY_SIZE);

        m_hasWriteKey = true;
    }
    catch(Botan::Exception e)
    {
        m_botanErrorMsg.reset(new std::string(e.what()));
    }
}

void Codec::dropWriteKey()
{
    m_hasWriteKey = false;
}

void Codec::setReadIsWrite()
{
    m_readKey = m_writeKey;
    m_ivReadKey = m_ivWriteKey;
    m_hasReadKey = m_hasWriteKey;
}

void Codec::setWriteIsRead()
{
    m_writeKey = m_readKey;
    m_ivWriteKey = m_ivReadKey;
    m_hasWriteKey = m_hasReadKey;
}

unsigned char* Codec::encrypt(unsigned int page, unsigned char *data, bool useWriteKey)
{
    assert(data);
    memcpy(m_page, data, m_pageSize);

    try
    {
        m_encipherFilter->set_key(useWriteKey ? m_writeKey : m_readKey);
        m_encipherFilter->set_iv(getIVForPage(page, useWriteKey));
        m_encipherPipe.process_msg(m_page, m_pageSize);
        m_encipherPipe.read(
                    m_page,
                    m_encipherPipe.remaining(Botan::Pipe::LAST_MESSAGE),
                    Botan::Pipe::LAST_MESSAGE);
    }
    catch(Botan::Exception e)
    {
        m_botanErrorMsg.reset(new std::string(e.what()));
    }

    return m_page; //return location of newly ciphered data
}

void Codec::decrypt(unsigned int page, unsigned char *data)
{
    assert(data);
    try
    {
        m_decipherFilter->set_key(m_readKey);
        m_decipherFilter->set_iv(getIVForPage(page, false));
        m_decipherPipe.process_msg(data, m_pageSize);
        m_decipherPipe.read(
                    data,
                    m_decipherPipe.remaining(Botan::Pipe::LAST_MESSAGE),
                    Botan::Pipe::LAST_MESSAGE);
    }
    catch(Botan::Exception e)
    {
        m_botanErrorMsg.reset(new std::string(e.what()));
    }
}

Botan::InitializationVector Codec::getIVForPage(uint32_t page, bool useWriteKey)
{
    unsigned char intiv[4];
    Botan::store_le(page, intiv);
    m_cmac->set_key(useWriteKey ? m_ivWriteKey : m_ivReadKey);
    m_macPipe.process_msg(intiv, 4);
    return m_macPipe.read_all(Botan::Pipe::LAST_MESSAGE);
}

const char* Codec::getError()
{
    if (!m_botanErrorMsg) return nullptr;
    return m_botanErrorMsg->c_str();
}

void Codec::resetError()
{
    m_botanErrorMsg.reset();
}

#include "codec_c_interface.h"

void* InitializeNewCodec(void *db)
{
    return new Codec(db);
}
void* InitializeFromOtherCodec(const void *otherCodec, void *db)
{
    return new Codec(static_cast<const Codec*>(otherCodec), db);
}
void GenerateWriteKey(void *codec, const char *userPassword, int passwordLength)
{
    assert(codec);
    static_cast<Codec*>(codec)->generateWriteKey(userPassword, passwordLength);
}
void DropWriteKey(void *codec)
{
    assert(codec);
    static_cast<Codec*>(codec)->dropWriteKey();
}
void SetWriteIsRead(void *codec)
{
    assert(codec);
    static_cast<Codec*>(codec)->setWriteIsRead();
}
void SetReadIsWrite(void *codec)
{
    assert(codec);
    static_cast<Codec*>(codec)->setReadIsWrite();
}
unsigned char* Encrypt(void *codec, unsigned int page, unsigned char *data, bool useWriteKey)
{
    assert(codec);
    return static_cast<Codec*>(codec)->encrypt(page, data, useWriteKey);
}
void Decrypt(void *codec, unsigned int page, unsigned char *data)
{
    assert(codec);
    static_cast<Codec*>(codec)->decrypt(page, data);
}
void SetPageSize(void *codec, size_t pageSize)
{
    assert(codec);
    static_cast<Codec*>(codec)->setPageSize(pageSize);
}
bool HasReadKey(void *codec)
{
    assert(codec);
    return static_cast<Codec*>(codec)->hasReadKey();
}
bool HasWriteKey(void *codec)
{
    assert(codec);
    return static_cast<Codec*>(codec)->hasWriteKey();
}
void* GetDB(void *codec)
{
    assert(codec);
    return static_cast<Codec*>(codec)->getDB();
}
const char* GetError(void *codec)
{
    assert(codec);
    return static_cast<Codec*>(codec)->getError();
}
void ResetError(void *codec)
{
    assert(codec);
    static_cast<Codec*>(codec)->resetError();
}
void DeleteCodec(void *codec)
{
    assert(codec);
    delete static_cast<Codec*>(codec);
}
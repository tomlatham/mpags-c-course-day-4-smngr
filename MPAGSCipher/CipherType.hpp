#ifndef MPAGSCIPHER_CIPHERTYPE_HPP
#define MPAGSCIPHER_CIPHERTYPE_HPP

/**
 * \file CipherType.hpp
 * \brief Contains the declaration of the CipherType enumeration
 */

/**
 * \enum CipherType
 * \brief Enumeration to define the Types in which the ciphers can operate
 */
enum class CipherType {
  Caesar, ///< Apply cipher in encrypt Type
  Playfair  ///< Apply cipher in decrypt Type
};

#endif // MPAGSCIPHER_CIPHERTYPE_HPP 

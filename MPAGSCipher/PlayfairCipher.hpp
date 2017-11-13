#ifndef MPAGSCIPHER_PlayfairCIPHER_HPP
#define MPAGSCIPHER_PlayfairCIPHER_HPP

// Standard library includes
#include <string>
#include <vector>
#include <map>

// Our project headers
#include "CipherMode.hpp"

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {
  public:
    /**
     * Create a new PlayfairCipher with the given key
     *
     * \param key the key to use in the cipher
     */
    PlayfairCipher( const std::string& key );

    /**
     * Set a new key for the already initialized PlayfairCipher
     * 
     *  \param key the key to use in the cipher
     */
    void setKey( const std::string& key);

    /** 
    * \brief A function to apply the Playfair cipher to input text and return a string
    * 
    * The Playfair cipher uses a 5 x 5 grid of letters to cipher a string. The grid contains all the letters in the alphabet once with the exception of J. The position of the letters on the
    * grid determine the cipher method. For this reason, we first need to replace all instances of the input "J" with "I". The string is then split into digraphs (pairs of letters).
    * If a digraph contains repeating letters then an "X" is inserted in between the letters, unless the pair is "XX", in which case a "Q" is inserted. Finally, the length of the input string
    * must be even so a "Z" is appended if this is not the case.
    * 
    * The playfair cipher locates the coordinates (coord1, coord2) of each pair of letters in a digraph on the cipher grid. If the two letters are on the same row, then the cipher replaces
    * the letters with the letters one position to the right (left) if encrypting (decrypting). If the pair of letters are in the same column then both letters are replaced with letters one
    * position below (above) if encrypting (decrypting). If the letters aren't in the same column or row, then they form the corners of a rectangle. Each letter of the digraph is then substituted
    * with the opposite corner on the same row. 
    * 
    * \param inputText is the string we want to convert
    * \param cipherMode tells the function whether to encrypt or decrypt the text
    * 
    * \return outputString
    *
    */
    std::string applyCipher( const std::string& inputText, const CipherMode cipherMode ) const;

  private:
    /// The cipher key
    std::string key_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    /// Type definition for the coordinates in the 5x5 table
    using PlayfairCoords = std::pair<int,int>;

    /// Map linking the letters with the corresponding coordinates on the cipher grid
    std::map<char,PlayfairCoords> letterToCoordMap_;

    /// Map linking the coordinates on the cipher grid with the corresponding letters
    std::map<PlayfairCoords,char> coordToLetterMap_ ;
};

#endif

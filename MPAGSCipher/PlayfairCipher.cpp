// Standard library includes
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
// Out project headers
#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher( const std::string& key )
  : key_{key}
{
}

/**
 * \brief A function to generate the cipher map and store as a private variable.
 * @details [long description]
 * 
 * @param key [description]
 */

void PlayfairCipher::setKey( const std::string& key)
{
  // store the original key
  PlayfairCipher::key_ = key;

  // Append the alphabet
  
  key_ = key + "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  // Make sure the key is upper case

  std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);
  
  // Remove non-alpha characters

  auto key_remove = std::remove_if(key_.begin(), key_.end(), [] (char k) {return !std::isalpha(k);}); /// < This line iterates over the string key_ and uses the lambda [] placeholder to define the conditions to remove characters. Char k is the character on the iterator and the curly brackets define the function.

  key_.erase(key_remove,key_.end());

  // Change J -> I

  auto j_sub = [] (char k){
    if (k=='J')
      k = 'I';
    return k;
  };

  std::transform(key_.begin(), key_.end(), key_.begin(), j_sub);
  
  // Remove duplicated letters

  std::string encountered{""};
  
  auto check_dupl = [&encountered] (char k){
    if(encountered.find(k)!=std::string::npos){
      return true;
    }
    else
    {
      encountered += k;
      return false;
    }
  };

  auto remove_dupl = std::remove_if(key_.begin(), key_.end(), check_dupl);
  key_.erase(remove_dupl,key_.end());

  // Store the coords of each letter

  // Create a new label for the type 
  
  using CharToCoordMap = std::map<char, std::vector<int> >;
  using CoordToCharMap = std::map<std::vector<int>, char >;

  // Create and instance of this map

  CharToCoordMap letterToCoordMap;
  CoordToCharMap coordToLetterMap;

  // // Create a pair and insert it
  // std::pair< std::string, std::vector<int> > p0{"A",{0,0}};
  // keyMap.insert ( p0 );

  // Range-based loop over key_.

  int i{0};
  int j{0};

  for (char letter : key_)
  {
    // The next two lines are for debugging and print out the letter and its coordinates in the map.
    // std::cout << "Char: " << letter << std::endl;
    // std::cout << "Coord: [" << i << ", " << j << "]" << std::endl;
    std::pair< char, std::vector<int> > letterCoord{letter,{i,j}};
    std::pair< std::vector<int>, char > coordLetter{{i,j},letter};

    letterToCoordMap.insert( letterCoord );
    coordToLetterMap.insert( coordLetter );

    if(i==4) // Reach the end of the columns
    {
      ++j;   // Jump to next row
      i=0;   // Reset to the first column
    }
    else
    {
     i++;
      if((i*j)>16){
      std::cerr << "[error] key map has too many elements" << std::endl;
      return;
    }}


  }

  // Store private variables

  letterToCoordMap_ = letterToCoordMap;
  coordToLetterMap_ = coordToLetterMap;

  // Print out the map contents

  std::cout << "--- Playfair Cipher Map ---" << std::endl;
  std::cout << coordToLetterMap.find({0,0})->second << " " <<  coordToLetterMap.find({1,0})->second << " " <<  coordToLetterMap.find({2,0})->second << " " <<  coordToLetterMap.find({3,0})->second << " " <<  coordToLetterMap.find({4,0})->second << " " << std::endl;
  std::cout << coordToLetterMap.find({0,1})->second << " " <<  coordToLetterMap.find({1,1})->second << " " <<  coordToLetterMap.find({2,1})->second << " " <<  coordToLetterMap.find({3,1})->second << " " <<  coordToLetterMap.find({4,1})->second << " " << std::endl;
  std::cout << coordToLetterMap.find({0,2})->second << " " <<  coordToLetterMap.find({1,2})->second << " " <<  coordToLetterMap.find({2,2})->second << " " <<  coordToLetterMap.find({3,2})->second << " " <<  coordToLetterMap.find({4,2})->second << " " << std::endl;
  std::cout << coordToLetterMap.find({0,3})->second << " " <<  coordToLetterMap.find({1,3})->second << " " <<  coordToLetterMap.find({2,3})->second << " " <<  coordToLetterMap.find({3,3})->second << " " <<  coordToLetterMap.find({4,3})->second << " " << std::endl;
  std::cout << coordToLetterMap.find({0,4})->second << " " <<  coordToLetterMap.find({1,4})->second << " " <<  coordToLetterMap.find({2,4})->second << " " <<  coordToLetterMap.find({3,4})->second << " " <<  coordToLetterMap.find({4,4})->second << " " << std::endl;
}


std::string PlayfairCipher::applyCipher( std::string& inputText, const CipherMode cipherMode ) const
{

  std::string outputText{""};

  switch( cipherMode ) {

    case CipherMode::Encrypt :
    {
    // Change J → I

    std::replace (inputText.begin(), inputText.end(), 'J', 'I');

    // If repeated chars in a digraph add an X or Q if XX

    for(std::string::size_type i = 1; i < inputText.size(); ++i) {
      
      if (inputText[i] == inputText[i-1])
      {
        if (inputText[i] == 'X')
        {
          inputText.insert(i,"Q");
        }
        else
        {
          inputText.insert(i,"X");
        }
      }
     }

    // if the size of input is odd, add a trailing Z

    if (inputText.size() % 2)
    {
      inputText.append("Z");
    }
    
    // Loop over the input in Digraphs

    // Initialize coordinates and outputText

    std::vector<int> coord1 {0,0};
    std::vector<int> coord2 {0,0};
    
    for(std::string::size_type i = 0; i < inputText.size(); i+=2) {

      // std::cout << "Digraph " << inputText[i] << inputText[i+1] << std::endl; // Helpful for debugging.

      //   - Find the coords in the grid for each digraph

      coord1 = letterToCoordMap_.find(inputText[i])->second;
      coord2 = letterToCoordMap_.find(inputText[i+1])->second;
      
      //   - Apply the rules to these coords to get 'new' coords
      // If letters are on the same row, replace with letter on the right

      // We will find the coordinates first, so that if one is assigned

      if (coord1[1] == coord2[1])
      { 
        outputText += coordToLetterMap_.find({((coord1[0]+1)%5),coord1[1]})->second;
        outputText += coordToLetterMap_.find({((coord2[0]+1)%5),coord2[1]})->second;
      }

      // If in the same column, replace with the letters below

      else if (coord1[0] == coord2[0])
      {
        outputText += coordToLetterMap_.find({coord1[0],((coord1[1]+1)%5)})->second;
        outputText += coordToLetterMap_.find({coord2[0],((coord2[1]+1)%5)})->second;
      }

      // If they form a rectangle, replace with ones from corner on the same row

      else
      {
        outputText += coordToLetterMap_.find({coord2[0],coord1[1]})->second;
        outputText += coordToLetterMap_.find({coord1[0],coord2[1]})->second;
      }
    }
    break;
    }

    case CipherMode::Decrypt :
    {
    
    // Loop over the input in Digraphs

    // Initialize coordinates and outputText

    std::vector<int> coord1 {0,0};
    std::vector<int> coord2 {0,0};
    
    for(std::string::size_type i = 0; i < inputText.size(); i+=2) {

      // std::cout << "Digraph " << inputText[i] << inputText[i+1] << std::endl;

      //   - Find the coords in the grid for each digraph

      coord1 = letterToCoordMap_.find(inputText[i])->second;
      coord2 = letterToCoordMap_.find(inputText[i+1])->second;
      
      //   - Apply the rules to these coords to get 'new' coords
      // If letters are on the same row, replace with letter on the right

      // We will find the coordinates first, so that if one is assigned

      if (coord1[1] == coord2[1])
      { 
        outputText += coordToLetterMap_.find({((coord1[0]+4)%5),coord1[1]})->second;
        outputText += coordToLetterMap_.find({((coord2[0]+4)%5),coord2[1]})->second;
      }

      // If in the same column, replace with the letters below

      else if (coord1[0] == coord2[0])
      {
        outputText += coordToLetterMap_.find({coord1[0],((coord1[1]+4)%5)})->second;
        outputText += coordToLetterMap_.find({coord2[0],((coord2[1]+4)%5)})->second;
      }

      // If they form a rectangle, replace with ones from corner on the same row

      else
      {
        outputText += coordToLetterMap_.find({coord2[0],coord1[1]})->second;
        outputText += coordToLetterMap_.find({coord1[0],coord2[1]})->second;
      }
    }
    break;
    }
  }

  return outputText;
}
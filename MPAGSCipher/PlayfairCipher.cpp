// Standard library includes
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
// Out project headers
#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher( const std::string& key )
{
  this->setKey(key);
}

void PlayfairCipher::setKey( const std::string& key)
{
  // store the original key
  key_ = key;

  // Append the alphabet
  
  key_ += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  // Make sure the key is upper case

  std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);
  
  // Remove non-alpha characters

  auto key_remove = std::remove_if(key_.begin(), key_.end(), [] (char k) {return !std::isalpha(k);}); // < This line iterates over the string key_ and uses the lambda [] placeholder to define the conditions to remove characters. Char k is the character on the iterator and the curly brackets define the function.

  key_.erase(key_remove,key_.end());

  // Change J -> I

  auto j_sub = [] (char k){
    if (k=='J') {
      k = 'I';
    }
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

  // Range-based loop over key_.

  int i{0};
  int j{0};

  for (char letter : key_)
  {
    // The next two lines are for debugging and print out the letter and its coordinates in the map.
    // std::cout << "Char: " << letter << std::endl;
    // std::cout << "Coord: [" << i << ", " << j << "]" << std::endl;
    PlayfairCoords coord{i,j};

    letterToCoordMap_[ letter ] = coord;
    coordToLetterMap_[ coord ] = letter;

    // TEL: this can also be done using an indexed loop over the key_ and using
    // the following expressions:
    // row = i/5
    // column = i%5
    // See our solution for details.

    if(i==4) // Reach the end of the columns
    {
      ++j;   // Jump to next row
      i=0;   // Reset to the first column
    }
    else
    {
      ++i;
    }
  }

  // Print out the map contents

  std::cout << "--- Playfair Cipher Map ---" << std::endl;
  std::cout << coordToLetterMap_.find({0,0})->second << " " <<  coordToLetterMap_.find({1,0})->second << " " <<  coordToLetterMap_.find({2,0})->second << " " <<  coordToLetterMap_.find({3,0})->second << " " <<  coordToLetterMap_.find({4,0})->second << " " << std::endl;
  std::cout << coordToLetterMap_.find({0,1})->second << " " <<  coordToLetterMap_.find({1,1})->second << " " <<  coordToLetterMap_.find({2,1})->second << " " <<  coordToLetterMap_.find({3,1})->second << " " <<  coordToLetterMap_.find({4,1})->second << " " << std::endl;
  std::cout << coordToLetterMap_.find({0,2})->second << " " <<  coordToLetterMap_.find({1,2})->second << " " <<  coordToLetterMap_.find({2,2})->second << " " <<  coordToLetterMap_.find({3,2})->second << " " <<  coordToLetterMap_.find({4,2})->second << " " << std::endl;
  std::cout << coordToLetterMap_.find({0,3})->second << " " <<  coordToLetterMap_.find({1,3})->second << " " <<  coordToLetterMap_.find({2,3})->second << " " <<  coordToLetterMap_.find({3,3})->second << " " <<  coordToLetterMap_.find({4,3})->second << " " << std::endl;
  std::cout << coordToLetterMap_.find({0,4})->second << " " <<  coordToLetterMap_.find({1,4})->second << " " <<  coordToLetterMap_.find({2,4})->second << " " <<  coordToLetterMap_.find({3,4})->second << " " <<  coordToLetterMap_.find({4,4})->second << " " << std::endl;
}


std::string PlayfairCipher::applyCipher( const std::string& inputText, const CipherMode cipherMode ) const
{

  std::string outputText{inputText};

  // TEL - there's a lot of duplicated code here due to you introducing the switch this early
  //     - also, having the replacement of J with I and insertion of X or Q to separate
  //       identical letters in a digraph only in the encrypt raises some potential problems,
  //       since you cannot guarantee that the text provided by the user for decryption
  //       necessarily obeys the rules
  //     - see our solution for another approach

  switch( cipherMode ) {

    case CipherMode::Encrypt :
    {
    // Change J → I

    std::replace (outputText.begin(), outputText.end(), 'J', 'I');

    // If repeated chars in a digraph add an X or Q if XX

    for(std::string::size_type i {0}; i < outputText.size(); i+=2) {
      
      if (outputText[i] == outputText[i+1])
      {
        if (outputText[i] == 'X')
        {
          outputText.insert(i+1,"Q");
        }
        else
        {
          outputText.insert(i+1,"X");
        }
      }
     }

    // if the size of input is odd, add a trailing Z
    // (or add an X if the last character is already a Z)
    if (outputText.size() % 2)
    {
      outputText += (outputText[outputText.size()-1] == 'Z') ? 'X' : 'Z';
    }
    
    // Loop over the input in Digraphs

    // Initialize coordinates and outputText

    PlayfairCoords coord1 {0,0};
    PlayfairCoords coord2 {0,0};
    
    for(std::string::size_type i = 0; i < outputText.size(); i+=2) {

      // std::cout << "Digraph " << outputText[i] << outputText[i+1] << std::endl; // Helpful for debugging.

      //   - Find the coords in the grid for each digraph

      coord1 = letterToCoordMap_.find(outputText[i])->second;
      coord2 = letterToCoordMap_.find(outputText[i+1])->second;
      
      //   - Apply the rules to these coords to get 'new' coords
      // If letters are on the same row, replace with letter on the right

      // We will find the coordinates first, so that if one is assigned

      if (coord1.second == coord2.second)
      { 
        outputText[i]   = coordToLetterMap_.find({((coord1.first+1)%5),coord1.second})->second;
        outputText[i+1] = coordToLetterMap_.find({((coord2.first+1)%5),coord2.second})->second;
      }

      // If in the same column, replace with the letters below

      else if (coord1.first == coord2.first)
      {
        outputText[i]   = coordToLetterMap_.find({coord1.first,((coord1.second+1)%5)})->second;
        outputText[i+1] = coordToLetterMap_.find({coord2.first,((coord2.second+1)%5)})->second;
      }

      // If they form a rectangle, replace with ones from corner on the same row

      else
      {
        outputText[i]   = coordToLetterMap_.find({coord2.first,coord1.second})->second;
        outputText[i+1] = coordToLetterMap_.find({coord1.first,coord2.second})->second;
      }
    }
    break;
    }

    case CipherMode::Decrypt :
    {
    
    // Loop over the input in Digraphs

    // Initialize coordinates and outputText

    PlayfairCoords coord1 {0,0};
    PlayfairCoords coord2 {0,0};
    
    for(std::string::size_type i = 0; i < outputText.size(); i+=2) {

      // std::cout << "Digraph " << outputText[i] << outputText[i+1] << std::endl;

      //   - Find the coords in the grid for each digraph

      coord1 = letterToCoordMap_.find(outputText[i])->second;
      coord2 = letterToCoordMap_.find(outputText[i+1])->second;
      
      //   - Apply the rules to these coords to get 'new' coords
      // If letters are on the same row, replace with letter on the right

      // We will find the coordinates first, so that if one is assigned

      if (coord1.second == coord2.second)
      { 
        outputText[i]   = coordToLetterMap_.find({((coord1.first+4)%5),coord1.second})->second;
        outputText[i+1] = coordToLetterMap_.find({((coord2.first+4)%5),coord2.second})->second;
      }

      // If in the same column, replace with the letters below

      else if (coord1.first == coord2.first)
      {
        outputText[i]   = coordToLetterMap_.find({coord1.first,((coord1.second+4)%5)})->second;
        outputText[i+1] = coordToLetterMap_.find({coord2.first,((coord2.second+4)%5)})->second;
      }

      // If they form a rectangle, replace with ones from corner on the same row

      else
      {
        outputText[i]   = coordToLetterMap_.find({coord2.first,coord1.second})->second;
        outputText[i+1] = coordToLetterMap_.find({coord1.first,coord2.second})->second;
      }
    }
    break;
    }
  }

  return outputText;
}

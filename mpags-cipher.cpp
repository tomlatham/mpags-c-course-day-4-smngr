// Standard Library includes
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Our project headers
#include "CipherMode.hpp"
#include "CipherType.hpp"
#include "TransformChar.hpp"
#include "ProcessCommandLine.hpp"
#include "CaesarCipher.hpp"
#include "PlayfairCipher.hpp"
  
// Main function of the mpags-cipher program
int main(int argc, char* argv[])
{
  // Convert the command-line arguments into a more easily usable form
  const std::vector<std::string> cmdLineArgs {argv, argv+argc};

  // Options that might be set by the command-line arguments
  ProgramSettings settings { false, false, "", "", "", CipherMode::Encrypt, CipherType::Caesar };

  // Process command line arguments
  bool cmdLineStatus { processCommandLine(cmdLineArgs, settings) };

  // Any failure in the argument processing means we can't continue
  // Use a non-zero return value to indicate failure
  if( !cmdLineStatus ) {
    return 1;
  }

  // Handle help, if requested
  if (settings.helpRequested) {
    // Line splitting for readability
    std::cout
      << "Usage: mpags-cipher [-i <file>] [-o <file>] [-k <key>] [--encrypt/--decrypt]\n\n"
      << "Encrypts/Decrypts input alphanumeric text using classical ciphers\n\n"
      << "Available options:\n\n"
      << "  -h|--help        Print this help message and exit\n\n"
      << "  --version        Print version information\n\n"
      << "  -i FILE          Read text to be processed from FILE\n"
      << "                   Stdin will be used if not supplied\n\n"
      << "  -o FILE          Write processed text to FILE\n"
      << "                   Stdout will be used if not supplied\n\n"
      << "  -k KEY           Specify the cipher KEY\n"
      << "                   A null key, i.e. no encryption, is used if not supplied\n\n"
      << "  --encrypt        Will use the cipher to encrypt the input text (default behaviour)\n\n"
      << "  --decrypt        Will use the cipher to decrypt the input text\n\n"
      << "  --cipher TYPE    Sets the cipher type, either playfair or caeser \n\n";
    // Help requires no further action, so return from main,
    // with 0 used to indicate success
    return 0;
  }

  // Handle version, if requested
  if (settings.versionRequested) {
    std::cout << "0.2.0" << std::endl;
    // Like help, requires no further action, so return from main,
    // with 0 used to indicate success
    return 0;
  }

  // Initialise variables for processing input text
  char inputChar {'x'};
  std::string inputText {""};

  // Read in user input from stdin/file
  if (!settings.inputFile.empty()) {

    // Open the file and check that we can read from it
    std::ifstream inputStream(settings.inputFile);
    if (!inputStream.good()) {
      std::cerr << "[error] failed to create istream on file '" << settings.inputFile << "'" << std::endl;
      return 1;
    }

    // Loop over each character from the file
    while(inputStream >> inputChar)
    {
      inputText += transformChar(inputChar);
    }

  } else {

    // Loop over each character from user input
    // (until Return then CTRL-D (EOF) pressed)
    while(std::cin >> inputChar)
    {
      inputText += transformChar(inputChar);
    }
  }

  // Create the output text, initially just a copy of the input
  std::string outputText {inputText};

  switch(settings.cipherType){

    case(CipherType::Caesar):{

    // We have the key as a string, but the Caesar cipher needs an unsigned long, so we first need to convert it
    // We default to having a key of 0, i.e. no encryption, if no key was provided on the command line
    size_t caesarKey {0};
    if ( ! settings.cipherKey.empty() ) {
      // Before doing the conversion we should check that the string contains a
      // valid positive integer.
      // Here we do that by looping through each character and checking that it
      // is a digit. What is rather hard to check is whether the number is too
      // large to be represented by an unsigned long, so we've omitted that for
      // the time being.
      // (Since the conversion function std::stoul will throw an exception if the
      // string does not represent a valid unsigned long, we could check for and
      // handled that instead but we only cover exceptions very briefly on the
      // final day of this course - they are a very complex area of C++ that
      // could take an entire course on their own!)
      for ( const auto& elem : settings.cipherKey ) {
        if ( ! std::isdigit(elem) ) {
  	std::cerr << "[error] cipher key must be an unsigned long integer for Caesar cipher,\n"
  	          << "        the supplied key (" << settings.cipherKey << ") could not be successfully converted" << std::endl;
  	return 1;
        }
      }
      caesarKey = std::stoul(settings.cipherKey);
    }

    // Run the Caesar cipher (using the specified key and encrypt/decrypt flag) on the input text
    CaesarCipher cipher { caesarKey };
    outputText = cipher.applyCipher( inputText, settings.cipherMode );

    break;
    }

    case(CipherType::Playfair):
    {
      // Initialise an instance of the cipherKey class
      PlayfairCipher cipher {settings.cipherKey};

      // Apply the cipher
      outputText = cipher.applyCipher( inputText, settings.cipherMode );
      
      break;
    }
  }

  // Output the transliterated text
  if (!settings.outputFile.empty()) {

    // Open the file and check that we can write to it
    std::ofstream outputStream(settings.outputFile);
    if (!outputStream.good()) {
      std::cerr << "[error] failed to create ostream on file '" << settings.outputFile << "'" << std::endl;
      return 1;
    }

    // Print the transliterated text to the file
    outputStream << outputText << std::endl;

  } else {

    // Print the transliterated text to the screen
    std::cout << outputText << std::endl;
  }

  // No requirement to return from main, but we do so for clarity
  // and for consistency with other functions
  return 0;
}

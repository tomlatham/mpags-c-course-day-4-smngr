//! Unit Tests for MPAGSCipher processCommandLine interface
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ProcessCommandLine.hpp"

TEST_CASE("Help found correctly", "[commandline]") {

  ProgramSettings prog_set{false, false, "", "", "", CipherMode::Encrypt, CipherType::Caesar};
  std::vector<std::string> cmd_line = {"mpags-cipher", "--help"};
  bool res = processCommandLine(cmd_line, prog_set);

  REQUIRE( res );
  REQUIRE( prog_set.helpRequested );
}

TEST_CASE("Version found correctly", "[commandline]") {

  ProgramSettings prog_set{false, false, "", "", "", CipherMode::Encrypt, CipherType::Caesar};
  std::vector<std::string> cmd_line = {"mpags-cipher", "--version"};
  bool res = processCommandLine(cmd_line, prog_set);

  REQUIRE( res );
  REQUIRE( prog_set.versionRequested );
}

TEST_CASE("All other options processed correctly", "[commandline]") {

  ProgramSettings prog_set{false, false, "", "", "", CipherMode::Encrypt, CipherType::Playfair};
  std::vector<std::string> cmd_line = {"mpags-cipher", "-i", "test-in.txt", "-o", "test-out.txt", "-k", "10", "--decrypt","--cipher","playfair"};
  bool res = processCommandLine(cmd_line, prog_set);

  REQUIRE( res );
  REQUIRE( prog_set.inputFile == "test-in.txt" );
  REQUIRE( prog_set.outputFile == "test-out.txt" );
  REQUIRE( prog_set.cipherKey == "10" );
  REQUIRE( prog_set.cipherMode == CipherMode::Decrypt );
}

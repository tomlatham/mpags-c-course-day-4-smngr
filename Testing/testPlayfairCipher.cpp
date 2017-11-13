//! Unit Tests for MPAGSCipher CaesarCipher Class
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "PlayfairCipher.hpp"

TEST_CASE("Playfair Cipher Encrypt", "[playfair]") {
  std::string key {"PlayfairExample"};
  std::string inputText {"HIDETHEGOLDINTHETREESTUMP"};
  PlayfairCipher pc{key};
  pc.setKey(key);
  std::string outputStr{"BMODZBXDNABEKUDMUIXMMOUVIF"};
  REQUIRE( pc.applyCipher(inputText, CipherMode::Encrypt) == outputStr);
}

TEST_CASE("Playfair Cipher Decrypt", "[playfair]") {
  std::string key {"Example"};
  std::string inputText {"UAARBEDEXAPOPRQNXAXANR"};
  PlayfairCipher pc{key};
  pc.setKey(key);
  std::string outputStr{"WEWILXLMEXETATTHEXEXIT"};
  REQUIRE( pc.applyCipher(inputText, CipherMode::Decrypt) == outputStr);
}
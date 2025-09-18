/**
 * @file compressor.cpp
 * @brief source file of compressor.exe
 */
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <cctype>
#include <iostream>

#include "libcompressor/libcompressor.hpp"

using namespace std;

bool check_args_count(int argc);
string make_lower_cases_string_from_char_array(char* str);
libcompressor_CompressionAlgorithm parse_algo(char* input_str_in_char);
libcompressor_Buffer init_input_buffer(char* input_data);
void print(libcompressor_Buffer buffer);


/**
 * @brief main function
 */
int main(int argc, char** argv) {
  spdlog::set_level(spdlog::level::debug);
  spdlog::debug("Check cnt args");
  if (check_args_count(argc)) {
    spdlog::error("Must be at least 2 arguments");
    exit(EXIT_FAILURE);
  }

  spdlog::debug("Parsing first arg");
  libcompressor_CompressionAlgorithm algo = parse_algo(argv[1]);

  switch (algo) {
    case libcompressor_Bzip:
      spdlog::debug("algo == bzip");
      break;
    case libcompressor_Zlib:
      spdlog::debug("algo == zlib");
    default:
      break;
  }

  spdlog::debug("Initialising buffer");
  libcompressor_Buffer input_buffer = init_input_buffer(argv[2]);
  spdlog::debug("Initialised buffer");

  spdlog::debug("Compressing");
  libcompressor_Buffer result_buffer =
      libcompressor_compress(algo, input_buffer);
  spdlog::debug("compressed");

  spdlog::debug("compressed size == {}", result_buffer.size);
  if (result_buffer.size == 0) {
    spdlog::error("Zero size of output buffer");
    exit(EXIT_FAILURE);
  }

  spdlog::debug("printing");
  print(result_buffer);

  free(result_buffer.data);
  return EXIT_SUCCESS;
}

/**
 * @brief function to check the cnt of arguments
 * 
 * @param argc cnt of arguments
 * @return result of checking
 */
bool check_args_count(int argc) { return argc < 3; }

/**
 * @brief function parse first algo into libcompressing algorithm
 * 
 * @param input_str_in_char argument to parse
 * @return compression algorithm from enum
 */
libcompressor_CompressionAlgorithm parse_algo(char* input_str_in_char) {
  spdlog::debug("making lowercase");
  string input_str = make_lower_cases_string_from_char_array(input_str_in_char);
  spdlog::debug("made lower case == {}", input_str);

  if (string("zlib") == input_str) {
    return libcompressor_Zlib;
  }

  if (string("bzip") == input_str) {
    return libcompressor_Bzip;
  }

  spdlog::error("uknown algo");
  exit(EXIT_FAILURE);
}


/**
 * @brief make from char string with lower case letters
 * 
 * @param str char array to transform
 * @return transformed string
 */
string make_lower_cases_string_from_char_array(char* str) {
  string input_str = string(str);
  transform(input_str.begin(), input_str.end(), input_str.begin(), ::tolower);
  return input_str;
}

/**
 * @brief function to initialaise input buffer
 * 
 * @param input_data input data to compress
 * @return initialised buffer
 */
libcompressor_Buffer init_input_buffer(char* input_data) {
  libcompressor_Buffer buffer;
  buffer.data = input_data;
  buffer.size = strlen(input_data) + 1;
  return buffer;
}


/**
 * @brief print data in format %0.2hhx
 */
void print(libcompressor_Buffer buffer) {
  for (int i = 0; i < buffer.size; i++) {
    printf("%0.2hhx", buffer.data[i]);
  }
}
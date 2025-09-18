/**
 * @file libcompressor.hpp
 * @brief file with declaration of compression function, structure and enum
 */

/**
 * @brief enum with algo
 */
enum libcompressor_CompressionAlgorithm {
  libcompressor_Zlib,
  libcompressor_Bzip
};

/**
 * @brief structure for libcompressing data
 */
struct libcompressor_Buffer {
  char* data;
  int size;
};

/**
 * @brief compress data basing on algo
 *
 * @param algo algorithm of compressing
 * @param input structure with input data that must be compressed
 * @return structure with compressed data and its size
 */
libcompressor_Buffer libcompressor_compress(
    libcompressor_CompressionAlgorithm algo, libcompressor_Buffer input);

/**
 * @file libcompressor.cpp
 * @brief main source for compression library
 */
#include "libcompressor/libcompressor.hpp"

#include <bzlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>

#include "spdlog/spdlog.h"

bool check_Null_input(libcompressor_Buffer input);
libcompressor_Buffer create_null_buffer();
int allocate_memmory_for_data(libcompressor_Buffer* buffer, int size);
int compress_by_bzip(libcompressor_Buffer* result_buffer,
                     libcompressor_Buffer* input);
int compress_by_zlib(libcompressor_Buffer* result_buffer,
                     libcompressor_Buffer* input);


/**
 * @brief compress data basing on algo
 * 
 * @param algo algorithm of compressing
 * @param input structure with input data that must be compressed
 * @return structure with compressed data and its size
 */
libcompressor_Buffer libcompressor_compress(
    libcompressor_CompressionAlgorithm algo, libcompressor_Buffer input) {
  libcompressor_Buffer coded_data;

  spdlog::debug("check null input");
  if (check_Null_input(input)) {
    return create_null_buffer();
  }

  spdlog::debug("allocating memory");
  if (allocate_memmory_for_data(&coded_data, input.size)) {
    return create_null_buffer();
  }
  spdlog::debug("allocated memory");

  switch (algo) {
    case libcompressor_Bzip:
      spdlog::debug("bzip compressing");
      if (compress_by_bzip(&coded_data, &input)) {
        free(coded_data.data);
        return create_null_buffer();
      }
      spdlog::debug("bzip compressed");

      return coded_data;

    case libcompressor_Zlib:
      spdlog::debug("zlib compressing");
      if (compress_by_zlib(&coded_data, &input)) {
        free(coded_data.data);
        return create_null_buffer();
      }
      spdlog::debug("zlib compressed");

      return coded_data;

    default:
      spdlog::error("unknown algo");
      return create_null_buffer();
      break;
  }
}

/**
 * @brief checking input data for being null
 * 
 * @param input structure to check
 * @return result of checking
 */
bool check_Null_input(libcompressor_Buffer input) {
  return input.size == 0 || input.data == NULL;
}


/**
 * @brief creteas structure with null parameters
 * 
 * @return structure with null parameters
 */
libcompressor_Buffer create_null_buffer() {
  libcompressor_Buffer buffer;
  buffer.data = NULL;
  buffer.size = 0;
  return buffer;
}


/**
 * @brief allocate memory for buffer with compressed data
 * 
 * @param buffer structure where memory must be allocated
 * @param size size of input string data to compress
 * @param return zero, if memory was allocated succesfully, -1 otherwise
 */
int allocate_memmory_for_data(libcompressor_Buffer* buffer, int size) {
  buffer->data = NULL;
  buffer->data = (char*)malloc(sizeof(char) * (size + 1024));
  if (buffer->data == NULL) {
    return -1;
  }

  return 0;
}


/**
 * @brief function to compress data with bzip
 * 
 * @param result_buffer structure where result must be placed
 * @param input structure with data to compress
 * @return zero if data was compressed succesfully, 1 otherwise
 */
int compress_by_bzip(libcompressor_Buffer* result_buffer,
                     libcompressor_Buffer* input) {
  int ret_code = 0;
  ret_code = BZ2_bzBuffToBuffCompress(result_buffer->data,
                                      (unsigned int*)&result_buffer->size,
                                      input->data, input->size, 1, 0, 0);
  spdlog::debug("bzip return code == {}", ret_code);

  if (ret_code == BZ_OK) {
    return 0;
  } else {
    return 1;
  }
}


/**
 * @brief function to compress data with zlib
 * 
 * @param result_buffer structure where data must be placed
 * @param input structure with data to compress
 * @return zero if data was compressed succesfully, 1 otherwise
 */
int compress_by_zlib(libcompressor_Buffer* result_buffer,
                     libcompressor_Buffer* input) {
  uLongf comp_size = compressBound(input->size);
  result_buffer->size = (int)comp_size;

  int ret_code = 0;
  ret_code =
      compress((Bytef*)result_buffer->data, (uLongf*)&result_buffer->size,
               (Bytef*)input->data, input->size);
  spdlog::debug("zlib return code == {}", ret_code);

  if (ret_code == Z_OK) {
    return 0;
  } else {
    return 1;
  }
}
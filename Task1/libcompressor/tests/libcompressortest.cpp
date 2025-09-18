#include <gtest/gtest.h>

#include <iostream>
#include <random>
#include <string>

#include "libcompressor/libcompressor.hpp"
using namespace std;

TEST(CompressTest, empty_buffer) {
  libcompressor_Buffer input_buffer;
  input_buffer.size = 5;
  input_buffer.data = NULL;

  libcompressor_Buffer res =
      libcompressor_compress(libcompressor_Bzip, input_buffer);
  ASSERT_TRUE(res.size == 0 && res.data == NULL);

  res = libcompressor_compress(libcompressor_Zlib, input_buffer);
  ASSERT_TRUE(res.size == 0 && res.data == NULL);
}

TEST(CompressTest, zero_length) {
  libcompressor_Buffer input_buffer;
  input_buffer.size = 0;
  char str[] = "testing stream";
  input_buffer.data = str;

  libcompressor_Buffer res =
      libcompressor_compress(libcompressor_Bzip, input_buffer);
  ASSERT_TRUE(res.data == NULL && res.size == 0);

  res = libcompressor_compress(libcompressor_Zlib, input_buffer);
  ASSERT_TRUE(res.data == NULL && res.size == 0);
}

char* generate_random_char_arr(int length) {
  const char chars[] =
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  const size_t chars_len = strlen(chars);

  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<> distribution(0, chars_len - 1);

  char* random_array = new char[length + 1];

  for (size_t i = 0; i < length; i++) {
    random_array[i] = chars[distribution(generator)];
  }
  random_array[length] = '\0';

  return random_array;
}

TEST(CompressTest, check_non_zero_size) {
  libcompressor_Buffer input;
  for (int i = 2; i < 10; i++) {
    char* str = generate_random_char_arr((i + 1) * 2);
    input.data = str;
    input.size = strlen(str);
    cout << endl << input.data << "\t" << "size = " << input.size << endl;
    libcompressor_Buffer res;

    res = libcompressor_compress(libcompressor_Bzip, input);
    ASSERT_TRUE(res.size > 0);

    res = libcompressor_compress(libcompressor_Zlib, input);
    ASSERT_TRUE(res.size > 0);

    delete[] input.data;
  }
}

TEST(CompressTest, check_test_string_bzip) {
  libcompressor_Buffer input;
  char str[] = "test_string";
  input.data = str;
  input.size = strlen(str);

  libcompressor_Buffer res;
  res = libcompressor_compress(libcompressor_Bzip, input);

  unsigned char exp_ans[] = {
      0x42, 0x5a, 0x68, 0x31, 0x31, 0x41, 0x59, 0x26, 0x53, 0x59, 0x4a, 0x7c,
      0x69, 0x05, 0x00, 0x00, 0x04, 0x83, 0x80, 0x00, 0x00, 0x82, 0xa1, 0x1c,
      0x00, 0x20, 0x00, 0x22, 0x03, 0x68, 0x84, 0x30, 0x22, 0x50, 0xdf, 0x04,
      0x99, 0xe2, 0xee, 0x48, 0xa7, 0x0a, 0x12, 0x09, 0x4f, 0x8d, 0x20, 0xa0,
  };

  for (int i = 0; i < res.size; i++) {
    ASSERT_TRUE(res.data[i] == (char)exp_ans[i]);
  }
}

TEST(CompressTest, check_test_string_zlib) {
  libcompressor_Buffer input;
  char str[] = "test_string";
  input.data = str;
  input.size = strlen(str);

  libcompressor_Buffer res;
  res = libcompressor_compress(libcompressor_Zlib, input);

  unsigned char exp_ans[] = {
      0x78, 0x9c, 0x2b, 0x49, 0x2d, 0x2e, 0x89, 0x2f, 0x2e, 0x29,
      0xca, 0xcc, 0x4b, 0x07, 0x00, 0x1c, 0x79, 0x04, 0xb7,
  };

  for (int i = 0; i < res.size; i++) {
    ASSERT_TRUE(res.data[i] == (char)exp_ans[i]);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
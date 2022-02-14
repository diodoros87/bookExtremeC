#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>
#include <stdlib.h>

#define LANGUAGE "C90"

extern void print_spacetime(const char *file, const int line, const char * date, const char * time);

extern int print_many(const char * msg,   /* message to be printed     */
  const char * types, /* parameter types (i,s,e)     */...);

extern int print_assert(const char *file, int line, const char * date, const char * time, const char *msg) ;

#define assert_many(EX, MSG, TYPES, ...) \
  ((EX) || (print_many((MSG), (TYPES), __VA_ARGS__)) \
   || (print_assert(__FILE__, __LINE__, __DATE__, __TIME__, #EX)))
  
#define assert_one(EX,...) \
  ((EX) || (print_assert(__FILE__, __LINE__, __DATE__, __TIME__, #EX)))

#define LOG(format, ...) \
   fprintf(stderr, format, __VA_ARGS__); \
      if (ferror (stderr)) \
         perror("\nError while printing to stderr\n")
   
#define LOG_FUNC(function_name) LOG("\nF: " #function_name  " = %s \t", function_name)
   
#define FUNCTION_INFO(function_name) \
   LOG("\n%s \t", LANGUAGE); \
   LOG_FUNC(function_name)
   
#define PRINT_SPACETIME() \
   LOG("Source file:\t\t%s , Line %d\n", __FILE__, __LINE__); \
   LOG("Date:\t\t%s\n", __DATE__); \
   LOG("Time:\t\t%s\n", __TIME__)
   
#define print_and_assert(value, expected_value, value_string, format, assert_format) \
   LOG("%s: %s %s = "#format"\n", LANGUAGE, __FUNCTION__, (value_string), (value)); \
   assert_many(((value) == (expected_value)), "assert failed: ", (assert_format), (value_string), (value))

#define LOG_EXIT(function_name, info, status) \
   FUNCTION_INFO(function_name); \
   LOG("\ninfo = %s\n exit status = %d\n", (info), (status)); \
   PRINT_SPACETIME(); \
   exit(status)
   
#define LOG_EXIT_FREE(function_name, info, status) \
   FUNCTION_INFO(function_name); \
   PRINT_SPACETIME(); \
   LOG("\ninfo = %s\n exit status = %d\n", (info), (status)); \
   free(info); \
   exit(status)
   
   
#endif

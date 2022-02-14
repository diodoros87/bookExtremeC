#include "c_string.h"
#include "print.h"
#include <math.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>

char * concatenate(const char * first, const char * second) {
   if (! first || ! second) { 
      LOG_EXIT(__FUNCTION__, "first / second string is null", EXIT_FAILURE);
   }
   char * result = calloc(strlen(first) + strlen(second) + 1, sizeof (char));
   if (result) {
      strcpy(result, first);
      strcat(result, second);
   }
   else {
      LOG_FUNC(__FUNCTION__);
      LOG("%s\n", "Out of memory");
   }
   return result;
}

char * concatenate_many(const char * first, ...) {
   if (! first) { 
      LOG_EXIT(__FUNCTION__, "first string is null", EXIT_FAILURE);
   }
   va_list arg_list;
   va_start( arg_list, first );
   const char * arg_string;
   const char * result = first;
   const char * previous = NULL;
   do {
      arg_string = va_arg(arg_list, char *);
      if (NULL == arg_string)
         break;
      else {
         result = concatenate(result, arg_string);
         free(previous);
         previous = result;
      }
   } while (1);
   va_end(arg_list);
   return result;
}

char * concatenate_many_free_args(const char * first, int call_free_first, ...) {
   if (! first) { 
      LOG_EXIT(__FUNCTION__, "first string is null", EXIT_FAILURE);
   }
   va_list arg_list;
   va_start( arg_list, call_free_first );
   const char * arg_string;
   const char * result = first;
   const char * previous = NULL;
   int call_free;
   do {
      arg_string = va_arg(arg_list, char *);
      if (NULL == arg_string)
         break;
      else {
         result = concatenate(result, arg_string);
         free(previous);
         previous = result;
      }
      call_free = va_arg(arg_list, int);
      if (0 != call_free)
         free(arg_string);
   } while (1);
   if (0 != call_free_first)
      free(first);
   va_end(arg_list);
   return result;
}

char * copy_string(const char * source) {
   if (! source) { 
      LOG_EXIT(__FUNCTION__, "source string is null", EXIT_FAILURE);
   }
   char * result = NULL;
   ALLOCATE_STRING(result, strlen(source) + 1);
   if (NULL != result)
      strcpy(result, source);
   return result;
}

#define digits(x) _Generic((x), \
                           short int:              digits_i, \
                           int:                    digits_i, \
                           long int:               digits_i, \
                           long long int:          digits_i, \
                           unsigned short int:     digits_u, \
                           unsigned int:           digits_u, \
                           unsigned long int:      digits_u, \
                           unsigned long long int: digits_u, \
                           float:                  digits_d, \
                           double:                 digits_d, \
                           long double:            digits_d  \
                  )(x)
                           
size_t digits_u(unsigned long long x) {
   LOG_FUNC(__FUNCTION__);
   LOG(" ++++++++++   x = %llu\n", x);
   size_t number_of_digits = 1;
   while (x > 9) {
      x /= 10;
      number_of_digits++;
   }
   return number_of_digits;
}

size_t digits_i(long long x) {
   LOG_FUNC(__FUNCTION__);
   LOG(" ++++++++++   x = %lld\n", x);
   if (x >= 0)
      return digits_u(x); 
   size_t number_of_digits = 1;
   while (x < -9) {
      x /= 10;
      number_of_digits++;
   }
   return number_of_digits;
}

const size_t precision = 6;

size_t digits_d(long double x) {
   LOG_FUNC(__FUNCTION__);
   LOG(" ++++++++++   x = %LG\n", x);
   long double integral, fractional;
   fractional = modf(x, &integral);
   if (integral < 0)
      integral = -integral;
   size_t number_of_digits = 1;
   while (x > 9) {
      x /= 10;
      number_of_digits++;
   }
   return number_of_digits + 1 + precision; /* number_of_digits + dot + precision  */ 
}

char * to_string_u(const unsigned long long x) {
   int length = digits(x);
   char * buffer; 
   ALLOCATE_STRING(buffer, length + 1);
   if (NULL == buffer)
      return NULL;
   buffer[length] = '\0';
   unsigned long long temp = x;
   do {
      buffer[--length] = temp % 10 + '0';
      temp /= 10;
   } while (temp > 0);
   assert_many(strtoull(buffer, NULL, 10) == x, "assert failed: buffer = \'", "s s llu s llu", buffer, "\' and ", strtoull(buffer, NULL, 10), " != ", x);
   return buffer;
}

char * to_string_i(const long long x) {
   if (x >= 0)
      return to_string_u(x);
   int length = digits(x);
   char * buffer; 
   if (x < 0)
      length++;
   ALLOCATE_STRING(buffer, length + 1);
   if (NULL == buffer)
      return NULL;
   if (x < 0)
      buffer[0] = '-';
   unsigned long long temp;
   if (LLONG_MIN == x)
      temp = -(x + 1) + (unsigned long long)1; /* LLONG_MIN == - LLONG_MAX - 1    */
   else
      temp = -x;
   char * digits_string = to_string_u(temp);
   strcpy(buffer + 1, digits_string);
   free(digits_string);
   assert_many(atoll(buffer) == x, "assert failed: buffer = \'", "s s lld s lld", buffer, "\' and ", atoll(buffer), " != ", x);
   return buffer;
}

char * to_string_d(const long double x) {
   const char *fmt = "%LG";
   char * buffer; 
   int length = snprintf(NULL, 0, fmt, x);
   LOG(" ++++++++++   length = %d\n", length);
   ALLOCATE_STRING(buffer, length + 1);
   if (NULL == buffer)
      return NULL;
   if (0 > snprintf(buffer, length + 1, fmt, x)) {
      LOG("%s", " snprintf: encoding error occurs\n");
   }/*
   LOG_FUNC(__FUNCTION__);
   LOG(" ++++++++++   buffer = %s\n", buffer);
   LOG(" ++++++++++   x = %Lg\n", x);
   LOG(" ++++++++++   strtold(buffer, NULL) = %Lg\n", strtold(buffer, NULL));
   assert_many(strtold(buffer, NULL) == x, "assert failed: \'", "Lg s Lg c", strtold(buffer, NULL), "\' != \'", x, '\'');*/
   return buffer;
}

/*
 * 可变形参表相关的宏定义
 */

#ifndef INCLUDE_VARGS_H_
#define INCLUDE_VARGS_H_

typedef __builtin_va_list valist;

#define va_start(ap, last) 	(__builtin_va_start(ap, last))
#define va_arg(ap, type) 	(__builtin_va_arg(ap, type))
#define va_end(ap)

#endif

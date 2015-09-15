struct _IO_FILE_plus;
extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;
struct _IO_FILE;
extern struct _IO_FILE *stdin;
extern struct _IO_FILE *stdout;
extern struct _IO_FILE *stderr;
extern int sys_nerr;
extern const char *const sys_errlist[];
typedef struct _IO_FILE _IO_FILE;
extern int _IO_getc(_IO_FILE *__fp);
extern struct _IO_FILE *stdin;
extern __inline __attribute__((__gnu_inline__)) int getchar(void)
{
  return _IO_getc(stdin);
}
typedef struct _IO_FILE FILE;
struct _IO_marker;
typedef long int __off_t;
typedef void _IO_lock_t;
typedef long int __off64_t;
typedef unsigned long int size_t;
struct  _IO_FILE
{
  int _flags;
  char *_IO_read_ptr;
  char *_IO_read_end;
  char *_IO_read_base;
  char *_IO_write_base;
  char *_IO_write_ptr;
  char *_IO_write_end;
  char *_IO_buf_base;
  char *_IO_buf_end;
  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;
  struct _IO_marker *_markers;
  struct _IO_FILE *_chain;
  int _fileno;
  int _flags2;
  __off_t _old_offset;
  unsigned short int _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1L];
  _IO_lock_t *_lock;
  __off64_t _offset;
  void *__pad1;
  void *__pad2;
  void *__pad3;
  void *__pad4;
  size_t __pad5;
  int _mode;
  char _unused2[20L];
};
extern int __uflow(_IO_FILE *);
extern __inline __attribute__((__gnu_inline__)) int fgetc_unlocked(FILE *__fp)
{
  return __builtin_expect((*__fp)._IO_read_ptr >= (*__fp)._IO_read_end, 0) ? __uflow(__fp) : *((unsigned char *)(*__fp)._IO_read_ptr++);
}
extern __inline __attribute__((__gnu_inline__)) int getc_unlocked(FILE *__fp)
{
  return __builtin_expect((*__fp)._IO_read_ptr >= (*__fp)._IO_read_end, 0) ? __uflow(__fp) : *((unsigned char *)(*__fp)._IO_read_ptr++);
}
extern __inline __attribute__((__gnu_inline__)) int getchar_unlocked(void)
{
  return __builtin_expect((*stdin)._IO_read_ptr >= (*stdin)._IO_read_end, 0) ? __uflow(stdin) : *((unsigned char *)(*stdin)._IO_read_ptr++);
}
extern int _IO_putc(int __c, _IO_FILE *__fp);
extern struct _IO_FILE *stdout;
extern __inline __attribute__((__gnu_inline__)) int putchar(int __c)
{
  return _IO_putc(__c, stdout);
}
extern int __overflow(_IO_FILE *, int);
extern __inline __attribute__((__gnu_inline__)) int fputc_unlocked(int __c, FILE *__stream)
{
  return __builtin_expect((*__stream)._IO_write_ptr >= (*__stream)._IO_write_end, 0) ? __overflow(__stream, (unsigned char)__c) : (unsigned char)(*(*__stream)._IO_write_ptr++ = __c);
}
extern __inline __attribute__((__gnu_inline__)) int putc_unlocked(int __c, FILE *__stream)
{
  return __builtin_expect((*__stream)._IO_write_ptr >= (*__stream)._IO_write_end, 0) ? __overflow(__stream, (unsigned char)__c) : (unsigned char)(*(*__stream)._IO_write_ptr++ = __c);
}
extern __inline __attribute__((__gnu_inline__)) int putchar_unlocked(int __c)
{
  return __builtin_expect((*stdout)._IO_write_ptr >= (*stdout)._IO_write_end, 0) ? __overflow(stdout, (unsigned char)__c) : (unsigned char)(*(*stdout)._IO_write_ptr++ = __c);
}
extern __inline __attribute__((__nothrow__)) __attribute__((__leaf__)) __attribute__((__warn_unused_result__)) __attribute__((__gnu_inline__)) int feof_unlocked(FILE *__stream)
{
  return ((*__stream)._flags & 16) != 0;
}
extern __inline __attribute__((__nothrow__)) __attribute__((__leaf__)) __attribute__((__warn_unused_result__)) __attribute__((__gnu_inline__)) int ferror_unlocked(FILE *__stream)
{
  return ((*__stream)._flags & 32) != 0;
}
extern __inline __attribute__((__nothrow__)) __attribute__((__leaf__)) __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) int sprintf(char *__restrict __s, const char *__restrict __fmt, ...)
{
  return __builtin___sprintf_chk(__s, 2 - 1, __builtin_object_size(__s, 2 > 1), __fmt, __builtin_va_arg_pack());
}
typedef __builtin_va_list __gnuc_va_list;
extern __inline __attribute__((__nothrow__)) __attribute__((__leaf__)) __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) int vsprintf(char *__restrict __s, const char *__restrict __fmt, __gnuc_va_list __ap)
{
  return __builtin___vsprintf_chk(__s, 2 - 1, __builtin_object_size(__s, 2 > 1), __fmt, __ap);
}
extern __inline __attribute__((__nothrow__)) __attribute__((__format__(__printf__, 3, 4))) __attribute__((__leaf__)) __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) int snprintf(char *__restrict __s, size_t __n, const char *__restrict __fmt, ...)
{
  return __builtin___snprintf_chk(__s, __n, 2 - 1, __builtin_object_size(__s, 2 > 1), __fmt, __builtin_va_arg_pack());
}
extern __inline __attribute__((__nothrow__)) __attribute__((__format__(__printf__, 3, 0))) __attribute__((__leaf__)) __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) int vsnprintf(char *__restrict __s, size_t __n, const char *__restrict __fmt, __gnuc_va_list __ap)
{
  return __builtin___vsnprintf_chk(__s, __n, 2 - 1, __builtin_object_size(__s, 2 > 1), __fmt, __ap);
}
extern int __fprintf_chk(FILE *__restrict __stream, int __flag, const char *__restrict __format, ...);
extern __inline __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) int fprintf(FILE *__restrict __stream, const char *__restrict __fmt, ...)
{
  return __fprintf_chk(__stream, 2 - 1, __fmt, __builtin_va_arg_pack());
}
extern int __printf_chk(int __flag, const char *__restrict __format, ...);
extern __inline __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) int printf(const char *__restrict __fmt, ...)
{
  return __printf_chk(2 - 1, __fmt, __builtin_va_arg_pack());
}
extern int __vfprintf_chk(FILE *__restrict __stream, int __flag, const char *__restrict __format, __gnuc_va_list __ap);
extern __inline __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) int vprintf(const char *__restrict __fmt, __gnuc_va_list __ap)
{
  return __vfprintf_chk(stdout, 2 - 1, __fmt, __ap);
}
extern __inline __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) int vfprintf(FILE *__restrict __stream, const char *__restrict __fmt, __gnuc_va_list __ap)
{
  return __vfprintf_chk(__stream, 2 - 1, __fmt, __ap);
}
extern int __dprintf_chk(int __fd, int __flag, const char *__restrict __fmt, ...) __attribute__((__format__(__printf__, 3, 4)));
extern __inline __attribute__((__format__(__printf__, 2, 3))) __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) int dprintf(int __fd, const char *__restrict __fmt, ...)
{
  return __dprintf_chk(__fd, 2 - 1, __fmt, __builtin_va_arg_pack());
}
extern int __vdprintf_chk(int __fd, int __flag, const char *__restrict __fmt, __gnuc_va_list __arg) __attribute__((__format__(__printf__, 3, 0)));
extern __inline __attribute__((__format__(__printf__, 2, 0))) __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) int vdprintf(int __fd, const char *__restrict __fmt, __gnuc_va_list __ap)
{
  return __vdprintf_chk(__fd, 2 - 1, __fmt, __ap);
}
extern char *__gets_chk(char *__str, size_t) __attribute__((__warn_unused_result__));
extern char *__gets_warn(char *__str) __asm("""gets") __attribute__((__warn_unused_result__)) __attribute__((__warning__("please use fgets or getline instead, gets can't ""specify buffer size")));
extern __inline __attribute__((__warn_unused_result__)) __attribute__((__deprecated__)) __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) char *gets(char *__str)
{
  if (__builtin_object_size(__str, 2 > 1) != (unsigned long int) -1)
    {
      return __gets_chk(__str, __builtin_object_size(__str, 2 > 1));
    }
  return __gets_warn(__str);
}
extern char *__fgets_chk(char *__restrict __s, size_t __size, int __n, FILE *__restrict __stream) __attribute__((__warn_unused_result__));
extern char *__fgets_chk_warn(char *__restrict __s, size_t __size, int __n, FILE *__restrict __stream) __asm("""__fgets_chk") __attribute__((__warn_unused_result__)) __attribute__((__warning__("fgets called with bigger size than length ""of destination buffer")));
extern char *__fgets_alias(char *__restrict __s, int __n, FILE *__restrict __stream) __asm("""fgets") __attribute__((__warn_unused_result__));
extern __inline __attribute__((__warn_unused_result__)) __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) char *fgets(char *__restrict __s, int __n, FILE *__restrict __stream)
{
  if (__builtin_object_size(__s, 2 > 1) != (unsigned long int) -1)
    {
      if (!__builtin_constant_p(__n) || __n <= 0)
        {
          return __fgets_chk(__s, __builtin_object_size(__s, 2 > 1), __n, __stream);
        }
      if ((unsigned long int)__n > __builtin_object_size(__s, 2 > 1))
        {
          return __fgets_chk_warn(__s, __builtin_object_size(__s, 2 > 1), __n, __stream);
        }
    }
  return __fgets_alias(__s, __n, __stream);
}
extern size_t __fread_chk(void *__restrict __ptr, size_t __ptrlen, size_t __size, size_t __n, FILE *__restrict __stream) __attribute__((__warn_unused_result__));
extern size_t __fread_chk_warn(void *__restrict __ptr, size_t __ptrlen, size_t __size, size_t __n, FILE *__restrict __stream) __asm("""__fread_chk") __attribute__((__warn_unused_result__)) __attribute__((__warning__("fread called with bigger size * nmemb than length ""of destination buffer")));
extern size_t __fread_alias(void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream) __asm("""fread") __attribute__((__warn_unused_result__));
extern __inline __attribute__((__warn_unused_result__)) __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) size_t fread(void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream)
{
  if (__builtin_object_size(__ptr, 0) != (unsigned long int) -1)
    {
      if ((!__builtin_constant_p(__size) || !__builtin_constant_p(__n)) || (__size | __n) >= (unsigned long int)1 << 8 * sizeof(size_t) / 2)
        {
          return __fread_chk(__ptr, __builtin_object_size(__ptr, 0), __size, __n, __stream);
        }
      if (__size * __n > __builtin_object_size(__ptr, 0))
        {
          return __fread_chk_warn(__ptr, __builtin_object_size(__ptr, 0), __size, __n, __stream);
        }
    }
  return __fread_alias(__ptr, __size, __n, __stream);
}
extern size_t __fread_unlocked_chk(void *__restrict __ptr, size_t __ptrlen, size_t __size, size_t __n, FILE *__restrict __stream) __attribute__((__warn_unused_result__));
extern size_t __fread_unlocked_chk_warn(void *__restrict __ptr, size_t __ptrlen, size_t __size, size_t __n, FILE *__restrict __stream) __asm("""__fread_unlocked_chk") __attribute__((__warn_unused_result__)) __attribute__((__warning__("fread_unlocked called with bigger size * nmemb than ""length of destination buffer")));
extern size_t __fread_unlocked_alias(void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream) __asm("""fread_unlocked") __attribute__((__warn_unused_result__));
extern __inline __attribute__((__warn_unused_result__)) __attribute__((__always_inline__)) __attribute__((__gnu_inline__)) __attribute__((__artificial__)) size_t fread_unlocked(void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream)
{
  if (__builtin_object_size(__ptr, 0) != (unsigned long int) -1)
    {
      if ((!__builtin_constant_p(__size) || !__builtin_constant_p(__n)) || (__size | __n) >= (unsigned long int)1 << 8 * sizeof(size_t) / 2)
        {
          return __fread_unlocked_chk(__ptr, __builtin_object_size(__ptr, 0), __size, __n, __stream);
        }
      if (__size * __n > __builtin_object_size(__ptr, 0))
        {
          return __fread_unlocked_chk_warn(__ptr, __builtin_object_size(__ptr, 0), __size, __n, __stream);
        }
    }
  if (((__builtin_constant_p(__size) && __builtin_constant_p(__n)) && (__size | __n) < (unsigned long int)1 << 8 * sizeof(size_t) / 2) && __size * __n <= 8)
    {
      size_t __cnt = __size * __n;
      char *__cptr = (char *)__ptr;
      if (__cnt == 0)
        {
          return 0;
        }
      for (; __cnt > 0;  --__cnt)
        {
          int __c = __builtin_expect((*__stream)._IO_read_ptr >= (*__stream)._IO_read_end, 0) ? __uflow(__stream) : *((unsigned char *)(*__stream)._IO_read_ptr++);
          if (__c ==  -1)
            {
              break;
            }
          *__cptr++ = __c;
        }
      return (__cptr - (char *)__ptr) / __size;
    }
  return __fread_unlocked_alias(__ptr, __size, __n, __stream);
}
void some_work(int *X, char task)
{
  int i;
  for (i = 0; i < 100000;  ++i)
    {
      X[i] = i;
    }
  printf("Thread %d have done task %c\n", omp_get_thread_num(), task);
}
struct  nanos_args_0_t
{
  int X[100000L];
  int *a;
};
enum mcc_enum_anon_5
{
  NANOS_OK = 0,
  NANOS_UNKNOWN_ERR = 1,
  NANOS_UNIMPLEMENTED = 2,
  NANOS_ENOMEM = 3,
  NANOS_INVALID_PARAM = 4,
  NANOS_INVALID_REQUEST = 5
};
typedef enum mcc_enum_anon_5 nanos_err_t;
typedef unsigned int nanos_copy_id_t;
typedef void *nanos_wd_t;
extern nanos_err_t nanos_get_addr(nanos_copy_id_t copy_id, void **addr, nanos_wd_t cwd);
extern void nanos_handle_error(nanos_err_t err);
static void nanos_xlate_fun_simpletestc_0(struct nanos_args_0_t *const arg, void *wd)
{
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(0, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).a = (int *)device_base_address;
  }
}
struct  nanos_args_1_t
{
  int X[100000L];
  int *a;
  int *b;
};
static void nanos_xlate_fun_simpletestc_1(struct nanos_args_1_t *const arg, void *wd)
{
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(0, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).a = (int *)device_base_address;
  }
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(1, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).b = (int *)device_base_address;
  }
}
struct  nanos_args_2_t
{
  int X[100000L];
  int *a;
  int *c;
};
static void nanos_xlate_fun_simpletestc_2(struct nanos_args_2_t *const arg, void *wd)
{
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(0, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).a = (int *)device_base_address;
  }
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(1, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).c = (int *)device_base_address;
  }
}
struct  nanos_args_3_t
{
  int X[100000L];
  int *c;
  int *d;
};
static void nanos_xlate_fun_simpletestc_3(struct nanos_args_3_t *const arg, void *wd)
{
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(0, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).c = (int *)device_base_address;
  }
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(1, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).d = (int *)device_base_address;
  }
}
struct  nanos_args_4_t
{
  int X[100000L];
  int *d;
  int *e;
};
static void nanos_xlate_fun_simpletestc_4(struct nanos_args_4_t *const arg, void *wd)
{
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(0, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).d = (int *)device_base_address;
  }
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(1, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).e = (int *)device_base_address;
  }
}
struct  nanos_args_5_t
{
  int X[100000L];
  int *d;
  int *f;
};
static void nanos_xlate_fun_simpletestc_5(struct nanos_args_5_t *const arg, void *wd)
{
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(0, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).d = (int *)device_base_address;
  }
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(1, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).f = (int *)device_base_address;
  }
}
struct  nanos_args_6_t
{
  int X[100000L];
  int *b;
  int *e;
  int *f;
};
static void nanos_xlate_fun_simpletestc_6(struct nanos_args_6_t *const arg, void *wd)
{
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(0, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).b = (int *)device_base_address;
  }
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(1, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).e = (int *)device_base_address;
  }
  {
    void *device_base_address;
    nanos_err_t nanos_err;
    device_base_address = 0;
    nanos_err = nanos_get_addr(2, &device_base_address, wd);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
    (*arg).f = (int *)device_base_address;
  }
}
extern nanos_err_t nanos_in_final(_Bool *result);
struct  mcc_struct_anon_15
{
  void (*outline)(void *);
};
typedef struct mcc_struct_anon_15 nanos_smp_args_t;
static void smp_ol_main_0(struct nanos_args_0_t *const args);
struct  mcc_struct_anon_11
{
  _Bool mandatory_creation:1;
  _Bool tied:1;
  _Bool clear_chunk:1;
  _Bool reserved0:1;
  _Bool reserved1:1;
  _Bool reserved2:1;
  _Bool reserved3:1;
  _Bool reserved4:1;
};
typedef struct mcc_struct_anon_11 nanos_wd_props_t;
struct  nanos_const_wd_definition_tag
{
  nanos_wd_props_t props;
  size_t data_alignment;
  size_t num_copies;
  size_t num_devices;
  size_t num_dimensions;
  const char *description;
};
typedef struct nanos_const_wd_definition_tag nanos_const_wd_definition_t;
struct  mcc_struct_anon_14
{
  void *(*factory)(void *);
  void *arg;
};
typedef struct mcc_struct_anon_14 nanos_device_t;
struct  nanos_const_wd_definition_1
{
  nanos_const_wd_definition_t base;
  nanos_device_t devices[1L];
};
extern void *nanos_smp_factory(void *args);
struct  mcc_struct_anon_12
{
  _Bool is_final:1;
  _Bool is_recover:1;
  _Bool is_implicit:1;
  _Bool reserved3:1;
  _Bool reserved4:1;
  _Bool reserved5:1;
  _Bool reserved6:1;
  _Bool reserved7:1;
};
typedef struct mcc_struct_anon_12 nanos_wd_dyn_flags_t;
typedef void *nanos_thread_t;
struct  mcc_struct_anon_13
{
  nanos_wd_dyn_flags_t flags;
  nanos_thread_t tie_to;
  int priority;
};
typedef struct mcc_struct_anon_13 nanos_wd_dyn_props_t;
struct mcc_struct_anon_4;
typedef struct mcc_struct_anon_4 nanos_copy_data_internal_t;
typedef nanos_copy_data_internal_t nanos_copy_data_t;
struct mcc_struct_anon_0;
typedef struct mcc_struct_anon_0 nanos_region_dimension_internal_t;
typedef void *nanos_wg_t;
extern nanos_err_t nanos_create_wd_compact(nanos_wd_t *wd, nanos_const_wd_definition_t *const_data, nanos_wd_dyn_props_t *dyn_props, size_t data_size, void **data, nanos_wg_t wg, nanos_copy_data_t **copies, nanos_region_dimension_internal_t **dimensions);
extern nanos_wd_t nanos_current_wd(void);
struct  mcc_struct_anon_0
{
  size_t size;
  size_t lower_bound;
  size_t accessed_length;
};
typedef nanos_region_dimension_internal_t nanos_region_dimension_t;
struct  mcc_struct_anon_1
{
  _Bool input:1;
  _Bool output:1;
  _Bool can_rename:1;
  _Bool concurrent:1;
  _Bool commutative:1;
};
typedef struct mcc_struct_anon_1 nanos_access_type_internal_t;
typedef long int ptrdiff_t;
struct  mcc_struct_anon_2
{
  void *address;
  nanos_access_type_internal_t flags;
  short int dimension_count;
  const nanos_region_dimension_internal_t *dimensions;
  ptrdiff_t offset;
};
typedef struct mcc_struct_anon_2 nanos_data_access_internal_t;
typedef nanos_data_access_internal_t nanos_data_access_t;
enum mcc_enum_anon_0
{
  NANOS_PRIVATE = 0,
  NANOS_SHARED = 1
};
typedef enum mcc_enum_anon_0 nanos_sharing_t;
struct  mcc_struct_anon_5
{
  _Bool input:1;
  _Bool output:1;
};
typedef unsigned long int uint64_t;
typedef unsigned int memory_space_id_t;
struct  mcc_struct_anon_4
{
  void *address;
  nanos_sharing_t sharing;
  struct mcc_struct_anon_5 flags;
  short int dimension_count;
  nanos_region_dimension_internal_t *dimensions;
  ptrdiff_t offset;
  uint64_t host_base_address;
  memory_space_id_t host_region_id;
  _Bool remote_host;
};
typedef void (*nanos_translate_args_t)(void *, nanos_wd_t);
extern nanos_err_t nanos_set_translate_function(nanos_wd_t wd, nanos_translate_args_t translate_args);
typedef void *nanos_team_t;
extern nanos_err_t nanos_submit(nanos_wd_t wd, size_t num_data_accesses, nanos_data_access_t *data_accesses, nanos_team_t team);
extern nanos_err_t nanos_create_wd_and_run_compact(nanos_const_wd_definition_t *const_data, nanos_wd_dyn_props_t *dyn_props, size_t data_size, void *data, size_t num_data_accesses, nanos_data_access_t *data_accesses, nanos_copy_data_t *copies, nanos_region_dimension_internal_t *dimensions, nanos_translate_args_t translate_args);
static void smp_ol_main_1(struct nanos_args_1_t *const args);
static void smp_ol_main_2(struct nanos_args_2_t *const args);
static void smp_ol_main_3(struct nanos_args_3_t *const args);
static void smp_ol_main_4(struct nanos_args_4_t *const args);
static void smp_ol_main_5(struct nanos_args_5_t *const args);
static void smp_ol_main_6(struct nanos_args_6_t *const args);
extern nanos_err_t nanos_wg_wait_completion(nanos_wg_t wg, _Bool avoid_flush);
int main(int argc, char **argv)
{
  int X[100000L];
  int a;
  int b;
  int c;
  int d;
  int e;
  int f;
  int x = argc;
  printf("starting program\n");
  Extrae_init();
  {
    _Bool mcc_is_in_final;
    nanos_err_t mcc_err_in_final = nanos_in_final(&mcc_is_in_final);
    if (mcc_err_in_final != NANOS_OK)
      {
        nanos_handle_error(mcc_err_in_final);
      }
    if (mcc_is_in_final)
      {
        {
          Extrae_event(1000, 1);
          some_work(X, 'a');
          Extrae_event(1000, 0);
        }
      }
    else
      {
        {
          nanos_wd_dyn_props_t nanos_wd_dyn_props;
          struct nanos_args_0_t *ol_args;
          nanos_err_t nanos_err;
          struct nanos_args_0_t imm_args;
          nanos_region_dimension_t dimensions_0[1L];
          nanos_data_access_t dependences[1L];
          static nanos_smp_args_t smp_ol_main_0_args = {.outline = (void (*)(void *))(void (*)(struct nanos_args_0_t *))&smp_ol_main_0};
          static struct nanos_const_wd_definition_1 nanos_wd_const_data = {.base = {.props = {.mandatory_creation = 0, .tied = 0, .clear_chunk = 0, .reserved0 = 0, .reserved1 = 0, .reserved2 = 0, .reserved3 = 0, .reserved4 = 0}, .data_alignment = __alignof__(struct nanos_args_0_t), .num_copies = 1, .num_devices = 1, .num_dimensions = 1, .description = 0}, .devices = {[0] = {.factory = &nanos_smp_factory, .arg = &smp_ol_main_0_args}}};
          nanos_wd_dyn_props.tie_to = 0;
          nanos_wd_dyn_props.priority = 0;
          nanos_wd_dyn_props.flags.is_final = 0;
          nanos_wd_dyn_props.flags.is_implicit = 0;
          ol_args = (struct nanos_args_0_t *)0;
          nanos_wd_t nanos_wd_ = (void *)0;
          nanos_copy_data_t *ol_copy_data = (nanos_copy_data_t *)0;
          nanos_region_dimension_internal_t *ol_copy_dimensions = (nanos_region_dimension_internal_t *)0;
          nanos_err = nanos_create_wd_compact(&nanos_wd_, &nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_0_t), (void **)&ol_args, nanos_current_wd(), &ol_copy_data, &ol_copy_dimensions);
          if (nanos_err != NANOS_OK)
            {
              nanos_handle_error(nanos_err);
            }
          dimensions_0[0].size = 1 * sizeof(int);
          dimensions_0[0].lower_bound = 0 * sizeof(int);
          dimensions_0[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[0].offset = 0L;
          dependences[0].flags.input = 0;
          dependences[0].flags.output = 1;
          dependences[0].flags.can_rename = 0;
          dependences[0].flags.concurrent = 0;
          dependences[0].flags.commutative = 0;
          dependences[0].dimension_count = 1;
          dependences[0].address = (void *)&a;
          dependences[0].dimensions = dimensions_0;
          if (nanos_wd_ != (void *)0)
            {
              __builtin_memcpy(&(*ol_args).X, &X, sizeof(int [100000L]));
              (*ol_args).a = &a;
              ol_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[0].sharing = NANOS_SHARED;
              ol_copy_data[0].address = (void *)&a;
              ol_copy_data[0].flags.input = 0;
              ol_copy_data[0].flags.output = 1;
              ol_copy_data[0].dimension_count = (short int)1;
              ol_copy_data[0].dimensions = &ol_copy_dimensions[0];
              ol_copy_data[0].offset = 0L;
              nanos_err = nanos_set_translate_function(nanos_wd_, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_0);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
              nanos_err = nanos_submit(nanos_wd_, 1, &dependences[0], (void *)0);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
          else
            {
              nanos_region_dimension_internal_t imm_copy_dimensions[1L];
              nanos_copy_data_t imm_copy_data[1L];
              __builtin_memcpy(&imm_args.X, &X, sizeof(int [100000L]));
              imm_args.a = &a;
              imm_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[0].sharing = NANOS_SHARED;
              imm_copy_data[0].address = (void *)&a;
              imm_copy_data[0].flags.input = 0;
              imm_copy_data[0].flags.output = 1;
              imm_copy_data[0].dimension_count = (short int)1;
              imm_copy_data[0].dimensions = &imm_copy_dimensions[0];
              imm_copy_data[0].offset = 0L;
              nanos_err = nanos_create_wd_and_run_compact(&nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_0_t), &imm_args, 1, &dependences[0], imm_copy_data, imm_copy_dimensions, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_0);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
        }
      }
  }
  {
    _Bool mcc_is_in_final;
    nanos_err_t mcc_err_in_final = nanos_in_final(&mcc_is_in_final);
    if (mcc_err_in_final != NANOS_OK)
      {
        nanos_handle_error(mcc_err_in_final);
      }
    if (mcc_is_in_final)
      {
        {
          Extrae_event(1000, 2);
          some_work(X, 'b');
          Extrae_event(1000, 0);
        }
      }
    else
      {
        {
          nanos_wd_dyn_props_t nanos_wd_dyn_props;
          struct nanos_args_1_t *ol_args;
          nanos_err_t nanos_err;
          struct nanos_args_1_t imm_args;
          nanos_region_dimension_t dimensions_1[1L];
          nanos_data_access_t dependences[2L];
          nanos_region_dimension_t dimensions_2[1L];
          static nanos_smp_args_t smp_ol_main_1_args = {.outline = (void (*)(void *))(void (*)(struct nanos_args_1_t *))&smp_ol_main_1};
          static struct nanos_const_wd_definition_1 nanos_wd_const_data = {.base = {.props = {.mandatory_creation = 0, .tied = 0, .clear_chunk = 0, .reserved0 = 0, .reserved1 = 0, .reserved2 = 0, .reserved3 = 0, .reserved4 = 0}, .data_alignment = __alignof__(struct nanos_args_1_t), .num_copies = 2, .num_devices = 1, .num_dimensions = 2, .description = 0}, .devices = {[0] = {.factory = &nanos_smp_factory, .arg = &smp_ol_main_1_args}}};
          nanos_wd_dyn_props.tie_to = 0;
          nanos_wd_dyn_props.priority = 0;
          nanos_wd_dyn_props.flags.is_final = 0;
          nanos_wd_dyn_props.flags.is_implicit = 0;
          ol_args = (struct nanos_args_1_t *)0;
          nanos_wd_t nanos_wd_ = (void *)0;
          nanos_copy_data_t *ol_copy_data = (nanos_copy_data_t *)0;
          nanos_region_dimension_internal_t *ol_copy_dimensions = (nanos_region_dimension_internal_t *)0;
          nanos_err = nanos_create_wd_compact(&nanos_wd_, &nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_1_t), (void **)&ol_args, nanos_current_wd(), &ol_copy_data, &ol_copy_dimensions);
          if (nanos_err != NANOS_OK)
            {
              nanos_handle_error(nanos_err);
            }
          dimensions_1[0].size = 1 * sizeof(int);
          dimensions_1[0].lower_bound = 0 * sizeof(int);
          dimensions_1[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[0].offset = 0L;
          dependences[0].flags.input = 1;
          dependences[0].flags.output = 0;
          dependences[0].flags.can_rename = 0;
          dependences[0].flags.concurrent = 0;
          dependences[0].flags.commutative = 0;
          dependences[0].dimension_count = 1;
          dependences[0].address = (void *)&a;
          dependences[0].dimensions = dimensions_1;
          dimensions_2[0].size = 1 * sizeof(int);
          dimensions_2[0].lower_bound = 0 * sizeof(int);
          dimensions_2[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[1].offset = 0L;
          dependences[1].flags.input = 0;
          dependences[1].flags.output = 1;
          dependences[1].flags.can_rename = 0;
          dependences[1].flags.concurrent = 0;
          dependences[1].flags.commutative = 0;
          dependences[1].dimension_count = 1;
          dependences[1].address = (void *)&b;
          dependences[1].dimensions = dimensions_2;
          if (nanos_wd_ != (void *)0)
            {
              __builtin_memcpy(&(*ol_args).X, &X, sizeof(int [100000L]));
              (*ol_args).a = &a;
              (*ol_args).b = &b;
              ol_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[0].sharing = NANOS_SHARED;
              ol_copy_data[0].address = (void *)&a;
              ol_copy_data[0].flags.input = 1;
              ol_copy_data[0].flags.output = 0;
              ol_copy_data[0].dimension_count = (short int)1;
              ol_copy_data[0].dimensions = &ol_copy_dimensions[0];
              ol_copy_data[0].offset = 0L;
              ol_copy_dimensions[1 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[1 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[1 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[1].sharing = NANOS_SHARED;
              ol_copy_data[1].address = (void *)&b;
              ol_copy_data[1].flags.input = 0;
              ol_copy_data[1].flags.output = 1;
              ol_copy_data[1].dimension_count = (short int)1;
              ol_copy_data[1].dimensions = &ol_copy_dimensions[1];
              ol_copy_data[1].offset = 0L;
              nanos_err = nanos_set_translate_function(nanos_wd_, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_1);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
              nanos_err = nanos_submit(nanos_wd_, 2, &dependences[0], (void *)0);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
          else
            {
              nanos_region_dimension_internal_t imm_copy_dimensions[2L];
              nanos_copy_data_t imm_copy_data[2L];
              __builtin_memcpy(&imm_args.X, &X, sizeof(int [100000L]));
              imm_args.a = &a;
              imm_args.b = &b;
              imm_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[0].sharing = NANOS_SHARED;
              imm_copy_data[0].address = (void *)&a;
              imm_copy_data[0].flags.input = 1;
              imm_copy_data[0].flags.output = 0;
              imm_copy_data[0].dimension_count = (short int)1;
              imm_copy_data[0].dimensions = &imm_copy_dimensions[0];
              imm_copy_data[0].offset = 0L;
              imm_copy_dimensions[1 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[1 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[1 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[1].sharing = NANOS_SHARED;
              imm_copy_data[1].address = (void *)&b;
              imm_copy_data[1].flags.input = 0;
              imm_copy_data[1].flags.output = 1;
              imm_copy_data[1].dimension_count = (short int)1;
              imm_copy_data[1].dimensions = &imm_copy_dimensions[1];
              imm_copy_data[1].offset = 0L;
              nanos_err = nanos_create_wd_and_run_compact(&nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_1_t), &imm_args, 2, &dependences[0], imm_copy_data, imm_copy_dimensions, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_1);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
        }
      }
  }
  {
    _Bool mcc_is_in_final;
    nanos_err_t mcc_err_in_final = nanos_in_final(&mcc_is_in_final);
    if (mcc_err_in_final != NANOS_OK)
      {
        nanos_handle_error(mcc_err_in_final);
      }
    if (mcc_is_in_final)
      {
        {
          Extrae_event(1000, 3);
          some_work(X, 'c');
          Extrae_event(1000, 0);
        }
      }
    else
      {
        {
          nanos_wd_dyn_props_t nanos_wd_dyn_props;
          struct nanos_args_2_t *ol_args;
          nanos_err_t nanos_err;
          struct nanos_args_2_t imm_args;
          nanos_region_dimension_t dimensions_3[1L];
          nanos_data_access_t dependences[2L];
          nanos_region_dimension_t dimensions_4[1L];
          static nanos_smp_args_t smp_ol_main_2_args = {.outline = (void (*)(void *))(void (*)(struct nanos_args_2_t *))&smp_ol_main_2};
          static struct nanos_const_wd_definition_1 nanos_wd_const_data = {.base = {.props = {.mandatory_creation = 0, .tied = 0, .clear_chunk = 0, .reserved0 = 0, .reserved1 = 0, .reserved2 = 0, .reserved3 = 0, .reserved4 = 0}, .data_alignment = __alignof__(struct nanos_args_2_t), .num_copies = 2, .num_devices = 1, .num_dimensions = 2, .description = 0}, .devices = {[0] = {.factory = &nanos_smp_factory, .arg = &smp_ol_main_2_args}}};
          nanos_wd_dyn_props.tie_to = 0;
          nanos_wd_dyn_props.priority = 0;
          nanos_wd_dyn_props.flags.is_final = 0;
          nanos_wd_dyn_props.flags.is_implicit = 0;
          ol_args = (struct nanos_args_2_t *)0;
          nanos_wd_t nanos_wd_ = (void *)0;
          nanos_copy_data_t *ol_copy_data = (nanos_copy_data_t *)0;
          nanos_region_dimension_internal_t *ol_copy_dimensions = (nanos_region_dimension_internal_t *)0;
          nanos_err = nanos_create_wd_compact(&nanos_wd_, &nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_2_t), (void **)&ol_args, nanos_current_wd(), &ol_copy_data, &ol_copy_dimensions);
          if (nanos_err != NANOS_OK)
            {
              nanos_handle_error(nanos_err);
            }
          dimensions_3[0].size = 1 * sizeof(int);
          dimensions_3[0].lower_bound = 0 * sizeof(int);
          dimensions_3[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[0].offset = 0L;
          dependences[0].flags.input = 1;
          dependences[0].flags.output = 0;
          dependences[0].flags.can_rename = 0;
          dependences[0].flags.concurrent = 0;
          dependences[0].flags.commutative = 0;
          dependences[0].dimension_count = 1;
          dependences[0].address = (void *)&a;
          dependences[0].dimensions = dimensions_3;
          dimensions_4[0].size = 1 * sizeof(int);
          dimensions_4[0].lower_bound = 0 * sizeof(int);
          dimensions_4[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[1].offset = 0L;
          dependences[1].flags.input = 0;
          dependences[1].flags.output = 1;
          dependences[1].flags.can_rename = 0;
          dependences[1].flags.concurrent = 0;
          dependences[1].flags.commutative = 0;
          dependences[1].dimension_count = 1;
          dependences[1].address = (void *)&c;
          dependences[1].dimensions = dimensions_4;
          if (nanos_wd_ != (void *)0)
            {
              __builtin_memcpy(&(*ol_args).X, &X, sizeof(int [100000L]));
              (*ol_args).a = &a;
              (*ol_args).c = &c;
              ol_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[0].sharing = NANOS_SHARED;
              ol_copy_data[0].address = (void *)&a;
              ol_copy_data[0].flags.input = 1;
              ol_copy_data[0].flags.output = 0;
              ol_copy_data[0].dimension_count = (short int)1;
              ol_copy_data[0].dimensions = &ol_copy_dimensions[0];
              ol_copy_data[0].offset = 0L;
              ol_copy_dimensions[1 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[1 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[1 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[1].sharing = NANOS_SHARED;
              ol_copy_data[1].address = (void *)&c;
              ol_copy_data[1].flags.input = 0;
              ol_copy_data[1].flags.output = 1;
              ol_copy_data[1].dimension_count = (short int)1;
              ol_copy_data[1].dimensions = &ol_copy_dimensions[1];
              ol_copy_data[1].offset = 0L;
              nanos_err = nanos_set_translate_function(nanos_wd_, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_2);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
              nanos_err = nanos_submit(nanos_wd_, 2, &dependences[0], (void *)0);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
          else
            {
              nanos_region_dimension_internal_t imm_copy_dimensions[2L];
              nanos_copy_data_t imm_copy_data[2L];
              __builtin_memcpy(&imm_args.X, &X, sizeof(int [100000L]));
              imm_args.a = &a;
              imm_args.c = &c;
              imm_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[0].sharing = NANOS_SHARED;
              imm_copy_data[0].address = (void *)&a;
              imm_copy_data[0].flags.input = 1;
              imm_copy_data[0].flags.output = 0;
              imm_copy_data[0].dimension_count = (short int)1;
              imm_copy_data[0].dimensions = &imm_copy_dimensions[0];
              imm_copy_data[0].offset = 0L;
              imm_copy_dimensions[1 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[1 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[1 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[1].sharing = NANOS_SHARED;
              imm_copy_data[1].address = (void *)&c;
              imm_copy_data[1].flags.input = 0;
              imm_copy_data[1].flags.output = 1;
              imm_copy_data[1].dimension_count = (short int)1;
              imm_copy_data[1].dimensions = &imm_copy_dimensions[1];
              imm_copy_data[1].offset = 0L;
              nanos_err = nanos_create_wd_and_run_compact(&nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_2_t), &imm_args, 2, &dependences[0], imm_copy_data, imm_copy_dimensions, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_2);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
        }
      }
  }
  {
    _Bool mcc_is_in_final;
    nanos_err_t mcc_err_in_final = nanos_in_final(&mcc_is_in_final);
    if (mcc_err_in_final != NANOS_OK)
      {
        nanos_handle_error(mcc_err_in_final);
      }
    if (mcc_is_in_final)
      {
        {
          Extrae_event(1000, 4);
          some_work(X, 'd');
          Extrae_event(1000, 0);
        }
      }
    else
      {
        {
          nanos_wd_dyn_props_t nanos_wd_dyn_props;
          struct nanos_args_3_t *ol_args;
          nanos_err_t nanos_err;
          struct nanos_args_3_t imm_args;
          nanos_region_dimension_t dimensions_5[1L];
          nanos_data_access_t dependences[2L];
          nanos_region_dimension_t dimensions_6[1L];
          static nanos_smp_args_t smp_ol_main_3_args = {.outline = (void (*)(void *))(void (*)(struct nanos_args_3_t *))&smp_ol_main_3};
          static struct nanos_const_wd_definition_1 nanos_wd_const_data = {.base = {.props = {.mandatory_creation = 0, .tied = 0, .clear_chunk = 0, .reserved0 = 0, .reserved1 = 0, .reserved2 = 0, .reserved3 = 0, .reserved4 = 0}, .data_alignment = __alignof__(struct nanos_args_3_t), .num_copies = 2, .num_devices = 1, .num_dimensions = 2, .description = 0}, .devices = {[0] = {.factory = &nanos_smp_factory, .arg = &smp_ol_main_3_args}}};
          nanos_wd_dyn_props.tie_to = 0;
          nanos_wd_dyn_props.priority = 0;
          nanos_wd_dyn_props.flags.is_final = 0;
          nanos_wd_dyn_props.flags.is_implicit = 0;
          ol_args = (struct nanos_args_3_t *)0;
          nanos_wd_t nanos_wd_ = (void *)0;
          nanos_copy_data_t *ol_copy_data = (nanos_copy_data_t *)0;
          nanos_region_dimension_internal_t *ol_copy_dimensions = (nanos_region_dimension_internal_t *)0;
          nanos_err = nanos_create_wd_compact(&nanos_wd_, &nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_3_t), (void **)&ol_args, nanos_current_wd(), &ol_copy_data, &ol_copy_dimensions);
          if (nanos_err != NANOS_OK)
            {
              nanos_handle_error(nanos_err);
            }
          dimensions_5[0].size = 1 * sizeof(int);
          dimensions_5[0].lower_bound = 0 * sizeof(int);
          dimensions_5[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[0].offset = 0L;
          dependences[0].flags.input = 1;
          dependences[0].flags.output = 0;
          dependences[0].flags.can_rename = 0;
          dependences[0].flags.concurrent = 0;
          dependences[0].flags.commutative = 0;
          dependences[0].dimension_count = 1;
          dependences[0].address = (void *)&c;
          dependences[0].dimensions = dimensions_5;
          dimensions_6[0].size = 1 * sizeof(int);
          dimensions_6[0].lower_bound = 0 * sizeof(int);
          dimensions_6[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[1].offset = 0L;
          dependences[1].flags.input = 0;
          dependences[1].flags.output = 1;
          dependences[1].flags.can_rename = 0;
          dependences[1].flags.concurrent = 0;
          dependences[1].flags.commutative = 0;
          dependences[1].dimension_count = 1;
          dependences[1].address = (void *)&d;
          dependences[1].dimensions = dimensions_6;
          if (nanos_wd_ != (void *)0)
            {
              __builtin_memcpy(&(*ol_args).X, &X, sizeof(int [100000L]));
              (*ol_args).c = &c;
              (*ol_args).d = &d;
              ol_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[0].sharing = NANOS_SHARED;
              ol_copy_data[0].address = (void *)&c;
              ol_copy_data[0].flags.input = 1;
              ol_copy_data[0].flags.output = 0;
              ol_copy_data[0].dimension_count = (short int)1;
              ol_copy_data[0].dimensions = &ol_copy_dimensions[0];
              ol_copy_data[0].offset = 0L;
              ol_copy_dimensions[1 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[1 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[1 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[1].sharing = NANOS_SHARED;
              ol_copy_data[1].address = (void *)&d;
              ol_copy_data[1].flags.input = 0;
              ol_copy_data[1].flags.output = 1;
              ol_copy_data[1].dimension_count = (short int)1;
              ol_copy_data[1].dimensions = &ol_copy_dimensions[1];
              ol_copy_data[1].offset = 0L;
              nanos_err = nanos_set_translate_function(nanos_wd_, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_3);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
              nanos_err = nanos_submit(nanos_wd_, 2, &dependences[0], (void *)0);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
          else
            {
              nanos_region_dimension_internal_t imm_copy_dimensions[2L];
              nanos_copy_data_t imm_copy_data[2L];
              __builtin_memcpy(&imm_args.X, &X, sizeof(int [100000L]));
              imm_args.c = &c;
              imm_args.d = &d;
              imm_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[0].sharing = NANOS_SHARED;
              imm_copy_data[0].address = (void *)&c;
              imm_copy_data[0].flags.input = 1;
              imm_copy_data[0].flags.output = 0;
              imm_copy_data[0].dimension_count = (short int)1;
              imm_copy_data[0].dimensions = &imm_copy_dimensions[0];
              imm_copy_data[0].offset = 0L;
              imm_copy_dimensions[1 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[1 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[1 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[1].sharing = NANOS_SHARED;
              imm_copy_data[1].address = (void *)&d;
              imm_copy_data[1].flags.input = 0;
              imm_copy_data[1].flags.output = 1;
              imm_copy_data[1].dimension_count = (short int)1;
              imm_copy_data[1].dimensions = &imm_copy_dimensions[1];
              imm_copy_data[1].offset = 0L;
              nanos_err = nanos_create_wd_and_run_compact(&nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_3_t), &imm_args, 2, &dependences[0], imm_copy_data, imm_copy_dimensions, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_3);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
        }
      }
  }
  {
    _Bool mcc_is_in_final;
    nanos_err_t mcc_err_in_final = nanos_in_final(&mcc_is_in_final);
    if (mcc_err_in_final != NANOS_OK)
      {
        nanos_handle_error(mcc_err_in_final);
      }
    if (mcc_is_in_final)
      {
        {
          Extrae_event(1000, 5);
          some_work(X, 'e');
          Extrae_event(1000, 0);
        }
      }
    else
      {
        {
          nanos_wd_dyn_props_t nanos_wd_dyn_props;
          struct nanos_args_4_t *ol_args;
          nanos_err_t nanos_err;
          struct nanos_args_4_t imm_args;
          nanos_region_dimension_t dimensions_7[1L];
          nanos_data_access_t dependences[2L];
          nanos_region_dimension_t dimensions_8[1L];
          static nanos_smp_args_t smp_ol_main_4_args = {.outline = (void (*)(void *))(void (*)(struct nanos_args_4_t *))&smp_ol_main_4};
          static struct nanos_const_wd_definition_1 nanos_wd_const_data = {.base = {.props = {.mandatory_creation = 0, .tied = 0, .clear_chunk = 0, .reserved0 = 0, .reserved1 = 0, .reserved2 = 0, .reserved3 = 0, .reserved4 = 0}, .data_alignment = __alignof__(struct nanos_args_4_t), .num_copies = 2, .num_devices = 1, .num_dimensions = 2, .description = 0}, .devices = {[0] = {.factory = &nanos_smp_factory, .arg = &smp_ol_main_4_args}}};
          nanos_wd_dyn_props.tie_to = 0;
          nanos_wd_dyn_props.priority = 0;
          nanos_wd_dyn_props.flags.is_final = 0;
          nanos_wd_dyn_props.flags.is_implicit = 0;
          ol_args = (struct nanos_args_4_t *)0;
          nanos_wd_t nanos_wd_ = (void *)0;
          nanos_copy_data_t *ol_copy_data = (nanos_copy_data_t *)0;
          nanos_region_dimension_internal_t *ol_copy_dimensions = (nanos_region_dimension_internal_t *)0;
          nanos_err = nanos_create_wd_compact(&nanos_wd_, &nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_4_t), (void **)&ol_args, nanos_current_wd(), &ol_copy_data, &ol_copy_dimensions);
          if (nanos_err != NANOS_OK)
            {
              nanos_handle_error(nanos_err);
            }
          dimensions_7[0].size = 1 * sizeof(int);
          dimensions_7[0].lower_bound = 0 * sizeof(int);
          dimensions_7[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[0].offset = 0L;
          dependences[0].flags.input = 1;
          dependences[0].flags.output = 0;
          dependences[0].flags.can_rename = 0;
          dependences[0].flags.concurrent = 0;
          dependences[0].flags.commutative = 0;
          dependences[0].dimension_count = 1;
          dependences[0].address = (void *)&d;
          dependences[0].dimensions = dimensions_7;
          dimensions_8[0].size = 1 * sizeof(int);
          dimensions_8[0].lower_bound = 0 * sizeof(int);
          dimensions_8[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[1].offset = 0L;
          dependences[1].flags.input = 0;
          dependences[1].flags.output = 1;
          dependences[1].flags.can_rename = 0;
          dependences[1].flags.concurrent = 0;
          dependences[1].flags.commutative = 0;
          dependences[1].dimension_count = 1;
          dependences[1].address = (void *)&e;
          dependences[1].dimensions = dimensions_8;
          if (nanos_wd_ != (void *)0)
            {
              __builtin_memcpy(&(*ol_args).X, &X, sizeof(int [100000L]));
              (*ol_args).d = &d;
              (*ol_args).e = &e;
              ol_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[0].sharing = NANOS_SHARED;
              ol_copy_data[0].address = (void *)&d;
              ol_copy_data[0].flags.input = 1;
              ol_copy_data[0].flags.output = 0;
              ol_copy_data[0].dimension_count = (short int)1;
              ol_copy_data[0].dimensions = &ol_copy_dimensions[0];
              ol_copy_data[0].offset = 0L;
              ol_copy_dimensions[1 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[1 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[1 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[1].sharing = NANOS_SHARED;
              ol_copy_data[1].address = (void *)&e;
              ol_copy_data[1].flags.input = 0;
              ol_copy_data[1].flags.output = 1;
              ol_copy_data[1].dimension_count = (short int)1;
              ol_copy_data[1].dimensions = &ol_copy_dimensions[1];
              ol_copy_data[1].offset = 0L;
              nanos_err = nanos_set_translate_function(nanos_wd_, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_4);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
              nanos_err = nanos_submit(nanos_wd_, 2, &dependences[0], (void *)0);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
          else
            {
              nanos_region_dimension_internal_t imm_copy_dimensions[2L];
              nanos_copy_data_t imm_copy_data[2L];
              __builtin_memcpy(&imm_args.X, &X, sizeof(int [100000L]));
              imm_args.d = &d;
              imm_args.e = &e;
              imm_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[0].sharing = NANOS_SHARED;
              imm_copy_data[0].address = (void *)&d;
              imm_copy_data[0].flags.input = 1;
              imm_copy_data[0].flags.output = 0;
              imm_copy_data[0].dimension_count = (short int)1;
              imm_copy_data[0].dimensions = &imm_copy_dimensions[0];
              imm_copy_data[0].offset = 0L;
              imm_copy_dimensions[1 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[1 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[1 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[1].sharing = NANOS_SHARED;
              imm_copy_data[1].address = (void *)&e;
              imm_copy_data[1].flags.input = 0;
              imm_copy_data[1].flags.output = 1;
              imm_copy_data[1].dimension_count = (short int)1;
              imm_copy_data[1].dimensions = &imm_copy_dimensions[1];
              imm_copy_data[1].offset = 0L;
              nanos_err = nanos_create_wd_and_run_compact(&nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_4_t), &imm_args, 2, &dependences[0], imm_copy_data, imm_copy_dimensions, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_4);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
        }
      }
  }
  {
    _Bool mcc_is_in_final;
    nanos_err_t mcc_err_in_final = nanos_in_final(&mcc_is_in_final);
    if (mcc_err_in_final != NANOS_OK)
      {
        nanos_handle_error(mcc_err_in_final);
      }
    if (mcc_is_in_final)
      {
        {
          Extrae_event(1000, 6);
          some_work(X, 'f');
          Extrae_event(1000, 0);
        }
      }
    else
      {
        {
          nanos_wd_dyn_props_t nanos_wd_dyn_props;
          struct nanos_args_5_t *ol_args;
          nanos_err_t nanos_err;
          struct nanos_args_5_t imm_args;
          nanos_region_dimension_t dimensions_9[1L];
          nanos_data_access_t dependences[2L];
          nanos_region_dimension_t dimensions_10[1L];
          static nanos_smp_args_t smp_ol_main_5_args = {.outline = (void (*)(void *))(void (*)(struct nanos_args_5_t *))&smp_ol_main_5};
          static struct nanos_const_wd_definition_1 nanos_wd_const_data = {.base = {.props = {.mandatory_creation = 0, .tied = 0, .clear_chunk = 0, .reserved0 = 0, .reserved1 = 0, .reserved2 = 0, .reserved3 = 0, .reserved4 = 0}, .data_alignment = __alignof__(struct nanos_args_5_t), .num_copies = 2, .num_devices = 1, .num_dimensions = 2, .description = 0}, .devices = {[0] = {.factory = &nanos_smp_factory, .arg = &smp_ol_main_5_args}}};
          nanos_wd_dyn_props.tie_to = 0;
          nanos_wd_dyn_props.priority = 0;
          nanos_wd_dyn_props.flags.is_final = 0;
          nanos_wd_dyn_props.flags.is_implicit = 0;
          ol_args = (struct nanos_args_5_t *)0;
          nanos_wd_t nanos_wd_ = (void *)0;
          nanos_copy_data_t *ol_copy_data = (nanos_copy_data_t *)0;
          nanos_region_dimension_internal_t *ol_copy_dimensions = (nanos_region_dimension_internal_t *)0;
          nanos_err = nanos_create_wd_compact(&nanos_wd_, &nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_5_t), (void **)&ol_args, nanos_current_wd(), &ol_copy_data, &ol_copy_dimensions);
          if (nanos_err != NANOS_OK)
            {
              nanos_handle_error(nanos_err);
            }
          dimensions_9[0].size = 1 * sizeof(int);
          dimensions_9[0].lower_bound = 0 * sizeof(int);
          dimensions_9[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[0].offset = 0L;
          dependences[0].flags.input = 1;
          dependences[0].flags.output = 0;
          dependences[0].flags.can_rename = 0;
          dependences[0].flags.concurrent = 0;
          dependences[0].flags.commutative = 0;
          dependences[0].dimension_count = 1;
          dependences[0].address = (void *)&d;
          dependences[0].dimensions = dimensions_9;
          dimensions_10[0].size = 1 * sizeof(int);
          dimensions_10[0].lower_bound = 0 * sizeof(int);
          dimensions_10[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[1].offset = 0L;
          dependences[1].flags.input = 0;
          dependences[1].flags.output = 1;
          dependences[1].flags.can_rename = 0;
          dependences[1].flags.concurrent = 0;
          dependences[1].flags.commutative = 0;
          dependences[1].dimension_count = 1;
          dependences[1].address = (void *)&f;
          dependences[1].dimensions = dimensions_10;
          if (nanos_wd_ != (void *)0)
            {
              __builtin_memcpy(&(*ol_args).X, &X, sizeof(int [100000L]));
              (*ol_args).d = &d;
              (*ol_args).f = &f;
              ol_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[0].sharing = NANOS_SHARED;
              ol_copy_data[0].address = (void *)&d;
              ol_copy_data[0].flags.input = 1;
              ol_copy_data[0].flags.output = 0;
              ol_copy_data[0].dimension_count = (short int)1;
              ol_copy_data[0].dimensions = &ol_copy_dimensions[0];
              ol_copy_data[0].offset = 0L;
              ol_copy_dimensions[1 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[1 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[1 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[1].sharing = NANOS_SHARED;
              ol_copy_data[1].address = (void *)&f;
              ol_copy_data[1].flags.input = 0;
              ol_copy_data[1].flags.output = 1;
              ol_copy_data[1].dimension_count = (short int)1;
              ol_copy_data[1].dimensions = &ol_copy_dimensions[1];
              ol_copy_data[1].offset = 0L;
              nanos_err = nanos_set_translate_function(nanos_wd_, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_5);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
              nanos_err = nanos_submit(nanos_wd_, 2, &dependences[0], (void *)0);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
          else
            {
              nanos_region_dimension_internal_t imm_copy_dimensions[2L];
              nanos_copy_data_t imm_copy_data[2L];
              __builtin_memcpy(&imm_args.X, &X, sizeof(int [100000L]));
              imm_args.d = &d;
              imm_args.f = &f;
              imm_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[0].sharing = NANOS_SHARED;
              imm_copy_data[0].address = (void *)&d;
              imm_copy_data[0].flags.input = 1;
              imm_copy_data[0].flags.output = 0;
              imm_copy_data[0].dimension_count = (short int)1;
              imm_copy_data[0].dimensions = &imm_copy_dimensions[0];
              imm_copy_data[0].offset = 0L;
              imm_copy_dimensions[1 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[1 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[1 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[1].sharing = NANOS_SHARED;
              imm_copy_data[1].address = (void *)&f;
              imm_copy_data[1].flags.input = 0;
              imm_copy_data[1].flags.output = 1;
              imm_copy_data[1].dimension_count = (short int)1;
              imm_copy_data[1].dimensions = &imm_copy_dimensions[1];
              imm_copy_data[1].offset = 0L;
              nanos_err = nanos_create_wd_and_run_compact(&nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_5_t), &imm_args, 2, &dependences[0], imm_copy_data, imm_copy_dimensions, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_5);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
        }
      }
  }
  {
    _Bool mcc_is_in_final;
    nanos_err_t mcc_err_in_final = nanos_in_final(&mcc_is_in_final);
    if (mcc_err_in_final != NANOS_OK)
      {
        nanos_handle_error(mcc_err_in_final);
      }
    if (mcc_is_in_final)
      {
        {
          Extrae_event(1000, 7);
          some_work(X, 'g');
          Extrae_event(1000, 0);
        }
      }
    else
      {
        {
          nanos_wd_dyn_props_t nanos_wd_dyn_props;
          struct nanos_args_6_t *ol_args;
          nanos_err_t nanos_err;
          struct nanos_args_6_t imm_args;
          nanos_region_dimension_t dimensions_11[1L];
          nanos_data_access_t dependences[3L];
          nanos_region_dimension_t dimensions_12[1L];
          nanos_region_dimension_t dimensions_13[1L];
          static nanos_smp_args_t smp_ol_main_6_args = {.outline = (void (*)(void *))(void (*)(struct nanos_args_6_t *))&smp_ol_main_6};
          static struct nanos_const_wd_definition_1 nanos_wd_const_data = {.base = {.props = {.mandatory_creation = 0, .tied = 0, .clear_chunk = 0, .reserved0 = 0, .reserved1 = 0, .reserved2 = 0, .reserved3 = 0, .reserved4 = 0}, .data_alignment = __alignof__(struct nanos_args_6_t), .num_copies = 3, .num_devices = 1, .num_dimensions = 3, .description = 0}, .devices = {[0] = {.factory = &nanos_smp_factory, .arg = &smp_ol_main_6_args}}};
          nanos_wd_dyn_props.tie_to = 0;
          nanos_wd_dyn_props.priority = 0;
          nanos_wd_dyn_props.flags.is_final = 0;
          nanos_wd_dyn_props.flags.is_implicit = 0;
          ol_args = (struct nanos_args_6_t *)0;
          nanos_wd_t nanos_wd_ = (void *)0;
          nanos_copy_data_t *ol_copy_data = (nanos_copy_data_t *)0;
          nanos_region_dimension_internal_t *ol_copy_dimensions = (nanos_region_dimension_internal_t *)0;
          nanos_err = nanos_create_wd_compact(&nanos_wd_, &nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_6_t), (void **)&ol_args, nanos_current_wd(), &ol_copy_data, &ol_copy_dimensions);
          if (nanos_err != NANOS_OK)
            {
              nanos_handle_error(nanos_err);
            }
          dimensions_11[0].size = 1 * sizeof(int);
          dimensions_11[0].lower_bound = 0 * sizeof(int);
          dimensions_11[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[0].offset = 0L;
          dependences[0].flags.input = 1;
          dependences[0].flags.output = 0;
          dependences[0].flags.can_rename = 0;
          dependences[0].flags.concurrent = 0;
          dependences[0].flags.commutative = 0;
          dependences[0].dimension_count = 1;
          dependences[0].address = (void *)&b;
          dependences[0].dimensions = dimensions_11;
          dimensions_12[0].size = 1 * sizeof(int);
          dimensions_12[0].lower_bound = 0 * sizeof(int);
          dimensions_12[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[1].offset = 0L;
          dependences[1].flags.input = 1;
          dependences[1].flags.output = 0;
          dependences[1].flags.can_rename = 0;
          dependences[1].flags.concurrent = 0;
          dependences[1].flags.commutative = 0;
          dependences[1].dimension_count = 1;
          dependences[1].address = (void *)&e;
          dependences[1].dimensions = dimensions_12;
          dimensions_13[0].size = 1 * sizeof(int);
          dimensions_13[0].lower_bound = 0 * sizeof(int);
          dimensions_13[0].accessed_length = (0 - 0 + 1) * sizeof(int);
          dependences[2].offset = 0L;
          dependences[2].flags.input = 1;
          dependences[2].flags.output = 0;
          dependences[2].flags.can_rename = 0;
          dependences[2].flags.concurrent = 0;
          dependences[2].flags.commutative = 0;
          dependences[2].dimension_count = 1;
          dependences[2].address = (void *)&f;
          dependences[2].dimensions = dimensions_13;
          if (nanos_wd_ != (void *)0)
            {
              __builtin_memcpy(&(*ol_args).X, &X, sizeof(int [100000L]));
              (*ol_args).b = &b;
              (*ol_args).e = &e;
              (*ol_args).f = &f;
              ol_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[0].sharing = NANOS_SHARED;
              ol_copy_data[0].address = (void *)&b;
              ol_copy_data[0].flags.input = 1;
              ol_copy_data[0].flags.output = 0;
              ol_copy_data[0].dimension_count = (short int)1;
              ol_copy_data[0].dimensions = &ol_copy_dimensions[0];
              ol_copy_data[0].offset = 0L;
              ol_copy_dimensions[1 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[1 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[1 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[1].sharing = NANOS_SHARED;
              ol_copy_data[1].address = (void *)&e;
              ol_copy_data[1].flags.input = 1;
              ol_copy_data[1].flags.output = 0;
              ol_copy_data[1].dimension_count = (short int)1;
              ol_copy_data[1].dimensions = &ol_copy_dimensions[1];
              ol_copy_data[1].offset = 0L;
              ol_copy_dimensions[2 + 0].size = 1 * sizeof(int);
              ol_copy_dimensions[2 + 0].lower_bound = 0 * sizeof(int);
              ol_copy_dimensions[2 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              ol_copy_data[2].sharing = NANOS_SHARED;
              ol_copy_data[2].address = (void *)&f;
              ol_copy_data[2].flags.input = 1;
              ol_copy_data[2].flags.output = 0;
              ol_copy_data[2].dimension_count = (short int)1;
              ol_copy_data[2].dimensions = &ol_copy_dimensions[2];
              ol_copy_data[2].offset = 0L;
              nanos_err = nanos_set_translate_function(nanos_wd_, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_6);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
              nanos_err = nanos_submit(nanos_wd_, 3, &dependences[0], (void *)0);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
          else
            {
              nanos_region_dimension_internal_t imm_copy_dimensions[3L];
              nanos_copy_data_t imm_copy_data[3L];
              __builtin_memcpy(&imm_args.X, &X, sizeof(int [100000L]));
              imm_args.b = &b;
              imm_args.e = &e;
              imm_args.f = &f;
              imm_copy_dimensions[0 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[0 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[0 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[0].sharing = NANOS_SHARED;
              imm_copy_data[0].address = (void *)&b;
              imm_copy_data[0].flags.input = 1;
              imm_copy_data[0].flags.output = 0;
              imm_copy_data[0].dimension_count = (short int)1;
              imm_copy_data[0].dimensions = &imm_copy_dimensions[0];
              imm_copy_data[0].offset = 0L;
              imm_copy_dimensions[1 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[1 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[1 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[1].sharing = NANOS_SHARED;
              imm_copy_data[1].address = (void *)&e;
              imm_copy_data[1].flags.input = 1;
              imm_copy_data[1].flags.output = 0;
              imm_copy_data[1].dimension_count = (short int)1;
              imm_copy_data[1].dimensions = &imm_copy_dimensions[1];
              imm_copy_data[1].offset = 0L;
              imm_copy_dimensions[2 + 0].size = 1 * sizeof(int);
              imm_copy_dimensions[2 + 0].lower_bound = 0 * sizeof(int);
              imm_copy_dimensions[2 + 0].accessed_length = (0 - 0 + 1) * sizeof(int);
              imm_copy_data[2].sharing = NANOS_SHARED;
              imm_copy_data[2].address = (void *)&f;
              imm_copy_data[2].flags.input = 1;
              imm_copy_data[2].flags.output = 0;
              imm_copy_data[2].dimension_count = (short int)1;
              imm_copy_data[2].dimensions = &imm_copy_dimensions[2];
              imm_copy_data[2].offset = 0L;
              nanos_err = nanos_create_wd_and_run_compact(&nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_6_t), &imm_args, 3, &dependences[0], imm_copy_data, imm_copy_dimensions, (void (*)(void *, nanos_wd_t))nanos_xlate_fun_simpletestc_6);
              if (nanos_err != NANOS_OK)
                {
                  nanos_handle_error(nanos_err);
                }
            }
        }
      }
  }
  Extrae_fini();
  {
    nanos_err_t nanos_err;
    nanos_wd_t nanos_wd_ = nanos_current_wd();
    nanos_err = nanos_wg_wait_completion(nanos_wd_, 0);
    if (nanos_err != NANOS_OK)
      {
        nanos_handle_error(nanos_err);
      }
  }
  return 0;
}
static void smp_ol_main_0_unpacked(int *__restrict X, int *const a)
{
  {
    {
      Extrae_event(1000, 1);
      some_work(X, 'a');
      Extrae_event(1000, 0);
    }
  }
}
static void smp_ol_main_0(struct nanos_args_0_t *const args)
{
  {
    smp_ol_main_0_unpacked((*args).X, (*args).a);
  }
}
static void smp_ol_main_1_unpacked(int *__restrict X, int *const a, int *const b)
{
  {
    {
      Extrae_event(1000, 2);
      some_work(X, 'b');
      Extrae_event(1000, 0);
    }
  }
}
static void smp_ol_main_1(struct nanos_args_1_t *const args)
{
  {
    smp_ol_main_1_unpacked((*args).X, (*args).a, (*args).b);
  }
}
static void smp_ol_main_2_unpacked(int *__restrict X, int *const a, int *const c)
{
  {
    {
      Extrae_event(1000, 3);
      some_work(X, 'c');
      Extrae_event(1000, 0);
    }
  }
}
static void smp_ol_main_2(struct nanos_args_2_t *const args)
{
  {
    smp_ol_main_2_unpacked((*args).X, (*args).a, (*args).c);
  }
}
static void smp_ol_main_3_unpacked(int *__restrict X, int *const c, int *const d)
{
  {
    {
      Extrae_event(1000, 4);
      some_work(X, 'd');
      Extrae_event(1000, 0);
    }
  }
}
static void smp_ol_main_3(struct nanos_args_3_t *const args)
{
  {
    smp_ol_main_3_unpacked((*args).X, (*args).c, (*args).d);
  }
}
static void smp_ol_main_4_unpacked(int *__restrict X, int *const d, int *const e)
{
  {
    {
      Extrae_event(1000, 5);
      some_work(X, 'e');
      Extrae_event(1000, 0);
    }
  }
}
static void smp_ol_main_4(struct nanos_args_4_t *const args)
{
  {
    smp_ol_main_4_unpacked((*args).X, (*args).d, (*args).e);
  }
}
static void smp_ol_main_5_unpacked(int *__restrict X, int *const d, int *const f)
{
  {
    {
      Extrae_event(1000, 6);
      some_work(X, 'f');
      Extrae_event(1000, 0);
    }
  }
}
static void smp_ol_main_5(struct nanos_args_5_t *const args)
{
  {
    smp_ol_main_5_unpacked((*args).X, (*args).d, (*args).f);
  }
}
static void smp_ol_main_6_unpacked(int *__restrict X, int *const b, int *const e, int *const f)
{
  {
    {
      Extrae_event(1000, 7);
      some_work(X, 'g');
      Extrae_event(1000, 0);
    }
  }
}
static void smp_ol_main_6(struct nanos_args_6_t *const args)
{
  {
    smp_ol_main_6_unpacked((*args).X, (*args).b, (*args).e, (*args).f);
  }
}

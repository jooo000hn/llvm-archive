#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <poll.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>

#include <map>

#define DEBUG (0)

/* Size of shadow memory.  We're hoping everything fits in 46bits. */
#define SIZE ((size_t)(1L << 46))

/* Fixed start of memory.  Needs to be page-aligned,
 * and it needs to be large enough that program itself is loaded below it
 * and so are the libraries. 
 * FIXME: This address has been picked for maute. Might not work on other
 * machines. Need a more robust way of picking base address.
 * For now, run a version of the tool without the base fixed, and 
 * choose address.
 */
#define BASE ((TypeTagTy *)(0x2aaaab88c000))
/*
 * Do some macro magic to get mmap macros defined properly on all platforms.
 */
#if defined(MAP_ANON) && !defined(MAP_ANONYMOUS)
# define MAP_ANONYMOUS MAP_ANON
#endif /* defined(MAP_ANON) && !defined(MAP_ANONYMOUS) */

typedef uint8_t TypeTagTy ;

struct va_info {
  uint64_t numElements;
  uint64_t counter;
  TypeTagTy *metadata;
};

// Map to store info about va lists
std::map<void *, struct va_info> VA_InfoMap;

// Pointer to the shadow_memory
TypeTagTy * const shadow_begin = BASE;

// Map from type numbers to type names.
extern char* typeNames[];

extern "C" {
  void trackInitInst(void *ptr, uint64_t size, uint32_t tag);
  void shadowInit();
  void trackArgvType(int argc, char **argv) ;
  void trackEnvpType(char **envp) ;
  void trackGlobal(void *ptr, TypeTagTy typeNumber, uint64_t size, uint32_t tag) ;
  void trackArray(void *ptr, uint64_t size, uint64_t count, uint32_t tag) ;
  void trackStoreInst(void *ptr, TypeTagTy typeNumber, uint64_t size, uint32_t tag) ;
  void trackStringInput(void *ptr, uint32_t tag) ;
  void compareTypes(TypeTagTy typeNumberSrc, TypeTagTy typeNumberDest, uint32_t tag) ;
  void compareNumber(uint64_t NumArgsPassed, uint64_t ArgAccessed, uint32_t tag);
  void compareTypeAndNumber(uint64_t NumArgsPassed, uint64_t ArgAccessed, uint8_t TypeAccessed, void *MD, uint32_t tag) ;
  void checkVAArgType(void *va_list, TypeTagTy TypeAccessed, uint32_t tag) ;
  void getTypeTag(void *ptr, uint64_t size, TypeTagTy *dest) ;
  void checkType(TypeTagTy typeNumber, uint64_t size, TypeTagTy *metadata, void *ptr, uint32_t tag);
  void trackInitInst(void *ptr, uint64_t size, uint32_t tag) ;
  void trackUnInitInst(void *ptr, uint64_t size, uint32_t tag) ;
  void copyTypeInfo(void *dstptr, void *srcptr, uint64_t size, uint32_t tag) ;
  void setTypeInfo(void *dstptr, void *metadata, uint64_t size, uint32_t tag) ;
  void setVAInfo(void *va_list, uint64_t totalCount, TypeTagTy *metadata_ptr, uint32_t tag) ;
  void copyVAInfo(void *va_list_dst, void *va_list_src, uint32_t tag) ;
  void trackctype(void *ptr, uint32_t tag) ;
  void trackctype_32(void *ptr, uint32_t tag) ;
  void trackStrncpyInst(void *dst, void *src, uint64_t size, uint32_t tag) ;
  void trackStrcpyInst(void *dst, void *src, uint32_t tag) ;
  void trackStrcatInst(void *dst, void *src, uint32_t tag) ;
  void trackgetcwd(void *ptr, uint32_t tag) ;
  void trackgetpwuid(void *ptr, uint32_t tag) ;
  void trackgethostname(void *ptr, uint32_t tag) ;
  void trackgetaddrinfo(void *ptr, uint32_t tag) ;
  void trackaccept(void *ptr, uint32_t tag) ;
  void trackpoll(void *ptr, uint64_t nfds, uint32_t tag) ;
}

void trackInitInst(void *ptr, uint64_t size, uint32_t tag);

inline uintptr_t maskAddress(void *ptr) {
  uintptr_t p = (uintptr_t)ptr;
  if (p >= (uintptr_t)BASE + SIZE) p -= SIZE;

#if DEBUG
  assert(p <= SIZE && "Pointer out of range!");
#endif
  return p;
}

/**
 * Initialize the shadow memory which records the 1:1 mapping of addresses to types.
 */
void shadowInit() {
  void * res = mmap(BASE, SIZE, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);

  if (res == MAP_FAILED) {
    fprintf(stderr, "Failed to map the shadow memory!\n");
    fflush(stderr);
    assert(0 && "MAP_FAILED");
  }
  VA_InfoMap.clear();
}

/**
 * initialize metadata to TOP/initialized.
 */
void trackArgvType(int argc, char **argv) {
  int index = 0;
  for (; index < argc; ++index) {
    trackInitInst(argv[index], (strlen(argv[index]) + 1)*sizeof(char), 0);
  }
  trackInitInst(argv, (argc + 1)*sizeof(char*), 0);
}

/**
 * initialize metadata to TOP/initialized.
 */
void trackEnvpType(char **envp) {
  int index = 0;
  for(;envp[index] != NULL; ++index)
    trackInitInst(envp[index], (strlen(envp[index]) + 1)*sizeof(char), 0);
  trackInitInst(envp, (index )*sizeof(char*), 0);
}

/**
 * Record the global type and address in the shadow memory.
 */
void trackGlobal(void *ptr, TypeTagTy typeNumber, uint64_t size, uint32_t tag) {
  uintptr_t p = maskAddress(ptr);
  shadow_begin[p] = typeNumber;
  memset(&shadow_begin[p + 1], 0, size - 1);
#if DEBUG
  printf("Global(%d): %p, %p = %u | %lu bytes\n", tag, ptr, (void *)p, typeNumber, size);
#endif
}

/**
 * Record the type stored at ptr(of size size) and replicate it
 */
void trackArray(void *ptr, uint64_t size, uint64_t count, uint32_t tag) {
  uintptr_t p = maskAddress(ptr);
  uintptr_t p1 = maskAddress(ptr);
  uint64_t i;

  for (i = 1; i < count; ++i) {
    p += size;
    memcpy(&shadow_begin[p], &shadow_begin[p1], size);
  }
}

/**
 * Record the stored type and address in the shadow memory.
 */
void trackStoreInst(void *ptr, TypeTagTy typeNumber, uint64_t size, uint32_t tag) {
  uintptr_t p = maskAddress(ptr);
  shadow_begin[p] = typeNumber;
  memset(&shadow_begin[p + 1], 0, size - 1);
#if DEBUG
  printf("Store(%d): %p, %p = %u | %lu bytes | \n", tag, ptr, (void *)p, typeNumber, size);
#endif
}

/**
 * Record that a string is stored at ptr
 */
void trackStringInput(void *ptr, uint32_t tag) {
  trackInitInst(ptr, strlen((const char *)ptr) + 1, tag);
}

/** 
 * Check that the two types match
 */
void compareTypes(TypeTagTy typeNumberSrc, TypeTagTy typeNumberDest, uint32_t tag) {
  if(typeNumberSrc != typeNumberDest) {
    printf("Type mismatch(%u): expecting %s, found %s! \n", tag, typeNames[typeNumberSrc], typeNames[typeNumberDest]);
  }
}

/**
 * Check that number of VAArg accessed is not greater than those passed
 */
void compareNumber(uint64_t NumArgsPassed, uint64_t ArgAccessed, uint32_t tag){
  if(ArgAccessed > NumArgsPassed) {
    printf("Type mismatch(%u): Accessing variable %lu, passed only %lu! \n", tag, ArgAccessed, NumArgsPassed);
  }
}

/**
 * Combined check for Va_arg. 
 * Check that no. of arguments is less than passed
 * Check that the type being accessed is correct
 */
void checkVAArgType(void *va_list, TypeTagTy TypeAccessed, uint32_t tag) {
  va_info v = VA_InfoMap[va_list];
  compareNumber(v.numElements, v.counter, tag);
  compareTypes(TypeAccessed, v.metadata[v.counter], tag);
  v.counter++;
  VA_InfoMap[va_list] = v;
}

/**
 * For loads, return the metadata(for size bytes) stored at the ptr
 * Store it in dest
 */
void getTypeTag(void *ptr, uint64_t size, TypeTagTy *dest) {
  uintptr_t p = maskAddress(ptr);
  assert(p + size < SIZE);

  memcpy(dest, &shadow_begin[p], size);
}

/**
 * Compare the typeNumber with the type info(for given size) stored at the metadata ptr.
 * ptr and tag are for debugging
 */
void 
checkType(TypeTagTy typeNumber, uint64_t size, TypeTagTy *metadata, void *ptr, uint32_t tag) {
  /* Check if this an initialized but untyped memory.*/
  if (typeNumber != metadata[0]) {
    if (metadata[0] != 0xFF) {
      printf("Type mismatch(%u): %p expecting %s, found %s!\n", tag, ptr, typeNames[typeNumber], typeNames[metadata[0]]);
      return;
    } else {
      /* If so, set type to the type being read.
         Check that none of the bytes are typed.*/
      for (unsigned i = 1; i < size; ++i) {
        if (0xFF != metadata[i]) {
          printf("Type alignment mismatch(%u): expecting %s, found %s!\n", tag, typeNames[typeNumber], typeNames[metadata[i]]);
          break;
        }
      }
      trackStoreInst(ptr, typeNumber, size, tag);
      return ;
    }
  }

  for (unsigned i = 1 ; i < size; ++i) {
    if (0 != metadata[i]) {
      printf("Type alignment mismatch(%u): expecting %s, found %s!\n", tag, typeNames[typeNumber], typeNames[metadata[0]]);
      break;
    }
  }
}

/**
 *  For memset type instructions, that set values. 
 *  0xFF type indicates that any type can be read, 
 */
void trackInitInst(void *ptr, uint64_t size, uint32_t tag) {
  uintptr_t p = maskAddress(ptr);
  memset(&shadow_begin[p], 0xFF, size);
#if DEBUG
  printf("Initialize: %p, %p | %lu bytes | %u\n", ptr, (void *)p, size, tag);
#endif
}

/**
 * Clear the metadata for given pointer
 */
void trackUnInitInst(void *ptr, uint64_t size, uint32_t tag) {
  uintptr_t p = maskAddress(ptr);
  memset(&shadow_begin[p], 0x00, size);
#if DEBUG
  printf("Unitialize: %p, %p | %lu bytes | %u\n", ptr, (void *)p, size, tag);
#endif
}

/**
 * Copy size bytes of metadata from src ptr to dest ptr.
 */
void copyTypeInfo(void *dstptr, void *srcptr, uint64_t size, uint32_t tag) {
  uintptr_t d = maskAddress(dstptr);
  uintptr_t s = maskAddress(srcptr);
  memcpy(&shadow_begin[d], &shadow_begin[s], size);
#if DEBUG
  printf("Copy(%d): %p, %p = %u | %lu bytes \n", tag, dstptr, srcptr, shadow_begin[s], size);
#endif
}

/**
 * Copy size bytes of metadata from metadata to dest ptr
 */
void setTypeInfo(void *dstptr, void *metadata, uint64_t size, uint32_t tag) {
  uintptr_t d = maskAddress(dstptr);
  memcpy(&shadow_begin[d], metadata, size);
}

/**
 * Initialize the metadata for a given VAList
 */
void setVAInfo(void *va_list, uint64_t totalCount, TypeTagTy *metadata_ptr, uint32_t tag) {
  struct va_info v = {totalCount, 0, metadata_ptr};
  VA_InfoMap[va_list] = v;
}

/**
 * Copy va list metadata from one list to the other.
 */
void copyVAInfo(void *va_list_dst, void *va_list_src, uint32_t tag) {
  VA_InfoMap[va_list_dst] = VA_InfoMap[va_list_src];
}

/**
 * Initialize metadata for the pointer returned by __ctype_b_loc
 */
void trackctype(void *ptr, uint32_t tag) {
  short *p, *p1;
  trackInitInst(ptr, sizeof(short*), tag);
  p = *(short**)ptr;
  p1 = p -128;
  trackInitInst(p1, sizeof(short)*384, tag);
}

void trackctype_32(void *ptr, uint32_t tag) {
  int *p, *p1;
  trackInitInst(ptr, sizeof(int*), tag);
  p = *(int**)ptr;
  p1 = p -128 ;
  trackInitInst(p1, sizeof(int)*384, tag);
}

/**
 * Initialize metadata for the dst pointer of strncpy
 */
void trackStrncpyInst(void *dst, void *src, uint64_t size, uint32_t tag) {
  if(strlen((const char *)src) < size)
    size = strlen((const char *)src) + 1;
  copyTypeInfo(dst, src, size, tag);
}

/**
 * Initialize metadata for the dst pointer of strcpy
 */
void trackStrcpyInst(void *dst, void *src, uint32_t tag) {
  copyTypeInfo(dst, src, strlen((const char *)src)+1, tag);
}

/**
 * Initialize the metadata fr dst pointer of strcap
 */
void trackStrcatInst(void *dst, void *src, uint32_t tag) {
  uintptr_t dst_start = (uintptr_t)(dst) + strlen((const char *)dst) -1;
  copyTypeInfo((void*)dst_start, src, strlen((const char *)src)+1, tag);
}

/**
 * Initialize metadata for some library functions
 */

void trackgetcwd(void *ptr, uint32_t tag) {
  if(!ptr)
    return;
  trackInitInst(ptr, strlen((const char *)ptr) + 1, tag);
}

void trackgetpwuid(void *ptr, uint32_t tag) {
  struct passwd *p = (struct passwd *)ptr;
  trackInitInst(p->pw_name, strlen(p->pw_name) + 1, tag);
  trackInitInst(p->pw_dir, strlen(p->pw_dir) + 1, tag);
  trackInitInst(p->pw_shell, strlen(p->pw_shell) + 1, tag);
  trackInitInst(p, sizeof(struct passwd), tag);
}

void trackgethostname(void *ptr, uint32_t tag) {
  trackInitInst(ptr, strlen((const char *)ptr) + 1, tag);
}

void trackgetaddrinfo(void *ptr, uint32_t tag) {
  struct addrinfo *res;
  struct addrinfo ** result = (struct addrinfo **)ptr;
  for(res = *result; res != NULL; res = res->ai_next) {
    trackInitInst(res->ai_addr, sizeof(struct sockaddr), tag);
    trackInitInst(res, sizeof(struct addrinfo), tag);
  }

  trackInitInst(result, sizeof(struct addrinfo*), tag);
}

void trackaccept(void *ptr, uint32_t tag) {
  trackInitInst(ptr, sizeof(struct sockaddr), tag);
}

void trackpoll(void *ptr, uint64_t nfds, uint32_t tag) {
  struct pollfd *fds = (struct pollfd *)ptr;
  unsigned i = 0;
  while (i < nfds) {
    trackInitInst(&fds[i], sizeof(struct pollfd), tag);
    i++;
  }
}

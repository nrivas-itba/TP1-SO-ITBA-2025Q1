typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef unsigned char    dwfenc;
typedef unsigned int    dword;
typedef unsigned long    qword;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    word;
typedef struct eh_frame_hdr eh_frame_hdr, *Peh_frame_hdr;

struct eh_frame_hdr {
    byte eh_frame_hdr_version; // Exception Handler Frame Header Version
    dwfenc eh_frame_pointer_encoding; // Exception Handler Frame Pointer Encoding
    dwfenc eh_frame_desc_entry_count_encoding; // Encoding of # of Exception Handler FDEs
    dwfenc eh_frame_table_encoding; // Exception Handler Table Encoding
};

typedef struct NoteGnuPropertyElement_4 NoteGnuPropertyElement_4, *PNoteGnuPropertyElement_4;

struct NoteGnuPropertyElement_4 {
    dword prType;
    dword prDatasz;
    byte data[4];
};

typedef struct fde_table_entry fde_table_entry, *Pfde_table_entry;

struct fde_table_entry {
    dword initial_loc; // Initial Location
    dword data_loc; // Data location
};

typedef union sem_t sem_t, *Psem_t;

union sem_t {
    char __size[32];
    long __align;
};

typedef void _IO_lock_t;

typedef struct _IO_marker _IO_marker, *P_IO_marker;

typedef struct _IO_FILE _IO_FILE, *P_IO_FILE;

typedef long __off_t;

typedef long __off64_t;

typedef ulong size_t;

struct _IO_FILE {
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
    ushort _cur_column;
    char _vtable_offset;
    char _shortbuf[1];
    _IO_lock_t *_lock;
    __off64_t _offset;
    void *__pad1;
    void *__pad2;
    void *__pad3;
    void *__pad4;
    size_t __pad5;
    int _mode;
    char _unused2[20];
};

struct _IO_marker {
    struct _IO_marker *_next;
    struct _IO_FILE *_sbuf;
    int _pos;
};

typedef struct _IO_FILE FILE;

typedef long __fd_mask;

typedef struct fd_set fd_set, *Pfd_set;

struct fd_set {
    __fd_mask fds_bits[16];
};

typedef uint __mode_t;

typedef long __ssize_t;

typedef __ssize_t ssize_t;

typedef __mode_t mode_t;

typedef int __pid_t;

typedef uint __uid_t;

typedef long __time_t;

typedef uint __useconds_t;

typedef long __suseconds_t;

typedef struct timeval timeval, *Ptimeval;

struct timeval {
    __time_t tv_sec;
    __suseconds_t tv_usec;
};

typedef __time_t time_t;

typedef enum Elf_ProgramHeaderType {
    PT_NULL=0,
    PT_LOAD=1,
    PT_DYNAMIC=2,
    PT_INTERP=3,
    PT_NOTE=4,
    PT_SHLIB=5,
    PT_PHDR=6,
    PT_TLS=7,
    PT_GNU_EH_FRAME=1685382480,
    PT_GNU_STACK=1685382481,
    PT_GNU_RELRO=1685382482
} Elf_ProgramHeaderType;

typedef struct Elf64_Shdr Elf64_Shdr, *PElf64_Shdr;

typedef enum Elf_SectionHeaderType {
    SHT_NULL=0,
    SHT_PROGBITS=1,
    SHT_SYMTAB=2,
    SHT_STRTAB=3,
    SHT_RELA=4,
    SHT_HASH=5,
    SHT_DYNAMIC=6,
    SHT_NOTE=7,
    SHT_NOBITS=8,
    SHT_REL=9,
    SHT_SHLIB=10,
    SHT_DYNSYM=11,
    SHT_INIT_ARRAY=14,
    SHT_FINI_ARRAY=15,
    SHT_PREINIT_ARRAY=16,
    SHT_GROUP=17,
    SHT_SYMTAB_SHNDX=18,
    SHT_ANDROID_REL=1610612737,
    SHT_ANDROID_RELA=1610612738,
    SHT_GNU_ATTRIBUTES=1879048181,
    SHT_GNU_HASH=1879048182,
    SHT_GNU_LIBLIST=1879048183,
    SHT_CHECKSUM=1879048184,
    SHT_SUNW_move=1879048186,
    SHT_SUNW_COMDAT=1879048187,
    SHT_SUNW_syminfo=1879048188,
    SHT_GNU_verdef=1879048189,
    SHT_GNU_verneed=1879048190,
    SHT_GNU_versym=1879048191
} Elf_SectionHeaderType;

struct Elf64_Shdr {
    dword sh_name;
    enum Elf_SectionHeaderType sh_type;
    qword sh_flags;
    qword sh_addr;
    qword sh_offset;
    qword sh_size;
    dword sh_link;
    dword sh_info;
    qword sh_addralign;
    qword sh_entsize;
};

typedef struct Elf64_Rela Elf64_Rela, *PElf64_Rela;

struct Elf64_Rela {
    qword r_offset; // location to apply the relocation action
    qword r_info; // the symbol table index and the type of relocation
    qword r_addend; // a constant addend used to compute the relocatable field value
};

typedef struct Elf64_Dyn Elf64_Dyn, *PElf64_Dyn;

typedef enum Elf64_DynTag {
    DT_NULL=0,
    DT_NEEDED=1,
    DT_PLTRELSZ=2,
    DT_PLTGOT=3,
    DT_HASH=4,
    DT_STRTAB=5,
    DT_SYMTAB=6,
    DT_RELA=7,
    DT_RELASZ=8,
    DT_RELAENT=9,
    DT_STRSZ=10,
    DT_SYMENT=11,
    DT_INIT=12,
    DT_FINI=13,
    DT_SONAME=14,
    DT_RPATH=15,
    DT_SYMBOLIC=16,
    DT_REL=17,
    DT_RELSZ=18,
    DT_RELENT=19,
    DT_PLTREL=20,
    DT_DEBUG=21,
    DT_TEXTREL=22,
    DT_JMPREL=23,
    DT_BIND_NOW=24,
    DT_INIT_ARRAY=25,
    DT_FINI_ARRAY=26,
    DT_INIT_ARRAYSZ=27,
    DT_FINI_ARRAYSZ=28,
    DT_RUNPATH=29,
    DT_FLAGS=30,
    DT_PREINIT_ARRAY=32,
    DT_PREINIT_ARRAYSZ=33,
    DT_RELRSZ=35,
    DT_RELR=36,
    DT_RELRENT=37,
    DT_ANDROID_REL=1610612751,
    DT_ANDROID_RELSZ=1610612752,
    DT_ANDROID_RELA=1610612753,
    DT_ANDROID_RELASZ=1610612754,
    DT_ANDROID_RELR=1879040000,
    DT_ANDROID_RELRSZ=1879040001,
    DT_ANDROID_RELRENT=1879040003,
    DT_GNU_PRELINKED=1879047669,
    DT_GNU_CONFLICTSZ=1879047670,
    DT_GNU_LIBLISTSZ=1879047671,
    DT_CHECKSUM=1879047672,
    DT_PLTPADSZ=1879047673,
    DT_MOVEENT=1879047674,
    DT_MOVESZ=1879047675,
    DT_FEATURE_1=1879047676,
    DT_POSFLAG_1=1879047677,
    DT_SYMINSZ=1879047678,
    DT_SYMINENT=1879047679,
    DT_GNU_XHASH=1879047924,
    DT_GNU_HASH=1879047925,
    DT_TLSDESC_PLT=1879047926,
    DT_TLSDESC_GOT=1879047927,
    DT_GNU_CONFLICT=1879047928,
    DT_GNU_LIBLIST=1879047929,
    DT_CONFIG=1879047930,
    DT_DEPAUDIT=1879047931,
    DT_AUDIT=1879047932,
    DT_PLTPAD=1879047933,
    DT_MOVETAB=1879047934,
    DT_SYMINFO=1879047935,
    DT_VERSYM=1879048176,
    DT_RELACOUNT=1879048185,
    DT_RELCOUNT=1879048186,
    DT_FLAGS_1=1879048187,
    DT_VERDEF=1879048188,
    DT_VERDEFNUM=1879048189,
    DT_VERNEED=1879048190,
    DT_VERNEEDNUM=1879048191,
    DT_AUXILIARY=2147483645,
    DT_FILTER=2147483647
} Elf64_DynTag;

struct Elf64_Dyn {
    enum Elf64_DynTag d_tag;
    qword d_val;
};

typedef struct GnuBuildId GnuBuildId, *PGnuBuildId;

struct GnuBuildId {
    dword namesz; // Length of name field
    dword descsz; // Length of description field
    dword type; // Vendor specific type
    char name[4]; // Vendor name
    byte hash[20];
};

typedef struct NoteGnuProperty_4 NoteGnuProperty_4, *PNoteGnuProperty_4;

struct NoteGnuProperty_4 {
    dword namesz; // Length of name field
    dword descsz; // Length of description field
    dword type; // Vendor specific type
    char name[4]; // Vendor name
};

typedef struct Elf64_Ehdr Elf64_Ehdr, *PElf64_Ehdr;

struct Elf64_Ehdr {
    byte e_ident_magic_num;
    char e_ident_magic_str[3];
    byte e_ident_class;
    byte e_ident_data;
    byte e_ident_version;
    byte e_ident_osabi;
    byte e_ident_abiversion;
    byte e_ident_pad[7];
    word e_type;
    word e_machine;
    dword e_version;
    qword e_entry;
    qword e_phoff;
    qword e_shoff;
    dword e_flags;
    word e_ehsize;
    word e_phentsize;
    word e_phnum;
    word e_shentsize;
    word e_shnum;
    word e_shstrndx;
};

typedef struct Elf64_Phdr Elf64_Phdr, *PElf64_Phdr;

struct Elf64_Phdr {
    enum Elf_ProgramHeaderType p_type;
    dword p_flags;
    qword p_offset;
    qword p_vaddr;
    qword p_paddr;
    qword p_filesz;
    qword p_memsz;
    qword p_align;
};

typedef struct NoteAbiTag NoteAbiTag, *PNoteAbiTag;

struct NoteAbiTag {
    dword namesz; // Length of name field
    dword descsz; // Length of description field
    dword type; // Vendor specific type
    char name[4]; // Vendor name
    dword abiType; // 0 == Linux
    dword requiredKernelVersion[3]; // Major.minor.patch
};

typedef struct Elf64_Sym Elf64_Sym, *PElf64_Sym;

struct Elf64_Sym {
    dword st_name;
    byte st_info;
    byte st_other;
    word st_shndx;
    qword st_value;
    qword st_size;
};




void _DT_INIT(void)

{
  __gmon_start__();
  return;
}



void FUN_00101020(void)

{
  (*(code *)(undefined *)0x0)();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

char * strncpy(char *__dest,char *__src,size_t __n)

{
  char *pcVar1;
  
  pcVar1 = strncpy(__dest,__src,__n);
  return pcVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int sem_wait(sem_t *__sem)

{
  int iVar1;
  
  iVar1 = sem_wait(__sem);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ssize_t write(int __fd,void *__buf,size_t __n)

{
  ssize_t sVar1;
  
  sVar1 = write(__fd,__buf,__n);
  return sVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

__pid_t getpid(void)

{
  __pid_t _Var1;
  
  _Var1 = getpid();
  return _Var1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void * mmap(void *__addr,size_t __len,int __prot,int __flags,int __fd,__off_t __offset)

{
  void *pvVar1;
  
  pvVar1 = mmap(__addr,__len,__prot,__flags,__fd,__offset);
  return pvVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int system(char *__command)

{
  int iVar1;
  
  iVar1 = system(__command);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int dup2(int __fd,int __fd2)

{
  int iVar1;
  
  iVar1 = dup2(__fd,__fd2);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int printf(char *__format,...)

{
  int iVar1;
  
  iVar1 = printf(__format);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

double difftime(time_t __time1,time_t __time0)

{
  double dVar1;
  
  dVar1 = difftime(__time1,__time0);
  return dVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int snprintf(char *__s,size_t __maxlen,char *__format,...)

{
  int iVar1;
  
  iVar1 = snprintf(__s,__maxlen,__format);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int ftruncate(int __fd,__off_t __length)

{
  int iVar1;
  
  iVar1 = ftruncate(__fd,__length);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void * memset(void *__s,int __c,size_t __n)

{
  void *pvVar1;
  
  pvVar1 = memset(__s,__c,__n);
  return pvVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int close(int __fd)

{
  int iVar1;
  
  iVar1 = close(__fd);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int pipe(int *__pipedes)

{
  int iVar1;
  
  iVar1 = pipe(__pipedes);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int sem_post(sem_t *__sem)

{
  int iVar1;
  
  iVar1 = sem_post(__sem);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

double cos(double __x)

{
  double dVar1;
  
  dVar1 = cos(__x);
  return dVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

ssize_t read(int __fd,void *__buf,size_t __nbytes)

{
  ssize_t sVar1;
  
  sVar1 = read(__fd,__buf,__nbytes);
  return sVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void srand(uint __seed)

{
  srand(__seed);
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int execve(char *__path,char **__argv,char **__envp)

{
  int iVar1;
  
  iVar1 = execve(__path,__argv,__envp);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int fprintf(FILE *__stream,char *__format,...)

{
  int iVar1;
  
  iVar1 = fprintf(__stream,__format);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

__mode_t umask(__mode_t __mask)

{
  __mode_t _Var1;
  
  _Var1 = umask(__mask);
  return _Var1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

char * __xpg_basename(char *__path)

{
  char *pcVar1;
  
  pcVar1 = __xpg_basename(__path);
  return pcVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

time_t time(time_t *__timer)

{
  time_t tVar1;
  
  tVar1 = time(__timer);
  return tVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int select(int __nfds,fd_set *__readfds,fd_set *__writefds,fd_set *__exceptfds,timeval *__timeout)

{
  int iVar1;
  
  iVar1 = select(__nfds,__readfds,__writefds,__exceptfds,__timeout);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int munmap(void *__addr,size_t __len)

{
  int iVar1;
  
  iVar1 = munmap(__addr,__len);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int sem_init(sem_t *__sem,int __pshared,uint __value)

{
  int iVar1;
  
  iVar1 = sem_init(__sem,__pshared,__value);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

__pid_t waitpid(__pid_t __pid,int *__stat_loc,int __options)

{
  __pid_t _Var1;
  
  _Var1 = waitpid(__pid,__stat_loc,__options);
  return _Var1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void perror(char *__s)

{
  perror(__s);
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int getopt(int ___argc,char **___argv,char *__shortopts)

{
  int iVar1;
  
  iVar1 = getopt(___argc,___argv,__shortopts);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

double sin(double __x)

{
  double dVar1;
  
  dVar1 = sin(__x);
  return dVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int atoi(char *__nptr)

{
  int iVar1;
  
  iVar1 = atoi(__nptr);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

void exit(int __status)

{
                    // WARNING: Subroutine does not return
  exit(__status);
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

size_t fwrite(void *__ptr,size_t __size,size_t __n,FILE *__s)

{
  size_t sVar1;
  
  sVar1 = fwrite(__ptr,__size,__n,__s);
  return sVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int shm_open(char *__name,int __oflag,mode_t __mode)

{
  int iVar1;
  
  iVar1 = shm_open(__name,__oflag,__mode);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int setuid(__uid_t __uid)

{
  int iVar1;
  
  iVar1 = setuid(__uid);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int shm_unlink(char *__name)

{
  int iVar1;
  
  iVar1 = shm_unlink(__name);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

uint sleep(uint __seconds)

{
  uint uVar1;
  
  uVar1 = sleep(__seconds);
  return uVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

__pid_t wait(void *__stat_loc)

{
  __pid_t _Var1;
  
  _Var1 = wait(__stat_loc);
  return _Var1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int sem_destroy(sem_t *__sem)

{
  int iVar1;
  
  iVar1 = sem_destroy(__sem);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

__pid_t fork(void)

{
  __pid_t _Var1;
  
  _Var1 = fork();
  return _Var1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int rand(void)

{
  int iVar1;
  
  iVar1 = rand();
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int usleep(__useconds_t __useconds)

{
  int iVar1;
  
  iVar1 = usleep(__useconds);
  return iVar1;
}



void __cxa_finalize(void)

{
  __cxa_finalize();
  return;
}



void processEntry entry(undefined8 param_1,undefined8 param_2)

{
  undefined1 auStack_8 [8];
  
  __libc_start_main(FUN_001013c9,param_2,&stack0x00000008,0,0,param_1,auStack_8);
  do {
                    // WARNING: Do nothing block with infinite loop
  } while( true );
}



// WARNING: Removing unreachable block (ram,0x00101323)
// WARNING: Removing unreachable block (ram,0x0010132f)

void FUN_00101310(void)

{
  return;
}



// WARNING: Removing unreachable block (ram,0x00101364)
// WARNING: Removing unreachable block (ram,0x00101370)

void FUN_00101340(void)

{
  return;
}



void _FINI_0(void)

{
  if (DAT_001061a8 != '\0') {
    return;
  }
  __cxa_finalize(PTR_LOOP_00106158);
  FUN_00101310();
  DAT_001061a8 = 1;
  return;
}



void _INIT_0(void)

{
  FUN_00101340();
  return;
}



// WARNING: Type propagation algorithm not settling

undefined8 FUN_001013c9(undefined4 param_1,undefined8 param_2)

{
  undefined4 uVar1;
  long lVar2;
  undefined8 uVar3;
  ulong uVar4;
  long lVar5;
  undefined8 uStack_120;
  undefined8 local_118;
  undefined4 local_10c;
  undefined1 local_108 [64];
  undefined1 local_c8 [64];
  undefined1 local_88 [16];
  undefined4 local_78;
  long local_68;
  long local_60;
  long local_58;
  long local_50;
  undefined8 local_48;
  undefined4 local_3c;
  
  uStack_120 = 0x1013ff;
  local_118 = param_2;
  local_10c = param_1;
  system("clear");
  uStack_120 = 0x10141a;
  getArgs(local_10c,local_118,local_88);
  uStack_120 = 0x101426;
  validateArgs(local_88);
  uStack_120 = 0x101432;
  printArgs(local_88);
  uStack_120 = 0x101441;
  local_48 = openGameSync(local_c8);
  uStack_120 = 0x101451;
  initializeGameSync(local_48);
  uStack_120 = 0x101467;
  local_50 = openGameState(local_88,local_108);
  uStack_120 = 0x10147e;
  initializeRandomBoard(local_88,local_50);
  uStack_120 = 0x10148a;
  spawnPlayers(local_50);
  if (local_68 != 0) {
    uStack_120 = 0x10149f;
    local_3c = forkToView(local_88);
  }
  local_58 = (ulong)*(uint *)(local_50 + 4) - 1;
  uVar4 = ((ulong)*(uint *)(local_50 + 4) * 4 + 0xf) / 0x10;
  lVar5 = (long)&local_118 + uVar4 * -0x10;
  uVar1 = *(undefined4 *)(local_50 + 4);
  local_60 = lVar5;
  (&uStack_120)[uVar4 * -2] = 0x10151b;
  createPipes(uVar1,lVar5);
  lVar2 = local_50;
  lVar5 = local_60;
  (&uStack_120)[uVar4 * -2] = 0x101532;
  spawnPlayerProcesses(local_88,lVar2,lVar5);
  lVar5 = local_60;
  (&uStack_120)[uVar4 * -2] = 0x101543;
  closeWritePipes(local_78,lVar5);
  uVar3 = local_48;
  lVar2 = local_50;
  lVar5 = local_60;
  (&uStack_120)[uVar4 * -2] = 0x10155b;
  game(local_88,lVar2,uVar3,lVar5);
  uVar1 = local_3c;
  if (local_68 != 0) {
    (&uStack_120)[uVar4 * -2] = 0x10156e;
    waitForView(uVar1);
  }
  lVar5 = local_50;
  (&uStack_120)[uVar4 * -2] = 0x10157a;
  waitAllPlayers(lVar5);
  uVar3 = local_48;
  (&uStack_120)[uVar4 * -2] = 0x101586;
  unInitializeGameSync(uVar3);
  (&uStack_120)[uVar4 * -2] = 0x101595;
  unlinkMem(local_c8);
  (&uStack_120)[uVar4 * -2] = 0x1015a4;
  unlinkMem(local_108);
  return 0;
}



void game(long param_1,long param_2,undefined8 param_3,undefined8 param_4)

{
  char cVar1;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  time_t local_10;
  
  local_10 = time((time_t *)0x0);
  local_14 = 0;
  while( true ) {
    if (*(long *)(param_1 + 0x20) != 0) {
      askViewToPrint(param_1,param_3);
    }
    if (*(char *)(param_2 + 0x170) != '\0') break;
    cVar1 = getNextMove(&local_14,param_1,param_2,param_3,param_4,&local_18,&local_1c,&local_10);
    if (cVar1 == '\0') {
      endGame(param_2,param_3);
    }
    else {
      processMove(param_2,param_3,local_18,local_1c,&local_10);
    }
  }
  return;
}



undefined8
getNextMove(int *param_1,undefined8 param_2,long param_3,undefined8 param_4,long param_5,
            undefined8 param_6,undefined8 param_7,undefined8 param_8)

{
  uint uVar1;
  uint uVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  undefined8 uVar6;
  uint uVar7;
  
  if (*param_1 == 0) {
    DAT_001061c0 = 0;
    *param_1 = 1;
  }
  uVar7 = DAT_001061c0;
  if (*param_1 == 1) {
    DAT_00106260 = getNumberOfReadyFd(param_2,param_3,param_5,&DAT_001061e0,param_8);
    if (DAT_00106260 == 0) {
      DAT_00106260 = 0;
      return 0;
    }
    *param_1 = 2;
    uVar7 = DAT_001061c0;
  }
  do {
    do {
      DAT_001061c0 = uVar7;
      uVar2 = DAT_001061c0;
      uVar7 = (DAT_001061c0 + 1) % *(uint *)(param_3 + 4);
    } while (*(char *)((ulong)DAT_001061c0 * 0x28 + param_3 + 0x2c) == '\x01');
    iVar4 = *(int *)(param_5 + (ulong)(DAT_001061c0 * 2) * 4);
    if (iVar4 < 0) {
      iVar4 = iVar4 + 0x3f;
    }
  } while ((1L << ((byte)*(undefined4 *)(param_5 + (ulong)(DAT_001061c0 * 2) * 4) & 0x3f) &
           *(ulong *)(&DAT_001061e0 + (long)(iVar4 >> 6) * 8)) == 0);
  iVar4 = *(int *)(param_5 + (ulong)(DAT_001061c0 * 2) * 4);
  if (iVar4 < 0) {
    iVar4 = iVar4 + 0x3f;
  }
  uVar1 = DAT_001061c0 * 2;
  iVar5 = *(int *)(param_5 + (ulong)(DAT_001061c0 * 2) * 4);
  if (iVar5 < 0) {
    iVar5 = iVar5 + 0x3f;
  }
  DAT_001061c0 = uVar7;
  *(ulong *)(&DAT_001061e0 + (long)(iVar5 >> 6) * 8) =
       ~(1L << ((byte)*(undefined4 *)(param_5 + (ulong)uVar1 * 4) & 0x3f)) &
       *(ulong *)(&DAT_001061e0 + (long)(iVar4 >> 6) * 8);
  DAT_00106260 = DAT_00106260 + -1;
  if (DAT_00106260 == 0) {
    *param_1 = 1;
  }
  cVar3 = readPlayer(param_5,uVar2,param_6,param_7);
  if (cVar3 == '\0') {
    lockGameStateReads(param_4);
    *(undefined1 *)((ulong)uVar2 * 0x28 + param_3 + 0x2c) = 1;
    unlockGameStateReads(param_4);
    cVar3 = canAnyPlayerMove(param_3);
    if (cVar3 == '\0') {
      uVar6 = getNextMove(param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8);
    }
    else {
      uVar6 = 0;
    }
  }
  else {
    uVar6 = 1;
  }
  return uVar6;
}



void processMove(long param_1,undefined8 param_2,uint param_3,undefined4 param_4,time_t *param_5)

{
  char cVar1;
  undefined1 uVar2;
  time_t tVar3;
  undefined4 local_c;
  
  cVar1 = isDirectionValid(param_1,param_3,param_4,&local_c);
  if (cVar1 == '\0') {
    lockGameStateReads(param_2);
    *(int *)((ulong)param_3 * 0x28 + param_1 + 0x1c) =
         *(int *)((ulong)param_3 * 0x28 + param_1 + 0x1c) + 1;
    unlockGameStateReads(param_2);
  }
  else {
    lockGameStateReads(param_2);
    FUN_00102b51(param_1,param_3,local_c);
    *(int *)((ulong)param_3 * 0x28 + param_1 + 0x20) =
         *(int *)((ulong)param_3 * 0x28 + param_1 + 0x20) + 1;
    FUN_00102c19(param_1);
    uVar2 = canAnyPlayerMove(param_1);
    *(undefined1 *)(param_1 + 0x170) = uVar2;
    unlockGameStateReads(param_2);
    tVar3 = time((time_t *)0x0);
    *param_5 = tVar3;
  }
  return;
}



void askViewToPrint(long param_1,sem_t *param_2)

{
  sem_post(param_2);
  sem_wait(param_2 + 1);
  usleep(*(int *)(param_1 + 4) * 1000);
  return;
}



void waitForView(__pid_t param_1)

{
  __pid_t _Var1;
  uint local_c;
  
  _Var1 = waitpid(param_1,(int *)&local_c,0);
  if (_Var1 == -1) {
    perror("waitpid");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  printf("View exited (%d)\n",(ulong)local_c);
  return;
}



void waitAllPlayers(long param_1)

{
  long lVar1;
  uint local_24;
  long local_20;
  __pid_t local_14;
  uint local_10;
  uint local_c;
  
  local_c = 0;
  do {
    if (*(uint *)(param_1 + 4) <= local_c) {
      return;
    }
    local_14 = wait(&local_24);
    if (local_14 == -1) {
      perror("wait");
                    // WARNING: Subroutine does not return
      exit(1);
    }
    for (local_10 = 0; local_10 < *(uint *)(param_1 + 4); local_10 = local_10 + 1) {
      if (local_14 == *(int *)((ulong)local_10 * 0x28 + param_1 + 0x28)) {
        lVar1 = (ulong)local_10 * 0x28 + param_1;
        local_20 = lVar1 + 8;
        printf("Player %s (%u) exited (%d) with a score of %u / %u / %u\n",local_20,(ulong)local_10,
               (ulong)local_24,(ulong)*(uint *)(lVar1 + 0x18),(ulong)*(uint *)(lVar1 + 0x20),
               (ulong)*(uint *)(lVar1 + 0x1c));
        break;
      }
    }
    local_c = local_c + 1;
  } while( true );
}



void execveWithArgs(char *param_1,ushort param_2,ushort param_3)

{
  int iVar1;
  char *local_70;
  char *local_68;
  char *local_60;
  char *local_58;
  undefined8 local_50;
  char local_48 [32];
  char local_28 [32];
  
  snprintf(local_28,0x14,"%d",(ulong)param_2);
  snprintf(local_48,0x14,"%d",(ulong)param_3);
  local_60 = local_28;
  local_58 = local_48;
  local_50 = 0;
  local_70 = (char *)0x0;
  local_68 = param_1;
  iVar1 = execve(param_1,&local_68,&local_70);
  if (iVar1 == -1) {
    perror("execve");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  return;
}



void spawnPlayerProcesses(undefined2 *param_1,long param_2,long param_3)

{
  __pid_t _Var1;
  int iVar2;
  uint local_c;
  
  local_c = 0;
  while( true ) {
    if (*(uint *)(param_1 + 8) <= local_c) {
      return;
    }
    _Var1 = fork();
    if (_Var1 == -1) break;
    if (_Var1 == 0) {
      iVar2 = setuid(1000);
      if (iVar2 == -1) {
        perror("setuid");
                    // WARNING: Subroutine does not return
        exit(1);
      }
      closeForeignPipes(local_c,*(undefined4 *)(param_1 + 8),param_3);
      close(*(int *)(param_3 + (ulong)(local_c * 2) * 4));
      dup2(*(int *)(param_3 + (ulong)(local_c * 2 + 1) * 4),1);
      close(*(int *)(param_3 + (ulong)(local_c * 2 + 1) * 4));
      execveWithArgs(*(undefined8 *)(*(long *)(param_1 + 0xc) + (ulong)local_c * 8),*param_1,
                   param_1[1]);
    }
    else {
      *(__pid_t *)((ulong)local_c * 0x28 + param_2 + 0x28) = _Var1; //game state set pid
    }
    local_c = local_c + 1;
  }
  perror("fork");
                    // WARNING: Subroutine does not return
  exit(1);
}



__pid_t forkToView(undefined2 *param_1)

{
  __pid_t _Var1;
  int iVar2;
  
  _Var1 = fork();
  if (_Var1 == -1) {
    perror("fork");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  if (_Var1 == 0) {
    iVar2 = setuid(1000);
    if (iVar2 == -1) {
      perror("setuid");
                    // WARNING: Subroutine does not return
      exit(1);
    }
    execveWithArgs(*(undefined8 *)(param_1 + 0x10),*param_1,param_1[1]);
  }
  return _Var1;
}



undefined8 FUN_00101ec8(undefined8 *param_1)

{
  *param_1 = "/game_sync";
  *(undefined4 *)(param_1 + 1) = 2;
  *(undefined4 *)((long)param_1 + 0xc) = 0;
  param_1[3] = 0xa8;
  param_1[4] = 0;
  *(undefined4 *)(param_1 + 5) = 3;
  *(undefined4 *)((long)param_1 + 0x2c) = 1;
  param_1[6] = 0;
  FUN_001039e4(param_1);
  return param_1[7];
}



undefined8 FUN_00101f48(ushort param_1,ushort param_2,undefined8 *param_3)

{
  *param_3 = "/game_state";
  *(undefined4 *)(param_3 + 1) = 0;
  *(undefined4 *)((long)param_3 + 0xc) = 0;
  param_3[3] = ((ulong)param_2 * (ulong)param_1 + 0x5d) * 4;
  param_3[4] = 0;
  *(undefined4 *)(param_3 + 5) = 1;
  *(undefined4 *)((long)param_3 + 0x2c) = 1;
  param_3[6] = 0;
  FUN_001039e4(param_3);
  return param_3[7];
}



undefined8 openGameSync(undefined8 *param_1)

{
  __mode_t __mask;
  
  *param_1 = "/game_sync";
  *(undefined4 *)(param_1 + 1) = 0x42;
  *(undefined4 *)((long)param_1 + 0xc) = 0x1b6;
  param_1[3] = 0xa8;
  param_1[4] = 0;
  *(undefined4 *)(param_1 + 5) = 3;
  *(undefined4 *)((long)param_1 + 0x2c) = 1;
  param_1[6] = 0;
  __mask = umask(0);
  FUN_001039e4(param_1);
  umask(__mask);
  return param_1[7];
}



undefined8 openGameState(ushort *param_1,undefined8 *param_2)

{
  *param_2 = "/game_state";
  *(undefined4 *)(param_2 + 1) = 0x42;
  *(undefined4 *)((long)param_2 + 0xc) = 0x1a4;
  param_2[3] = ((ulong)param_1[1] * (ulong)*param_1 + 0x5d) * 4;
  param_2[4] = 0;
  *(undefined4 *)(param_2 + 5) = 3;
  *(undefined4 *)((long)param_2 + 0x2c) = 1;
  param_2[6] = 0;
  FUN_001039e4(param_2);
  return param_2[7];
}



void createPipes(uint param_1,long param_2)

{
  int iVar1;
  uint local_c;
  
  local_c = 0;
  while( true ) {
    if (param_1 <= local_c) {
      return;
    }
    iVar1 = pipe((int *)(param_2 + (ulong)(local_c * 2) * 4));
    if (iVar1 == -1) break;
    local_c = local_c + 1;
  }
  perror("pipe");
                    // WARNING: Subroutine does not return
  exit(1);
}



void closeForeignPipes(uint param_1,uint param_2,long param_3)

{
  undefined4 local_c;
  
  for (local_c = 0; local_c < param_2; local_c = local_c + 1) {
    if (param_1 != local_c) {
      close(*(int *)(param_3 + (ulong)(local_c * 2) * 4));
      close(*(int *)(param_3 + (ulong)(local_c * 2 + 1) * 4));
    }
  }
  return;
}



void closeWritePipes(uint param_1,long param_2)

{
  undefined4 local_c;
  
  for (local_c = 0; local_c < param_1; local_c = local_c + 1) {
    close(*(int *)(param_2 + (ulong)(local_c * 2 + 1) * 4));
  }
  return;
}



int buildFdSet(long param_1,long param_2,long param_3)

{
  int iVar1;
  int iVar2;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  local_c = 0;
  for (local_10 = 0; local_10 < 0x10; local_10 = local_10 + 1) {
    *(undefined8 *)(param_3 + (ulong)local_10 * 8) = 0;
  }
  for (local_14 = 0; local_14 < *(uint *)(param_1 + 4); local_14 = local_14 + 1) {
    iVar1 = local_c;
    if (*(char *)((ulong)local_14 * 0x28 + param_1 + 0x2c) != '\x01') {
      iVar1 = *(int *)(param_2 + (ulong)(local_14 * 2) * 4);
      if (iVar1 < 0) {
        iVar1 = iVar1 + 0x3f;
      }
      iVar2 = *(int *)(param_2 + (ulong)(local_14 * 2) * 4);
      if (iVar2 < 0) {
        iVar2 = iVar2 + 0x3f;
      }
      *(ulong *)(param_3 + (long)(iVar2 >> 6) * 8) =
           *(ulong *)(param_3 + (long)(iVar1 >> 6) * 8) |
           1L << ((byte)*(undefined4 *)(param_2 + (ulong)(local_14 * 2) * 4) & 0x3f);
      iVar1 = *(int *)(param_2 + (ulong)(local_14 * 2) * 4);
      if (iVar1 <= local_c) {
        iVar1 = local_c;
      }
    }
    local_c = iVar1;
  }
  return local_c;
}



// WARNING: Removing unreachable block (ram,0x001023c7)

int getNumberOfReadyFd(long param_1,undefined8 param_2,undefined8 param_3,fd_set *param_4,time_t *param_5)

{
  uint uVar1;
  time_t __time0;
  time_t __time1;
  double dVar2;
  timeval local_38;
  int local_20;
  int local_1c;
  
  uVar1 = *(uint *)(param_1 + 0xc);
  __time0 = *param_5;
  __time1 = time((time_t *)0x0);
  dVar2 = difftime(__time1,__time0);
  local_38.tv_sec = (__time_t)((double)uVar1 - dVar2);
  local_38.tv_usec = 0;
  local_1c = buildFdSet(param_2,param_3,param_4);
  if (local_38.tv_sec < 1) {
    local_20 = 0;
  }
  else {
    local_20 = select(local_1c + 1,param_4,(fd_set *)0x0,(fd_set *)0x0,&local_38);
    if (local_20 == -1) {
      perror("select");
                    // WARNING: Subroutine does not return
      exit(1);
    }
  }
  return local_20;
}



bool readPlayer(long param_1,int param_2,int *param_3,uint *param_4)

{
  ssize_t sVar1;
  byte local_9;
  
  sVar1 = read(*(int *)(param_1 + (ulong)(uint)(param_2 * 2) * 4),&local_9,1);
  if (sVar1 == -1) {
    perror("read");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  if (sVar1 != 0) {
    *param_4 = (uint)local_9;
    *param_3 = param_2;
  }
  return sVar1 != 0;
}



void FUN_0010251f(int param_1,undefined1 param_2)

{
  ssize_t sVar1;
  undefined1 local_9;
  
  local_9 = param_2;
  sVar1 = write(param_1,&local_9,1);
  if (sVar1 == -1) {
    perror("write");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  return;
}



// WARNING: Removing unreachable block (ram,0x001026f6)
// WARNING: Removing unreachable block (ram,0x00102734)

void spawnPlayers(ushort *param_1)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  uint uVar5;
  uint uVar6;
  double dVar7;
  double __x;
  uint local_c;
  
  uVar1 = *param_1;
  uVar2 = param_1[1];
  uVar3 = *param_1;
  uVar4 = param_1[1];
  if (*(int *)(param_1 + 2) == 1) {
    param_1[0x12] = (ushort)(int)((double)uVar1 / 2.0);
    param_1[0x13] = (ushort)(int)((double)uVar2 / 2.0);
    uVar1 = param_1[0x13];
    uVar2 = *param_1;
    uVar3 = param_1[0x12];
    (param_1 + ((long)(int)((uint)uVar1 * (uint)uVar2 + (uint)uVar3) + 0x5c) * 2 + 2)[0] = 0;
    (param_1 + ((long)(int)((uint)uVar1 * (uint)uVar2 + (uint)uVar3) + 0x5c) * 2 + 2)[1] = 0;
  }
  else {
    for (local_c = 0; local_c < *(uint *)(param_1 + 2); local_c = local_c + 1) {
      __x = ((double)local_c * 6.283185307179586) / (double)*(uint *)(param_1 + 2);
      dVar7 = cos(__x);
      uVar5 = (uint)(dVar7 * ((double)uVar3 / 3.0) + (double)uVar1 / 2.0 + 0.5);
      dVar7 = sin(__x);
      uVar6 = (uint)(dVar7 * ((double)uVar4 / 3.0) + (double)uVar2 / 2.0 + 0.5);
      param_1[(ulong)local_c * 0x14 + 0x12] = (ushort)uVar5;
      param_1[(ulong)local_c * 0x14 + 0x13] = (ushort)uVar6;
      *(uint *)(param_1 +
               ((long)(int)((uVar6 & 0xffff) * (uint)*param_1 + (uVar5 & 0xffff)) + 0x5c) * 2 + 2) =
           -local_c;
    }
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_00102851(ushort *param_1,uint param_2)

{
  if (DAT_00106280 == 0) {
    _DAT_00106290 = (uint)*param_1;
    DAT_00106280 = -_DAT_00106290;
    _DAT_00106284 = 1 - _DAT_00106290;
    _DAT_00106288 = 1;
    _DAT_0010628c = _DAT_00106290 + 1;
    _DAT_00106294 = _DAT_00106290 - 1;
    _DAT_00106298 = 0xffffffff;
    _DAT_0010629c = ~_DAT_00106290;
  }
  return (&DAT_00106280)[param_2];
}



int twoDimensionCoordsToOneDimension(ushort *param_1,ushort param_2,ushort param_3)

{
  return (uint)param_2 + (uint)param_3 * (uint)*param_1;
}



void oneDimentionCoordsToTwoDimentionsCoords(ushort *param_1,uint param_2,undefined2 *param_3,undefined2 *param_4)

{
  *param_3 = (short)(param_2 % (uint)*param_1);
  *param_4 = (short)(param_2 / *param_1);
  return;
}



undefined8 isInvalidDirection(ushort *param_1,uint param_2,int param_3)

{
  undefined8 uVar1;
  
  if ((param_1[(ulong)param_2 * 0x14 + 0x13] == 0) &&
     (((param_3 == 7 || (param_3 == 0)) || (param_3 == 1)))) {
    uVar1 = 1;
  }
  else if (((uint)param_1[(ulong)param_2 * 0x14 + 0x13] == param_1[1] - 1) &&
          (((param_3 == 5 || (param_3 == 4)) || (param_3 == 3)))) {
    uVar1 = 1;
  }
  else if ((param_1[(ulong)param_2 * 0x14 + 0x12] == 0) &&
          (((param_3 == 7 || (param_3 == 6)) || (param_3 == 5)))) {
    uVar1 = 1;
  }
  else if (((uint)param_1[(ulong)param_2 * 0x14 + 0x12] == *param_1 - 1) &&
          (((param_3 == 1 || (param_3 == 2)) || (param_3 == 3)))) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}



undefined4 FUN_00102a5f(long param_1,uint param_2)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 4 + ((ulong)param_2 + 0x5c) * 4);
  return CONCAT31((int3)((uint)iVar1 >> 8),0 < iVar1);
}



undefined8 isDirectionValid(long param_1,uint param_2,uint param_3,int *param_4)

{
  char cVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  
  if (param_3 < 8) {
    cVar1 = isInvalidDirection(param_1,param_2,param_3);
    if (cVar1 == '\0') {
      iVar2 = FUN_00102851(param_1,param_3);
      iVar3 = twoDimensionCoordsToOneDimension(param_1,*(undefined2 *)((ulong)param_2 * 0x28 + param_1 + 0x24),
                           *(undefined2 *)((ulong)param_2 * 0x28 + param_1 + 0x26));
      *param_4 = iVar2 + iVar3;
      uVar4 = FUN_00102a5f(param_1,*param_4);
    }
    else {
      uVar4 = 0;
    }
  }
  else {
    uVar4 = 0;
  }
  return uVar4;
}



void FUN_00102b51(long param_1,uint param_2,uint param_3)

{
  oneDimentionCoordsToTwoDimentionsCoords(param_1,param_3,param_1 + (ulong)param_2 * 0x28 + 0x24,
               param_1 + (ulong)param_2 * 0x28 + 0x26);
  *(int *)((ulong)param_2 * 0x28 + param_1 + 0x18) =
       *(int *)((ulong)param_2 * 0x28 + param_1 + 0x18) +
       *(int *)(param_1 + 4 + ((ulong)param_3 + 0x5c) * 4);
  *(uint *)(param_1 + 4 + ((ulong)param_3 + 0x5c) * 4) = -param_2;
  return;
}



void FUN_00102c19(long param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_c;
  
  for (local_c = 0; local_c < *(uint *)(param_1 + 4); local_c = local_c + 1) {
    if (*(char *)((ulong)local_c * 0x28 + param_1 + 0x2c) == '\0') {
      uVar1 = twoDimensionCoordsToOneDimension(param_1,*(undefined2 *)((ulong)local_c * 0x28 + param_1 + 0x24),
                           *(undefined2 *)((ulong)local_c * 0x28 + param_1 + 0x26));
      iVar2 = FUN_00102cfe(param_1,uVar1);
      *(bool *)((ulong)local_c * 0x28 + param_1 + 0x2c) = iVar2 == 0;
    }
  }
  return;
}



undefined8 FUN_00102cfe(ushort *param_1,int param_2)

{
  char cVar1;
  int iVar2;
  ushort local_c;
  ushort local_a;
  
  oneDimentionCoordsToTwoDimentionsCoords(param_1,param_2,&local_a,&local_c);
  if (local_c != 0) {
    iVar2 = FUN_00102851(param_1,0);
    cVar1 = FUN_00102a5f(param_1,iVar2 + param_2);
    if (cVar1 != '\0') {
      return 1;
    }
    if (local_a != 0) {
      iVar2 = FUN_00102851(param_1,7);
      cVar1 = FUN_00102a5f(param_1,iVar2 + param_2);
      if (cVar1 != '\0') {
        return 1;
      }
    }
    if ((int)(uint)local_a < (int)(*param_1 - 1)) {
      iVar2 = FUN_00102851(param_1,1);
      cVar1 = FUN_00102a5f(param_1,iVar2 + param_2);
      if (cVar1 != '\0') {
        return 1;
      }
    }
  }
  if ((int)(uint)local_c < (int)(param_1[1] - 1)) {
    iVar2 = FUN_00102851(param_1,4);
    cVar1 = FUN_00102a5f(param_1,iVar2 + param_2);
    if (cVar1 != '\0') {
      return 1;
    }
    if (local_a != 0) {
      iVar2 = FUN_00102851(param_1,5);
      cVar1 = FUN_00102a5f(param_1,iVar2 + param_2);
      if (cVar1 != '\0') {
        return 1;
      }
    }
    if ((int)(uint)local_a < (int)(*param_1 - 1)) {
      iVar2 = FUN_00102851(param_1,3);
      cVar1 = FUN_00102a5f(param_1,iVar2 + param_2);
      if (cVar1 != '\0') {
        return 1;
      }
    }
  }
  if (local_a != 0) {
    iVar2 = FUN_00102851(param_1,6);
    cVar1 = FUN_00102a5f(param_1,iVar2 + param_2);
    if (cVar1 != '\0') {
      return 1;
    }
  }
  if ((int)(uint)local_a < (int)(*param_1 - 1)) {
    iVar2 = FUN_00102851(param_1,2);
    cVar1 = FUN_00102a5f(param_1,iVar2 + param_2);
    if (cVar1 != '\0') {
      return 1;
    }
  }
  return 0;
}



undefined8 canAnyPlayerMove(long param_1)

{
  uint local_c;
  
  local_c = 0;
  while( true ) {
    if (*(uint *)(param_1 + 4) <= local_c) {
      return 1;
    }
    if (*(char *)((ulong)local_c * 0x28 + param_1 + 0x2c) != '\x01') break;
    local_c = local_c + 1;
  }
  return 0;
}



uint FUN_00102fa7(long param_1)

{
  __pid_t _Var1;
  uint local_c;
  
  _Var1 = getpid();
  local_c = 0;
  while( true ) {
    if (*(uint *)(param_1 + 4) <= local_c) {
      return 0xffffffff;
    }
    if (_Var1 == *(int *)((ulong)local_c * 0x28 + param_1 + 0x28)) break;
    local_c = local_c + 1;
  }
  return local_c;
}



void endGame(long param_1,undefined8 param_2)

{
  lockGameStateReads(param_2);
  *(undefined1 *)(param_1 + 0x170) = 1;
  unlockGameStateReads(param_2);
  return;
}



void initializeRandomBoard(ushort *param_1,ushort *param_2)

{
  int iVar1;
  char *__src;
  int local_14;
  int local_10;
  uint local_c;
  
  *param_2 = *param_1;
  param_2[1] = param_1[1];
  *(undefined4 *)(param_2 + 2) = *(undefined4 *)(param_1 + 8);
  *(undefined1 *)(param_2 + 0xb8) = 0; //I think this is "isOver"
  memset(param_2 + 4,0,0x168); //Sets the player area to 0
  for (local_c = 0; local_c < *(uint *)(param_1 + 8); local_c = local_c + 1) { //This should copy players
    __src = __xpg_basename(*(char **)(*(long *)(param_1 + 0xc) + (ulong)local_c * 8)); //Only taks the name of the file from the path
    strncpy((char *)(param_2 + (ulong)local_c * 0x14 + 4),__src,0x10);
  }
  srand(*(uint *)(param_1 + 4));
  for (local_10 = 0; local_10 < (int)(uint)param_2[1]; local_10 = local_10 + 1) {
    for (local_14 = 0; local_14 < (int)(uint)*param_2; local_14 = local_14 + 1) {
      iVar1 = rand();
      *(int *)(param_2 + ((long)(int)((uint)*param_2 * local_10 + local_14) + 0x5c) * 2 + 2) =
           iVar1 % 9 + 1;
    }
  }
  return;
}



void initializeGameSync(sem_t *param_1)

{
  int iVar1;
  
  iVar1 = sem_init(param_1,1,0);
  if (iVar1 == -1) {
    perror("sem_init");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  iVar1 = sem_init(param_1 + 1,1,0);
  if (iVar1 == -1) {
    perror("sem_init");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  iVar1 = sem_init(param_1 + 2,1,1);
  if (iVar1 == -1) {
    perror("sem_init");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  iVar1 = sem_init(param_1 + 3,1,1);
  if (iVar1 == -1) {
    perror("sem_init");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  iVar1 = sem_init(param_1 + 4,1,1);
  if (iVar1 == -1) {
    perror("sem_init");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  *(undefined4 *)(param_1 + 5) = 0;
  return;
}



void FUN_001032d2(long param_1)

{
  int iVar1;
  
  sem_wait((sem_t *)(param_1 + 0x40));
  sem_post((sem_t *)(param_1 + 0x40));
  sem_wait((sem_t *)(param_1 + 0x80));
  iVar1 = *(int *)(param_1 + 0xa0);
  *(int *)(param_1 + 0xa0) = iVar1 + 1;
  if (iVar1 == 0) {
    sem_wait((sem_t *)(param_1 + 0x60));
  }
  sem_post((sem_t *)(param_1 + 0x80));
  return;
}



void FUN_0010334c(long param_1)

{
  sem_wait((sem_t *)(param_1 + 0x80));
  *(int *)(param_1 + 0xa0) = *(int *)(param_1 + 0xa0) + -1;
  if (*(int *)(param_1 + 0xa0) == 0) {
    sem_post((sem_t *)(param_1 + 0x60));
  }
  sem_post((sem_t *)(param_1 + 0x80));
  return;
}



void lockGameStateReads(long param_1)

{
  sem_wait((sem_t *)(param_1 + 0x40));
  sem_wait((sem_t *)(param_1 + 0x60));
  sem_post((sem_t *)(param_1 + 0x40));
  return;
}



void unlockGameStateReads(long param_1)

{
  sem_post((sem_t *)(param_1 + 0x60));
  return;
}



void unInitializeGameSync(sem_t *param_1)

{
  int iVar1;
  
  iVar1 = sem_destroy(param_1);
  if (iVar1 == -1) {
    perror("sem_destroy");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  iVar1 = sem_destroy(param_1 + 1);
  if (iVar1 == -1) {
    perror("sem_destroy");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  iVar1 = sem_destroy(param_1 + 2);
  if (iVar1 == -1) {
    perror("sem_destroy");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  iVar1 = sem_destroy(param_1 + 3);
  if (iVar1 == -1) {
    perror("sem_destroy");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  iVar1 = sem_destroy(param_1 + 4);
  if (iVar1 == -1) {
    perror("sem_destroy");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  return;
}



void getArgs(int param_1,char **param_2,short *param_3)

{
  bool bVar1;
  int iVar2;
  time_t tVar3;
  char **local_18;
  uint local_10;
  
  *param_3 = 0;
  param_3[1] = 0;
  param_3[2] = 200;
  param_3[3] = 0;
  param_3[6] = 10;
  param_3[7] = 0;
  tVar3 = time((time_t *)0x0);
  *(int *)(param_3 + 4) = (int)tVar3;
  param_3[0x10] = 0;
  param_3[0x11] = 0;
  param_3[0x12] = 0;
  param_3[0x13] = 0;
  bVar1 = false;
  local_10 = 0;
  local_18 = (char **)0x0;
  while (iVar2 = getopt(param_1,param_2,"w:h:d:p:v:s:t:"), iVar2 != -1) {
    switch(iVar2) {
    case 100:
      bVar1 = true;
      iVar2 = atoi(optarg);
      *(int *)(param_3 + 2) = iVar2;
      break;
    default:
      fprintf(stderr,
              "Usage: %s [-w width] [-h height] [-d delay] [-s seed] [-v view] [-t timeout] -p player1 player2 ...\n"
              ,*param_2);
                    // WARNING: Subroutine does not return
      exit(1);
    case 0x68:
      iVar2 = atoi(optarg);
      param_3[1] = (short)iVar2;
      break;
    case 0x70:
      local_18 = param_2 + (long)optind + -1;
      while ((local_18[local_10] != (char *)0x0 && (*local_18[local_10] != '-'))) {
        local_10 = local_10 + 1;
      }
      optind = local_10 + optind + -1;
      break;
    case 0x73:
      iVar2 = atoi(optarg);
      *(int *)(param_3 + 4) = iVar2;
      break;
    case 0x74:
      iVar2 = atoi(optarg);
      *(int *)(param_3 + 6) = iVar2;
      break;
    case 0x76:
      *(char **)(param_3 + 0x10) = param_2[(long)optind + -1];
      break;
    case 0x77:
      iVar2 = atoi(optarg);
      *param_3 = (short)iVar2;
    }
  }
  if ((bVar1) && (*(long *)(param_3 + 0x10) == 0)) {
    fwrite("Info: Delay parameter ignored since there is no view.\n",1,0x36,stderr);
  }
  if ((*param_3 == 0) || (param_3[1] == 0)) {
    if (*param_3 == 0) {
      *param_3 = 10;
    }
    if (param_3[1] == 0) {
      param_3[1] = 10;
    }
  }
  *(uint *)(param_3 + 8) = local_10;
  *(char ***)(param_3 + 0xc) = local_18;
  return;
}



void validateArgs(ushort *param_1)

{
  if (*(int *)(param_1 + 8) == 0) {
    fwrite("Error: At least one player must be specified using -p.\n",1,0x37,stderr);
                    // WARNING: Subroutine does not return
    exit(1);
  }
  if (9 < *(uint *)(param_1 + 8)) {
    fprintf(stderr,"Error: At most %d players can be specified using -p.\n",9);
                    // WARNING: Subroutine does not return
    exit(1);
  }
  if ((9 < *param_1) && (9 < param_1[1])) {
    return;
  }
  fprintf(stderr,"Error: Minimal board dimensions: %dx%d. Given %dx%d\n",10,10,(ulong)*param_1,
          (ulong)param_1[1]);
                    // WARNING: Subroutine does not return
  exit(1);
}



void printArgs(ushort *param_1)

{
  undefined *puVar1;
  uint local_c;
  
  printf("width: %d\n",(ulong)*param_1);
  printf("height: %d\n",(ulong)param_1[1]);
  printf("delay: %u\n",(ulong)*(uint *)(param_1 + 2));
  printf("timeout: %u\n",(ulong)*(uint *)(param_1 + 6));
  printf("seed: %u\n",(ulong)*(uint *)(param_1 + 4));
  if (*(long *)(param_1 + 0x10) == 0) {
    puVar1 = &DAT_001042de;
  }
  else {
    puVar1 = *(undefined **)(param_1 + 0x10);
  }
  printf("view: %s\n",puVar1);
  printf("num_players: %u\n",(ulong)*(uint *)(param_1 + 8));
  for (local_c = 0; local_c < *(uint *)(param_1 + 8); local_c = local_c + 1) {
    printf("  %s\n",*(undefined8 *)(*(long *)(param_1 + 0xc) + (ulong)local_c * 8));
  }
  sleep(2);
  return;
}



void FUN_001039e4(undefined8 *param_1)

{
  int iVar1;
  void *pvVar2;
  
  iVar1 = shm_open((char *)*param_1,*(int *)(param_1 + 1),*(mode_t *)((long)param_1 + 0xc));
  *(int *)(param_1 + 2) = iVar1;
  if (*(int *)(param_1 + 2) == -1) {
    perror("shm_open");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  if ((*(uint *)(param_1 + 1) & 0x40) != 0) {
    iVar1 = ftruncate(*(int *)(param_1 + 2),param_1[3]);
    if (iVar1 == -1) {
      perror("ftruncate");
                    // WARNING: Subroutine does not return
      exit(1);
    }
  }
  pvVar2 = mmap((void *)param_1[4],param_1[3],*(int *)(param_1 + 5),*(int *)((long)param_1 + 0x2c),
                *(int *)(param_1 + 2),param_1[6]);
  param_1[7] = pvVar2;
  if (param_1[7] == -1) {
    perror("mmap");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  return;
}



void unlinkMem(undefined8 *param_1)

{
  int iVar1;
  
  iVar1 = munmap((void *)param_1[7],param_1[3]);
  if (iVar1 == -1) {
    perror("munmap");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  iVar1 = close(*(int *)(param_1 + 2));
  if (iVar1 == -1) {
    perror("close");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  iVar1 = shm_unlink((char *)*param_1);
  if (iVar1 == -1) {
    perror("shm_unlink");
                    // WARNING: Subroutine does not return
    exit(1);
  }
  return;
}



void _DT_FINI(void)

{
  return;
}



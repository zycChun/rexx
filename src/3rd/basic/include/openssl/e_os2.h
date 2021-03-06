
#include <openssl/opensslconf.h>

#ifndef HEADER_E_OS2_H
# define HEADER_E_OS2_H

#ifdef  __cplusplus
extern "C" {
#endif


# define OPENSSL_SYS_UNIX

/* ---------------------- Macintosh, before MacOS X ----------------------- */
# if defined(__MWERKS__) && defined(macintosh) || defined(OPENSSL_SYSNAME_MAC)
#  undef OPENSSL_SYS_UNIX
#  undef _WIN32
#  undef OPENSSL_SYSNAME_WIN32
#  define OPENSSL_SYS_MACINTOSH_CLASSIC
# endif

/* ---------------------- NetWare ----------------------------------------- */
# if defined(NETWARE) || defined(OPENSSL_SYSNAME_NETWARE)
#  undef OPENSSL_SYS_UNIX
#  define OPENSSL_SYS_NETWARE
# endif

/* --------------------- Microsoft operating systems ---------------------- */

/*
 * Note that MSDOS actually denotes 32-bit environments running on top of
 * MS-DOS, such as DJGPP one.
 */
# if defined(OPENSSL_SYSNAME_MSDOS)
#  undef OPENSSL_SYS_UNIX
#  define OPENSSL_SYS_MSDOS
# endif

/*
 * For 32 bit environment, there seems to be the CygWin environment and then
 * all the others that try to do the same thing Microsoft does...
 */
# if defined(OPENSSL_SYSNAME_UWIN)
#  undef OPENSSL_SYS_UNIX
#  define OPENSSL_SYS_WIN32_UWIN
# else
#  if defined(__CYGWIN__) || defined(OPENSSL_SYSNAME_CYGWIN)
#   undef OPENSSL_SYS_UNIX
#   define OPENSSL_SYS_WIN32_CYGWIN
#  else
#   if defined(_WIN32) || defined(OPENSSL_SYSNAME_WIN32)
#    undef OPENSSL_SYS_UNIX
#    define OPENSSL_SYS_WIN32
#   endif
#   if defined(_WIN64) || defined(OPENSSL_SYSNAME_WIN64)
#    undef OPENSSL_SYS_UNIX
#    if !defined(OPENSSL_SYS_WIN64)
#     define OPENSSL_SYS_WIN64
#    endif
#   endif
#   if defined(OPENSSL_SYSNAME_WINNT)
#    undef OPENSSL_SYS_UNIX
#    define OPENSSL_SYS_WINNT
#   endif
#   if defined(OPENSSL_SYSNAME_WINCE)
#    undef OPENSSL_SYS_UNIX
#    define OPENSSL_SYS_WINCE
#   endif
#  endif
# endif

/* Anything that tries to look like Microsoft is "Windows" */
# if defined(OPENSSL_SYS_WIN32) || defined(OPENSSL_SYS_WIN64) || defined(OPENSSL_SYS_WINNT) || defined(OPENSSL_SYS_WINCE)
#  undef OPENSSL_SYS_UNIX
#  define OPENSSL_SYS_WINDOWS
#  ifndef OPENSSL_SYS_MSDOS
#   define OPENSSL_SYS_MSDOS
#  endif
# endif

/*
 * DLL settings.  This part is a bit tough, because it's up to the
 * application implementor how he or she will link the application, so it
 * requires some macro to be used.
 */
# ifdef OPENSSL_SYS_WINDOWS
#  ifndef OPENSSL_OPT_WINDLL
#   if defined(_WINDLL)         /* This is used when building OpenSSL to
                                 * indicate that DLL linkage should be used */
#    define OPENSSL_OPT_WINDLL
#   endif
#  endif
# endif

/* ------------------------------- OpenVMS -------------------------------- */
# if defined(__VMS) || defined(VMS) || defined(OPENSSL_SYSNAME_VMS)
#  undef OPENSSL_SYS_UNIX
#  define OPENSSL_SYS_VMS
#  if defined(__DECC)
#   define OPENSSL_SYS_VMS_DECC
#  elif defined(__DECCXX)
#   define OPENSSL_SYS_VMS_DECC
#   define OPENSSL_SYS_VMS_DECCXX
#  else
#   define OPENSSL_SYS_VMS_NODECC
#  endif
# endif

/* -------------------------------- OS/2 ---------------------------------- */
# if defined(__EMX__) || defined(__OS2__)
#  undef OPENSSL_SYS_UNIX
#  define OPENSSL_SYS_OS2
# endif

/* -------------------------------- Unix ---------------------------------- */
# ifdef OPENSSL_SYS_UNIX
#  if defined(linux) || defined(__linux__) || defined(OPENSSL_SYSNAME_LINUX)
#   define OPENSSL_SYS_LINUX
#  endif
#  ifdef OPENSSL_SYSNAME_MPE
#   define OPENSSL_SYS_MPE
#  endif
#  ifdef OPENSSL_SYSNAME_SNI
#   define OPENSSL_SYS_SNI
#  endif
#  ifdef OPENSSL_SYSNAME_ULTRASPARC
#   define OPENSSL_SYS_ULTRASPARC
#  endif
#  ifdef OPENSSL_SYSNAME_NEWS4
#   define OPENSSL_SYS_NEWS4
#  endif
#  ifdef OPENSSL_SYSNAME_MACOSX
#   define OPENSSL_SYS_MACOSX
#  endif
#  ifdef OPENSSL_SYSNAME_MACOSX_RHAPSODY
#   define OPENSSL_SYS_MACOSX_RHAPSODY
#   define OPENSSL_SYS_MACOSX
#  endif
#  ifdef OPENSSL_SYSNAME_SUNOS
#   define OPENSSL_SYS_SUNOS
#  endif
#  if defined(_CRAY) || defined(OPENSSL_SYSNAME_CRAY)
#   define OPENSSL_SYS_CRAY
#  endif
#  if defined(_AIX) || defined(OPENSSL_SYSNAME_AIX)
#   define OPENSSL_SYS_AIX
#  endif
# endif

/* -------------------------------- VOS ----------------------------------- */
# if defined(__VOS__) || defined(OPENSSL_SYSNAME_VOS)
#  define OPENSSL_SYS_VOS
#  ifdef __HPPA__
#   define OPENSSL_SYS_VOS_HPPA
#  endif
#  ifdef __IA32__
#   define OPENSSL_SYS_VOS_IA32
#  endif
# endif

/* ------------------------------ VxWorks --------------------------------- */
# ifdef OPENSSL_SYSNAME_VXWORKS
#  define OPENSSL_SYS_VXWORKS
# endif

/* -------------------------------- BeOS ---------------------------------- */
# if defined(__BEOS__)
#  define OPENSSL_SYS_BEOS
#  include <sys/socket.h>
#  if defined(BONE_VERSION)
#   define OPENSSL_SYS_BEOS_BONE
#  else
#   define OPENSSL_SYS_BEOS_R5
#  endif
# endif

/**
 * That's it for OS-specific stuff
 *****************************************************************************/

/* Specials for I/O an exit */
# ifdef OPENSSL_SYS_MSDOS
#  define OPENSSL_UNISTD_IO <io.h>
#  define OPENSSL_DECLARE_EXIT extern void exit(int);
# else
#  define OPENSSL_UNISTD_IO OPENSSL_UNISTD
#  define OPENSSL_DECLARE_EXIT  /* declared in unistd.h */
# endif



# if defined(OPENSSL_SYS_VMS_NODECC)
#  define OPENSSL_EXPORT globalref
#  define OPENSSL_IMPORT globalref
#  define OPENSSL_GLOBAL globaldef
# elif defined(OPENSSL_SYS_WINDOWS) && defined(OPENSSL_OPT_WINDLL)
#  define OPENSSL_EXPORT extern __declspec(dllexport)
#  define OPENSSL_IMPORT extern __declspec(dllimport)
#  define OPENSSL_GLOBAL
# else
#  define OPENSSL_EXPORT extern
#  define OPENSSL_IMPORT extern
#  define OPENSSL_GLOBAL
# endif
# define OPENSSL_EXTERN OPENSSL_IMPORT

# ifdef OPENSSL_EXPORT_VAR_AS_FUNCTION
#  define OPENSSL_IMPLEMENT_GLOBAL(type,name,value)                      \
        type *_shadow_##name(void)                                      \
        { static type _hide_##name=value; return &_hide_##name; }
#  define OPENSSL_DECLARE_GLOBAL(type,name) type *_shadow_##name(void)
#  define OPENSSL_GLOBAL_REF(name) (*(_shadow_##name()))
# else
#  define OPENSSL_IMPLEMENT_GLOBAL(type,name,value) OPENSSL_GLOBAL type _shadow_##name=value;
#  define OPENSSL_DECLARE_GLOBAL(type,name) OPENSSL_EXPORT type _shadow_##name
#  define OPENSSL_GLOBAL_REF(name) _shadow_##name
# endif

# if defined(OPENSSL_SYS_MACINTOSH_CLASSIC) && macintosh==1 && !defined(MAC_OS_GUSI_SOURCE)
#  define ossl_ssize_t long
# endif

# ifdef OPENSSL_SYS_MSDOS
#  define ossl_ssize_t long
# endif

# if defined(NeXT) || defined(OPENSSL_SYS_NEWS4) || defined(OPENSSL_SYS_SUNOS)
#  define ssize_t int
# endif

# if defined(__ultrix) && !defined(ssize_t)
#  define ossl_ssize_t int
# endif

# ifndef ossl_ssize_t
#  define ossl_ssize_t ssize_t
# endif

#ifdef  __cplusplus
}
#endif
#endif

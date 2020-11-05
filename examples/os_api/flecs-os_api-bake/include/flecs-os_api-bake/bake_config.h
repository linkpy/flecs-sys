/*
                                   )
                                  (.)
                                  .|.
                                  | |
                              _.--| |--._
                           .-';  ;`-'& ; `&.
                          \   &  ;    &   &_/
                           |"""---...---"""|
                           \ | | | | | | | /
                            `---.|.|.|.---'

 * This file is generated by bake.lang.c for your convenience. Headers of
 * dependencies will automatically show up in this file. Include bake_config.h
 * in your main project file. Do not edit! */

#ifndef FLECS_OS_API_BAKE_BAKE_CONFIG_H
#define FLECS_OS_API_BAKE_BAKE_CONFIG_H

/* Headers of public dependencies */
#include <flecs.h>
#ifdef __BAKE__
#include <bake_util.h>
#endif

/* Convenience macro for exporting symbols */
#ifndef flecs_os_api_bake_STATIC
#if flecs_os_api_bake_APIS && (defined(_MSC_VER) || defined(__MINGW32__))
  #define FLECS_OS_API_BAKE_API __declspec(dllexport)
#elif flecs_os_api_bake_APIS
  #define FLECS_OS_API_BAKE_API __attribute__((__visibility__("default")))
#elif defined _MSC_VER
  #define FLECS_OS_API_BAKE_API __declspec(dllimport)
#else
  #define FLECS_OS_API_BAKE_API
#endif
#else
  #define FLECS_OS_API_BAKE_API
#endif

#endif


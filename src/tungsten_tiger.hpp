#ifndef __TUNGSTEN_TIGER_HPP__
#define __TUNGSTEN_TIGER_HPP__

#ifdef _WIN32
  #define TungstenTiger_EXPORT __declspec(dllexport)
#else
  #define TungstenTiger_EXPORT
#endif

TungstenTiger_EXPORT void tungsten_tiger();

#endif    // !__TUNGSTEN_TIGER_HPP__

#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef SERVER_CORE_EXPORTS
#define SERVER_CORE_API __declspec(dllexport) 
#else
#define SERVER_CORE_API __declspec(dllimport) 
#endif

#endif
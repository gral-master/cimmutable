#ifndef _IMC_DEBUG_H
#define _IMC_DEBUG_H

#define LOG_FATAL    (1)
#define LOG_ERR      (2)
#define LOG_WARN     (3)
#define LOG_INFO     (4)
#define LOG_DBG      (5)

#define LOG(level, ...) do {                                    \
        if (level <= debug_level) {                             \
            fprintf(dbgstream,"%s:%d:", __FILE__, __LINE__);    \
            fprintf(dbgstream, __VA_ARGS__);                    \
            fprintf(dbgstream, "\n");                           \
            fflush(dbgstream);                                  \
        }                                                       \
    } while (0)

extern FILE* dbgstream;
extern int debug_level;

void imc_set_debug(char* filename, int level);

void imc_close_debug();

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "debug.h"

FILE* dbgstream;
int debug_level;

void imc_set_debug_settings(char* filename, int level) {

    //set default debug level
    if(level < 1 || level > 5) debug_level = 1;
    
    dbgstream = fopen(filename, "w");
    if(dbgstream == NULL){
        fprintf(stderr, "Error opening log file %s : %s\n",
               filename,
               strerror(errno));

        dbgstream = fopen("./debug.log", "w");
        
        if(dbgstream == NULL){
            fprintf(stderr, "Error opening default log file: %s\n",
                   strerror(errno));            
            // can't open log files, we choose stderr as output 
            dbgstream = stderr;
        }
    }
    
    debug_level = level;
}


void imc_close_debug() {
    int err = fflush(dbgstream);
    if(err != 0) fprintf(stderr, "Error when flushing dgbstream : %s\n",
                        strerror(errno));
    err = fclose(dbgstream);
    if(err != 0) fprintf(stderr, "Error when closing dgbstream : %s\n",
                        strerror(errno));
    
}

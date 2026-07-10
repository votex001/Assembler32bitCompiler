
#include <stdio.h>
#include "global.h"
#include "first_pass.h"
#include "utils.h"





bool fpassLine(cur_line line,long *ic,long *dc,FILE *obFile){
    int i = 0;
    skipSpaces(line.code,&i);

    if(isEmptyStr(line.code,i)){
        return TRUE;
    }
    return TRUE;
}
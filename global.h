#ifndef GLOBAL_H
#define GLOBAL_H





/*Boolean (t/f) definition */
typedef enum bool {
    FALSE = 0,
    TRUE = 1
} bool;

/*single line maximum length*/
#define MAX_LINE_LENGTH 80

/* Initial IC value */
#define IC_INIt_VAL 100

typedef struct cur_line
{
    /*to print where we has errors*/
    long num;

    /*line string*/
    char *code;
    
    char *fileName;
    
} cur_line;


typedef enum opcode {
    /*OP 0 GROUP*/
    ADD_OP = 0,
    SUB_OP = 0,
    AND_OP = 0,
    OR_OP = 0,
    NOR_OP = 0,
    
    /*OP 1 GROUP*/
    MOVE_OP = 1,
    MVHI_OP = 1,
    MVLO_OP = 1,

    /*OP 10-24 */
    ADDI_OP = 10,
    SUBI_OP = 11,
    ANDI_OP = 12,
    ORI_OP  = 13,
    NORI_OP = 14,
    BNE_OP  = 15,
    BEQ_OP  = 16,
    BLT_OP  = 17,
    BGT_OP  = 18,
    LB_OP   = 19,
    SB_OP   = 20,
    LW_OP   = 21,
    SW_OP   = 22,
    LH_OP   = 23,
    SH_OP   = 24,

    /*OP 30-32*/
    JMP_OP  = 30,
    LA_OP   = 31,
    CALL_OP = 32,

    /*OP 63*/
    HLT_OP = 63,
    /*default*/
    NONE_OP = -1



} opcode;


typedef enum funct{


    ADD_FUNCT  = 1,
    SUB_FUNCT  = 2,
    AND_FUNCT  = 3,
    OR_FUNCT   = 4,
    NOR_FUNCT  = 5,

    MOVE_FUNCT = 1,
    MVHI_FUNCT = 2,
    MVLO_FUNCT = 3,

    /*default or no need*/
    NONE_FUNCT = 0
} funct;


typedef enum directive{
    DH_DIR,
    DW_DIR,
    DB_DIR,
    ASCIZ_DIR,
    ENTRY_DIR,
    EXTERN_DIR,

    /*not a directive*/
    NONE_DIR = -1
} directive;

 

#endif
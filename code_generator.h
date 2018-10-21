typedef struct instruction tInstr;
typedef struct instructionList tIList;

typedef struct instruction
{
    int instNum;
    union add1 
    {
        int i;
        float f;
        string str;
    }adr1;

    union add2 
    {
        int i;
        float f;
        string str;
    }adr2;
    
    union add3 
    {
        int i;
        float f;
        string str;
    }adr3;
}instruction;

typedef struct instructionList 
{
    tInstr *instr;
    tInstr *next;
}instructionList;
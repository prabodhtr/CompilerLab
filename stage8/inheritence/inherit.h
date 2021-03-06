#define CONSTANT 0
#define VAR 1
#define READ_NODE 2
#define WRITE_NODE 3
#define ASSIGN_NODE 4
#define PLUS_OP 5
#define MINUS_OP 6
#define MUL_OP 7
#define DIV_OP 8
#define RELOP_LT 9
#define RELOP_LE 10
#define RELOP_GT 11
#define RELOP_GE 12
#define RELOP_EQ 13
#define RELOP_NE 14
#define IF_COND 15 
#define WHILE_LOOP 16
#define BREAK_NODE 17
#define CONTINUE_NODE 18
#define DO_WHILE 19
#define TYPE_NODE 20
#define DECL_NODE 21
#define POINTER_NODE 22
#define ADDRESS_NODE 23
#define MODULO_OP 24
#define FUN_DEF_NODE 25
#define MAIN_NODE 26
#define RETURN_NODE 27
#define FUN_CALL_NODE 28
#define FIELD 29
#define HEAPSET_NODE 30
#define ALLOC_NODE 31
#define FREE_NODE 32
#define BREAKPOINT_NODE 33
#define SELF_NODE 34
#define CLASS_FUN_CALL_NODE 35
#define CLASS_FIELD 36
#define NEW_NODE 37
#define DELETE_NODE 38
#define CLASS_DEF_LIST_NODE 39
#define CLASS_FUN_DEF_NODE 40
#define CONNECT_NODE -1
//optype
#define BOOLTYPE 0
#define INTTYPE 1
#define STRTYPE 2
#define INTPTR 3
#define STRPTR 4
#define INTADDRTYPE 5
#define STRADDRTYPE 6
#define NULLTYPE 7
#define VOIDTYPE 8

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

FILE* fin;
FILE* fout;

typedef struct Typetable{
    char *name;                 //type name
    int size;                   //size of the type
    struct Fieldlist *fields;   //pointer to the head of fields list
    struct Typetable *next;     // pointer to the next type table entry
}Typetable;

typedef struct Fieldlist{
  char *name;              //name of the field
  int fieldIndex;          //the position of the field in the field list
  struct Typetable *type;  //pointer to type table entry of the field's type
  struct Classtable *ownerclass;	//pointer to the class containing the field
  struct Classtable *Ctype;//pointer to the class type if the field is of type class
  struct Fieldlist *next;  //pointer to the next field
}Fieldlist;

typedef struct ParamList{
	char *name;
	Typetable* type;
	struct ParamList* next;
}Par_List;

typedef struct LSymbol{
	char *name;		//name of the variable
	Typetable* type;		//type of the variable:(Integer / String) 
	int binding;	//local binding of the variable
	int Arg_flag;	//Arg_flag is set for those local variables passed into fun as arguments
	struct LSymbol* next;	//points to the next Local Symbol Table entry
}LocSymTable;

typedef struct Memberfunclist {
 	char *Name;                      //name of the member function in the class
	struct Typetable *Type;          //pointer to typetable
	Par_List *paramlist;   			 //pointer to the head of the formal parameter list
	int Funcposition;                //position of the function in the class table
 	int Flabel;                      //A label for identifying the starting address of the function's code in the memory
	struct Memberfunclist *next;     //pointer to next Memberfunclist entry
}Memberfunclist;

typedef struct Classtable {
 	char *Name;                           //name of the class
	struct Fieldlist *memfieldptr;        //pointer to Fieldlist 
	struct Memberfunclist *memfunptr;      //pointer to Memberfunclist
	struct Classtable *Parentptr;         //pointer to the parent's class table
	int Class_index;                      //position of the class in the virtual function table
	int Fieldcount;                       //count of fields
  	int Methodcount;                      //count of methods
	struct Classtable *next;              //pointer to next class table entry
}Classtable;

typedef struct Gsymbol {
	char* name;	// name of the variable
	Typetable* type;	// type of the variable
	Classtable *Ctype;  //THIS FIELD IS REQUIRED ONLY FOR OEXPL
	int size;	// size of the type of the variable/array
	int col_size;//size of col for 2d arrays
	int binding;	// stores the static memory address allocated to the variable
	Par_List *paramlist;
	int flabel;
	struct Gsymbol *next;
}gst_entry;

typedef struct tnode { 
	int val;	// value of a number for NUM nodes.
	int nodetype;  // information about non-leaf nodes - read/write/connector/+/* etc.  
	char* varname;	//name of a variable for ID nodes  
	int optype;	//type of variable
	Par_List* arglist; //arglist of function
	gst_entry* gst_ptr; //pointer to GST entry 
	LocSymTable* lst_ptr;	//pointer to local symbol table
	struct tnode* field;	//pointer to data member of userdefined data type
	struct tnode *left,*mid,*right;	//left and right branches   
}tnode;



struct Gsymbol *Lookup(char * name); // Returns a pointer to the symbol table entry for the variable, returns NULL otherwise.

int Install(char *name, Typetable* type, Classtable* Ctype, int size,int col_size,Par_List* plistheader); // Creates a global symbol table entry. 

/*Create a node tnode*/
struct tnode* createTree(int val, int nodetype, char* c, int optype, struct tnode *l,struct tnode* mid, struct tnode *r,struct tnode* field);
	
/*To evaluate an expression tree*/
int evaluate(struct tnode *t);

//to get smallest free register
int GetReg();

//to release highest allocated register
int FreeReg();

//to generate header argument: no of variables to be read so as to adjust SP
void XsmHeader();

//to generate xsm code
int CodeGen(struct tnode*,FILE*);

//generated xsm code to call exit
void XsmExit();

//generate xsm code to call read
void XsmRead(int);

//generate xsm code to call write
void XsmWrite(int);


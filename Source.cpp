
#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>

using namespace std;

ifstream inData;
ofstream outDataScan;
ofstream outData;
ofstream outError;
ofstream Symbolout;
ofstream moonout;


struct Symboltable
{
	char *name;
	char *kind;
	char *type;
	Symboltable *link;
	Symboltable *next;
};

Symboltable *headglobal = NULL, *headL1 = NULL, *headL2 = NULL, *tmp = NULL;

Symboltable STnode;

Symboltable *InsertNode(Symboltable *HeadT, Symboltable item);
int SearchByName(Symboltable *HeadT, char *item);
Symboltable *SearchByscop(char  *clasnam, char *funcnam, char *item);
Symboltable *Searchfun(char  *clasnam, char *funcnam);
void printSymbolTable(Symboltable *tn);
int count_dim_arr(char *type);

char *ConCat(char *s1, char  *s2);


char *NextToken(char *str,int &,int &);  // Lexical Analyzer


int Isidnum(char ch);
int Isidch(char ch);
int Isop(char ch);
int Ispun(char ch);
int Iskey(char * str);
int Isid(char * str);
int Isconst(char * str);
int Isinteger(char * str);
int IsrelOp(char *str);
int Istype(char * str);


void fprog();
void ClassDecl();
void ProgBody();
void varDeclRepeatFunc();
void funcDefRepeat();
void fParams();
void statementRepeat();
void statement();
void Ifexp();
void Forexp();
void Getexp();
void Putexp();
void Returnexp();
void expr();
void arithExpr();
void term();
void Factor();
void varDeclRepSt();
void ArraySize();
void indiceRepeat();
void idnestRepeatIdRaFa();
void aParams();
void idnestRptIdRaIn();




char op[7] = { '+', '-', '*', '/', '=', '<', '>' };
char pun[9] = { '}', '{', ';', '(', ')', '[', ']', ',', '.'};
char *relOp[] = { "<",">","<=",">=","==","<>" };
char *fset[] = { "if","for","get","put","return" };
char *key[] = { "and","not","or","if","then","else","for","class","int", "float","get","put","return","program"};

int i = 0,err=0, path=1, scop=1, forscop=0, numdim=0,iddim=0,numparam=0,paramnum=0,line_num=0;
char *str,ch,*clasnamescop,*funcnamescop;
char *token,*prvtoken;


int main()
{
    string File_in_name;
    char ch;
	int index = -1;

	outDataScan.open("Scan.out");
	outData.open("Filepars.out");
	outError.open("Error.out");
	Symbolout.open("Symboltable.out");
	moonout.open("FileMoon.out");


	STnode.name = new char[50];
	STnode.kind = new char[10];
	STnode.type = new char[50];

	clasnamescop = new char[50];
	funcnamescop = new char[50];

	prvtoken = new char[50];

	File_in_name = "Src.in";
	cout << "\n please enter the Input file Name: ";
	cin>>File_in_name;

	inData.open(File_in_name);

	if (inData) {

		inData.get(ch);

		while (inData)
		{

			index++;
			inData.get(ch);
		}

		inData.close();

		str = new char[index];

		inData.open(File_in_name);
		index = -1;
		inData.get(ch);

		while (inData)
		{
			index++;
			str[index] = ch;
			cout << ch;
			outData << ch;
			inData.get(ch);
		}
		str[++index] = '\0';

		inData.close();

		fprog();      // call Syntactic Analyzer

		outDataScan.close();
		outData.close();
		outError.close();
		moonout.close();

		printSymbolTable(headglobal);      //print Symbol Table

		path = 2;
		i = 0;
		line_num = 1;

		outDataScan.open("Scan.out");
		outData.open("Filepars.out");
		outError.open("Error.out");
		moonout.open("FileMoon.out");


		outDataScan << "\n\nToken Name\tType\t   Address\n---------------------------------------";


		fprog();      // call Syntactic Analyzer


		outDataScan.close();
		outData.close();
		outError.close();
		Symbolout.close();
		moonout.close();

	}
	else cout << "\n\n  File not found.\n";

	ch = cin.get();
	cout <<"\n\n\n Program Lines Number: "<< line_num;

	cout << "\n\nPress any key to countinue . . . ";
	ch = cin.get();
	return 0;
}

char *NextToken(char *str,int &i,int &err)
{
	int  addr = 0,idj =0, rep =1;

	err = 0;

	char *token = new char[50];
	token[0] = '\0';

	while ((rep == 1) && (err != 2) && (str[i] != '\0'))
	{
		if (Isidch(str[i]))     //for identifiers 
		{
			rep = 0;
			addr = i;
			if (isdigit(str[i])) {
				while (Isidnum(str[i]))
				{
					token[idj++] = str[i++];
				}
				token[idj] = '\0';
			}
			else if (isalpha(str[i])) {
				while (Isidch(str[i]))
				{
					token[idj++] = str[i++];
				}
				token[idj] = '\0';
			}

			if (Iskey(token)) {
				//cout << "\n\n" << token ;
				outDataScan << "\n\n" << token << "\t     Key         \t" << addr;
				outData << "\n" << token ;
			}
			else if (Isid(token)) {
				err = -1;
				outDataScan << "\n\n" << token << "\t     Identifier  \t" << addr;
				outData << "\t"<<token<<"\t";
			}
			else if (Isconst(token)) {
				err = -2;
				outDataScan << "\n\n" << token << "\t     Constant    \t" << addr;
				outData << "\tConstant\t";
			}
			else {
				    if(path  ==2)
			    	      cout << "\n\n" << token << "\t unknown identifier \t" << addr;
					outDataScan << "\n\n" << token << "\t unknown identifier \t" << addr;
					outData << "\n\n" << token << "\t unknown identifier \t" << addr;
					outError << "\n\n" << token << "\t unknown identifier \t" << addr;
					err = 1;
				}

		}
		else
			if (Isop(str[i]))        // for operators 
			{
				addr = i;
				if ((str[i] == '/') && (str[i + 1] == '*'))
				{
					if (path == 2)
					    cout << "\n\n\tmultiple lines comment: \n\n" << str[i] << str[i + 1];
					i += 2;
					int comment_cont = 1;
					while (str[i] != '\0')
					{
						if (str[i] == '\n') line_num++;
						if (path == 2)
						     cout << str[i];
						i++;
						if ((str[i] == '/') && (str[i + 1] == '*')) comment_cont++;

						if ((str[i] == '*') && (str[i + 1] == '/'))
							if (comment_cont > 1) comment_cont--;
							else  break;
					}
					if (path == 2)
						cout << str[i] << str[i + 1] << "\n\n";
					if (str[i] == '\0') {
						if (path == 2)
							cout << "Comment Error";
						outDataScan << "Comment Error";
						outError << "Comment Error";
						err = 2;
					}
					else i += 2;
				}
				else if ((str[i] == '/') && (str[i + 1] == '/'))
				{
					if (path == 2)
						cout << "\n\n\tsingle line Comment: \n\n" << str[i] << str[i + 1];
					i += 2;
					while (str[i] != '\n')
					{
						if (path == 2)
							cout << str[i];
						i++;
						if (str[i] == '\n') line_num++;
					}
					i++;
				}
				else {
					rep = 0;
					while (Isop(str[i]))
					{
						token[idj++] = str[i++];
					}
					token[idj] = '\0';

					if (strlen(token) > 2) { err = 3; }					
					else if (strlen(token) == 2) {
						if (token[0] == '>' || token[0] == '=')
						{
							if (token[1] == '=') { err = 0; }
							else { err = 3; }
						}
						else if (token[0] == '<')
						{
							if (token[1] == '=' || token[1] == '>') { err = 0; }
							else { err = 3; }
						}
						else
						{ err = 3; }
					}
					else { err = 0; }
					if (err == 3) {
						if (path == 2)
							cout  << "\n\n" << token << "\tOperator Error\t" << addr;
						outDataScan << "\n\n" << token << "\tOperator Error\t" << addr;						
						outData << "\n\n" << token << "\tOperator Error\t" << addr;
						outError << "\n\n" << token << "\tOperator Error\t" << addr;
					}
					else if (err == 0) {
						outDataScan << "\n\n" << token << "\t      Operator    \t" << addr;
						outData << "\t" << token << "\t";
					}
				}
			}
			else if (Ispun(str[i]))     //for punctuation 
			{
				token[0] = str[i];
				token[1] = '\0';
				rep = 0;

				outDataScan << "\n\n" << str[i] << "\t      punctuation \t" << i;
				outData << "\t" << str[i];
				i++;

			}
			else if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
			{
				if (str[i] == '\n') line_num++;
				i++;
			}
			else
			{
				token[0] = str[i];
				token[1] = '\0';
				rep = 0;
				if (path == 2)
					cout << "\n\n " << str[i] << "\t unknown character\t" << i;
				outDataScan << "\n\n " << str[i] << "\t unknown character\t" << i;
				outData << "\n\n " << str[i] << "\t unknown character\t" << i;
				outError << "\n\n " << str[i] << "\t unknown character\t" << i;
				i++;
				err = 4;
			}
	}	
	return token;	
}

int Isidnum(char ch)
{
	if (isalpha(ch) || ch == '_' || isdigit(ch) || ch == '.')
		return 1;
	else
		return 0;
}

int Isidch(char ch)
{
	if (isalpha(ch) || ch == '_' || isdigit(ch))
		return 1;
	else
		return 0;
}

int Isop(char ch)
{
	int b = 0, i;
	for (i = 0; i<7 && !b; i++)
	{
		if (ch == op[i])
			b = 1;
	}
	return b;
}

int Ispun(char ch)
{
	int b = 0, i;
	for (i = 0; i<9 && !b; i++)
	{
		if (ch == pun[i])
			b = 1;
	}
	return b;
}

int Iskey(char * str)
{
	int i, b = 0;
	for (i = 0; i<14 && !b; i++)
	{
		if (!strcmp(key[i], str))
			b = 1;
	}
	return b;
}

int Isid(char * str)
{
	int i,b=0;
	if(!Iskey(token))
	if (isalpha(str[0])) {
		b = 1;
		for (i = 1; i < strlen(str); i++)
			if (!(isalpha(str[i]) || str[i] == '_' || isdigit(str[i]))) b=0;				
		return b;
	}
	else return b;
}

int Isconst(char * str)
{
	int i, b = 0, pont =0;
	if (isdigit(str[0])) {
		b = 1;
		for (i = 1; i < strlen(str); i++)
			if (!(isdigit(str[i]))) {
			       	if (str[i] == '.' && pont == 0) pont =1;
					else b=0;
			}
		return b;
	}
	else return b;
}

int Isinteger(char * str)
{
	int i, b = 1;
	for (i = 0; i < strlen(str); i++)
			if (!(isdigit(str[i]))) {
				                  b = 0;
								  break;
		          	                }
		return b;
}

int Istype(char * str)
{
	int b = 0;
	
	if (!strcmp("int", str))
			b = 1;
	if (!strcmp("float", str))
		b = 1;
	if (err == -1)
		b = 1;
	
	return b;
}

int IsFset(char * str)
{
	int i, b = 0;
	for (i = 0; i<5 && !b; i++)
	{
		if (!strcmp(fset[i], str))
			b = 1;
	}
	return b;
}

int IsrelOp(char *str)
{
	int i, b = 0;
	for (i = 0; i<6 && !b; i++)
	{
		if (!strcmp(relOp[i], str))
			b = 1;
	}
	return b;
}

void fprog()
{
	token = NextToken(str, i, err);

	if (strcmp("class", token) == 0) {
		                              ClassDecl();
									  fprog();
	                                 }
	else if (strcmp("program", token) == 0)
		                              ProgBody();
	else if (str[i] != '\0')
		  cout << "  Syntax Error   ";
}

void ClassDecl()
{
	scop = 2;
	token = NextToken(str, i, err);
	if (err == -1) {
		       outData << "\tClassName";
			   STnode.kind = "class";
			   STnode.name = token;
			   clasnamescop = token;
			   STnode.type = "";
			   if (path == 1) {
				   if (SearchByName(headglobal, STnode.name) == 0)
					   headglobal = InsertNode(headglobal, STnode);   // insert symbol table
				   else {
					   cout << "\n Line Number: " << line_num;
					   cout << "\tDuplicate definition\t class: " << token;
					   cout << "\n\nPress any key to countinue . . . ";
					   cin >> ch;
					   exit(1);
				        }
			   }

			   token = NextToken(str, i, err);

			   if (strcmp("{", token) == 0) {
				   token = NextToken(str, i, err);
				   if (Istype(token)) 
					         varDeclRepeatFunc();
				   if (strcmp("}", token) == 0) {
					   token = NextToken(str, i, err);
					   if (strcmp(";", token) == 0) {
						   outData << "\n\n  *** class Declaration Ok ***\n\n";
					   }
					   else
						   outData << "  Class Declaration ERROR   ";
				   }
				   else
					   outData << "  Class Declaration ERROR   ";
			   }
			   else 
				   outData << "  Class Declaration ERROR   ";

	                }
	else 
		outData << "  Class ERROR   ";
}

void ProgBody()
{
	STnode.kind = "function";
	STnode.name = "program";
	STnode.type = "";
	clasnamescop = "";
	funcnamescop = "program";

	if (path == 1)
		headglobal = InsertNode(headglobal, STnode);   // insert symbol table

	token = NextToken(str, i, err);

	if (strcmp("{", token) == 0) {
		token = NextToken(str, i, err);
		if (Istype(token))
			      varDeclRepSt();
		else if (IsFset(token))
			       varDeclRepSt();
		if (strcmp("}", token) == 0) {
			token = NextToken(str, i, err);
			if (strcmp(";", token) == 0) {
				outData << "\n\n  *** Program Secion Checked successfully ****\n\n";
				token = NextToken(str, i, err);
				if (token[0] != '\0') 
				{
					scop = 1;
					if (Istype(token))
					{
						STnode.type = token;
						token = NextToken(str, i, err);
						if (err == -1)
						{
							STnode.name = token;
							token = NextToken(str, i, err);
							if (token[0] == '(')
							{
								STnode.kind = "function";
								funcnamescop = STnode.name;
								if (path == 1) {
									if (SearchByName(headglobal, STnode.name) == 0)
										headglobal = InsertNode(headglobal, STnode);   // insert symbol table
									else {
										cout << "\n Line Number: " << line_num;
										cout << "\tDuplicate definition\t function: " << STnode.name;
										cout << "\n\nPress any key to countinue . . . ";
										cin >> ch;
										exit(1);
									}
								}
								funcDefRepeat();
							}
							else
								outData << "  Program Secion Error   ";
						}
						else
							outData << "  Program Secion Error   ";
					}
					else
						outData << "  Program Secion Error   ";
				}
				else  outData << "\n\n  *** Program Section Checked successfully ****\n\n";

			}
			else
				outData << "  Program Section Error   ";
		}
		else
			outData << "  Program Section Error   ";
	}
	else
		outData << "  Program Section Error   ";
}

void varDeclRepeatFunc()
{
	if (Istype(token))
	 {
		if (err == -1)
		{
			if (SearchByName(headglobal, token) == 0) {
				outData << "Type not found: " << token;
				cout << "\n Line Number: " << line_num;
				cout << "\tType not found: " << token;
				cout << "\n\nPress any key to countinue . . . ";
				cin >> ch;
				exit(1);
			}
		}
		STnode.type = token;
		token = NextToken(str, i, err);
		if (err == -1)
		{
			STnode.name = token;
			token = NextToken(str, i, err);
			if ((token[0] == '[') || (strcmp(";", token) == 0))
			{
				numdim = 0;
				if (token[0] == '[')
					ArraySize();
				STnode.kind = "variable";
				if (strcmp(";", token) == 0)
				{
					if (path == 1)
					{
						if (SearchByName(headL1->link, STnode.name) == 0)
							headL1->link = InsertNode(headL1->link, STnode);   // insert symbol table
						else {
							cout << "\n Line Number: " << line_num;
							cout << "\tDuplicate definition\t variable: " << STnode.name;
							cout << "\n\nPress any key to countinue . . . ";
							cin >> ch;
							exit(1);
						}

					}
					outData << "  *** Variable declaration OK ****\n\n";
					token = NextToken(str, i, err);
					if (Istype(token))
						varDeclRepeatFunc();
				}
				else
					outData << "  Variable declaration ERROR   ";
			}
			else if (token[0] == '(')
			{ 
				STnode.kind = "function";
				funcnamescop = STnode.name;
				if (path == 1)
				{			
						if (SearchByName(headL1->link, STnode.name) == 0)
							headL1->link = InsertNode(headL1->link, STnode);   // insert symbol table
						else {
							cout << "\n Line Number: " << line_num;
							cout << "\tDuplicate definition\t function: " << STnode.name;
							cout << "\n\nPress any key to countinue . . . ";
							cin >> ch;
							exit(1);
						}

				}
				funcDefRepeat();
			}
			else
				outData << "  Variable declaration ERROR   ";

		}
		else
			outData << "  Variable declaration Error ";
	 }
	else
		outData << "  Variable declaration  Error ";
}

void funcDefRepeat()
{
	token = NextToken(str, i, err);

	if (Istype(token))
	{
		fParams();
	}
	if (token[0] == ')')
		{
			token = NextToken(str, i, err);
			if (token[0] == '{')
			{
					token = NextToken(str, i, err);
					if (Istype(token))
						    varDeclRepSt();
					else if (IsFset(token))
						    varDeclRepSt();
					if (token[0] == '}')
					{
						token = NextToken(str, i, err);
						if (token[0] == ';')
						{
							outData << " Function Declaration ok \n\n";
							token = NextToken(str, i, err);
							if (Istype(token)) 
							{
								STnode.type = token;
								token = NextToken(str, i, err);
								if (err == -1)
								{
									STnode.name = token;
									funcnamescop = STnode.name;
									token = NextToken(str, i, err);
									if (token[0] == '(')
									{
										STnode.kind = "function";
										if (path == 1) 
										{
											if (scop == 1)
											{
												if (SearchByName(headglobal, STnode.name) == 0)
													headglobal = InsertNode(headglobal, STnode);   // insert symbol table
												else {
													cout << "\n Line Number: " << line_num;
													cout << "\tDuplicate definition\t function: " << STnode.name;
													cout << "\n\nPress any key to countinue . . . ";
													cin >> ch;
													exit(1);
												}
											}
											else {
												if (SearchByName(headL1->link, STnode.name) == 0)
													headL1->link = InsertNode(headL1->link, STnode);   // insert symbol table
												else {
													cout << "\n Line Number: " << line_num;
													cout << "\tDuplicate definition\t function: " << STnode.name;
													cout << "\n\nPress any key to countinue . . . ";
													cin >> ch;
													exit(1);
												}

											}
										}
										funcDefRepeat();
									}
								}
							}
						}
						else
							outData << "  Function Declaration ERROR   ";
					}
				    else
						outData << "  Function Declaration ERROR   ";
			}
			
			else
				outData << "  Function Declaration ERROR   ";

		}
		else
			outData << "  Function Declaration ERROR   ";

}

void varDeclRepSt()
{
	if (Istype(token)) 
	{
		STnode.type = token;
		token = NextToken(str, i, err);
		if (err == -1) 
		 {
			if ((strcmp("int", STnode.type)!=0) && (strcmp("float", STnode.type)!=0))
			if (SearchByName(headglobal, STnode.type) == 0) {
					outData << "  <----Type not found: ";
					cout << "\n Line Number: " << line_num;
					cout << "\tType not found: " << STnode.type;
					cout << "\n\nPress any key to countinue . . . ";
					cin >> ch;
					exit(1);
			}
			STnode.name = token;
			token = NextToken(str, i, err);	
			numdim = 0;
			if (token[0] == '[')
				        ArraySize();
			if (strcmp(";", token) == 0)
			{
				STnode.kind = "variable";
				if (path == 1)
				{
					if (SearchByName(headL2->link, STnode.name) == 0)
						headL2->link = InsertNode(headL2->link, STnode);   // insert symbol table
					else {
						cout << "\n Line Number: " << line_num;
						cout << "\tDuplicate definition\t variable: " << STnode.name;
						cout << "\n\nPress any key to countinue . . . ";
						cin >> ch;
						exit(1);
					}

				}
				outData << "  *** Variable declaration OK ****\n\n";
				token = NextToken(str, i, err);

				if (Istype(token))
					    varDeclRepSt();
				else if (IsFset(token))
					varDeclRepSt();
			}
			else
				outData << "  *** Variable declaration Error ****\n\n";
		}
		else if (token[0] == '=')
		{
			numdim = 0;
			if (path == 2) {
				tmp = SearchByscop(clasnamescop, funcnamescop, STnode.type);
				if (tmp == NULL) {
					outData << "  *** Id Error ****\n\n";
					cout << "\n Line Number: " << line_num;
					cout << "\t" << STnode.type << "\t unknown identifier";
				                 }
				else {
					outData << "  *** Id OK ****\n\n";
					iddim = count_dim_arr(tmp->type);
					if (iddim == numdim)
						outData << " ** Dim Match ***";
					else{
						outData << " ** Dim Error ***";
						cout << "\n Line Number: " << line_num;
						cout << "\t ** Dim Error ***";
						numdim = 0;

					}

				}
			}
			statementRepeat();
		}
		else if ((token[0] == '[') || (token[0] == '.'))
		{
			numdim = 0;
			idnestRptIdRaIn();
			if (token[0] == '=') {
				if (path == 2) {
				tmp = SearchByscop(clasnamescop, funcnamescop, STnode.type);
				if (tmp == NULL) {
					outData << "  *** Id Error ****\n\n";
					cout << "\n Line Number: " << line_num;
					cout << "\t" << STnode.type << "\t unknown identifier";
				}
				else {
					outData << "  *** Id OK ****\n\n";
					iddim = count_dim_arr(tmp->type);
					if (iddim == numdim)
						outData << " ** Dim Match ***";
					else{
						outData << " ** Dim Error ***";
						cout << "\n Line Number: " << line_num;
						cout << "\t ** Dim Error ***";
						numdim = 0;

					}

				    }
			     }
				statementRepeat();
			}
			else outData << "   ** assign statement error ***";
		}
		else
			outData << "  var ERROR   ";
	}
	else if (IsFset(token))
	{
		statementRepeat();
	}
	else 
		outData << "  Var ERROR   ";
}

void fParams()
{
	if (Istype(token))
	{
		if (err == -1)
		{
			if (SearchByName(headglobal, token) == 0) {
				cout << "\n Line Number: " << line_num;
				cout << "\tType not found: " << token;
				cout << "\n\nPress any key to countinue . . . ";
				cin >> ch;
				exit(1);
			}
		}
		STnode.type = token;
		token = NextToken(str, i, err);
		if (err == -1)
		{
			STnode.name = token;
			STnode.kind = "parameter";
			token = NextToken(str, i, err);
			numdim = 0;
			if (token[0] == '[')
				ArraySize();
			if (path == 1)
			{
					if (SearchByName(headL2->link, STnode.name) == 0)
						headL2->link = InsertNode(headL2->link, STnode);   // insert symbol table
					else {
						cout << "\n Line Number: " << line_num;
						cout << "\tDuplicate definition\t parameter: " << STnode.name;
						cout << "\n\nPress any key to countinue . . . ";
						cin >> ch;
						exit(1);
					}

			}
			outData << "  *** Parameter Declaration OK numdime = "<<numdim<<"****\n\n";
			if (strcmp(",", token) == 0)
			{
				token = NextToken(str, i, err);
				if (Istype(token))
					fParams();
				else
					outData << "  Param ERROR   ";
			}
			
		}
		else
			outData << "  Param ERROR   ";
	}
	else
		outData << "  Param ERROR   ";
}


void statementRepeat()
{
	if (strcmp("if", token) == 0)
	{
	  Ifexp();
	  token = NextToken(str, i, err);
	  if ((err == -1))
	  {
		  if (path == 2) {
			  tmp = SearchByscop(clasnamescop, funcnamescop, token);
			  if (tmp == NULL) {
				  outData << "  *** Id Error ****\n\n";
				  cout << "\n Line Number: " << line_num;
				  cout << "\t" << token << "\t unknown identifier";
			  }
			  else
				  outData << "  *** Id OK ****\n\n";
		  }
		  token = NextToken(str, i, err);
		  if ((token[0] == '[') || (token[0] == '.'))
			  idnestRptIdRaIn();
		  if ((path == 2)&&(tmp != NULL))
			  {
				  iddim = count_dim_arr(tmp->type);
				  if (iddim == numdim)
					  outData << " ** Dim Match ***";
				  else{
					  outData << " ** Dim Error ***";
					  cout << "\n Line Number: " << line_num;
					  cout << "\t ** Dim Error ***";
					  numdim = 0;

				  }

			  }
	  }
	  statementRepeat();
     }
	else if (token[0] == '=')
	{
		token = NextToken(str, i, err);
		expr();
		if (token[0] == ';')
		{
			outData << "   ***assign statement ok*****\n\n";
			token = NextToken(str, i, err);
			if ((err == -1))
			{
				if (path == 2) {
					tmp = SearchByscop(clasnamescop, funcnamescop, token);
					if (tmp == NULL){
						outData << "  *** Id Error ****\n\n";
					    cout << "\n Line Number: " << line_num;
					    cout << "\t" << token << "\t unknown identifier";
				    }				
					else
						outData << "  *** Id OK ****\n\n";
				}
				token = NextToken(str, i, err);
				if ((token[0] == '[') || (token[0] == '.'))
					idnestRptIdRaIn();
				if ((path == 2) && (tmp != NULL))
				{
					iddim = count_dim_arr(tmp->type);
					if (iddim == numdim)
						outData << " ** Dim Match ***";
					else{
						outData << " ** Dim Error ***";
						cout << "\n Line Number: " << line_num;
						cout << "\t ** Dim Error ***";
						numdim = 0;

					}

				}
			}
			statementRepeat();
		}
		else outData << "   assign statement Error " ;
	}
	else if (strcmp("for", token) == 0)
	{ 
		Forexp();
		token = NextToken(str, i, err);
		if ((err == -1))
		{
			if (path == 2) {
				tmp = SearchByscop(clasnamescop, funcnamescop, token);
				if (tmp == NULL){
					outData << "  *** Id Error ****\n\n";
			    	cout << "\n Line Number: " << line_num;
				    cout << "\t" << token << "\t unknown identifier";
			     }
				else
					outData << "  *** Id OK ****\n\n";
			}
			token = NextToken(str, i, err);
			if ((token[0] == '[') || (token[0] == '.'))
				idnestRptIdRaIn();
			if ((path == 2) && (tmp != NULL))
			{
				iddim = count_dim_arr(tmp->type);
				if (iddim == numdim)
					outData << " ** Dim Match ***";
				else{
					outData << " ** Dim Error ***";
					cout << "\n Line Number: " << line_num;
					cout << "\t ** Dim Error ***";
					numdim = 0;

				}

			}
		}
		statementRepeat();
	}
	else if (strcmp("get", token) == 0)
	{ 
		Getexp();
		token = NextToken(str, i, err);
		if ((err == -1))
		{
			if (path == 2) {
				tmp = SearchByscop(clasnamescop, funcnamescop, token);
				if (tmp == NULL){
					outData << "  *** Id Error ****\n\n";
				    cout << "\n Line Number: " << line_num;
				    cout << "\t" << token << "\t unknown identifier";
			    }
				else
					outData << "  *** Id OK ****\n\n";
			}
			token = NextToken(str, i, err);
			if ((token[0] == '[') || (token[0] == '.'))
				idnestRptIdRaIn();
			if ((path == 2) && (tmp != NULL))
			{
				iddim = count_dim_arr(tmp->type);
				if (iddim == numdim)
					outData << " ** Dim Match ***";
				else{
					outData << " ** Dim Error ***";
					cout << "\n Line Number: " << line_num;
					cout << "\t ** Dim Error ***";
					numdim = 0;

				}

			}
		}
		statementRepeat();
	}
	else if (strcmp("put", token) == 0)
	{
		Putexp();
		token = NextToken(str, i, err);
		if ((err == -1))
		{
			if (path == 2) {
				tmp = SearchByscop(clasnamescop, funcnamescop, token);
				if (tmp == NULL){
					outData << "  *** Id Error ****\n\n";
				    cout << "\n Line Number: " << line_num;
				    cout << "\t" << token << "\t unknown identifier";
			    }
				else
					outData << "  *** Id OK ****\n\n";
			}
			token = NextToken(str, i, err);
			if ((token[0] == '[') || (token[0] == '.'))
				idnestRptIdRaIn();
			if ((path == 2) && (tmp != NULL))
			{
				iddim = count_dim_arr(tmp->type);
				if (iddim == numdim)
					outData << " ** Dim Match ***";
				else{
					outData << " ** Dim Error ***";
					cout << "\n Line Number: " << line_num;
					cout << "\t ** Dim Error ***";
					numdim = 0;

				}

			}
		}
		statementRepeat();
	}
	else if (strcmp("return", token) == 0)
	{ 
		Returnexp();
		token = NextToken(str, i, err);
		if ((err == -1))
		{
			if (path == 2) {
				tmp = SearchByscop(clasnamescop, funcnamescop, token);
				if (tmp == NULL){
					outData << "  *** Id Error ****\n\n";
				    cout << "\n Line Number: " << line_num;
				    cout << "\t" << token << "\t unknown identifier";
			    }
				else
					outData << "  *** Id OK ****\n\n";
			}
			token = NextToken(str, i, err);
			if ((token[0] == '[') || (token[0] == '.'))
				idnestRptIdRaIn();
			if ((path == 2) && (tmp != NULL))
			{
				iddim = count_dim_arr(tmp->type);
				if (iddim == numdim)
					outData << " ** Dim Match ***";
				else{
					outData << " ** Dim Error ***";
					cout << "\n Line Number: " << line_num;
					cout << "\t ** Dim Error ***";
					numdim = 0;

				}

			}
		}
		statementRepeat();
	}
}

void statement()
{
	if (strcmp("if", token) == 0)
	{
		Ifexp();
	}
	else if (token[0] == '=')
	{
		token = NextToken(str, i, err);
		expr();
		if (token[0] == ';')
		{
			outData << "   *** assign statement ok ***\n\n";
		}
		else outData << "   assign statement Error ";
	}
	else if (strcmp("for", token) == 0)
	{
		Forexp();
	}
	else if (strcmp("get", token) == 0)
	{
		Getexp();	
	}
	else if (strcmp("put", token) == 0)
	{
		Putexp();
	}
	else if (strcmp("return", token) == 0)
	{
		Returnexp();
	}
}


void Ifexp()
{
	token = NextToken(str, i, err);
	if (strcmp("(", token)) outData << "   If Error    ";
	else {
		token = NextToken(str, i, err);
		expr();
		if (strcmp(")", token)) outData << "   If Error    ";
		else {
			token = NextToken(str, i, err);
			if (strcmp("then", token)) outData << "   If Error    ";
			else {
				token = NextToken(str, i, err);
				if (strcmp("{", token) == 0) 
				{
					token = NextToken(str, i, err);
					if ((err == -1))
					{
						if (path == 2) {
							tmp = SearchByscop(clasnamescop, funcnamescop, token);
							if (tmp == NULL){
								outData << "  *** Id Error ****\n\n";
							    cout << "\n Line Number: " << line_num;
							    cout << "\t" << token << "\t unknown identifier";
						    }
							else
								outData << "  *** Id OK ****\n\n";
						}
						token = NextToken(str, i, err);
						if ((token[0] == '[') || (token[0] == '.'))
							idnestRptIdRaIn();
						if ((path == 2) && (tmp != NULL))
						{
							iddim = count_dim_arr(tmp->type);
							if (iddim == numdim)
								outData << " ** Dim Match ***";
							else{
								outData << " ** Dim Error ***";
								cout << "\n Line Number: " << line_num;
								cout << "\t ** Dim Error ***";
								numdim = 0;

							}

						}
					}
					statementRepeat();
					if (strcmp("}", token)) outData << "   If Error    ";
				}
				else statement();
				token = NextToken(str, i, err);
				if (strcmp("else", token)) outData << "   If Error    ";
				else {
					token = NextToken(str, i, err);
					if (strcmp("{", token) == 0)
					{
						token = NextToken(str, i, err);
						if ((err == -1))
						{
							if (path == 2) {
								tmp = SearchByscop(clasnamescop, funcnamescop, token);
								if (tmp == NULL){
									outData << "  *** Id Error ****\n\n";
								    cout << "\n Line Number: " << line_num;
								    cout << "\t" << token << "\t unknown identifier";
							    }
								else
									outData << "  *** Id OK ****\n\n";
							}
							token = NextToken(str, i, err);
							if ((token[0] == '[') || (token[0] == '.'))
								idnestRptIdRaIn();
							if ((path == 2) && (tmp != NULL))
							{
								iddim = count_dim_arr(tmp->type);
								if (iddim == numdim)
									outData << " ** Dim Match ***";
								else{
									outData << " ** Dim Error ***";
									cout << "\n Line Number: " << line_num;
									cout << "\t ** Dim Error ***";
									numdim = 0;

								}

							}
						}
						statementRepeat();
						if (strcmp("}", token)) outData << "   If Error    ";
					}
					else statement();
					token = NextToken(str, i, err);
					if (strcmp(";", token)) outData << "   If Error    ";
					else outData << "   **  If Ok  ****\n\n";
				}			
				
			}
		}
	}

}

void Forexp()
{
	token = NextToken(str, i, err);
	if (strcmp("(", token)) outData << "   For Error    ";
	else
	{
		token = NextToken(str, i, err);
		if (Istype(token))
		{
			STnode.type = token;
			token = NextToken(str, i, err);
			if (err == -1)
			{   
				STnode.name = token;
				STnode.kind = "For var";
				if (path == 1)
				{
					if (SearchByName(headL2->link, STnode.name) == 0)
						headL2->link = InsertNode(headL2->link, STnode);   // insert symbol table
					else {
						cout << "\n Line Number: " << line_num;
						cout << "\tDuplicate definition\t variable: " << STnode.name;
						cout << "\n\nPress any key to countinue . . . ";
						cin >> ch;
						exit(1);
					}

				}
				token = NextToken(str, i, err);
				if (strcmp("=", token)) outData << "   For Error    ";
				else
				{
					token = NextToken(str, i, err);
					expr();
					if (strcmp(";", token)) outData << "   For Error    ";
					else
					{
						token = NextToken(str, i, err);
						arithExpr();
						if (IsrelOp(token))
						{
							token = NextToken(str, i, err);
							arithExpr();
							if (strcmp(";", token)) outData << "   For Error    ";
							else
							{
								token = NextToken(str, i, err);
								if (err == -1)
								{
									if (path == 2) {
										tmp = SearchByscop(clasnamescop, funcnamescop, token);
										if (tmp == NULL){
											outData << "  *** Id Error ****\n\n";
										    cout << "\n Line Number: " << line_num;
										    cout << "\t" << token << "\t unknown identifier";
									    }
										else
											outData << "  *** Id OK ****\n\n";
									}
									token = NextToken(str, i, err);
									if (strcmp("=", token)) outData << "   For Error    ";
									else
									{
										token = NextToken(str, i, err);
										expr();
										if (strcmp(")", token)) outData << "   For Error    ";
										else
										{
											token = NextToken(str, i, err);
											if (strcmp("{", token) == 0)
											{
												token = NextToken(str, i, err);
												if ((err == -1))
												{
													if (path == 2) {
														tmp = SearchByscop(clasnamescop, funcnamescop, token);
														if (tmp == NULL){
															outData << "  *** Id Error ****\n\n";
													  	    cout << "\n Line Number: " << line_num;
														    cout << "\t" << token << "\t unknown identifier";
													     }
														else
															outData << "  *** Id OK ****\n\n";
													}
													token = NextToken(str, i, err);
													if ((token[0] == '[') || (token[0] == '.'))
														idnestRptIdRaIn();
													if ((path == 2) && (tmp != NULL))
													{
														iddim = count_dim_arr(tmp->type);
														if (iddim == numdim)
															outData << " ** Dim Match ***";
														else{
															outData << " ** Dim Error ***";
															cout << "\n Line Number: " << line_num;
															cout << "\t ** Dim Error ***";
															numdim = 0;

														}

													}
												}
												statementRepeat();
												if (strcmp("}", token)) outData << "   For Error    ";
												else token = NextToken(str, i, err);

											}
											else {
												if ((err == -1))
												{
													if (path == 2) {
														tmp = SearchByscop(clasnamescop, funcnamescop, token);
														if (tmp == NULL){
															outData << "  *** Id Error ****\n\n";
														    cout << "\n Line Number: " << line_num;
														    cout << "\t" << token << "\t unknown identifier";
													     }
														else
															outData << "  *** Id OK ****\n\n";
													}
													token = NextToken(str, i, err);
													if ((token[0] == '[') || (token[0] == '.'))
														idnestRptIdRaIn();
													if ((path == 2) && (tmp != NULL))
													{
														iddim = count_dim_arr(tmp->type);
														if (iddim == numdim)
															outData << " ** Dim Match ***";
														else{
															outData << " ** Dim Error ***";
															cout << "\n Line Number: " << line_num;
															cout << "\n ** Dim Error ***";
															numdim = 0;

														}

													}
												}
												statement();	
											}
											
											if (strcmp(";", token)) outData << "   For Error    ";
											else outData << "   **  For Ok  ****\n\n";
										}

									}
								}
								else outData << "   For Error    ";
							}
						}
						else outData << "   For Error    ";
					}
				}
			}
			else outData << "   For Error";
		}
		else outData << "   For Error";
	}
 }

void Getexp()
{
	token = NextToken(str, i, err);
	if (strcmp("(", token)) outData << "   Get Error    ";
	else {
		token = NextToken(str, i, err);
		if ((err == -1))
		{
			if (path == 2) {
				tmp = SearchByscop(clasnamescop, funcnamescop, token);
				if (tmp == NULL){
					outData << "  *** Id Error ****\n\n";
				    cout << "\n Line Number: " << line_num;
				    cout << "\t" << token << "\t unknown identifier";
			     }
				else
					outData << "  *** Id OK ****\n\n";
			}
			numdim = 0;
			token = NextToken(str, i, err);
			if ((token[0] == '[')|| (token[0]=='.'))
				idnestRptIdRaIn();
			if ((path == 2) && (tmp != NULL))
			{
				iddim = count_dim_arr(tmp->type);
				if (iddim == numdim)
					outData << " ** Dim Match ***";
				else{
					outData << " ** Dim Error ***";
					cout << "\n Line Number: " << line_num;
					cout << "\t ** Dim Error ***";
					numdim = 0;

				}

			}
		}
		if (strcmp(")", token)) outData << "   Get Error    ";
		else {
			token = NextToken(str, i, err);
			if (strcmp(";", token)) cout << "   Get Error    ";
			else 			outData << " ***  Get Ok ******";
		}
	}
}

void Putexp()
{
	token = NextToken(str, i, err);
	if (strcmp("(", token)) outData << "   Put Error    ";
	else {
		token = NextToken(str, i, err);
		expr();
		if (strcmp(")", token)) outData << "   Put Error    ";
		else {
			token = NextToken(str, i, err);
			if (strcmp(";", token)) outData << "   Put Error    ";
			else 			outData << " ***  Put Ok ******";
		}
	    }
}

void Returnexp()
{
	token = NextToken(str, i, err);
	if (strcmp("(", token)) outData << "   Return Error    ";
	else {
		token = NextToken(str, i, err);
		expr();
		if (strcmp(")", token)) outData << "   Return Error    ";
		else {
			token = NextToken(str, i, err);
			if (strcmp(";", token)) outData << "   Return Error    ";
			else 			outData << " ***  Reurn Ok ******";
		}
		}
}

void expr()
{
	arithExpr();
	if (IsrelOp(token))
	{
		token = NextToken(str, i, err);
		expr();
	}
}

void arithExpr()
{
	term();
	if ((strcmp("or", token) == 0) || (token[0] == '+') || (token[0] == '-'))
	{
		token = NextToken(str, i, err);
		moonout << "\naddi  " << prvtoken<<","<<token;
		arithExpr();
	}
}

void term()
{
	Factor();
	if ((strcmp("and", token) == 0) || (token[0] == '*') || (token[0] == '/'))
	{
		token = NextToken(str, i, err);
		term();
	}
}

void Factor()
{
  Symboltable *tmp=NULL;
  if (err == -1)
  {
     prvtoken = token;
	 token = NextToken(str, i, err);
	 if ((token[0] == '(')){
		 if (path == 2) {
			 tmp = Searchfun(clasnamescop, prvtoken);
			 if (tmp == NULL){
				 outData << "  *** Function call Error ****\n\n";
			     cout << "\n Line Number: " << line_num;
			     cout << "\t" << prvtoken << "\t unknown Function";
		     }
			 else
				 outData << "  *** Function call OK ****\n\n";
		 }
		 idnestRepeatIdRaFa();
	 }
	 else {
	   	 if (path == 2) {
			 tmp = SearchByscop(clasnamescop, funcnamescop, prvtoken);
			 if (tmp == NULL){
				 outData << "  *** Unknown Identifier ****\n\n";
				 cout << "\n Line Number: " << line_num;
				 cout << "\t" << prvtoken << "\t unknown identifier";
			 }
			 else
				 outData << "  *** Factor OK ****\n\n";
		 }
		 if ((token[0] == '[') || (token[0] == '.'))
			 idnestRepeatIdRaFa();
		 if ((path == 2) && (tmp != NULL))
		 {
			 iddim = count_dim_arr(tmp->type);
			 if (iddim == numdim)
				 outData << " ** Dim Match ***";
			 else{
				 cout << "\n Line Number: " << line_num;
				 cout << "\t ** Dim Error ***";
				 outData << " ** Dim Error ***";
				 numdim = 0;

			 }

		 }
	 }
  }
  else if (err == -2)
  {
	  prvtoken = token;
	  outData << "  *** Factor OK ****\n\n";
	  token = NextToken(str, i, err);
  }
  else if ((strcmp("not", token) == 0)|| (token[0] == '+')|| (token[0]=='-')) 
  {
	  token = NextToken(str, i, err);
	  Factor();
  }
  else if (strcmp("(", token) == 0)
  {
	  token = NextToken(str, i, err);
	  arithExpr();
	  if (strcmp(")", token) == 0)
	  {
		  outData << " ** Fator ok **\n\n";
		  token = NextToken(str, i, err);
	  }
	  else outData <<  "    factor Error ";

  }
  else
	  outData << "  factor ERROR   ";
}

void ArraySize()
{
	if (token[0] == '[' ) {
		numdim++;
		STnode.type = ConCat(STnode.type,token);
		token = NextToken(str, i, err);
		if (Isinteger(token)) {
			STnode.type = ConCat(STnode.type, token);
			token = NextToken(str, i, err);
			if (strcmp("]", token) == 0) {
				STnode.type = ConCat(STnode.type, token);
				outData << " ** index OK **\n\n";
				token = NextToken(str, i, err);
				if (token[0] == '[')
					ArraySize();
			}
			else
				outData << "  Array size Error   ";
		}
		else
			outData << "  Array size Error   ";
	}
	else
		outData << "  Array size Error   ";

}

void indiceRepeat()
{
	if (token[0] == '[') {
		numdim++;
		token = NextToken(str, i, err);
		arithExpr();
		if (strcmp("]", token) == 0) {
			outData << " ** index OK **\n\n";
			token = NextToken(str, i, err);
			if (token[0] == '[')
				   indiceRepeat();
			else outData << "   num dim :"<<numdim<<"\n";

			
		}
		else
			outData << "  Array size Error   ";
	}
	else
		outData << "  Array size Error   ";

}

void idnestRepeatIdRaFa()
{
	if (token[0] == '[')
	{
		numdim = 0;
		indiceRepeat();
		if (token[0] == '.')
			idnestRepeatIdRaFa();
	}
	else if (token[0] == '.')
	{
		token = NextToken(str, i, err);
		if (err == -1) {
			outData << "  *** Id nest Ok ***  \n\n";
			token = NextToken(str, i, err);
			if (token[0] == '(')
				idnestRepeatIdRaFa();
			else {
				numdim = 0;
				if (token[0] == '[')
					indiceRepeat();
				if (token[0] == '.')
					idnestRepeatIdRaFa();
			     }
		              }
	}
	else if (token[0] == '(')
	{
		token = NextToken(str, i, err);
		if (token[0] != ')')
		{
			numparam = 0;
			aParams();
			if (token[0] != ')')
				outData << "  Function call Error \n\n  ";
			else outData << "  Function call Ok \n\n  ";
		}
		else 	outData << "  Function call Ok \n\n  ";
		if (path == 2)
		{
			if (paramnum == numparam)
				outData << "Funtion param ok\n\n";
			else{
				outData << "Funtion param mismacth\n\n";
				cout << "\n Line Number: " << line_num;
				cout << "\tFuntion param mismacth\n\n";
			}
		}
		token = NextToken(str, i, err);
	}
	else 
			outData << "  Id nest Error   ";

}

void aParams()
{
	expr();
	outData << "  *** Parameter OK ****\n\n";
	numparam++;
	if (strcmp(",", token) == 0)
	{
		token = NextToken(str, i, err);
		aParams();
	}
}

void idnestRptIdRaIn()
{
	if (token[0] == '[')
	{
		numdim = 0;
		indiceRepeat();
		if (token[0] == '.')
			idnestRptIdRaIn();
	}
	else if (token[0] == '.')
	{
		token = NextToken(str, i, err);
		if (err == -1) {
			outData << "  *** Id nest Ok ***  \n\n";
			token = NextToken(str, i, err);
			numdim = 0;
			if (token[0] == '[')
					indiceRepeat();
			if (token[0] == '.')
				   idnestRptIdRaIn();	
		              }
		else
			outData << "  Id nest Error   ";
	}
	else
		outData << "  Id nest Error   ";

}

Symboltable *InsertNode(Symboltable *HeadT, Symboltable item)
{
	Symboltable *tmp;
	if (HeadT == NULL)
	{
		HeadT = new Symboltable;
		HeadT->name = new char[50];
		HeadT->name=item.name;
		HeadT->type = new char[50];
		HeadT->type = item.type;
		HeadT->kind = new char[50];
		HeadT->kind = item.kind;
		HeadT->next = NULL;
		HeadT->link = NULL;
		if (strcmp(item.kind, "class") == 0)
		                      headL1 = HeadT;
		if (strcmp(item.kind, "function") == 0)
			                  headL2 = HeadT;
	}
	else
	{
		tmp = HeadT;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new Symboltable;
		tmp = tmp->next;
		tmp->name = new char[50];
		tmp->name = item.name;
		tmp->type = new char[50];
		tmp->type = item.type;
		tmp->kind = new char[50];
		tmp->kind = item.kind;
		tmp->next = NULL;
		tmp->link = NULL;
		if (strcmp(item.kind, "class") == 0)
		                          headL1 = tmp;
		if (strcmp(item.kind, "function") == 0)
			                      headL2 = tmp;
	}
	return HeadT;
}

int SearchByName(Symboltable *HeadT, char  *item)
{
	Symboltable *tmp;
	int found = 0;
	if (HeadT != NULL)
	{
		tmp = HeadT;
		while (tmp != NULL)
		{
			if (strcmp(tmp->name, item) == 0)
			{
				found = 1;
				break;
			}
			tmp = tmp->next;
		}

	}
	return found;
}

Symboltable *SearchByscop(char  *clasnam,char *funcnam,char *item)
{
	Symboltable *tmp, *tmp2, *tmp3;
	tmp = headglobal;
	if (tmp != NULL)
	{
		if (clasnam != "") {
			while (tmp != NULL)
			{
				if ((strcmp(tmp->kind, "class") == 0) && (strcmp(tmp->name, clasnam) == 0))
				{
					tmp2 = tmp->link;
					while (tmp2 != NULL)
					{
						if ((strcmp(tmp2->kind, "function") == 0) && (strcmp(tmp2->name, funcnam) == 0))
						{
							tmp3 = tmp2->link;
							while (tmp3 != NULL)
							{
								if (strcmp(tmp3->name, item) == 0)
									return tmp3;
								tmp3 = tmp3->next;
							}
						}
						tmp2 = tmp2->next;
					}
				
		     	}
				tmp = tmp->next;
			}
		}
		else {
			while (tmp != NULL)
			{
				if ((strcmp(tmp->kind, "function") == 0) && (strcmp(tmp->name, funcnam) == 0))
				{
					tmp2 = tmp->link;

					while (tmp2 != NULL)
					{
						if (strcmp(tmp2->name, item) == 0)
							return tmp2;
						tmp2 = tmp2->next;
					}

				}
				tmp = tmp->next;
			}
		}
	}
	return NULL;
}

Symboltable *Searchfun(char  *clasnam, char *funcnam)
{
	paramnum = 0;
	Symboltable *tmp, *tmp2, *tmp3;
	tmp = headglobal;
	if (tmp != NULL)
	{
		if (clasnam != "") {
			while (tmp != NULL)
			{
				if ((strcmp(tmp->kind, "class") == 0) && (strcmp(tmp->name, clasnam) == 0))
				{
					tmp2 = tmp->link;
					while (tmp2 != NULL)
					{
						if ((strcmp(tmp2->kind, "function") == 0) && (strcmp(tmp2->name, funcnam) == 0))
						{
							tmp3 = tmp2->link;

							while (tmp3 != NULL)
							{
								if (strcmp(tmp3->kind, "parameter") == 0) paramnum++;

								tmp3 = tmp3->next;
							}
							return tmp2;
						}
						tmp2 = tmp2->next;
					}

				}
				tmp = tmp->next;
			}
		}
		else {
			while (tmp != NULL)
			{
				if ((strcmp(tmp->kind, "function") == 0) && (strcmp(tmp->name, funcnam) == 0))
				{
					tmp2 = tmp->link;

					while (tmp2 != NULL)
					{
						if (strcmp(tmp2->kind, "parameter") == 0) paramnum++;
						
						tmp2 = tmp2->next;
					}
					return tmp;
				}
				tmp = tmp->next;
			}
		}
	}
	return NULL;
}

int count_dim_arr(char *type)
{
	int i, con = 0;
	for (i = 0; i < strlen(type); i++)
		if (type[i] == '[')
			    con ++;		
	return con;
}


char *ConCat(char *s1, char  *s2)
{
	int i,j;
	char *tmp;
	tmp = new char[50];
	for (i = 0; i < strlen(s1); i++)
		tmp[i] = s1[i];
	for (j =0; j < strlen(s2); j++)
		tmp[j+i] = s2[j];
	tmp[j + i] = '\0';
	return tmp;
}

void printSymbolTable(Symboltable *tn)
{
	Symboltable *tmp,*tmp2, *tmp3;

	tmp = tn;
	Symbolout << "\n\tSymbol table: Global\n-------------------------------------------------";
	Symbolout << "\n\tName\t\tKind\t\tType\n-------------------------------------------------";

	while (tmp != NULL)
	{
		Symbolout << "\n\t" << tmp->name;
		Symbolout << "\t\t" << tmp->kind;
		Symbolout << "\t\t" << tmp->type;

		tmp = tmp->next;
	}

	tmp = tn;
	while (tmp != NULL)
	{
		Symbolout << " \n\n\n\tSymbol table: " << tmp->kind << ": " << tmp->name;
		Symbolout << "\n--------------------------------------------------------";
		Symbolout << "\n\tName\t\tKind\t\tType\n---------------------------------------------------";

		tmp2 = tmp->link;
		while (tmp2 != NULL)
		{
			Symbolout << "\n\t" << tmp2->name;
			Symbolout << "\t\t" << tmp2->kind;
			Symbolout << "\t\t" << tmp2->type;

			tmp2 = tmp2->next;
		}

		if (strcmp(tmp->kind, "class") == 0)
		{
			tmp2 = tmp->link;
			while (tmp2 != NULL)
			{
				if (strcmp(tmp2->kind, "function") == 0)
				{
					Symbolout << " \n\n\n\tSymbol table: " << tmp->name << ": " << tmp2->name;
					Symbolout << "\n\tName\t\tKind\t\tType\n-------------------------------------------------";

					tmp3 = tmp2->link;
					while (tmp3 != NULL)
					{
						Symbolout << "\n\t" << tmp3->name;
						Symbolout << "\t\t" << tmp3->kind;
						Symbolout << "\t\t" << tmp3->type;

						tmp3 = tmp3->next;
					}
				}

				tmp2 = tmp2->next;
			}
		}
		tmp = tmp->next;
	}
}



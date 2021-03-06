#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <vector>
#include <cstdlib>
#include <io.h>
using namespace std;
typedef unsigned char uchar;
typedef vector<uchar> text;
enum dir {ENCODE, DECODE};
//typedef unsigned int uint;

void help(){
	printf("%s\n\n", "$ crypt [options] <key> <source> [<dest>]");
	printf("%s\n", "options:");
	printf("%s\n", "-a, --alphabet=<alphabet>");
	printf("%s\n", "-t, --type=<type>");
	printf("%s\n", "-h, --help");
	printf("%s\n", "<source> - input file");
	printf("%s\n", "<dest> - output file");
}

void incorrectInput(){
	printf("%s\n\n", "Arguments are incorrect!");
	help();
}

int arrSize(char arr[]) {
	int i=0;
	while (arr[i]) {i++;};
	return i;
}

//Returns new number of symbol in alphabet
//
//@param int sym - current number of symbol in alphabet
//@param int key - key of encoding
//@param int pow - nunber of symbols in alphabet
//@param bool direction - direct or reverse coding (direct when true)
int findNewSymbolPosition(int sym, int key, int pow, dir direction){
	if (direction==ENCODE) {return abs((sym+key)%pow);}
	else {return abs((sym-key+pow)%pow);};
}

//Returns number of symbol in alphabet.
//
//@param char sym - symbol
//@param char *arr - pointer to array with alphabet
//@param int size - size of alphabet
int charNum(char sym, char *arr, int size){
	for (int i=0; i<size; i++) {
		if (arr[i]==sym) {return i;};
		if (i==(size-1)) {return -1;};
	}
}

//Returns answer: Is there file?
//
//@param const char *fname - name of file
bool fileExists(const char *fname)
			{
			  return access(fname, 0) != -1;
			}

//Read characters from file wich name is "filename" to array "fileText"
//
//@param char *filename - name of input file
//@param text &fileText - link to array for input
void readFile(char *filename, text &fileText) {
FILE *f;
int *buf=new int();
f=fopen(filename,"r");
while (*buf!=EOF) {
	*buf=fgetc(f);
	fileText.push_back(*buf);
}
fclose(f);
delete(buf);
}

//encode characters in array "alphaLink" by "key" and "direction"
//
//@param text &fileText - link to array with symbols for encoding
//@param char *alphaLink - link to array with alphabet
//@param int size - nubmer of symbols in alphabet
//@param int key - key of encoding
//@param dir direction - ENCODE or DECODE
void encodeSymbols(text &fileText,char *alphaLink, int size, int key, dir direction){
long textSize=fileText.size();
int *symbol=new int();
for (long i=0;i<textSize; i++) {
	if (charNum(fileText.at(i),alphaLink,size)!=-1) {
		*symbol=charNum(fileText.at(i),alphaLink,size);

		fileText.at(i)=alphaLink[findNewSymbolPosition(*symbol, key, size, direction)];
	}
}
delete(symbol);
}

int main(int argc, char *argv[]) {

if (argc==1) {incorrectInput(); return -1;};
int alphabet=0; //Contants adress of cell with alphabet in argv[]. If default alphabet is enable then contants 0;
int key=0; //contants key of encoding;
int inFile=0; //Contants adress of cell with name of input file in argv[];
int outFile=0; //Contants adress of cell with with name of output file in argv[]. If default file is using then contants 0;
bool alphabetInput=false; //Did user input alphabet with "--aplphabet="?
dir direction=ENCODE;


for (int i=1;i<argc;i++) {

	if (argv[i][0]=='-') {
		
		if (argv[i][1]=='a')  { if (argv[i+1][0]!='-') {alphabet=i+1; i++;} else {incorrectInput(); return 0;} };

		if (argv[i][1]=='t') {if (argv[i+1][0]!='-') {if (argv[i+1][0]=='e') {direction=ENCODE;} else {if (argv[i+1][0]=='d') {direction=DECODE;} else {incorrectInput(); return 0;} } i++;}};

		if (argv[i][1]=='h') {help(); return 0;};

		if (argv[i][1]=='-') {

				if (argv[i][2]=='a') {alphabet=i; alphabetInput=true;};

				if (argv[i][2]=='t') {if (argv[i][7]=='e') {direction=ENCODE;} else {if (argv[i][7]=='d') {direction=DECODE;} else {incorrectInput(); return 0;} } };

				if (argv[i][2]=='h') {help(); return 0;}

		}

	} else {

		if (key==0) {key=atoi(argv[i]);}
		else { if (inFile==0) {inFile=i;}
				else {if (outFile==0) {outFile=i;}
						else {incorrectInput(); return 0;}
					}

			}

	} 
};

if (key<1) {printf("%s\n","Key must be positive"); incorrectInput(); return 0;};
if (!fileExists(argv[inFile])) {printf("%s\n","File not found"); return 0;} ;

//Create array for alphabet
int size;
if (alphabetInput) {size=arrSize(argv[alphabet])-11; }
else {if (alphabet!=0) {size=sizeof(argv[alphabet]);} else {size=62;} };
char alphabetArr[size];
char *alphaLink=&alphabetArr[0];

while (key>size) {key-=size;};


//Fill array by symbols
if (alphabetInput) {
	for (int i=0; i<size; i++) {
		alphabetArr[i]=argv[alphabet][i+11];
	}
}
else {
	if (alphabet!=0) {
		for (int i=0; i<size; i++) {
			alphabetArr[i]=argv[alphabet][i];
		}
	}
		else {
				for (int i=48; i<58; i++) {
					alphabetArr[i-48]=(i);
				};
				for (int i=65; i<91; i++) {
					alphabetArr[i-55]=(i);
				};
				for (int i=97; i<123; i++) {
					alphabetArr[i-61]=(i);
				};
		}
	};


//Compare to work with files
text *fileText=new text();//array with characters from file
char *options=argv[inFile];

readFile(options,*fileText);
encodeSymbols(*fileText,alphaLink,size,key,direction);


//Create output file
FILE * inF;
long textSize=fileText->size();
if (outFile!=0) {inF=fopen(argv[outFile],"w");}
else {inF=fopen("result.txt","w");};
for (long i=0;i<textSize-1;i++) {fputc(fileText->at(i),inF);}
fclose(inF);

printf("%s","Complete. Result is in ");
if (outFile!=0) {printf("%s\n",argv[outFile]);} else {printf("%s\n","result.txt");};
return 0;
}
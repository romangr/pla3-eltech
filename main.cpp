#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <vector>
#include <cstdlib>
using namespace std;
typedef unsigned char uchar;
typedef vector<uchar> text;
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

int encode(int sym, int key, int pow, bool direction){
	if (direction) {return abs((sym+key)%pow);}
	else {return abs((sym-key+pow)%pow);};
}

int charNum(char sym, char *arr, int size){
	for (int i=0; i<size; i++) {
		if (arr[i]==sym) {return i;};
		if (i==(size-1)) {return -1;};
	}
}

int main(int argc, char *argv[]) {
if (argc==1) {return -1;};
int alphabet=0;
int key=0;
int inFile=0;
int outFile=0;
bool alphabetInput=false;
bool endOfOptions=false;
bool direction=1;

for (int i=1;i<argc;i++) {

	if (argv[i][0]=='-') {
		
		if (argv[i][1]=='a')  { if (argv[i+1][0]!='-') {alphabet=i+1; i++;} else {incorrectInput(); return 0;} };

		if (argv[i][1]=='t') {if (argv[i+1][0]!='-') {if (argv[i+1][0]=='e') {direction=1;} else {if (argv[i+1][0]=='d') {direction=0;} else {incorrectInput(); return 0;} } i++;}};

		if (argv[i][1]=='h') {help(); return 0;};

		if (argv[i][1]=='-') {

				if (argv[i][2]=='a') {alphabet=i; alphabetInput=true;};

				if (argv[i][2]=='t') {if (argv[i][7]=='e') {direction=1;} else {if (argv[i][7]=='d') {direction=0;} else {incorrectInput(); return 0;} } };

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

int size;
if (alphabetInput) {size=arrSize(argv[alphabet])-11; }
else {if (alphabet!=0) {size=sizeof(argv[alphabet]);} else {size=62;} };
char alphabetArr[size];
char *alphaLink=&alphabetArr[0];

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


FILE * inF;
char cbuf;
text *fileText=new text();
int *buf=new int();
inF=fopen(argv[inFile],"r");
while (*buf!=EOF) {
	*buf=fgetc(inF);
	printf("%d\n",*buf );
	//itoa(*buf,&cbuf,10);
	fileText->push_back(*buf);
}
fclose(inF);
delete(buf);



long textSize=fileText->size();
int *symbol=new int();
for (long i=0;i<textSize; i++) {
	if (charNum(fileText->at(i),alphaLink,size)!=-1) {
		*symbol=charNum(fileText->at(i),alphaLink,size);

		fileText->at(i)=alphabetArr[encode(*symbol, key, size, direction)];
	}
}
delete(symbol);

if (outFile!=0) {inF=fopen(argv[outFile],"w");}
else {inF=fopen("result.txt","w");};
//inF=fopen("result.txt","w");
for (long i=0;i<textSize-1;i++) {fputc(fileText->at(i),inF);}

fclose(inF);

printf("%i\n", fileText->at(0));
//printf("%d\n",charNum('l',alphaLink,size) );
printf("%s\n",argv[inFile] );
printf("%d\n", key);
printf("%d\n", alphabetArr[0]);
printf("%d\n", alphabetArr[1]);
printf("%d\n", alphaLink[1]);
printf("%d\n", fileText->at(1));
return 0;
}
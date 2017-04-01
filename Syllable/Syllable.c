#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define D_OK 13
#define D_NO 14

int isKorean(char cWord);
int access(char *szInput,char* cfileName);

int main(int argc, char* argv[]){
	// argv[1] = input file name 
	// argv[2] = output file name
	// argv[3] = gram window size
	// argv[4] = counted output file name

	FILE *fpInput, *fpOutput;

	char cTemp;
	char cUpperByte,cLowerByte;


	int iGramWindowSize;
	int iCounter = 0, iNext = 0;
	int Counter = 0;
	int iter = 0,iFlag =0;
	int iCountOfWord = 0;

	char szInput[1024];
	char* szInstruction;
	char* szWordGramChunk = "";
	char szFileName[1024];

	fpInput = fopen(argv[1],"r");
	fpOutput = fopen(argv[2],"w");	
	strcpy(szFileName,argv[4]);

	iGramWindowSize = atoi(argv[3]);
	szWordGramChunk = (char *)malloc(sizeof(char) * iGramWindowSize * 2);

	printf("chunking start with %d gram\n",iGramWindowSize);
	while(1){
		strcpy(szWordGramChunk,"");
		iCounter = 0;
		iNext = 0;
		iFlag = 0;
		for(iter = 0; iter < iGramWindowSize; iter++){
			cTemp  = (char)fgetc(fpInput);
			if(cTemp == EOF) break;
			if(cTemp == '\n'){
				if(iter == 0)	break;
				iCounter += 2;
				iter -= 1;
				continue;
			}
			if(cTemp == ' '){
				if(iter == 0)	break;
				iCounter += 1;
				iter -= 1;
				continue;
			}
			if(isKorean(cTemp) == D_OK){
				cUpperByte = cTemp;
				cTemp = (char)fgetc(fpInput);
				cLowerByte = cTemp;
				strncat(szWordGramChunk,&cUpperByte,1);
				strncat(szWordGramChunk,&cLowerByte,1);
				iCounter += 2;
				if(iter == 0){		
					iNext = 2;
					if(iFlag == 1)
						iFlag += 1;
				}
			}
			else{
				cUpperByte = cTemp;
				strncat(szWordGramChunk,&cUpperByte,1);
				iCounter += 1;

				if(iter == 0)		iNext = 1;
				
			}
		}
		if(cTemp == EOF) break;
		if(!strcmp(szWordGramChunk,"")) continue;
		fprintf(fpOutput,"%s\n",szWordGramChunk);

		if(iGramWindowSize != 1)
			fseek(fpInput,-1*iCounter + iNext + iFlag,SEEK_CUR);
	}

	sprintf(szInstruction, "wordcount.exe -new -l -6 %s %s",argv[2],argv[4]);
	system(szInstruction);
	
	printf("What do you want to find : ");
	scanf("%s",szInput);
	iCountOfWord = access(szInput,szFileName);
	printf("%d",iCountOfWord);

	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}

int isKorean(char cWord){

	if((cWord & 0x80) == 0x80){
		
		return D_OK;
	}
	else
		return D_NO;

}

int access(char* szInput,char* szFileName){
	FILE *fpCountFile;
	int iCount = 0;
	int iFlag = 0;
	char szTemp[1024];

	fpCountFile = fopen(szFileName,"r");
	if(fpCountFile == NULL){
		printf("error");
		return -1;
	}


	while(1){
		if(fscanf(fpCountFile,"%d %s\n",&iCount,szTemp) != EOF){
			if(!strcmp(szInput,szTemp)){
				fclose(fpCountFile);
				return iCount;
			}
		}
		else
			break;
	}
	if(iFlag == 0) iCount = 0;
	fclose(fpCountFile);
	return iCount;
}
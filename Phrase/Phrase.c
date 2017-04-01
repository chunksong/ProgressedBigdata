#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define D_OK 13
#define D_NO 14

int access(char *szInput,char* cfileName);

int main(int argc, char* argv[]){
	// argv[1] = input file name 
	// argv[2] = output file name
	// argv[3] = gram window size
	// argv[4] = counted output file name

	FILE *fpInput, *fpOutput;

	char szTemp[1024];
	char szInput[1024];

	
	int iGramWindowSize;
	int iCounter = 0, iNext = 0;
	int Counter = 0;
	int iter = 0,iFlag =0,iTempFlag = 0;
	int iCountOfWord = 0;

	char* szInstruction;
	char szWordGramChunk[1024];
	char szFileName[1024];

	fpInput = fopen(argv[1],"r");
	fpOutput = fopen(argv[2],"w");
	strcpy(szFileName,argv[4]);
	iGramWindowSize = atoi(argv[3]);
	
	printf("chunking start with %d gram\n",iGramWindowSize);
	while(1){
		strcpy(szWordGramChunk,"");
		//printf("iNext : %d\t iCounter : %d \t",iNext,iCounter);
		iTempFlag = 0;
		iFlag = 0;
		for(iter = 0; iter < iGramWindowSize; iter++){
			if(fscanf(fpInput,"%s",szTemp) == 1){
				if(iter == 0){
					if(strlen(szTemp)== 1){
						iNext += 2;
						iTempFlag = 1; 
						break;
					}
					iNext += (strlen(szTemp) + 1);	
				}
				strcat(szWordGramChunk, szTemp);
			}
			else{
				iFlag = 1;
				break;
			}
		}

		if(iFlag == 1)	break;
		if(iTempFlag != 1){
			//printf("\[%d\]word gram : %s \n",++Counter,szWordGramChunk);
			fprintf(fpOutput,"%s\n",szWordGramChunk);
		}
		if(iGramWindowSize != 1)
			fseek(fpInput,iNext+iCounter,SEEK_SET);
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
		
		if(fscanf(fpCountFile,"%d %s",&iCount,szTemp) != EOF){
			if(!strcmp(szInput,szTemp)){
				fclose(fpCountFile);
				iFlag = 1;
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
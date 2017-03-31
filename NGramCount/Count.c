#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define D_OK 13
#define D_NO 14

int isKorean(char cWord);

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

	char* szInstruction;
	char* szWordGramChunk = "";

	fpInput = fopen(argv[1],"r");
	fpOutput = fopen(argv[2],"w");

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
			if(isKorean(cTemp) == D_OK){
				cUpperByte = cTemp;
				cTemp = (char)fgetc(fpInput);
				cLowerByte = cTemp;
				//printf("cUpperByte: %x\n",cUpperByte);
				//printf("cLowerByte: %x\n",cLowerByte);
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
				//printf("cByte: %x\n",cUpperByte);
				strncat(szWordGramChunk,&cUpperByte,1);
				iCounter += 1;

				if(iter == 0)		iNext = 1;
				
			}
		}
//		printf("iCounter : %d\niNext : %d\n",iCounter,iNext);
		if(cTemp == EOF) break;
		if(!strcmp(szWordGramChunk,"")) continue;
//		printf("\[%d\]word gram : %s \n",++Counter,szWordGramChunk);
		fprintf(fpOutput,"%s\n",szWordGramChunk);

		if(iGramWindowSize != 1)
			fseek(fpInput,-1*iCounter + iNext + iFlag,SEEK_CUR);
	}

	sprintf(szInstruction, "wordcount.exe -new -l -6 %s %s",argv[2],argv[4]);
	system(szInstruction);

	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}

int isKorean(char cWord){

	//printf("MSB : %x   ==   %x \n",(cWord & 0x80),0x80);
	if((cWord & 0x80) == 0x80){
		
		return D_OK;
	}
	else
		return D_NO;

}

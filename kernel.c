//lixian xie
//loading and executing files for project08.

int myMod(int a, int b);
int myDiv(int a, int b);
void printString(char* str);
void readString(char* str);
void readSector(char* buffer, int sector);
int readFile(char* fileName, char* buffer);
void executeProgram(char* name, int segment);
void terminate();
void handleInterrupt21(int ax, int bx, int cx, int dx);

int main(){
	char buffer[13312];
 	makeInterrupt21();   
 	interrupt(0x21,3,"messag",buffer,0);//read file into buffer
 	interrupt(0x21,0,buffer,0,0); //print out the file
 	//interrupt(0x21,4,"tstprg",0x2000,0);
 	//interrupt(0x21,4,"tstpr2",0x2000,0);
 	interrupt(0x21,4,"shell",0x2000,0);//load and execute shell
	while(1){
		//todo
	}
	return 0;
}

int myMod(int a, int b){
	while(a>b)a=a-b;
	return a;
}

int myDiv(int a, int b){
	int q;
	int i;
	for(q=0;q*b<a;q++);
	return q-1;
}

void printString(char* str){
	int i;
	for(i=0;str[i]!=0;i++){
		interrupt(0x10,0xE*256+str[i],0,0,0);			
	}	
}

void readString(char* str){
	int i=0;
	char tempChar;
	tempChar=interrupt(0x16,0,0,0,0);
	while(tempChar!=0xd){
		interrupt(0x10,0xE*256+tempChar,0,0,0);
		if(tempChar!=0x8){
			str[i]=tempChar;		
			tempChar=interrupt(0x16,0,0,0,0);
			i++;
		}else tempChar=interrupt(0x16,0,0,0,0);
	}
	str[i]=0xa;	   //jump to next line
	str[i+1]=0xd;  //return to head of current line
	str[i+2]=0x0;  //end of string
	interrupt(0x10,0xE*256+0xa,0,0,0);
	interrupt(0x10,0xE*256+0xd,0,0,0);
}

void readSector(char* buffer, int sector){
	int relativeSector;
	int head;
	int track;
	relativeSector=myMod(sector,18)+1;
	head=myMod(myDiv(sector,18),2);
	track=myDiv(sector,36);
	interrupt(0x13,2*256+1,buffer,track*256+relativeSector,head*256);
}

int readFile(char* fileName, char* buffer){
	int i,j,k;
	char temp[512];
	readSector(temp,2);//load dir sector to temp	
	for(i=0;i<16;i++){  //maximum 16 file entries in dir sector
		for(j=0;j<6;j++){
			if(temp[j+32*i]!=fileName[j])break;
		}
		if(j==6)break;
	}
	if(j==6){  //means found matched fileName
		while(j<32){  //load sector by sector
			if(temp[j+32*i]!=0x00)readSector(buffer+512*(j-6),temp[j+32*i]);
			j++;
		}
	}
	if(i==16){
		printString("File not found!\r\n");
		return 0;
	}else return 1;
}

void executeProgram(char* name, int segment){
	char temp[13312];
	int i;
	printString("launching program...\r\n");
	if(readFile(name,temp)==1){//means that the program file exists
		for(i=0;i<13312;i++){  //put temp into memory
			putInMemory(segment,i,temp[i]);
		}		
		launchProgram(segment);
	}	
}

void terminate(){
	interrupt(0x21,4,"shell",0x2000,0);	
	//interrupt(0x21,0,"I will be back!\r\n",0,0);
	//while(1);
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	if(ax==0){
		printString(bx);
	}else if(ax==1){
		readString(bx);
	}else if(ax==2){
		readSector(bx,cx);
	}else if(ax==3){
		readFile(bx,cx);
	}else if(ax==4){		
		executeProgram(bx,cx);
	}else if(ax==5){
		terminate();
	}else{
		printString("error:invalid int!\r\n");
	}
}




























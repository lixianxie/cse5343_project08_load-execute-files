//lixian xie
//write some simple system calls.

int myMod(int a, int b);
int myDiv(int a, int b);
void printString(char* str);
void readString(char* str);
void readSector(char* buffer, int sector);
void handleInterrupt21(int ax, int bx, int cx, int dx);

int main(){
	char line[80];
	char buffer[512];
	printString("Enter a line: ");//this part is for step1
	readString(line);  //this part is for step2
 	printString(line);	
 	readSector(buffer,30); //this part is for step3
 	printString(buffer);
 	makeInterrupt21();   //this part is for step4
 	//interrupt(0x21,0,0,0,0);
 	interrupt(0x21,1,line,0,0);//this part is for step5
 	interrupt(0x21,0,line,0,0);
	//interrupt(0x21,3,line,0,0);
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

void handleInterrupt21(int ax, int bx, int cx, int dx){
	//printString("hello world\r\n"); //testing in step4
	if(ax==0){
		printString(bx);
	}else if(ax==1){
		readString(bx);
	}else if(ax==2){
		readSector(bx,cx);
	}else{
		printString("error:invalid int!\r\n");
	}
}


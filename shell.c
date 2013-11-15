//lixianxie
//make shell for project08

int main(){
	char cmdLine[128];
	while(1){
		interrupt(0x21,0,"shell> ",0,0);
		interrupt(0x21,1,cmdLine,0,0);
		interrupt(0x21,0,"shell> Bad command!\r\n",0,0);
	}
	return 0;
}

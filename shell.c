//lixianxie
//make shell for project08

int main(){
	int i;
	char cmd[128];
	char tst[128];
	char buf[13312];
	while(1){
		interrupt(0x21,0,"shell> ",0,0);
		interrupt(0x21,1,cmd,0,0);
		if(cmd[0]=='t'&&cmd[1]=='y'&&cmd[2]=='p'&&cmd[3]=='e'&&cmd[4]==' '){
			for(i=5;cmd[i]!=0;i++){
					tst[i-5]=cmd[i];
			}
			//interrupt(0x21,0,"shell> ",0,0);
			interrupt(0x21,3,tst,buf,0);//read file tst to buf
			interrupt(0x21,0,buf,0,0);//print out buf
			//interrupt(0x21,5,0,0,0);//invoke terminate(),that is shell again
		}else if(cmd[0]=='e'&&cmd[1]=='x'&&cmd[2]=='e'&&cmd[3]=='c'&&cmd[4]=='u'&&cmd[5]=='t'&&cmd[6]=='e'&&cmd[7]==' '){
			for(i=8;cmd[i]!=0;i++){
					tst[i-8]=cmd[i];
			}
			//interrupt(0x21,0,"shell> ",0,0);
			interrupt(0x21,4,tst,0x2000,0);//execute tst
		}else{
			//interrupt(0x21,0,"shell> ",0,0);
			interrupt(0x21,0,"Bad command!\r\n",0,0);
		}		
	}
	return 0;
}
//the shell command here is not as robust as we built in project05,
//we just learn how to invoke interrupt0x21, so no need to write it too complicated here.
//you can see my shell.c in project05 if you like robust shell.
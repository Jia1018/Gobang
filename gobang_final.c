#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define SIZE 15//棋盘大小
#define CHARSIZE 2//中文字符大小
#define LEN 100
#define DIRECTION 8//一个棋子周围的八个方向
#define NO_FORBIDDEN 0//无禁手
#define THREE_THREE_FORBIDDEN 1//三三禁手
#define FOUR_FOUR_FORBIDDEN 2//四四禁手
#define LONG_FORBIDDEN 3//长连禁手

//-------------------------------------以下是函数声明-----------------------------------------------
void initRecordBoard(void);
void recordtoDisplayArray(void);
void displayBoard(void);
void RecordScore(int Score1[SIZE][SIZE],int Score2[SIZE][SIZE]);
struct best find(int Score1[SIZE][SIZE],int Score2[SIZE][SIZE]);
void player1(void);
void player2(void);
void machine1(void);
void machine2(void);
int forbid(void);
void RecordAround1(int,int);
void RecordAround2(int,int);
struct point count(void);
int win(void);
//---------------------------------------------------------------------------------------------------



//此数组用于记录棋盘格局 
int aRecordBoard[SIZE][SIZE];
//设置模式串“quit”
char t[]="quit";
//此数组记录输入字符
char s[LEN];
//此函数将玩家输入的落子坐标或其他相关信息（字符串）存入数组s
void getinput(char s[]){
		int i,c;
		i=0;
		while((c=getchar())!=EOF&&c!='\n')
		s[i++]=c;
		if(c=='\n')
		s[i++]='\n';
		s[i]='\0';
}
//此函数将数组s中记录的行信息转换成整数形式
int gety(char s[]){
		int j;
		int y=0;
		for(j=0;s[j]!='\n';j++){
		  if(isdigit(s[j]))
		    y=10*y+s[j]-'0';
		}
		return y;
}
//此函数将数组s中记录的列信息转换成整数形式
int getx(char s[]){
		int j,k=0;
		char x;
		for(j=0;s[j]!='\n';j++){
		    if((s[j]>='A'&&s[j]<='O')){
		      x=s[j]-'A';
		      k++;
		    }
		    if((s[j]>='a'&&s[j]<='o')){
		      x=s[j]-'a';
		      k++;
		    }
		}
		if(k!=1){
		   printf("error!\n");
		   return 0;
		}
		else
		return x;
}
//此函数判断玩家输入文本中是否存在与模式串“quit”匹配的字符串
int strindex(char s[],char t[]){
		int i,j,k;
		for(i=0;s[i]!='\0';i++){
			for(j=i,k=0;t[k]!='\0'&&s[j]==t[k];j++,k++);
				if(k>0&&t[k]=='\0')
				return 1;
		}
		return 0;
}

//定义一个结构，记录目标落子点的位置信息
struct position{
		int x;
		int y;
};
//定义一个结构，记录最佳落子点的分值及位置信息
struct best{
		struct position pos;
		int score1;
		int score2;
};
//定义一个结构，用来存分值最高且相同的点的位置信息
struct chess_queue
{
		struct position chess[SIZE*SIZE];//各个点的坐标信息存入数组chess中
		int len;//记录数组的有效长度（分值最高且相同的点的个数）
};
//此函数通过评估当前落子后的双方棋盘分值找出最佳落子点
struct best find(int Score1[SIZE][SIZE],int Score2[SIZE][SIZE]){//Score1为我方棋盘分数矩阵，Score2为对方棋盘分数矩阵
		struct best bst={{0,0},0,0};//定义一个best类型的结构bst，并初始化
		struct position optimal={0,0};//定义一个position类型的结构optimal，并初始化
		struct chess_queue queue;//定义一个chess_queue类型的结构queue
		//通过循环将queue中的数组queue.chess初始化为0
		for(int k=0;k<SIZE*SIZE;k++){
			queue.chess[k].x=0;
			queue.chess[k].y=0;
		}
		int i,j,l=0;
		//双重循环，比较双方的棋盘分值，找到我方和对方分值最高的点
		for(i=0;i<SIZE;i++){
			for(j=0;j<SIZE;j++){
			  	if(aRecordBoard[i][j]==0){
					if(Score1[i][j]>bst.score1){//遇到分值更高的点，数组queue.chess清零并重新赋值
						bst.score1=Score1[i][j];//将我方在棋盘中分值最高点记录在bst.score1
						for(int k=0;k<SIZE*SIZE;k++){
							queue.chess[k].x=0;
							queue.chess[k].y=0;
						}
						queue.chess[0].x=i;
						queue.chess[0].y=j;
						queue.len=1;
						l=0;
					}
					if(Score1[i][j]==bst.score1){//我方若有多个分值最高点则存入数组queue.chess
						queue.chess[l+1].x=i;
						queue.chess[l+1].y=j;
						queue.len=l+2;
						l++;//最佳点个数+1
					}
					if(Score2[i][j]>=bst.score2){//将对方分值最高的点的坐标信息记录在optimal中，并将该最高的分值记录在bst.score2中
						bst.score2=Score2[i][j];
						optimal.x=i;
						optimal.y=j;
					}
			  	}
			}			
		}
		//若我方最高分值高于对方最高分值
		if(bst.score1>=bst.score2&&queue.len==1){
				bst.pos.x=queue.chess[0].x;
				bst.pos.y=queue.chess[0].y;	
		}
		//我方最高分值点有多个时，选择其中对方分值最高的点
		if(bst.score1>=bst.score2&&queue.len>1){
			for(int n=0;n<queue.len;n++){
				if(Score2[queue.chess[n].x][queue.chess[n].y]>=Score2[queue.chess[0].x][queue.chess[0].y]){
					bst.pos.x=queue.chess[n].x;
					bst.pos.y=queue.chess[n].y;	
				}
			}
		}
		//若我方最高分值低于对方最高分值，选择对方分值最高的点		
		if(bst.score1<bst.score2){
				bst.pos.x=optimal.x;
				bst.pos.y=optimal.y;	
		}	
		return bst;
}

//落子坐标的关联量
int line1,line2;//行
int column1,column2;//列
int Score1[SIZE][SIZE];//记录黑子的棋盘形势分数
int Score2[SIZE][SIZE];//记录白子的棋盘形势分数

//此函数进行机器先手的落子操作
void machine1(void){
		//获得当前棋盘局势下黑子和白子的分数
		RecordScore(Score1,Score2);
		//调用find函数获得黑子的最佳落子点
		struct best bst1=find(Score1,Score2);
		//落子并显示落子后的棋盘
		line1=15-bst1.pos.x;
		column1=bst1.pos.y;
		aRecordBoard[bst1.pos.x][bst1.pos.y]=3;
		recordtoDisplayArray();
    	displayBoard();
		//打印坐标
		printf("The position is (%d,%c)\n",15-bst1.pos.x,'A'+bst1.pos.y);
		aRecordBoard[15-line1][column1]=1;
		//记录落子点周围的棋盘局势
    	RecordAround1(15-line1,column1);
}

//此函数进行机器后手的落子操作
void machine2(void){
		//获得当前棋盘局势下黑子和白子的分数
		RecordScore(Score1,Score2);
		//调用find函数获得白子的最佳落子点
		struct best bst2=find(Score2,Score1);
		//落子并显示落子后的棋盘
		line2=15-bst2.pos.x;
		column2=bst2.pos.y;
		aRecordBoard[bst2.pos.x][bst2.pos.y]=4;
		recordtoDisplayArray();
    	displayBoard();
		//打印坐标
		printf("The position is (%d,%c)\n",15-bst2.pos.x,'A'+bst2.pos.y);
		aRecordBoard[15-line2][column2]=2;
		//记录落子点周围的棋盘局势
    	RecordAround2(15-line2,column2);
}

//此函数进行先手玩家的落子操作
void player1(void){
    	printf("Player1 please place a piece:");
		//获得玩家输入的落子位置
    	void getinput(char s[]);
    	getinput(s);
    	int getx(char s[]);
    	int gety(char s[]);
    	line1=gety(s);
    	column1=getx(s);
		//不合法输入报错处理
    	if(line1>15||line1==0||column1>14){
			//判断玩家是否输入了“quit”
	   		if(strindex(s,t)>0)
				return;
			//要求玩家进行合法输入并重新调用落子函数
       		printf("error!Please input a valid position.\n");
        	player1();
			return ;
		}
    	return;
}

//此函数进行后手玩家的落子操作
void player2(void){
		printf("Player2 please place a piece:");
		//获得玩家输入的落子位置
    	void getinput(char s[]);
    	getinput(s);
    	int getx(char s[]);
    	int gety(char s[]);
    	line2=gety(s);
    	column2=getx(s);
		//不合法输入报错处理
    	if(line2>15||line2==0||column2>14){
			//判断玩家是否输入了“quit”
	    	if(strindex(s,t)>0)
				return;
			//要求玩家进行合法输入并重新调用落子函数
       		printf("error!Please input a valid position.\n");
       		player2();
        	return;
    	}
    	return;
}

//棋盘使用的是GBK编码，每一个中文字符占用2个字节。

//棋盘基本模板 
char aInitDisplayBoardArray[SIZE][SIZE*CHARSIZE+1] = 
{
		"┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"
};

//代表下棋轮数的变量
int r;
//主函数
int main(){
	//棋盘初始化
    initRecordBoard();
	recordtoDisplayArray();
	displayBoard();
	int mode,order;
	printf("Work of Zhou Ziqi\nPlease choose a mode: 1 man VS man; 2 man VS machine\n");
	scanf("%d",&mode);
	getchar();
	//人机对战模式
	if(mode==2){
			printf("Please choose your order: 1 first player; 2 second player\n");
			scanf("%d",&order);
			getchar();
			//玩家先手，机器后手
			if(order==1){
					  //双方落子轮数不得超过113（下满棋盘），否则直接退出程序
					  for(r=0;r<113;r++){
								//玩家落子
								player1();
						  		if(strindex(s,t)>0)
										return 0;
								//判断是否为不合法落子（落在已落子的位置）
						  		if(aRecordBoard[15-line1][column1]==0)
										aRecordBoard[15-line1][column1]=3;
						 		else{
										printf("OCCUPIED! Please choose another place.\n");
										player1();
						  		}
						  		recordtoDisplayArray();
						  		displayBoard();
								aRecordBoard[15-line1][column1]=1;
						  		RecordAround1(15-line1,column1);
								//判断落子点是否构成禁手，若是，判黑子输
						  		if(forbid()>0){
							  			printf("FORBIDDEN PLACEMENT! You lose, game over!\n");
							  			return 0;
								}
								//判断落子后是否成五连珠，若是，则判赢
								if(win()==1){
							  			printf("You win!\n");
							  			return 0;
					   			}
								//机器落子
								machine2();
								//判断落子后是否成五连珠，若是，则判赢
								if(win()==1){
							  			printf("Machine wins!\n");
							  			return 0;
								}
					  }
			}
			//玩家后手，机器先手
			if(order==2){
					 //机器先手首次落子直接落在棋盘中心位置
					 aRecordBoard[7][7]=3;
					 recordtoDisplayArray();
				  	 displayBoard();
					 aRecordBoard[7][7]=1;
					 for(r=0;r<113;r++){
								//玩家落子
								player2();
						  		if(strindex(s,t)>0)
										return 0;
								//判断是否为不合法落子（落在已落子的位置）
						  		if(aRecordBoard[15-line2][column2]==0)
							   	    	aRecordBoard[15-line2][column2]=4;
						  		else{
										printf("OCCUPIED! Please choose another place.\n");
										player2();
						  		}
						  		recordtoDisplayArray();
						  		displayBoard();
								aRecordBoard[15-line2][column2]=2;
						  		RecordAround2(15-line2,column2);
								//判断落子后是否成五连珠，若是，则判赢
								if(win()==1){
							  			printf("You win!\n");
							  			return 0;
								}
								//机器落子
								machine1();
								//判断落子点是否构成禁手，若是，判黑子输
								if(forbid()>0){
							  			printf("FORBIDDEN PLACEMENT! Machine loses, game over!\n");
							  			return 0;
								}
								//判断落子后是否成五连珠，若是，则判赢
								if(win()==1){
							  			printf("Machine wins!\n");
							  			return 0;
					   			}
					  }
			}
    }
	//人人对战模式
	if(mode==1){
			for(r=0;r<=112;r++){
					  //先手玩家落子
					  player1();
					  if(strindex(s,t)>0)
								return 0;
					  //判断是否为不合法落子（落在已落子的位置）
					  if(aRecordBoard[15-line1][column1]==0)
								aRecordBoard[15-line1][column1]=3;
					  else{
								printf("OCCUPIED! Please choose another place.\n");
								player1();
					  }
					  recordtoDisplayArray();
					  displayBoard();
					  aRecordBoard[15-line1][column1]=1;
					  RecordAround1(15-line1,column1);
					  //判断落子点是否构成禁手，若是，判黑子输
					  if(forbid()>0){
							  	printf("FORBIDDEN PLACEMENT! Player1 loses, game over!\n");
							  	return 0;
					  }
					  //判断落子后是否成五连珠，若是，则判赢
					  if(win()==1){
								printf("Player1 is the winner!\n");
								return 0;
					  }
					  //后手玩家落子
					  player2();
					  if(strindex(s,t)>0)
								return 0;
					  //判断是否为不合法落子（落在已落子的位置）
					  if(aRecordBoard[15-line2][column2]==0)
								aRecordBoard[15-line2][column2]=4;
					  else{
								printf("OCCUPIED! Please choose another place.\n");
								player2();
					  }
					  recordtoDisplayArray();
					  displayBoard();
					  aRecordBoard[15-line2][column2]=2;
					  RecordAround2(15-line2,column2);
					  //判断落子后是否成五连珠，若是，则判赢
					  if(win()==1){
								printf("Player2 is the winner!\n");
								return 0;
					  }
			}
	}
    return 0;
}


//此数组用于显示棋盘 
char aDisplayBoardArray[SIZE][SIZE*CHARSIZE+1];
 
char play1Pic[]="●";//黑棋子;
char play1CurrentPic[]="▲"; 

char play2Pic[]="◎";//白棋子;
char play2CurrentPic[]="△";


//初始化棋盘格局 
void initRecordBoard(void){
int i,j;
//通过双重循环，将aRecordBoard清0
for(i=0;i<SIZE;i++){
      for(j=0;j<SIZE;j++)
          aRecordBoard[i][j]=0;
   }
}

//将aRecordBoard中记录的棋子位置，转化到aDisplayBoardArray中
void recordtoDisplayArray(void){
int i,j;
//第一步：将aInitDisplayBoardArray中记录的空棋盘，复制到aDisplayBoardArray中
   for(i=0;i<SIZE;i++){
      for(j=0;j<SIZE*2+1;j++)
          aDisplayBoardArray[i][j]=aInitDisplayBoardArray[i][j];
   }
//第二步：扫描aRecordBoard，当遇到非0的元素，将●或者◎复制到aDisplayBoardArray的相应位置上
//注意：aDisplayBoardArray所记录的字符是中文字符，每个字符占2个字节。●和◎也是中文字符，每个也占2个字节。
   for(i=0;i<SIZE;i++){
	for(j=0;j<SIZE;j++)
	    if(aRecordBoard[i][j]==1){
		   aDisplayBoardArray[i][2*j]=play1Pic[0];
		   aDisplayBoardArray[i][2*j+1]=play1Pic[1];
	    }else if(aRecordBoard[i][j]==2){
		   aDisplayBoardArray[i][2*j]=play2Pic[0];
		   aDisplayBoardArray[i][2*j+1]=play2Pic[1];
	    } 
            else if(aRecordBoard[i][j]==3){
		   aDisplayBoardArray[i][2*j]=play1CurrentPic[0];
		   aDisplayBoardArray[i][2*j+1]=play1CurrentPic[1];
	    } 
            else if(aRecordBoard[i][j]==4){
		   aDisplayBoardArray[i][2*j]=play2CurrentPic[0];
		   aDisplayBoardArray[i][2*j+1]=play2CurrentPic[1];
	    } 
   }             
}


//显示棋盘格局 
void displayBoard(void){
	int i;
	//第一步：清屏
	system("clear");   //清屏  
	//第二步：将aDisplayBoardArray输出到屏幕上
	for(i=0;i<SIZE;i++){
        if(i>=0&&i<=5)
           printf("%d%s\n",SIZE-i,aDisplayBoardArray[i]);
        if(i>=6&&i<=15)
           printf(" %d%s\n",SIZE-i,aDisplayBoardArray[i]);
	}
	//第三步：输出最下面的一行字母A B .... 
           printf("   A B C D E F G H I J K L M N O\n");
}


//此数组记录与当前点各个方向上紧挨的同色连子数
int a[DIRECTION];
//此数组记录与当前点各个方向上紧挨的同色棋子外的空格数之后的同色连子数
int b[DIRECTION];
//此数组记录与当前点各个方向上紧挨的同色棋子外的空格数之后的同色连子数外的空格数之后的同色连子数
int c[DIRECTION];
//此数组记录与当前点各个方向上紧挨的同色棋子外的空格数
int space1[DIRECTION];
//此数组记录与当前点各个方向上紧挨的同色棋子外的空格数之后的同色连子数外的空格数
int space2[DIRECTION];
//isFour,isThree记录棋子某个方向的成子状态,并初始化
int isFour=0,isThree=0;
//此函数记录黑子周围的同色子或空格（棋盘局势）
void RecordAround1(int i,int j){
	int n;
    for(a[0]=0;aRecordBoard[i][j+1]==1&&j+1<=SIZE-1;a[0]++,j++);
	for(space1[0]=0;aRecordBoard[i][j+1]==0&&j+1<=SIZE-1;space1[0]++,j++);
    for(b[0]=0;aRecordBoard[i][j+1]==1&&j+1<=SIZE-1;b[0]++,j++);
	for(space2[0]=0;aRecordBoard[i][j+1]==0&&j+1<=SIZE-1;space2[0]++,j++);
    for(c[0]=0;aRecordBoard[i][j+1]==1&&j+1<=SIZE-1;c[0]++,j++);
	//用循环记录右边一线的棋局
			j=j-a[0]-space1[0]-b[0]-space2[0]-c[0];//还原坐标
	for(a[1]=0;aRecordBoard[i][j-1]==1&&j-1>=0;a[1]++,j--);
	for(space1[1]=0;aRecordBoard[i][j-1]==0&&j-1>=0;space1[1]++,j--);
    for(b[1]=0;aRecordBoard[i][j-1]==1&&j-1>=0;b[1]++,j--);
	for(space2[1]=0;aRecordBoard[i][j-1]==0&&j-1>=0;space2[1]++,j--);
    for(c[1]=0;aRecordBoard[i][j-1]==1&&j-1>=0;c[1]++,j--);
	//用循环记录左边一线的棋局
			j=j+a[1]+space1[1]+b[1]+space2[1]+c[1];//还原坐标
    for(a[2]=0;aRecordBoard[i+1][j]==1&&i+1<=SIZE-1;a[2]++,i++);
	for(space1[2]=0;aRecordBoard[i+1][j]==0&&i+1<=SIZE-1;space1[2]++,i++);
    for(b[2]=0;aRecordBoard[i+1][j]==1&&i+1<=SIZE-1;b[2]++,i++);
	for(space2[2]=0;aRecordBoard[i+1][j]==0&&i+1<=SIZE-1;space2[1]++,i++);
    for(c[2]=0;aRecordBoard[i+1][j]==1&&i+1<=SIZE-1;c[2]++,i++);
	//用循环记录下边一线的棋局
			i=i-a[2]-space1[2]-b[2]-space2[2]-c[2];//还原坐标
	for(a[3]=0;aRecordBoard[i-1][j]==1&&i-1>=0;a[3]++,i--);
	for(space1[3]=0;aRecordBoard[i-1][j]==0&&i-1>=0;space1[3]++,i--);
    for(b[3]=0;aRecordBoard[i-1][j]==1&&i-1>=0;b[3]++,i--);
	for(space2[3]=0;aRecordBoard[i-1][j]==0&&i-1>=0;space2[3]++,i--);
    for(c[3]=0;aRecordBoard[i-1][j]==1&&i-1>=0;c[3]++,i--);
	//用循环记录上边一线的棋局
			i=i+a[3]+space1[3]+b[3]+space2[3]+c[3];//还原坐标
	for(a[4]=0;aRecordBoard[i+1][j+1]==1&&i+1<=SIZE-1&&j+1<=SIZE-1;a[4]++,i++,j++);
	for(space1[4]=0;aRecordBoard[i+1][j+1]==0&&i+1<=SIZE-1&&j+1<=SIZE-1;space1[4]++,i++,j++);
    for(b[4]=0;aRecordBoard[i+1][j+1]==1&&i+1<=SIZE-1&&j+1<=SIZE-1;b[4]++,i++,j++);
	for(space2[4]=0;aRecordBoard[i+1][j+1]==0&&i+1<=SIZE-1&&j+1<=SIZE-1;space2[4]++,i++,j++);
    for(c[4]=0;aRecordBoard[i+1][j+1]==1&&i+1<=SIZE-1&&j+1<=SIZE-1;c[4]++,i++,j++);
    //用循环记录右下一线的棋局
			i=i-a[4]-space1[4]-b[4]-space2[4]-c[4];
			j=j-a[4]-space1[4]-b[4]-space2[4]-c[4];//还原坐标
	for(a[5]=0;aRecordBoard[i-1][j-1]==1&&i-1>=0&&j-1>=0;a[5]++,i--,j--);
	for(space1[5]=0;aRecordBoard[i-1][j-1]==0&&i-1>=0&&j-1>=0;space1[5]++,i--,j--);
    for(b[5]=0;aRecordBoard[i-1][j-1]==1&&i-1>=0&&j-1>=0;b[5]++,i--,j--);
	for(space2[5]=0;aRecordBoard[i-1][j-1]==0&&i-1>=0&&j-1>=0;space2[5]++,i--,j--);
    for(c[5]=0;aRecordBoard[i-1][j-1]==1&&i-1>=0&&j-1>=0;c[5]++,i--,j--);
	//用循环记录左上一线的棋局
			i=i+a[5]+space1[5]+b[5]+space2[5]+c[5];
			j=j+a[5]+space1[5]+b[5]+space2[5]+c[5];//还原坐标
	for(a[6]=0;aRecordBoard[i+1][j-1]==1&&i+1<=SIZE-1&&j-1>=0;a[6]++,i++,j--);
	for(space1[6]=0;aRecordBoard[i+1][j-1]==0&&i+1<=SIZE-1&&j-1>=0;space1[6]++,i++,j--);
    for(b[6]=0;aRecordBoard[i+1][j-1]==1&&i+1<=SIZE-1&&j-1>=0;b[6]++,i++,j--);
	for(space2[6]=0;aRecordBoard[i+1][j-1]==0&&i+1<=SIZE-1&&j-1>=0;space2[6]++,i++,j--);
    for(c[6]=0;aRecordBoard[i+1][j-1]==1&&i+1<=SIZE-1&&j-1>=0;c[6]++,i++,j--);
	//用循环记录左下一线的棋局
			i=i-a[6]-space1[6]-b[6]-space2[6]-c[6];
			j=j+a[6]+space1[6]+b[6]+space2[6]+c[6];//还原坐标
    for(a[7]=0;aRecordBoard[i-1][j+1]==1&&i-1>=0&&j+1<=SIZE-1;a[7]++,i--,j++);
	for(space1[7]=0;aRecordBoard[i-1][j+1]==0&&i-1>=0&&j+1<=SIZE-1;space1[7]++,i--,j++);
    for(b[7]=0;aRecordBoard[i-1][j+1]==1&&i-1>=0&&j+1<=SIZE-1;b[7]++,i--,j++);
	for(space2[7]=0;aRecordBoard[i-1][j+1]==0&&i-1>=0&&j+1<=SIZE-1;space2[7]++,i--,j++);
    for(c[7]=0;aRecordBoard[i-1][j+1]==1&&i-1>=0&&j+1<=SIZE-1;c[7]++,i--,j++);
	//用循环记录右上一线的棋局
			i=i+a[7]+space1[7]+b[7]+space2[7]+c[7];
			j=j-a[7]-space1[7]-b[7]-space2[7]-c[7];//还原坐标
}
//此函数记录白子周围的同色子或空格（棋盘局势）
void RecordAround2(int i,int j){
	int n;
    for(a[0]=0;aRecordBoard[i][j+1]==2&&j+1<=SIZE-1;a[0]++,j++);
	for(space1[0]=0;aRecordBoard[i][j+1]==0&&j+1<=SIZE-1;space1[0]++,j++);
    for(b[0]=0;aRecordBoard[i][j+1]==2&&j+1<=SIZE-1;b[0]++,j++);
	for(space2[0]=0;aRecordBoard[i][j+1]==0&&j+1<=SIZE-1;space2[0]++,j++);
    for(c[0]=0;aRecordBoard[i][j+1]==2&&j+1<=SIZE-1;c[0]++,j++);
	//用循环记录右边一线的棋局
			j=j-a[0]-space1[0]-b[0]-space2[0]-c[0];//还原坐标
	for(a[1]=0;aRecordBoard[i][j-1]==2&&j-1>=0;a[1]++,j--);
	for(space1[1]=0;aRecordBoard[i][j-1]==0&&j-1>=0;space1[1]++,j--);
    for(b[1]=0;aRecordBoard[i][j-1]==2&&j-1>=0;b[1]++,j--);
	for(space2[1]=0;aRecordBoard[i][j-1]==0&&j-1>=0;space2[1]++,j--);
    for(c[1]=0;aRecordBoard[i][j-1]==2&&j-1>=0;c[1]++,j--);
	//用循环记录左边一线的棋局
			j=j+a[1]+space1[1]+b[1]+space2[1]+c[1];//还原坐标
    for(a[2]=0;aRecordBoard[i+1][j]==2&&i+1<=SIZE-1;a[2]++,i++);
	for(space1[2]=0;aRecordBoard[i+1][j]==0&&i+1<=SIZE-1;space1[2]++,i++);
    for(b[2]=0;aRecordBoard[i+1][j]==2&&i+1<=SIZE-1;b[2]++,i++);
	for(space2[2]=0;aRecordBoard[i+1][j]==0&&i+1<=SIZE-1;space2[1]++,i++);
    for(c[2]=0;aRecordBoard[i+1][j]==2&&i+1<=SIZE-1;c[2]++,i++);
	//用循环记录下边一线的棋局
			i=i-a[2]-space1[2]-b[2]-space2[2]-c[2];//还原坐标
	for(a[3]=0;aRecordBoard[i-1][j]==2&&i-1>=0;a[3]++,i--);
	for(space1[3]=0;aRecordBoard[i-1][j]==0&&i-1>=0;space1[3]++,i--);
    for(b[3]=0;aRecordBoard[i-1][j]==2&&i-1>=0;b[3]++,i--);
	for(space2[3]=0;aRecordBoard[i-1][j]==0&&i-1>=0;space2[3]++,i--);
    for(c[3]=0;aRecordBoard[i-1][j]==2&&i-1>=0;c[3]++,i--);
	//用循环记录上边一线的棋局
			i=i+a[3]+space1[3]+b[3]+space2[3]+c[3];//还原坐标
	for(a[4]=0;aRecordBoard[i+1][j+1]==2&&i+1<=SIZE-1&&j+1<=SIZE-1;a[4]++,i++,j++);
	for(space1[4]=0;aRecordBoard[i+1][j+1]==0&&i+1<=SIZE-1&&j+1<=SIZE-1;space1[4]++,i++,j++);
    for(b[4]=0;aRecordBoard[i+1][j+1]==2&&i+1<=SIZE-1&&j+1<=SIZE-1;b[4]++,i++,j++);
	for(space2[4]=0;aRecordBoard[i+1][j+1]==0&&i+1<=SIZE-1&&j+1<=SIZE-1;space2[4]++,i++,j++);
    for(c[4]=0;aRecordBoard[i+1][j+1]==2&&i+1<=SIZE-1&&j+1<=SIZE-1;c[4]++,i++,j++);
    //用循环记录右下一线的棋局
			i=i-a[4]-space1[4]-b[4]-space2[4]-c[4];
			j=j-a[4]-space1[4]-b[4]-space2[4]-c[4];//还原坐标
	for(a[5]=0;aRecordBoard[i-1][j-1]==2&&i-1>=0&&j-1>=0;a[5]++,i--,j--);
	for(space1[5]=0;aRecordBoard[i-1][j-1]==0&&i-1>=0&&j-1>=0;space1[5]++,i--,j--);
    for(b[5]=0;aRecordBoard[i-1][j-1]==2&&i-1>=0&&j-1>=0;b[5]++,i--,j--);
	for(space2[5]=0;aRecordBoard[i-1][j-1]==0&&i-1>=0&&j-1>=0;space2[5]++,i--,j--);
    for(c[5]=0;aRecordBoard[i-1][j-1]==2&&i-1>=0&&j-1>=0;c[5]++,i--,j--);
	//用循环记录左上一线的棋局
			i=i+a[5]+space1[5]+b[5]+space2[5]+c[5];
			j=j+a[5]+space1[5]+b[5]+space2[5]+c[5];//还原坐标
	for(a[6]=0;aRecordBoard[i+1][j-1]==2&&i+1<=SIZE-1&&j-1>=0;a[6]++,i++,j--);
	for(space1[6]=0;aRecordBoard[i+1][j-1]==0&&i+1<=SIZE-1&&j-1>=0;space1[6]++,i++,j--);
    for(b[6]=0;aRecordBoard[i+1][j-1]==2&&i+1<=SIZE-1&&j-1>=0;b[6]++,i++,j--);
	for(space2[6]=0;aRecordBoard[i+1][j-1]==0&&i+1<=SIZE-1&&j-1>=0;space2[6]++,i++,j--);
    for(c[6]=0;aRecordBoard[i+1][j-1]==2&&i+1<=SIZE-1&&j-1>=0;c[6]++,i++,j--);
	//用循环记录左下一线的棋局
			i=i-a[6]-space1[6]-b[6]-space2[6]-c[6];
			j=j+a[6]+space1[6]+b[6]+space2[6]+c[6];//还原坐标
    for(a[7]=0;aRecordBoard[i-1][j+1]==2&&i-1>=0&&j+1<=SIZE-1;a[7]++,i--,j++);
	for(space1[7]=0;aRecordBoard[i-1][j+1]==0&&i-1>=0&&j+1<=SIZE-1;space1[7]++,i--,j++);
    for(b[7]=0;aRecordBoard[i-1][j+1]==2&&i-1>=0&&j+1<=SIZE-1;b[7]++,i--,j++);
	for(space2[7]=0;aRecordBoard[i-1][j+1]==0&&i-1>=0&&j+1<=SIZE-1;space2[7]++,i--,j++);
    for(c[7]=0;aRecordBoard[i-1][j+1]==2&&i-1>=0&&j+1<=SIZE-1;c[7]++,i--,j++);
	//用循环记录右上一线的棋局
			i=i+a[7]+space1[7]+b[7]+space2[7]+c[7];
			j=j-a[7]-space1[7]-b[7]-space2[7]-c[7];//还原坐标
}

//该函数判断当前落子后是否成五连子
int win(){
    if(a[0]+a[1]>=4||a[2]+a[3]>=4||a[4]+a[5]>=4||a[6]+a[7]>=4)
		return 1;
    else
		return 0;
}
//定义一个结构，记录当前棋子周围（八个方向上的）各种棋形的数量
struct point{
		int single;//单子
		int live2;//活二
		int lowlive2;//低级活二
		int go2;//冲二
		int live3;//活三
		int go3;//冲三
		int jump3;//跳活三
		int count_3;//可能成五的三子数
		int live4;//活四
		int go4;//冲四
		int lowgo4;//低级冲四
		int count_4;//可能成五的四子数
		int isFive;//成五
	};	
//此函数计算当前棋子周围（八个方向上的）各种棋形数目
struct point count(){
	//初始化结构record为0
	struct point record={0,0,0,0,0,0,0,0,0,0,0,0,0};	
	int k;
	//利用循环，两个相对的方向为一组（分别为数组的相邻两个元素）
	for(k=0;k<=6;k=k+2){
		  if(a[k]+a[k+1]>=4)
				record.isFive++;
		  else if(a[k]+a[k+1]==3){//四连子
				if(space1[k]>0&&space1[k+1]==0){//OAAAA?冲四
					   isFour=1;//说明该方向上存在可能成五的四子,下同
					   record.go4++;
				}
				if(space1[k+1]>0&&space1[k]==0){//?AAAAO冲四
					   record.go4++;
					   isFour=1;
				}
				if(space1[k+1]>0&&space1[k]>0){//OAAAAO活四
					   record.live4++;
					   isFour=1;
				}
				if(isFour==1){//存在可能成五的四子则record中记录四子个数的量+1，下同
					   isFour=0;
					   record.count_4++;
				}
		  }
		  else if(a[k]+a[k+1]==2){//三连子
				if(space1[k]==1&&b[k]==1){//AOAAA冲四
					   record.lowgo4++;
					   isFour=1;
				}
				if(space1[k+1]==1&&b[k+1]==1){//AAAOA冲四
					   record.lowgo4++;
					   isFour=1;
				}
				if(space1[k]>=2||space1[k+1]>=2){
					   if(space1[k+1]>=2&&space1[k]>=2){//OOAAAOO活三
						   record.live3++;
						   isThree=1;//说明该方向上存在活三，下同
					   }
					   if(space1[k+1]>=2&&space1[k]==0){//OOAAA?眠三
					   	   record.go3++;
					   }
					   if(space1[k]>=2&&space1[k+1]==0){//?AAAOO眠三
					   	   record.go3++;
					   }
					   if(space1[k]==1&&space1[k+1]==1){//OAAAO眠三
						   record.go3++;
					   }
				}
				if(isFour==1){
					   isFour=0;
					   record.count_4++;
				}
				if(isThree==1){//存在活三则record中记录三子个数的量+1，下同
					   isThree=0;
					   record.count_3++;
				}
		  }
		  else if(a[k]+a[k+1]==1){//二连子
				if(space1[k]==1&&b[k]==2){//AAOAA低级冲四
					   record.lowgo4++;
					   isFour=1;
				}
				if(space1[k+1]==1&&b[k+1]==2){//AAOAA低级冲四
					   record.lowgo4++;
					   isFour=1;
				}
				if(space1[k]==1&&b[k]==1&&space2[k]>=1&&space1[k+1]>=1){//OAOAAO跳活三
					   record.jump3++;
					   record.live3++;
					   isThree=1;
				}
				if(space1[k+1]==1&&b[k+1]==1&&space2[k+1]>=1&&space1[k]>=1){//OAAOAO跳活三
					   record.jump3++;
					   record.live3++;
					   isThree=1;
				}
				if(space1[k+1]==1&&b[k+1]==1&&((space2[k+1]==0&&space1[k]>=1)||(space2[k+1]>=1&&space1[k]==0))){//?AAOAO?or?OAAOA?眠三
					   record.go3++;
				}
				if(space1[k]==1&&b[k]==1&&((space2[k]==0&&space1[k+1]>=1)||(space2[k]>=1&&space1[k+1]==0))){//?AOAAO?or?OAOAA?眠三
					   record.go3++;
				}
				if(space1[k]==2&&b[k]==1||space1[k+1]==2&&b[k+1]==1){//AOOAA眠三
					   record.go3++;
				}
				if(space1[k]>=3&&space1[k+1]>=3){//?OOOAAOOO?活二
					   record.live2++;
				}
				if(space1[k]==0&&space1[k+1]>=3||space1[k+1]==0&&space1[k]>=3){//?AAOOO?or?OOOAA?冲二
					   record.go2++;
				}
				if(isFour==1){
					   isFour=0;
					   record.count_4++;
				}
				if(isThree==1){
					   isThree=0;
					   record.count_3++;
				}
		  }
		  else if(a[k]+a[k+1]==0){//单子
				if((space1[k]==1&&b[k]==3)||(space1[k+1]==1&&b[k+1]==3)){//AOAAA低级冲四
					   isFour=1;
					   record.lowgo4++;
				}
				if((space1[k]==2&&b[k]==2)||(space1[k+1]==2&&b[k+1]==2)){//AOOAA眠三
					   record.go3++;
				}
				if((space1[k]==1&&b[k]==2&&space2[k]>=1&&space1[k+1]>=1)||(space1[k+1]==1&&b[k+1]==2&&space2[k+1]>=1&&space1[k]>=1)){//OAOAAO跳活三
					   isThree=1;
					   record.jump3++;
					   record.live3++;
				}
				if((space1[k]==1&&b[k]==1&&space2[k]==1&&c[k]==1)||(space1[k+1]==1&&b[k+1]==1&&space2[k+1]==1&&c[k+1]==1)){//AOAOA眠三
					   record.go3++;
				}
				if((space1[k]==1&&b[k]==2&&((space2[k]>=1&&space1[k+1]==0)||(space2[k]==0&&space1[k+1]>=1)))||(space1[k+1]==1&&b[k+1]==2&&((space2[k+1]>=1&&space1[k]==0)||(space2[k+1]==0&&space1[k]>=1)))){//?AOAA?眠三
					   record.go3++;
				}
				if(space1[k]==1&&b[k]==1&&space1[k+1]>=2&&space2[k]>=2||space1[k+1]==1&&b[k+1]==1&&space2[k+1]>=2&&space1[k]>=2){//OAOAO低级活二
					   record.lowlive2++;
				}
				if(space1[k]==2&&b[k]==1&&space1[k+1]>=1&&space2[k]>=1||space1[k+1]==2&&b[k+1]==1&&space2[k+1]>=1&&space1[k]>=1){//?AOOA?低级活二
					   record.lowlive2++;
				}
				if(space1[k]==1&&b[k]==1&&space1[k+1]==0&&space2[k]>=2||space1[k+1]==1&&b[k+1]==1&&space2[k+1]>=2&&space1[k]==0){//?AOAOOorOOAOA?冲二
					   record.go2++;
				}
				if(space1[k]==2&&b[k]==1&&space1[k+1]==0&&space2[k]>=1||space1[k+1]==2&&b[k+1]==1&&space2[k+1]>=1&&space1[k]==0){//?AOOAOorOAOOA?冲二
					   record.go2++;
				}
				if(space1[k]==3&&b[k]==1||space1[k+1]==3&&b[k+1]==1){//?AOOOA?冲二
					   record.go2++;
				}
				if(space1[k]>=3&&space1[k+1]>=3){//OOOAOOO单子
					   record.single++;
				}
				if(isFour==1){
					   isFour=0;
					   record.count_4++;
				}
				if(isThree==1){
					   isThree=0;
					   record.count_3++;
				}
		 }
    }
	return record;
}


void initScore(int Score[SIZE][SIZE]){
int i,j;
//通过双重循环，将Score清0
for(i=0;i<SIZE;i++){
      for(j=0;j<SIZE;j++)
          Score[i][j]=0;
   }
}
//此函数记录黑子和白子的棋盘分数
void RecordScore(int Score1[SIZE][SIZE],int Score2[SIZE][SIZE]){
	//初始化双方棋盘分数矩阵为0
	initScore(Score1);
	initScore(Score2);
	//通过双重循环给两棋盘分数矩阵赋值
	int i,j;
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			//假设在当前位置下黑子
			RecordAround1(i,j);
			//假设在当前位置下黑子，将该子周围局势记录在结构record中
			struct point record=count();
			//判断在该点落子后是否会形成禁手
			if(forbid()==0){//若不会形成禁手，正常估分
				if(record.isFive>=1||record.live4>=2)
						Score1[i][j]+=100000;//落子后能成五或双活四+100000分
				if(record.live4==1||record.go4>=2||record.go4>=1&&record.live3>=1)
						Score1[i][j]+=10000;//落子后能成活四或双冲四或冲四活三+10000分
				if(record.live3>=2)
						Score1[i][j]+=5000;//落子后能成双活三+5000分
				if(record.go3>=1&&record.live3>=1)
						Score1[i][j]+=1000;//落子后能成一活三一冲三+1000分
				if(record.go4>=1||record.go3>=2)
						Score1[i][j]+=500;//落子后能成一活四或双冲三+500分
				if(record.lowgo4>=1)
						Score1[i][j]+=400;//落子后能成低级冲四+400分
				if(record.live3==1&&record.jump3==0)
						Score1[i][j]+=100;//落子后能成活三（非跳活三）+100分
				if(record.jump3==1)
						Score1[i][j]+=90;//落子后能成跳活三+90分
				if(record.live2>=2)
						Score1[i][j]+=50;//落子后能成双活二+50分
				if(record.live2==1)
						Score1[i][j]+=10;//落子后能成活二+10分
				if(record.lowlive2>=1)
						Score1[i][j]+=9;//落子后能成低级活二+9分
				if(record.go3==1)
						Score1[i][j]+=5;//落子后能成眠三+5分
				if(record.go2>=1)
						Score1[i][j]+=2;//落子后能成冲二+2分
				if(record.single>=1)
						Score1[i][j]+=1;//落子后能成单子+1分
			}
			else//若会形成禁手，该位置估值为0
				Score1[i][j]=0;
			//假设在当前位置下白子
			RecordAround2(i,j);
			//假设在当前位置下白子，将该子周围局势记录在结构record中
			record=count();
			if(record.isFive>=1||record.live4>=2)
					Score2[i][j]+=100000;//落子后能成五或双活四+100000分
			if(record.live4==1||record.go4>=2||record.go4>=1&&record.live3>=1)
					Score2[i][j]+=10000;//落子后能成活四或双冲四或冲四活三+10000分
			if(record.live3>=2)
					Score2[i][j]+=5000;//落子后能成双活三+5000分
			if(record.go3>=1&&record.live3>=1)
					Score2[i][j]+=1000;//落子后能成一活三一冲三+1000分
			if(record.go4>=1||record.go3>=2)
					Score2[i][j]+=500;//落子后能成一活四或双冲三+500分
			if(record.lowgo4>=1)
					Score2[i][j]+=400;//落子后能成低级冲四+400分
			if(record.live3==1&&record.jump3==0)
					Score2[i][j]+=100;//落子后能成活三（非跳活三）+100分
			if(record.jump3==1)
					Score2[i][j]+=90;//落子后能成跳活三+90分
			if(record.live2>=2)
					Score2[i][j]+=50;//落子后能成双活二+50分
			if(record.live2==1)
					Score2[i][j]+=10;//落子后能成活二+10分
			if(record.lowlive2>=1)
					Score2[i][j]+=9;//落子后能成低级活二+9分
			if(record.go3==1)
					Score2[i][j]+=5;//落子后能成眠三+5分
			if(record.go2>=1)
					Score2[i][j]+=2;//落子后能成冲二+2分
			if(record.single>=1)
					Score2[i][j]+=1;//落子后能成单子+1分
		}
	}
}
//此函数判断当前落子是否会形成禁手
int forbid(){
	int k;
	//通过循环，判断四个方向上是否存在长禁手或均无禁手
	for(k=0;k<=6;k=k+2){
      	if(a[k]+a[k+1]==4)
			return NO_FORBIDDEN;
	  	else if(a[k]+a[k+1]>4)
			return LONG_FORBIDDEN;
	}
	struct point record=count();
	//若四个方向中有至少两个方向上存在可能成五的四子，则该位置构成四四禁手
	if(record.count_4>=2)
		return FOUR_FOUR_FORBIDDEN;
	//若四个方向中有至少两个方向上存在可能活三，则该位置构成三三禁手
	if(record.count_3>=2)
		return THREE_THREE_FORBIDDEN;
	//其他情况不构成禁手
	else
		return NO_FORBIDDEN;
}






#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define SIZE 15//���̴�С
#define CHARSIZE 2//�����ַ���С
#define LEN 100
#define DIRECTION 8//һ��������Χ�İ˸�����
#define NO_FORBIDDEN 0//�޽���
#define THREE_THREE_FORBIDDEN 1//��������
#define FOUR_FOUR_FORBIDDEN 2//���Ľ���
#define LONG_FORBIDDEN 3//��������

//-------------------------------------�����Ǻ�������-----------------------------------------------
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



//���������ڼ�¼���̸�� 
int aRecordBoard[SIZE][SIZE];
//����ģʽ����quit��
char t[]="quit";
//�������¼�����ַ�
char s[LEN];
//�˺�������������������������������Ϣ���ַ�������������s
void getinput(char s[]){
		int i,c;
		i=0;
		while((c=getchar())!=EOF&&c!='\n')
		s[i++]=c;
		if(c=='\n')
		s[i++]='\n';
		s[i]='\0';
}
//�˺���������s�м�¼������Ϣת����������ʽ
int gety(char s[]){
		int j;
		int y=0;
		for(j=0;s[j]!='\n';j++){
		  if(isdigit(s[j]))
		    y=10*y+s[j]-'0';
		}
		return y;
}
//�˺���������s�м�¼������Ϣת����������ʽ
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
//�˺����ж���������ı����Ƿ������ģʽ����quit��ƥ����ַ���
int strindex(char s[],char t[]){
		int i,j,k;
		for(i=0;s[i]!='\0';i++){
			for(j=i,k=0;t[k]!='\0'&&s[j]==t[k];j++,k++);
				if(k>0&&t[k]=='\0')
				return 1;
		}
		return 0;
}

//����һ���ṹ����¼Ŀ�����ӵ��λ����Ϣ
struct position{
		int x;
		int y;
};
//����һ���ṹ����¼������ӵ�ķ�ֵ��λ����Ϣ
struct best{
		struct position pos;
		int score1;
		int score2;
};
//����һ���ṹ���������ֵ�������ͬ�ĵ��λ����Ϣ
struct chess_queue
{
		struct position chess[SIZE*SIZE];//�������������Ϣ��������chess��
		int len;//��¼�������Ч���ȣ���ֵ�������ͬ�ĵ�ĸ�����
};
//�˺���ͨ��������ǰ���Ӻ��˫�����̷�ֵ�ҳ�������ӵ�
struct best find(int Score1[SIZE][SIZE],int Score2[SIZE][SIZE]){//Score1Ϊ�ҷ����̷�������Score2Ϊ�Է����̷�������
		struct best bst={{0,0},0,0};//����һ��best���͵Ľṹbst������ʼ��
		struct position optimal={0,0};//����һ��position���͵Ľṹoptimal������ʼ��
		struct chess_queue queue;//����һ��chess_queue���͵Ľṹqueue
		//ͨ��ѭ����queue�е�����queue.chess��ʼ��Ϊ0
		for(int k=0;k<SIZE*SIZE;k++){
			queue.chess[k].x=0;
			queue.chess[k].y=0;
		}
		int i,j,l=0;
		//˫��ѭ�����Ƚ�˫�������̷�ֵ���ҵ��ҷ��ͶԷ���ֵ��ߵĵ�
		for(i=0;i<SIZE;i++){
			for(j=0;j<SIZE;j++){
			  	if(aRecordBoard[i][j]==0){
					if(Score1[i][j]>bst.score1){//������ֵ���ߵĵ㣬����queue.chess���㲢���¸�ֵ
						bst.score1=Score1[i][j];//���ҷ��������з�ֵ��ߵ��¼��bst.score1
						for(int k=0;k<SIZE*SIZE;k++){
							queue.chess[k].x=0;
							queue.chess[k].y=0;
						}
						queue.chess[0].x=i;
						queue.chess[0].y=j;
						queue.len=1;
						l=0;
					}
					if(Score1[i][j]==bst.score1){//�ҷ����ж����ֵ��ߵ����������queue.chess
						queue.chess[l+1].x=i;
						queue.chess[l+1].y=j;
						queue.len=l+2;
						l++;//��ѵ����+1
					}
					if(Score2[i][j]>=bst.score2){//���Է���ֵ��ߵĵ��������Ϣ��¼��optimal�У���������ߵķ�ֵ��¼��bst.score2��
						bst.score2=Score2[i][j];
						optimal.x=i;
						optimal.y=j;
					}
			  	}
			}			
		}
		//���ҷ���߷�ֵ���ڶԷ���߷�ֵ
		if(bst.score1>=bst.score2&&queue.len==1){
				bst.pos.x=queue.chess[0].x;
				bst.pos.y=queue.chess[0].y;	
		}
		//�ҷ���߷�ֵ���ж��ʱ��ѡ�����жԷ���ֵ��ߵĵ�
		if(bst.score1>=bst.score2&&queue.len>1){
			for(int n=0;n<queue.len;n++){
				if(Score2[queue.chess[n].x][queue.chess[n].y]>=Score2[queue.chess[0].x][queue.chess[0].y]){
					bst.pos.x=queue.chess[n].x;
					bst.pos.y=queue.chess[n].y;	
				}
			}
		}
		//���ҷ���߷�ֵ���ڶԷ���߷�ֵ��ѡ��Է���ֵ��ߵĵ�		
		if(bst.score1<bst.score2){
				bst.pos.x=optimal.x;
				bst.pos.y=optimal.y;	
		}	
		return bst;
}

//��������Ĺ�����
int line1,line2;//��
int column1,column2;//��
int Score1[SIZE][SIZE];//��¼���ӵ��������Ʒ���
int Score2[SIZE][SIZE];//��¼���ӵ��������Ʒ���

//�˺������л������ֵ����Ӳ���
void machine1(void){
		//��õ�ǰ���̾����º��ӺͰ��ӵķ���
		RecordScore(Score1,Score2);
		//����find������ú��ӵ�������ӵ�
		struct best bst1=find(Score1,Score2);
		//���Ӳ���ʾ���Ӻ������
		line1=15-bst1.pos.x;
		column1=bst1.pos.y;
		aRecordBoard[bst1.pos.x][bst1.pos.y]=3;
		recordtoDisplayArray();
    	displayBoard();
		//��ӡ����
		printf("The position is (%d,%c)\n",15-bst1.pos.x,'A'+bst1.pos.y);
		aRecordBoard[15-line1][column1]=1;
		//��¼���ӵ���Χ�����̾���
    	RecordAround1(15-line1,column1);
}

//�˺������л������ֵ����Ӳ���
void machine2(void){
		//��õ�ǰ���̾����º��ӺͰ��ӵķ���
		RecordScore(Score1,Score2);
		//����find������ð��ӵ�������ӵ�
		struct best bst2=find(Score2,Score1);
		//���Ӳ���ʾ���Ӻ������
		line2=15-bst2.pos.x;
		column2=bst2.pos.y;
		aRecordBoard[bst2.pos.x][bst2.pos.y]=4;
		recordtoDisplayArray();
    	displayBoard();
		//��ӡ����
		printf("The position is (%d,%c)\n",15-bst2.pos.x,'A'+bst2.pos.y);
		aRecordBoard[15-line2][column2]=2;
		//��¼���ӵ���Χ�����̾���
    	RecordAround2(15-line2,column2);
}

//�˺�������������ҵ����Ӳ���
void player1(void){
    	printf("Player1 please place a piece:");
		//���������������λ��
    	void getinput(char s[]);
    	getinput(s);
    	int getx(char s[]);
    	int gety(char s[]);
    	line1=gety(s);
    	column1=getx(s);
		//���Ϸ����뱨����
    	if(line1>15||line1==0||column1>14){
			//�ж�����Ƿ������ˡ�quit��
	   		if(strindex(s,t)>0)
				return;
			//Ҫ����ҽ��кϷ����벢���µ������Ӻ���
       		printf("error!Please input a valid position.\n");
        	player1();
			return ;
		}
    	return;
}

//�˺������к�����ҵ����Ӳ���
void player2(void){
		printf("Player2 please place a piece:");
		//���������������λ��
    	void getinput(char s[]);
    	getinput(s);
    	int getx(char s[]);
    	int gety(char s[]);
    	line2=gety(s);
    	column2=getx(s);
		//���Ϸ����뱨����
    	if(line2>15||line2==0||column2>14){
			//�ж�����Ƿ������ˡ�quit��
	    	if(strindex(s,t)>0)
				return;
			//Ҫ����ҽ��кϷ����벢���µ������Ӻ���
       		printf("error!Please input a valid position.\n");
       		player2();
        	return;
    	}
    	return;
}

//����ʹ�õ���GBK���룬ÿһ�������ַ�ռ��2���ֽڡ�

//���̻���ģ�� 
char aInitDisplayBoardArray[SIZE][SIZE*CHARSIZE+1] = 
{
		"���өөөөөөөөөөөөө�",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"���۩۩۩۩۩۩۩۩۩۩۩۩۩�"
};

//�������������ı���
int r;
//������
int main(){
	//���̳�ʼ��
    initRecordBoard();
	recordtoDisplayArray();
	displayBoard();
	int mode,order;
	printf("Work of Zhou Ziqi\nPlease choose a mode: 1 man VS man; 2 man VS machine\n");
	scanf("%d",&mode);
	getchar();
	//�˻���սģʽ
	if(mode==2){
			printf("Please choose your order: 1 first player; 2 second player\n");
			scanf("%d",&order);
			getchar();
			//������֣���������
			if(order==1){
					  //˫�������������ó���113���������̣�������ֱ���˳�����
					  for(r=0;r<113;r++){
								//�������
								player1();
						  		if(strindex(s,t)>0)
										return 0;
								//�ж��Ƿ�Ϊ���Ϸ����ӣ����������ӵ�λ�ã�
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
								//�ж����ӵ��Ƿ񹹳ɽ��֣����ǣ��к�����
						  		if(forbid()>0){
							  			printf("FORBIDDEN PLACEMENT! You lose, game over!\n");
							  			return 0;
								}
								//�ж����Ӻ��Ƿ�������飬���ǣ�����Ӯ
								if(win()==1){
							  			printf("You win!\n");
							  			return 0;
					   			}
								//��������
								machine2();
								//�ж����Ӻ��Ƿ�������飬���ǣ�����Ӯ
								if(win()==1){
							  			printf("Machine wins!\n");
							  			return 0;
								}
					  }
			}
			//��Һ��֣���������
			if(order==2){
					 //���������״�����ֱ��������������λ��
					 aRecordBoard[7][7]=3;
					 recordtoDisplayArray();
				  	 displayBoard();
					 aRecordBoard[7][7]=1;
					 for(r=0;r<113;r++){
								//�������
								player2();
						  		if(strindex(s,t)>0)
										return 0;
								//�ж��Ƿ�Ϊ���Ϸ����ӣ����������ӵ�λ�ã�
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
								//�ж����Ӻ��Ƿ�������飬���ǣ�����Ӯ
								if(win()==1){
							  			printf("You win!\n");
							  			return 0;
								}
								//��������
								machine1();
								//�ж����ӵ��Ƿ񹹳ɽ��֣����ǣ��к�����
								if(forbid()>0){
							  			printf("FORBIDDEN PLACEMENT! Machine loses, game over!\n");
							  			return 0;
								}
								//�ж����Ӻ��Ƿ�������飬���ǣ�����Ӯ
								if(win()==1){
							  			printf("Machine wins!\n");
							  			return 0;
					   			}
					  }
			}
    }
	//���˶�սģʽ
	if(mode==1){
			for(r=0;r<=112;r++){
					  //�����������
					  player1();
					  if(strindex(s,t)>0)
								return 0;
					  //�ж��Ƿ�Ϊ���Ϸ����ӣ����������ӵ�λ�ã�
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
					  //�ж����ӵ��Ƿ񹹳ɽ��֣����ǣ��к�����
					  if(forbid()>0){
							  	printf("FORBIDDEN PLACEMENT! Player1 loses, game over!\n");
							  	return 0;
					  }
					  //�ж����Ӻ��Ƿ�������飬���ǣ�����Ӯ
					  if(win()==1){
								printf("Player1 is the winner!\n");
								return 0;
					  }
					  //�����������
					  player2();
					  if(strindex(s,t)>0)
								return 0;
					  //�ж��Ƿ�Ϊ���Ϸ����ӣ����������ӵ�λ�ã�
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
					  //�ж����Ӻ��Ƿ�������飬���ǣ�����Ӯ
					  if(win()==1){
								printf("Player2 is the winner!\n");
								return 0;
					  }
			}
	}
    return 0;
}


//������������ʾ���� 
char aDisplayBoardArray[SIZE][SIZE*CHARSIZE+1];
 
char play1Pic[]="��";//������;
char play1CurrentPic[]="��"; 

char play2Pic[]="��";//������;
char play2CurrentPic[]="��";


//��ʼ�����̸�� 
void initRecordBoard(void){
int i,j;
//ͨ��˫��ѭ������aRecordBoard��0
for(i=0;i<SIZE;i++){
      for(j=0;j<SIZE;j++)
          aRecordBoard[i][j]=0;
   }
}

//��aRecordBoard�м�¼������λ�ã�ת����aDisplayBoardArray��
void recordtoDisplayArray(void){
int i,j;
//��һ������aInitDisplayBoardArray�м�¼�Ŀ����̣����Ƶ�aDisplayBoardArray��
   for(i=0;i<SIZE;i++){
      for(j=0;j<SIZE*2+1;j++)
          aDisplayBoardArray[i][j]=aInitDisplayBoardArray[i][j];
   }
//�ڶ�����ɨ��aRecordBoard����������0��Ԫ�أ�������ߡ��Ƶ�aDisplayBoardArray����Ӧλ����
//ע�⣺aDisplayBoardArray����¼���ַ��������ַ���ÿ���ַ�ռ2���ֽڡ���͡�Ҳ�������ַ���ÿ��Ҳռ2���ֽڡ�
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


//��ʾ���̸�� 
void displayBoard(void){
	int i;
	//��һ��������
	system("clear");   //����  
	//�ڶ�������aDisplayBoardArray�������Ļ��
	for(i=0;i<SIZE;i++){
        if(i>=0&&i<=5)
           printf("%d%s\n",SIZE-i,aDisplayBoardArray[i]);
        if(i>=6&&i<=15)
           printf(" %d%s\n",SIZE-i,aDisplayBoardArray[i]);
	}
	//������������������һ����ĸA B .... 
           printf("   A B C D E F G H I J K L M N O\n");
}


//�������¼�뵱ǰ����������Ͻ�����ͬɫ������
int a[DIRECTION];
//�������¼�뵱ǰ����������Ͻ�����ͬɫ������Ŀո���֮���ͬɫ������
int b[DIRECTION];
//�������¼�뵱ǰ����������Ͻ�����ͬɫ������Ŀո���֮���ͬɫ��������Ŀո���֮���ͬɫ������
int c[DIRECTION];
//�������¼�뵱ǰ����������Ͻ�����ͬɫ������Ŀո���
int space1[DIRECTION];
//�������¼�뵱ǰ����������Ͻ�����ͬɫ������Ŀո���֮���ͬɫ��������Ŀո���
int space2[DIRECTION];
//isFour,isThree��¼����ĳ������ĳ���״̬,����ʼ��
int isFour=0,isThree=0;
//�˺�����¼������Χ��ͬɫ�ӻ�ո����̾��ƣ�
void RecordAround1(int i,int j){
	int n;
    for(a[0]=0;aRecordBoard[i][j+1]==1&&j+1<=SIZE-1;a[0]++,j++);
	for(space1[0]=0;aRecordBoard[i][j+1]==0&&j+1<=SIZE-1;space1[0]++,j++);
    for(b[0]=0;aRecordBoard[i][j+1]==1&&j+1<=SIZE-1;b[0]++,j++);
	for(space2[0]=0;aRecordBoard[i][j+1]==0&&j+1<=SIZE-1;space2[0]++,j++);
    for(c[0]=0;aRecordBoard[i][j+1]==1&&j+1<=SIZE-1;c[0]++,j++);
	//��ѭ����¼�ұ�һ�ߵ����
			j=j-a[0]-space1[0]-b[0]-space2[0]-c[0];//��ԭ����
	for(a[1]=0;aRecordBoard[i][j-1]==1&&j-1>=0;a[1]++,j--);
	for(space1[1]=0;aRecordBoard[i][j-1]==0&&j-1>=0;space1[1]++,j--);
    for(b[1]=0;aRecordBoard[i][j-1]==1&&j-1>=0;b[1]++,j--);
	for(space2[1]=0;aRecordBoard[i][j-1]==0&&j-1>=0;space2[1]++,j--);
    for(c[1]=0;aRecordBoard[i][j-1]==1&&j-1>=0;c[1]++,j--);
	//��ѭ����¼���һ�ߵ����
			j=j+a[1]+space1[1]+b[1]+space2[1]+c[1];//��ԭ����
    for(a[2]=0;aRecordBoard[i+1][j]==1&&i+1<=SIZE-1;a[2]++,i++);
	for(space1[2]=0;aRecordBoard[i+1][j]==0&&i+1<=SIZE-1;space1[2]++,i++);
    for(b[2]=0;aRecordBoard[i+1][j]==1&&i+1<=SIZE-1;b[2]++,i++);
	for(space2[2]=0;aRecordBoard[i+1][j]==0&&i+1<=SIZE-1;space2[1]++,i++);
    for(c[2]=0;aRecordBoard[i+1][j]==1&&i+1<=SIZE-1;c[2]++,i++);
	//��ѭ����¼�±�һ�ߵ����
			i=i-a[2]-space1[2]-b[2]-space2[2]-c[2];//��ԭ����
	for(a[3]=0;aRecordBoard[i-1][j]==1&&i-1>=0;a[3]++,i--);
	for(space1[3]=0;aRecordBoard[i-1][j]==0&&i-1>=0;space1[3]++,i--);
    for(b[3]=0;aRecordBoard[i-1][j]==1&&i-1>=0;b[3]++,i--);
	for(space2[3]=0;aRecordBoard[i-1][j]==0&&i-1>=0;space2[3]++,i--);
    for(c[3]=0;aRecordBoard[i-1][j]==1&&i-1>=0;c[3]++,i--);
	//��ѭ����¼�ϱ�һ�ߵ����
			i=i+a[3]+space1[3]+b[3]+space2[3]+c[3];//��ԭ����
	for(a[4]=0;aRecordBoard[i+1][j+1]==1&&i+1<=SIZE-1&&j+1<=SIZE-1;a[4]++,i++,j++);
	for(space1[4]=0;aRecordBoard[i+1][j+1]==0&&i+1<=SIZE-1&&j+1<=SIZE-1;space1[4]++,i++,j++);
    for(b[4]=0;aRecordBoard[i+1][j+1]==1&&i+1<=SIZE-1&&j+1<=SIZE-1;b[4]++,i++,j++);
	for(space2[4]=0;aRecordBoard[i+1][j+1]==0&&i+1<=SIZE-1&&j+1<=SIZE-1;space2[4]++,i++,j++);
    for(c[4]=0;aRecordBoard[i+1][j+1]==1&&i+1<=SIZE-1&&j+1<=SIZE-1;c[4]++,i++,j++);
    //��ѭ����¼����һ�ߵ����
			i=i-a[4]-space1[4]-b[4]-space2[4]-c[4];
			j=j-a[4]-space1[4]-b[4]-space2[4]-c[4];//��ԭ����
	for(a[5]=0;aRecordBoard[i-1][j-1]==1&&i-1>=0&&j-1>=0;a[5]++,i--,j--);
	for(space1[5]=0;aRecordBoard[i-1][j-1]==0&&i-1>=0&&j-1>=0;space1[5]++,i--,j--);
    for(b[5]=0;aRecordBoard[i-1][j-1]==1&&i-1>=0&&j-1>=0;b[5]++,i--,j--);
	for(space2[5]=0;aRecordBoard[i-1][j-1]==0&&i-1>=0&&j-1>=0;space2[5]++,i--,j--);
    for(c[5]=0;aRecordBoard[i-1][j-1]==1&&i-1>=0&&j-1>=0;c[5]++,i--,j--);
	//��ѭ����¼����һ�ߵ����
			i=i+a[5]+space1[5]+b[5]+space2[5]+c[5];
			j=j+a[5]+space1[5]+b[5]+space2[5]+c[5];//��ԭ����
	for(a[6]=0;aRecordBoard[i+1][j-1]==1&&i+1<=SIZE-1&&j-1>=0;a[6]++,i++,j--);
	for(space1[6]=0;aRecordBoard[i+1][j-1]==0&&i+1<=SIZE-1&&j-1>=0;space1[6]++,i++,j--);
    for(b[6]=0;aRecordBoard[i+1][j-1]==1&&i+1<=SIZE-1&&j-1>=0;b[6]++,i++,j--);
	for(space2[6]=0;aRecordBoard[i+1][j-1]==0&&i+1<=SIZE-1&&j-1>=0;space2[6]++,i++,j--);
    for(c[6]=0;aRecordBoard[i+1][j-1]==1&&i+1<=SIZE-1&&j-1>=0;c[6]++,i++,j--);
	//��ѭ����¼����һ�ߵ����
			i=i-a[6]-space1[6]-b[6]-space2[6]-c[6];
			j=j+a[6]+space1[6]+b[6]+space2[6]+c[6];//��ԭ����
    for(a[7]=0;aRecordBoard[i-1][j+1]==1&&i-1>=0&&j+1<=SIZE-1;a[7]++,i--,j++);
	for(space1[7]=0;aRecordBoard[i-1][j+1]==0&&i-1>=0&&j+1<=SIZE-1;space1[7]++,i--,j++);
    for(b[7]=0;aRecordBoard[i-1][j+1]==1&&i-1>=0&&j+1<=SIZE-1;b[7]++,i--,j++);
	for(space2[7]=0;aRecordBoard[i-1][j+1]==0&&i-1>=0&&j+1<=SIZE-1;space2[7]++,i--,j++);
    for(c[7]=0;aRecordBoard[i-1][j+1]==1&&i-1>=0&&j+1<=SIZE-1;c[7]++,i--,j++);
	//��ѭ����¼����һ�ߵ����
			i=i+a[7]+space1[7]+b[7]+space2[7]+c[7];
			j=j-a[7]-space1[7]-b[7]-space2[7]-c[7];//��ԭ����
}
//�˺�����¼������Χ��ͬɫ�ӻ�ո����̾��ƣ�
void RecordAround2(int i,int j){
	int n;
    for(a[0]=0;aRecordBoard[i][j+1]==2&&j+1<=SIZE-1;a[0]++,j++);
	for(space1[0]=0;aRecordBoard[i][j+1]==0&&j+1<=SIZE-1;space1[0]++,j++);
    for(b[0]=0;aRecordBoard[i][j+1]==2&&j+1<=SIZE-1;b[0]++,j++);
	for(space2[0]=0;aRecordBoard[i][j+1]==0&&j+1<=SIZE-1;space2[0]++,j++);
    for(c[0]=0;aRecordBoard[i][j+1]==2&&j+1<=SIZE-1;c[0]++,j++);
	//��ѭ����¼�ұ�һ�ߵ����
			j=j-a[0]-space1[0]-b[0]-space2[0]-c[0];//��ԭ����
	for(a[1]=0;aRecordBoard[i][j-1]==2&&j-1>=0;a[1]++,j--);
	for(space1[1]=0;aRecordBoard[i][j-1]==0&&j-1>=0;space1[1]++,j--);
    for(b[1]=0;aRecordBoard[i][j-1]==2&&j-1>=0;b[1]++,j--);
	for(space2[1]=0;aRecordBoard[i][j-1]==0&&j-1>=0;space2[1]++,j--);
    for(c[1]=0;aRecordBoard[i][j-1]==2&&j-1>=0;c[1]++,j--);
	//��ѭ����¼���һ�ߵ����
			j=j+a[1]+space1[1]+b[1]+space2[1]+c[1];//��ԭ����
    for(a[2]=0;aRecordBoard[i+1][j]==2&&i+1<=SIZE-1;a[2]++,i++);
	for(space1[2]=0;aRecordBoard[i+1][j]==0&&i+1<=SIZE-1;space1[2]++,i++);
    for(b[2]=0;aRecordBoard[i+1][j]==2&&i+1<=SIZE-1;b[2]++,i++);
	for(space2[2]=0;aRecordBoard[i+1][j]==0&&i+1<=SIZE-1;space2[1]++,i++);
    for(c[2]=0;aRecordBoard[i+1][j]==2&&i+1<=SIZE-1;c[2]++,i++);
	//��ѭ����¼�±�һ�ߵ����
			i=i-a[2]-space1[2]-b[2]-space2[2]-c[2];//��ԭ����
	for(a[3]=0;aRecordBoard[i-1][j]==2&&i-1>=0;a[3]++,i--);
	for(space1[3]=0;aRecordBoard[i-1][j]==0&&i-1>=0;space1[3]++,i--);
    for(b[3]=0;aRecordBoard[i-1][j]==2&&i-1>=0;b[3]++,i--);
	for(space2[3]=0;aRecordBoard[i-1][j]==0&&i-1>=0;space2[3]++,i--);
    for(c[3]=0;aRecordBoard[i-1][j]==2&&i-1>=0;c[3]++,i--);
	//��ѭ����¼�ϱ�һ�ߵ����
			i=i+a[3]+space1[3]+b[3]+space2[3]+c[3];//��ԭ����
	for(a[4]=0;aRecordBoard[i+1][j+1]==2&&i+1<=SIZE-1&&j+1<=SIZE-1;a[4]++,i++,j++);
	for(space1[4]=0;aRecordBoard[i+1][j+1]==0&&i+1<=SIZE-1&&j+1<=SIZE-1;space1[4]++,i++,j++);
    for(b[4]=0;aRecordBoard[i+1][j+1]==2&&i+1<=SIZE-1&&j+1<=SIZE-1;b[4]++,i++,j++);
	for(space2[4]=0;aRecordBoard[i+1][j+1]==0&&i+1<=SIZE-1&&j+1<=SIZE-1;space2[4]++,i++,j++);
    for(c[4]=0;aRecordBoard[i+1][j+1]==2&&i+1<=SIZE-1&&j+1<=SIZE-1;c[4]++,i++,j++);
    //��ѭ����¼����һ�ߵ����
			i=i-a[4]-space1[4]-b[4]-space2[4]-c[4];
			j=j-a[4]-space1[4]-b[4]-space2[4]-c[4];//��ԭ����
	for(a[5]=0;aRecordBoard[i-1][j-1]==2&&i-1>=0&&j-1>=0;a[5]++,i--,j--);
	for(space1[5]=0;aRecordBoard[i-1][j-1]==0&&i-1>=0&&j-1>=0;space1[5]++,i--,j--);
    for(b[5]=0;aRecordBoard[i-1][j-1]==2&&i-1>=0&&j-1>=0;b[5]++,i--,j--);
	for(space2[5]=0;aRecordBoard[i-1][j-1]==0&&i-1>=0&&j-1>=0;space2[5]++,i--,j--);
    for(c[5]=0;aRecordBoard[i-1][j-1]==2&&i-1>=0&&j-1>=0;c[5]++,i--,j--);
	//��ѭ����¼����һ�ߵ����
			i=i+a[5]+space1[5]+b[5]+space2[5]+c[5];
			j=j+a[5]+space1[5]+b[5]+space2[5]+c[5];//��ԭ����
	for(a[6]=0;aRecordBoard[i+1][j-1]==2&&i+1<=SIZE-1&&j-1>=0;a[6]++,i++,j--);
	for(space1[6]=0;aRecordBoard[i+1][j-1]==0&&i+1<=SIZE-1&&j-1>=0;space1[6]++,i++,j--);
    for(b[6]=0;aRecordBoard[i+1][j-1]==2&&i+1<=SIZE-1&&j-1>=0;b[6]++,i++,j--);
	for(space2[6]=0;aRecordBoard[i+1][j-1]==0&&i+1<=SIZE-1&&j-1>=0;space2[6]++,i++,j--);
    for(c[6]=0;aRecordBoard[i+1][j-1]==2&&i+1<=SIZE-1&&j-1>=0;c[6]++,i++,j--);
	//��ѭ����¼����һ�ߵ����
			i=i-a[6]-space1[6]-b[6]-space2[6]-c[6];
			j=j+a[6]+space1[6]+b[6]+space2[6]+c[6];//��ԭ����
    for(a[7]=0;aRecordBoard[i-1][j+1]==2&&i-1>=0&&j+1<=SIZE-1;a[7]++,i--,j++);
	for(space1[7]=0;aRecordBoard[i-1][j+1]==0&&i-1>=0&&j+1<=SIZE-1;space1[7]++,i--,j++);
    for(b[7]=0;aRecordBoard[i-1][j+1]==2&&i-1>=0&&j+1<=SIZE-1;b[7]++,i--,j++);
	for(space2[7]=0;aRecordBoard[i-1][j+1]==0&&i-1>=0&&j+1<=SIZE-1;space2[7]++,i--,j++);
    for(c[7]=0;aRecordBoard[i-1][j+1]==2&&i-1>=0&&j+1<=SIZE-1;c[7]++,i--,j++);
	//��ѭ����¼����һ�ߵ����
			i=i+a[7]+space1[7]+b[7]+space2[7]+c[7];
			j=j-a[7]-space1[7]-b[7]-space2[7]-c[7];//��ԭ����
}

//�ú����жϵ�ǰ���Ӻ��Ƿ��������
int win(){
    if(a[0]+a[1]>=4||a[2]+a[3]>=4||a[4]+a[5]>=4||a[6]+a[7]>=4)
		return 1;
    else
		return 0;
}
//����һ���ṹ����¼��ǰ������Χ���˸������ϵģ��������ε�����
struct point{
		int single;//����
		int live2;//���
		int lowlive2;//�ͼ����
		int go2;//���
		int live3;//����
		int go3;//����
		int jump3;//������
		int count_3;//���ܳ����������
		int live4;//����
		int go4;//����
		int lowgo4;//�ͼ�����
		int count_4;//���ܳ����������
		int isFive;//����
	};	
//�˺������㵱ǰ������Χ���˸������ϵģ�����������Ŀ
struct point count(){
	//��ʼ���ṹrecordΪ0
	struct point record={0,0,0,0,0,0,0,0,0,0,0,0,0};	
	int k;
	//����ѭ����������Եķ���Ϊһ�飨�ֱ�Ϊ�������������Ԫ�أ�
	for(k=0;k<=6;k=k+2){
		  if(a[k]+a[k+1]>=4)
				record.isFive++;
		  else if(a[k]+a[k+1]==3){//������
				if(space1[k]>0&&space1[k+1]==0){//OAAAA?����
					   isFour=1;//˵���÷����ϴ��ڿ��ܳ��������,��ͬ
					   record.go4++;
				}
				if(space1[k+1]>0&&space1[k]==0){//?AAAAO����
					   record.go4++;
					   isFour=1;
				}
				if(space1[k+1]>0&&space1[k]>0){//OAAAAO����
					   record.live4++;
					   isFour=1;
				}
				if(isFour==1){//���ڿ��ܳ����������record�м�¼���Ӹ�������+1����ͬ
					   isFour=0;
					   record.count_4++;
				}
		  }
		  else if(a[k]+a[k+1]==2){//������
				if(space1[k]==1&&b[k]==1){//AOAAA����
					   record.lowgo4++;
					   isFour=1;
				}
				if(space1[k+1]==1&&b[k+1]==1){//AAAOA����
					   record.lowgo4++;
					   isFour=1;
				}
				if(space1[k]>=2||space1[k+1]>=2){
					   if(space1[k+1]>=2&&space1[k]>=2){//OOAAAOO����
						   record.live3++;
						   isThree=1;//˵���÷����ϴ��ڻ�������ͬ
					   }
					   if(space1[k+1]>=2&&space1[k]==0){//OOAAA?����
					   	   record.go3++;
					   }
					   if(space1[k]>=2&&space1[k+1]==0){//?AAAOO����
					   	   record.go3++;
					   }
					   if(space1[k]==1&&space1[k+1]==1){//OAAAO����
						   record.go3++;
					   }
				}
				if(isFour==1){
					   isFour=0;
					   record.count_4++;
				}
				if(isThree==1){//���ڻ�����record�м�¼���Ӹ�������+1����ͬ
					   isThree=0;
					   record.count_3++;
				}
		  }
		  else if(a[k]+a[k+1]==1){//������
				if(space1[k]==1&&b[k]==2){//AAOAA�ͼ�����
					   record.lowgo4++;
					   isFour=1;
				}
				if(space1[k+1]==1&&b[k+1]==2){//AAOAA�ͼ�����
					   record.lowgo4++;
					   isFour=1;
				}
				if(space1[k]==1&&b[k]==1&&space2[k]>=1&&space1[k+1]>=1){//OAOAAO������
					   record.jump3++;
					   record.live3++;
					   isThree=1;
				}
				if(space1[k+1]==1&&b[k+1]==1&&space2[k+1]>=1&&space1[k]>=1){//OAAOAO������
					   record.jump3++;
					   record.live3++;
					   isThree=1;
				}
				if(space1[k+1]==1&&b[k+1]==1&&((space2[k+1]==0&&space1[k]>=1)||(space2[k+1]>=1&&space1[k]==0))){//?AAOAO?or?OAAOA?����
					   record.go3++;
				}
				if(space1[k]==1&&b[k]==1&&((space2[k]==0&&space1[k+1]>=1)||(space2[k]>=1&&space1[k+1]==0))){//?AOAAO?or?OAOAA?����
					   record.go3++;
				}
				if(space1[k]==2&&b[k]==1||space1[k+1]==2&&b[k+1]==1){//AOOAA����
					   record.go3++;
				}
				if(space1[k]>=3&&space1[k+1]>=3){//?OOOAAOOO?���
					   record.live2++;
				}
				if(space1[k]==0&&space1[k+1]>=3||space1[k+1]==0&&space1[k]>=3){//?AAOOO?or?OOOAA?���
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
		  else if(a[k]+a[k+1]==0){//����
				if((space1[k]==1&&b[k]==3)||(space1[k+1]==1&&b[k+1]==3)){//AOAAA�ͼ�����
					   isFour=1;
					   record.lowgo4++;
				}
				if((space1[k]==2&&b[k]==2)||(space1[k+1]==2&&b[k+1]==2)){//AOOAA����
					   record.go3++;
				}
				if((space1[k]==1&&b[k]==2&&space2[k]>=1&&space1[k+1]>=1)||(space1[k+1]==1&&b[k+1]==2&&space2[k+1]>=1&&space1[k]>=1)){//OAOAAO������
					   isThree=1;
					   record.jump3++;
					   record.live3++;
				}
				if((space1[k]==1&&b[k]==1&&space2[k]==1&&c[k]==1)||(space1[k+1]==1&&b[k+1]==1&&space2[k+1]==1&&c[k+1]==1)){//AOAOA����
					   record.go3++;
				}
				if((space1[k]==1&&b[k]==2&&((space2[k]>=1&&space1[k+1]==0)||(space2[k]==0&&space1[k+1]>=1)))||(space1[k+1]==1&&b[k+1]==2&&((space2[k+1]>=1&&space1[k]==0)||(space2[k+1]==0&&space1[k]>=1)))){//?AOAA?����
					   record.go3++;
				}
				if(space1[k]==1&&b[k]==1&&space1[k+1]>=2&&space2[k]>=2||space1[k+1]==1&&b[k+1]==1&&space2[k+1]>=2&&space1[k]>=2){//OAOAO�ͼ����
					   record.lowlive2++;
				}
				if(space1[k]==2&&b[k]==1&&space1[k+1]>=1&&space2[k]>=1||space1[k+1]==2&&b[k+1]==1&&space2[k+1]>=1&&space1[k]>=1){//?AOOA?�ͼ����
					   record.lowlive2++;
				}
				if(space1[k]==1&&b[k]==1&&space1[k+1]==0&&space2[k]>=2||space1[k+1]==1&&b[k+1]==1&&space2[k+1]>=2&&space1[k]==0){//?AOAOOorOOAOA?���
					   record.go2++;
				}
				if(space1[k]==2&&b[k]==1&&space1[k+1]==0&&space2[k]>=1||space1[k+1]==2&&b[k+1]==1&&space2[k+1]>=1&&space1[k]==0){//?AOOAOorOAOOA?���
					   record.go2++;
				}
				if(space1[k]==3&&b[k]==1||space1[k+1]==3&&b[k+1]==1){//?AOOOA?���
					   record.go2++;
				}
				if(space1[k]>=3&&space1[k+1]>=3){//OOOAOOO����
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
//ͨ��˫��ѭ������Score��0
for(i=0;i<SIZE;i++){
      for(j=0;j<SIZE;j++)
          Score[i][j]=0;
   }
}
//�˺�����¼���ӺͰ��ӵ����̷���
void RecordScore(int Score1[SIZE][SIZE],int Score2[SIZE][SIZE]){
	//��ʼ��˫�����̷�������Ϊ0
	initScore(Score1);
	initScore(Score2);
	//ͨ��˫��ѭ���������̷�������ֵ
	int i,j;
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			//�����ڵ�ǰλ���º���
			RecordAround1(i,j);
			//�����ڵ�ǰλ���º��ӣ���������Χ���Ƽ�¼�ڽṹrecord��
			struct point record=count();
			//�ж��ڸõ����Ӻ��Ƿ���γɽ���
			if(forbid()==0){//�������γɽ��֣���������
				if(record.isFive>=1||record.live4>=2)
						Score1[i][j]+=100000;//���Ӻ��ܳ����˫����+100000��
				if(record.live4==1||record.go4>=2||record.go4>=1&&record.live3>=1)
						Score1[i][j]+=10000;//���Ӻ��ܳɻ��Ļ�˫���Ļ���Ļ���+10000��
				if(record.live3>=2)
						Score1[i][j]+=5000;//���Ӻ��ܳ�˫����+5000��
				if(record.go3>=1&&record.live3>=1)
						Score1[i][j]+=1000;//���Ӻ��ܳ�һ����һ����+1000��
				if(record.go4>=1||record.go3>=2)
						Score1[i][j]+=500;//���Ӻ��ܳ�һ���Ļ�˫����+500��
				if(record.lowgo4>=1)
						Score1[i][j]+=400;//���Ӻ��ܳɵͼ�����+400��
				if(record.live3==1&&record.jump3==0)
						Score1[i][j]+=100;//���Ӻ��ܳɻ���������������+100��
				if(record.jump3==1)
						Score1[i][j]+=90;//���Ӻ��ܳ�������+90��
				if(record.live2>=2)
						Score1[i][j]+=50;//���Ӻ��ܳ�˫���+50��
				if(record.live2==1)
						Score1[i][j]+=10;//���Ӻ��ܳɻ��+10��
				if(record.lowlive2>=1)
						Score1[i][j]+=9;//���Ӻ��ܳɵͼ����+9��
				if(record.go3==1)
						Score1[i][j]+=5;//���Ӻ��ܳ�����+5��
				if(record.go2>=1)
						Score1[i][j]+=2;//���Ӻ��ܳɳ��+2��
				if(record.single>=1)
						Score1[i][j]+=1;//���Ӻ��ܳɵ���+1��
			}
			else//�����γɽ��֣���λ�ù�ֵΪ0
				Score1[i][j]=0;
			//�����ڵ�ǰλ���°���
			RecordAround2(i,j);
			//�����ڵ�ǰλ���°��ӣ���������Χ���Ƽ�¼�ڽṹrecord��
			record=count();
			if(record.isFive>=1||record.live4>=2)
					Score2[i][j]+=100000;//���Ӻ��ܳ����˫����+100000��
			if(record.live4==1||record.go4>=2||record.go4>=1&&record.live3>=1)
					Score2[i][j]+=10000;//���Ӻ��ܳɻ��Ļ�˫���Ļ���Ļ���+10000��
			if(record.live3>=2)
					Score2[i][j]+=5000;//���Ӻ��ܳ�˫����+5000��
			if(record.go3>=1&&record.live3>=1)
					Score2[i][j]+=1000;//���Ӻ��ܳ�һ����һ����+1000��
			if(record.go4>=1||record.go3>=2)
					Score2[i][j]+=500;//���Ӻ��ܳ�һ���Ļ�˫����+500��
			if(record.lowgo4>=1)
					Score2[i][j]+=400;//���Ӻ��ܳɵͼ�����+400��
			if(record.live3==1&&record.jump3==0)
					Score2[i][j]+=100;//���Ӻ��ܳɻ���������������+100��
			if(record.jump3==1)
					Score2[i][j]+=90;//���Ӻ��ܳ�������+90��
			if(record.live2>=2)
					Score2[i][j]+=50;//���Ӻ��ܳ�˫���+50��
			if(record.live2==1)
					Score2[i][j]+=10;//���Ӻ��ܳɻ��+10��
			if(record.lowlive2>=1)
					Score2[i][j]+=9;//���Ӻ��ܳɵͼ����+9��
			if(record.go3==1)
					Score2[i][j]+=5;//���Ӻ��ܳ�����+5��
			if(record.go2>=1)
					Score2[i][j]+=2;//���Ӻ��ܳɳ��+2��
			if(record.single>=1)
					Score2[i][j]+=1;//���Ӻ��ܳɵ���+1��
		}
	}
}
//�˺����жϵ�ǰ�����Ƿ���γɽ���
int forbid(){
	int k;
	//ͨ��ѭ�����ж��ĸ��������Ƿ���ڳ����ֻ���޽���
	for(k=0;k<=6;k=k+2){
      	if(a[k]+a[k+1]==4)
			return NO_FORBIDDEN;
	  	else if(a[k]+a[k+1]>4)
			return LONG_FORBIDDEN;
	}
	struct point record=count();
	//���ĸ����������������������ϴ��ڿ��ܳ�������ӣ����λ�ù������Ľ���
	if(record.count_4>=2)
		return FOUR_FOUR_FORBIDDEN;
	//���ĸ����������������������ϴ��ڿ��ܻ��������λ�ù�����������
	if(record.count_3>=2)
		return THREE_THREE_FORBIDDEN;
	//������������ɽ���
	else
		return NO_FORBIDDEN;
}






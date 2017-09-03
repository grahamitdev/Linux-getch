#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int getch();
void getPassword(char *pcPWD);
int getch()//无缓存 无回显
{
        struct termios tm, tm_old;
		int c;
        int fd = STDIN_FILENO;//0
        if(tcgetattr(fd, &tm) < 0)	//获取终端原始属性
        {
            return -1;
        }
        tm_old = tm;//备份终端原始属性
        cfmakeraw(&tm);	//就是将终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理。
        				//在原始模式下，终端是不可回显的，而且所有特定的终端输入/输出模式不可用。
        if(tcsetattr(fd, TCSANOW, &tm) < 0)	//不等数据传输完毕就立即改变属性   成功返回0   失败-1
        {
            return -1;
        }
        c = fgetc(stdin);
        if(tcsetattr(fd, TCSANOW, &tm_old) < 0)//回复属性
        {
            return -1;
        }
        return c;
}

void getPassword(char *pcPWD)
{
	int ch = 0;
	int i = 0;
	for(i = 0;i < 10;)//密码长度限制
	{
		ch=getch();//立即获取
		if(ch==-1)//获取失败，重来不记个数
		{
			continue;
		}
		if((ch == 127 || ch == 8) && i>0)//W || 删除键
		{
			putchar('\b');//光标左移1
			putchar(' '); //放一个空格
			putchar('\b');//光标再左移1
			i--;          //密码位数减1
		}
		else if(ch==10 || ch == 13)//HTab VTab
		{
			pcPWD[i]=0;//'\0'
			putchar('\n');
			return;//不加1
		}
		else if(i==0&&(ch==127 ||ch == 8))//开始输密码&&(W || 删除)
		{
			printf("\a");//响铃
		}
		else if(i<9)//正常收
		{
			pcPWD[i]=(char)ch;
			#if 1
			FILE *fp = fopen("pass.txt","a+");
			fprintf(fp,"%c",pcPWD[i]);
			fclose(fp);
			#endif
			putchar('*');
			i++;
		}
		else
		{
			printf("\a");//位数超限了
		}
	}
}

int main(void)
{
	char passwd[1024] = "";
	printf("passwd:");
	getPassword(passwd);
	printf("passwd = %s",passwd);

}

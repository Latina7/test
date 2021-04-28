#include<stdio.h>
int AvgBytesPerSec;
short Channel;
int SamplesPerSec;
int DataSize;
short BitsPerSample;
short PCM;
int factSize;
int fileData;
int fileInfoLen;
int attach;
void readFileinfo (char *nameStr)
{
    unsigned char ch[4];
    int temp;
    short t,fix;
    FILE *fp1=fopen(nameStr,"rb"); //�����ƶ���ʽ��ָ������Ƶ�ļ�if (fp1==NULL)
    if(fp1==NULL)
    {
        printf("can not open the file1");
        return ;
    }
    fread(ch,1,4, fp1);

    if(!(ch[0]=='R'&&ch[1]=='I'&&ch[2]=='F'&&ch[3]=='F'))
    {
        printf ("����RIFF�ļ�");
        return;
    }
    fread (&fileData,4,1,fp1);
    fread(ch,1,4,fp1);
    fread(ch,1,4, fp1);
    fread (&attach,4,1,fp1);
    fread(&t,2,1,fp1);

//�������࣬��ֵΪ1ʱ����ʾ����Ϊ����PCM����(����������)����һ�ֽ�ģ���źű任Ϊ�����źŵı��뷽ʽ���������
    if(t!=1)
    {
        PCM=0;
        printf("����PCM����");
        return;
    }
    PCM=1;
    fread(&Channel,2,1,fp1);
    fread(&SamplesPerSec,4,1,fp1);
    fread(&AvgBytesPerSec, 4,1, fp1);
    fread(&t,2,1,fp1);
    fread(&BitsPerSample,2,1,fp1);

    if (attach!=16)
    {
        fseek (fp1,2,SEEK_CUR);
        fix=2;
    }
    else
        fix=0;

    fread (ch,1,4,fp1);

//���ݱ�־��fact,�ò����ǿ�ѡ���֣�һ�㵱WAVE�ļ�����ĳЩ���ת������ʱ�������ò���

    if(ch[0]=='f'&&ch[1]=='a'&&ch[2]=='c'&&ch[3]=='t')
            {
                fread(&temp,4,1,fp1);
                factSize=temp+8;
                fseek (fp1,temp,SEEK_CUR) ;
                fread(ch,1,4,fp1);
                if(!(ch[0]=='d'&ch[1]=='a'&&ch[2]=='t'&&ch[3]=='a'))
    {
        printf ("��ʶ����ļ��ṹ");
            return;
        }
            }
        else if (ch[0]=='d'&&ch[1]=='a'&&ch[2]=='t'&&ch[3]=='a')
        factSize=0;

                 else if(!(ch[0]=='d'&&ch[1]=='a'&&ch[2]=='t'&&ch[3]=='a'))
            {
                printf ("��ʶ����ļ��ṹ");
                return;
            }
    fread (&DataSize,4,1,fp1);
    fileInfoLen=ftell(fp1);
    fileInfoLen=fix+factSize+44;
    fclose (fp1);
}
void cut (char *nameStr,int begin,int end)
{
    FILE *fp1=fopen(nameStr,"rb");
    FILE *fp2=fopen("changed.wav","wb");
    int start,finish,t;
    int newDataLen,keepLen;
    char ch[4];
    if(fp1==0||fp2==0)
    {
        printf("can not open the file2");
        return ;
    }
    unsigned char *pHead1;
    start=AvgBytesPerSec*begin;
    finish=AvgBytesPerSec*end;
    if(start<0||begin>end)
    {
        printf("start=%d\nfinish=%d\ndatasize=%d\n",start,finish,DataSize);
        printf ("����������Χ");
        return;
    }

    fread(ch,1,4,fp1);
    fwrite(ch,1,4,fp2);

    fseek(fp1,4,SEEK_CUR);

    keepLen=(end-begin+1)*AvgBytesPerSec;//
    newDataLen=fileInfoLen-8+keepLen;

    fwrite(&newDataLen,4,1,fp2);
    if(attach==16)
    t=0;
      else if(attach=18)
          t=2;
    if(factSize!=0)
        t=t+factSize;
    unsigned char *p=new unsigned char[28+t];
    fread(p,sizeof(char),28+t,fp1);
    fwrite(p,sizeof(char),28+t, fp2);

    fread(ch,1,4,fp1);
    fwrite(ch,1,4,fp2);
    fseek(fp1,4,SEEK_CUR);
    fwrite(&keepLen,4,1,fp1);
    fseek(fp1,start,SEEK_CUR);
    pHead1=new unsigned char[keepLen];
    fread(pHead1,sizeof(char),keepLen,fp1);
    fwrite(pHead1,sizeof(char),keepLen,fp2);
    fclose(fp1);
    fclose(fp2);
}
int main()
{
    char *nameStr="kiss.wav";
    readFileinfo(nameStr);
    cut(nameStr,5,29);
    return 0;
}



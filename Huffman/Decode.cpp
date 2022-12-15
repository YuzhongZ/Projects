#include<iostream.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define leafNumber 27
#define totalNumber 53
#define Maxvalue 3000

#include"Huffman.h"

void DecodeHFCode(HFTree& HT)                        //对报文进行解码
{               
    int k=HT.root;                                   //从根结点序号开始
    int len=0,i=0;
	char EnterCode[Maxvalue];
	char RealStr[Maxvalue];
	cout<<"请输入要解码的内容：";
	cin>>EnterCode;
    while(EnterCode[i]!='\0')
	{
        if(EnterCode[i]=='0')                        //判断数码是否为0,若为0则下一条路径通往左孩子
            k=HT.elem[k].lchild;
        else 
			if(EnterCode[i]=='1')                    //判断数码是否为1,若为1则下一条路径通往右孩子
               k=HT.elem[k].rchild;
            else                                         //密文存在错误
			{
                cout<<"\n错误! 密文中仅能含有1和0!\n\n";
                return;
			}
        if(HT.elem[k].lchild==-1&&HT.elem[k].rchild==-1) //若该结点不存在左右孩子，说明为该点对应字符
		{
            RealStr[len++]=HT.elem[k].data;
            k=HT.root;                                   //k重新置为根结点
        }
        i++;
    }
    RealStr[len]='\0';
    if(k==HT.root)                                      //输出解码结果
	{
        cout<<"解码结果:"<<RealStr<<endl;
        return;
    }
    cout<<"\n错误! 部分密文无法解密!\n\n";               
    return;
}
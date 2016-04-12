#include<stdio.h>
#include<math.h>
#include<string.h>
#include<conio.h>
#include<iostream.h>

unsigned long leftrotate(unsigned long,int);
double myabs(double x);

int main()
{
	clrscr();
	char tempchar;
	int m,h;
	char fileName[75];
	char message[1000];
	FILE *fp;
	char ch;
	unsigned long temp;
	unsigned long w[20];
	int n;
	unsigned long sineInt[64];
	int i;

	int r[64]={7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
	for(i=0;i<64;i++)
	{
		sineInt[i]=(unsigned long)floor(myabs(sin(i+1)*pow(2,32)));
	}
	printf("\nEnter name of the file containing message: ");
	scanf("%s",&fileName);
	fp=fopen(fileName,"r");


	i=0;
	while(!feof(fp))
	{
		ch=fgetc(fp);
		message[i]=ch;
		i++;
	}

	printf("\nThe content of the file is: %s",message);

	message[i-1]=0x80;
	message[i]='\0';

	int no_of_bits=strlen(message)*8;
	int message_length=no_of_bits-8;
	unsigned long msg_length=message_length;

	int bits_to_pad=((512-(no_of_bits%512))+448)%512;

	int j=0;
	for (j=0;j<bits_to_pad/8;j++)
	{
		message[i+j]=0x00;
	}

	message[i+j]='\0';
	i=i+j;

	unsigned char *ptrchar;
	ptrchar= (unsigned char *)(&msg_length);
	message[i]=ptrchar[0];
	message[i+1]=ptrchar[1];
	message[i+2]=ptrchar[2];
	message[i+3]=ptrchar[3];
	message[i+4]=0x00;
	message[i+5]=0x00;
	message[i+6]=0x00;
	message[i+7]=0x00;

	i=i+8;

	unsigned long A,B,C,D;

	A=0x67452301;
	B=0xefcdab89;
	C=0x98badcfe;
	D=0x10325476;

	unsigned long a,b,c,d,f,g;

	char message_chunk[70];
	int no_of_chunks=(i*8)/512;
	int k;
	for(j=0;j<no_of_chunks;j++)
	{
		for(k=0;k<64;k++)
		{
			message_chunk[k]=message[(j*64)+k];
		}

		for(n=0;n<16;n++)
		{
			w[n]=*((unsigned long *)(&message_chunk[n*4]));
		}


		a=A;
		b=B;
		c=C;
		d=D;

		for(k=0;k<64;k++)
		{
			if (k>=0 && k<=15)       //round one
			{
				f=(b & c) | ((~b) & d);
				g=k;
			}
			else if(k>=16 && k<=31)  //round two
			{
				f=(b & d) | (c & (~d));
				g=(5*k+1) % 16;
			}
			else if(k>=32 && k<=47)  //round three
			{
				f=b^c^d;
				g=(3*k+5)%16;
			}
			else if(k>=48 && k<=63)  //round four
			{
				f=c ^ (b | (~d));
				g=(7*k)%16;
			}
		  temp=d;
		  d=c;
		  c=b;
		  b=b+leftrotate((a+f+sineInt[k]+w[g]),r[k]);
		  a=temp;

		}
		A=A+a;
		B=B+b;
		C=C+c;
		D=D+d;
	}

	printf("\nThe MD5 hash is:");
	ptrchar=(unsigned char *)(&A);
	for (h=0;h<4;h++)
		printf("%0.2X",ptrchar[h]);
	cout<<" ";

	ptrchar=(unsigned char *)(&B);
	for (h=0;h<4;h++)
		printf("%0.2X",ptrchar[h]);
	cout<<" ";

	ptrchar=(unsigned char *)(&C);
	for (h=0;h<4;h++)
		printf("%0.2X",ptrchar[h]);
	cout<<" ";

	ptrchar=(unsigned char *)(&D);
	for (h=0;h<4;h++)
		printf("%0.2X",ptrchar[h]);

	 getch();
	return 0;
}



//This function circularly left-shifts 'x' by 'c' bits
unsigned long leftrotate(unsigned long x,int c)
{
	return ((x<<c) | (x>>(32-c)));
}
double myabs(double x)
{
		 if (x<0)
				return -1*x;
		 else
				 return x;
}
#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
const int w=256,h=256,Ps=w*h*3; //Ps:总像素数*通道数
const double pi=acos(-1);
const int Qy[8][8]={
{16,11,10,16,24,40,51,61},
{12,12,14,19,26,58,60,55},
{14,13,16,24,40,57,69,56},
{14,17,22,29,51,87,80,62},
{18,22,37,56,68,109,103,77},
{24,35,55,64,81,104,113,92},
{49,64,78,87,103,121,120,101},
{72,92,95,98,112,100,103,99}
},
Qc[8][8]={
{17,18,24,47,99,99,99,99},
{18,21,26,66,99,99,99,99},
{24,26,56,99,99,99,99,99},
{47,66,99,99,99,99,99,99},
{99,99,99,99,99,99,99,99},
{99,99,99,99,99,99,99,99},
{99,99,99,99,99,99,99,99},
{99,99,99,99,99,99,99,99}
};//量化表
const int
YDCL[12]={2,3,3,3,3,3,4,5,6,7,8,9},
YDC[12]={0,2,3,4,5,6,14,30,62,126,254,510},
CDCL[12]={2,2,2,3,4,5,6,7,8,9,10,11},
CDC[12]={0,1,2,6,14,30,62,126,254,510,1022,2046},
YACL[162]={4,2,2,3,4,5,7,8,10,16,16,4,5,7,9,11,16,16,16,16,16,5,8,10,12,16,16,16,16,16,16,6,9,12,16,16,16,16,16,16,16,6,10,16,16,16,16,16,16,16,16,7,11,16,16,16,16,16,16,16,16,7,12,16,16,16,16,16,16,16,16,8,12,16,16,16,16,16,16,16,16,9,15,16,16,16,16,16,16,16,16,9,16,16,16,16,16,16,16,16,16,9,16,16,16,16,16,16,16,16,16,10,16,16,16,16,16,16,16,16,16,10,16,16,16,16,16,16,16,16,16,11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11,16,16,16,16,16,16,16,16,16,16},
YAC[162]={10,0,1,4,11,26,120,248,1014,65410,65411,12,27,121,502,2038,65412,65413,65414,65415,65416,28,249,1015,4084,65417,65418,65419,65420,65421,65422,58,503,4085,65423,65424,65425,65426,65427,65428,65429,59,1016,65430,65431,65432,65433,65434,65435,65436,65437,122,2039,65438,65439,65440,65441,65442,65443,65444,65445,123,4086,65446,65447,65448,65449,65450,65451,65452,65453,250,4087,65454,65455,65456,65457,65458,65459,65460,65461,504,32704,65462,65463,65464,65465,65466,65467,65468,65469,505,65470,65471,65472,65473,65474,65475,65476,65477,65478,506,65479,65480,65481,65482,65483,65484,65485,65486,65487,1017,65488,65489,65490,65491,65492,65493,65494,65495,65496,1018,65497,65498,65499,65500,65501,65502,65503,65504,65505,2040,65506,65507,65508,65509,65510,65511,65512,65513,65514,65515,65516,65517,65518,65519,65520,65521,65522,65523,65524,2041,65525,65526,65527,65528,65529,65530,65531,65532,65533,65534},
CACL[162]={2,2,3,4,5,5,6,7,9,10,12,4,6,8,9,11,12,16,16,16,16,5,8,10,12,15,16,16,16,16,16,5,8,10,12,16,16,16,16,16,16,6,9,16,16,16,16,16,16,16,16,6,10,16,16,16,16,16,16,16,16,7,11,16,16,16,16,16,16,16,16,7,11,16,16,16,16,16,16,16,16,8,16,16,16,16,16,16,16,16,16,9,16,16,16,16,16,16,16,16,16,9,16,16,16,16,16,16,16,16,16,9,16,16,16,16,16,16,16,16,16,9,16,16,16,16,16,16,16,16,16,11,16,16,16,16,16,16,16,16,16,14,16,16,16,16,16,16,16,16,16,10,15,16,16,16,16,16,16,16,16,16},
CAC[162]={0,1,4,10,24,25,56,120,500,1014,4084,11,57,246,501,2038,4085,65416,65417,65418,65419,26,247,1015,4086,32706,65420,65421,65422,65423,65424,27,248,1016,4087,65425,65426,65427,65428,65429,65430,58,502,65431,65432,65433,65434,65435,65436,65437,65438,59,1017,65439,65440,65441,65442,65443,65444,65445,65446,121,2039,65447,65448,65449,65450,65451,65452,65453,65454,122,2040,65455,65456,65457,65458,65459,65460,65461,65462,249,65463,65464,65465,65466,65467,65468,65469,65470,65471,503,65472,65473,65474,65475,65476,65477,65478,65479,65480,504,65481,65482,65483,65484,65485,65486,65487,65488,65489,505,65490,65491,65492,65493,65494,65495,65496,65497,65498,506,65499,65500,65501,65502,65503,65504,65505,65506,65507,2041,65508,65509,65510,65511,65512,65513,65514,65515,65516,16352,65517,65518,65519,65520,65521,65522,65523,65524,65525,1018,32707,65526,65527,65528,65529,65530,65531,65532,65533,65534};
//Huffman表(长度+编码)
unsigned char RGB[Ps+5];
unsigned char RES[(Ps<<6)+5];
double YCC[Ps+5],DYCC[Ps+5];
int MCnt,DRES[Ps+5],ZRES[Ps+5],Cnt[Ps/(8*8)+5],L[(1<<15)+5];
int q[17];
int Rlen;
std::vector <int> ans;
struct RMa{
	int c,x;
	int l,h,s;
}RRES[Ps/(8*8)+5][8*8];
int Abs(int x){return x<0?-x:x;}
namespace In{
	void ReadFile(const char *FileName,unsigned char *buf,int Len){
		FILE *fp=fopen(FileName,"rb");
		fread(buf,1,Len,fp),fclose(fp);
	}
} //读入二进制文件
namespace Task1{
	void Work(){
		for(int i=0;i<Ps;i+=3){
			int r=RGB[i],g=RGB[i+1],b=RGB[i+2];
			YCC[i]=0.299*r+0.587*g+0.114*b,
			YCC[i+1]=0.5*r-0.4187*g-0.0813*b+128.0,
			YCC[i+2]=-0.1687*r-0.3313*g+0.5*b+128.0;
		}
		//for(int i=0;i<Ps;++i) printf("%lf ",YCC[i]);
	}
} //RGB转YCrCb
int Loc(int x,int y,int c){return 3*(x*w+y)+c;}
namespace Task2{
	double C(int x){return x?1.0:1.0/sqrt(2.0);}
	double f(int x,int y,int u,int v,int c){
		double res=0.0;
		for(int i=0;i<8;++i){
			for(int j=0;j<8;++j){
				res+=DYCC[Loc(x+i,y+j,c)]*cos((i+i+1)*u*pi/16.0)*cos((j+j+1)*v*pi/16.0);
			}
		}
		return res;
	}
	void DCT(){
		for(int i=0;i<Ps;++i) DYCC[i]=YCC[i]-128.0;
		int i,j; //(i,j)为矩阵左上角坐标
		for(int c=0;c<3;++c){ //c=0:Y通道;c=1:Cr通道;c=2:Cb通道
			i=j=0;
			while(i<h){
				//printf("%d %d\n",i,j);
				for(int u=0;u<8;++u){
					for(int v=0;v<8;++v){
						double t=0.25*C(u)*C(v)*f(i,j,u,v,c);
						if(c) DRES[Loc(i+u,j+v,c)]=(int)floor(t/(double)Qc[u][v]+0.5);
						else DRES[Loc(i+u,j+v,c)]=(int)floor(t/(double)Qy[u][v]+0.5);
					}
				}
				j+=8;
				if(j>=w) i+=8,j=0;
			}
		}
	}
} //离散余弦变换+量化
namespace Task3{
	void Zigzag(){
		int i,j,cnt=0;
		for(int c=0;c<3;++c){
			i=j=0;
			while(i<h){
				int u=0,v=0,f=1;
				while(u<8&&v<8){
					ZRES[cnt++]=DRES[Loc(i+u,j+v,c)];
					if(f){
						if(v==7&&u&1){
							f=0,++u;
						}
						else if(!u&&!(v&1)){
							f=0,++v;
						}
						else --u,++v;
					}
					else{
						if(u==7&&!(v&1)){
							f=1,++v;
						}
						else if(!v&&u&1){
							f=1,++u;
						}
						else ++u,--v;
					}
				}
				j+=8;
				if(j>=w) i+=8,j=0;
			}
		}
	}
	void ChaFen(){
		for(int c=0;c<3;++c){
			for(int i=w*h*(c+1)-8*8;i>=w*h*c+8*8;i-=8*8){
				ZRES[i]-=ZRES[i-8*8];
			}
		}
	} //差分
	void Work(){Zigzag(),ChaFen();}
} //Zigzag排列+差分
namespace Task4{
	void RLE(){
		int j=0;
		for(int i=0;i<Ps;i+=8*8,++j){
			int pos=i,rpos=i+8*8;
			while(!ZRES[rpos-1]&&rpos>pos) --rpos; //rpos定位末尾0
			//printf("s%d\n",rpos);
			while(pos<rpos){
				int ed=pos-1;
				while(ed+1<i+8*8&&!ZRES[ed+1]&&ed-pos+1<15) ++ed;
				RRES[j][Cnt[j]++]=(RMa){ed-pos+1,ZRES[ed+1]};
				pos=ed+2;
			}
			RRES[j][Cnt[j]++]=(RMa){0,0};
			//for(int k=0;k<Cnt[j];++k) printf("s%d %d\n",RRES[k].c,RRES[k].x);
		}
		MCnt=j;
	} //游程编码
	void Lengthen(){
		for(int i=0,j=1,l=0;i<(1<<15);++i){
			if(i==j) j<<=1,++l;
			L[i]=l;
		} //预处理每个绝对值的编码长度
		for(int i=0;i<MCnt;++i){
			for(int j=0;j<Cnt[i];++j){
				int x=RRES[i][j].x;
				RRES[i][j].l=L[Abs(x)];
				if(x<0){
					x+=(1<<RRES[i][j].l)-1;
				}
				else if(x>0){
					++x;
				}
				else x=-1;
				RRES[i][j].s=x; //(c,l)+s
			}
		}
	} //变长编码
	void Work(){RLE(),Lengthen();}
} //游程编码+变长编码
namespace Task5{
	void Insert(int l,int x){
		int tl=0;
		for(int i=0;i<l;++i) q[++tl]=x&1,x>>=1;
		for(int i=tl;i;--i) ans.push_back(q[i]);
	}
	void Rep(int i){
		RMa u=RRES[i][0];
		Insert(CDCL[u.l],CDC[u.l]);
		Insert(u.l,u.s);
		for(int j=1;j<Cnt[i];++j){
			u=RRES[i][j];
			int id;
			if(u.c==15) id=151+u.l;
			else id=u.c*10+u.l;
			Insert(CACL[id],CAC[id]);
			Insert(u.l,u.s);
		}
	} //Cr,Cb
	void Huffman(){
		int DMCnt=MCnt/3;
		for(int i=0;i<DMCnt;++i){
			RMa u=RRES[i][0];
			Insert(YDCL[u.l],YDC[u.l]);
			Insert(u.l,u.s);
			for(int j=1;j<Cnt[i];++j){
				u=RRES[i][j];
				int id;
				if(u.c==15) id=151+u.l;
				else id=u.c*10+u.l; //参考推荐huffman表
				Insert(YACL[id],YAC[id]);
				Insert(u.l,u.s);
			} //Y
			Rep(i+DMCnt),Rep(i+DMCnt+DMCnt);
		}
		/*for(int i=DMCnt;i<MCnt;++i){

		}*/
		//for(int i=0;i<(int)ans.size();++i) printf("%d",ans[i]);
	}
} //范式Huffman编码
namespace Out{
	struct H{
		int l,v;
		int operator <(const H &p)const{return l<p.l||(l==p.l&&v<p.v);}
	}qq[162+5];
	std::vector <int> HYDC,HCDC,HCAC,HYAC;
	std::vector <int> _C[17];
	void WriteFile(const char *FileName,unsigned char *duf,int Len){
		FILE *fp=fopen(FileName,"wb");
		fwrite(duf,1,Len,fp),fclose(fp);
	} //输出为二进制文件
	int G(int x){
		if(x<=10) return x;
		if(x<=150){
			int l=(x-1)/10,r=x-l*10;
			return (l<<4)+r;
		}
		x-=151;
		return (15<<4)+x;
	} //参见Line 210,反推
	void Sort(const int *L,int r,std::vector <int> &res,int o){
		for(int i=1;i<=16;++i) _C[i].clear();
		for(int i=0;i<r;++i) _C[L[i]].push_back(i);
		for(int i=1;i<=16;++i) res.push_back((int)_C[i].size());
		for(int i=1;i<=16;++i){
			for(int j=0;j<(int)_C[i].size();++j){
				if(o==1) res.push_back(_C[i][j]);
				else res.push_back(G(_C[i][j]));
			}
		}
	}
	void Deal_Huffman(){
		Sort(YDCL,12,HYDC,1),
		Sort(CDCL,12,HCDC,1),
		Sort(YACL,162,HYAC,2),
		Sort(CACL,162,HCAC,2);
	} //排列huffman表
	void Push(unsigned char x){RES[Rlen++]=x;}
	void Write_Huffman(int k,std::vector <int> res){
		Push(0xff),Push(0xc4),
		Push((int)res.size()+3),
		Push(k);
		//freopen("test1.out","w",stdout);
		for(int i=0;i<(int)res.size();++i) Push(res[i]);
	}
	void ZPrint(const int (*a)[8]){
		int u=0,v=0,f=1;
		//freopen("test1.out","w",stdout);
		while(u<8&&v<8){
			//printf("%d %d %d\n",u,v,a[u][v]);
			Push(a[u][v]);
			if(f){
				if(v==7&&u&1){
					f=0,++u;
				}
				else if(!u&&!(v&1)){
					f=0,++v;
				}
				else --u,++v;
			}
			else{
				if(u==7&&!(v&1)){
					f=1,++v;
				}
				else if(!v&&u&1){
					f=1,++u;
				}
				else ++u,--v;
			}
		}
	} //以Zigzag形式排列量化表
	void Work(){
		Push(0xff),Push(0xd8);
		Push(0xff),Push(0xe0),
		Push(0x00),Push(0x10),
		Push(0x4a),Push(0x46),Push(0x49),Push(0x46),
		Push(0x00),
		Push(0x01),Push(0x02),//
		Push(0x01),
		Push(0x00),Push(0x40),Push(0x00),Push(0x40),//
		Push(0x00),Push(0x00);
		Push(0xff),Push(0xdb),
		Push(0x00),Push(0x43),
		Push(0x00);
		ZPrint(Qy);//for(int i=0;i<8;++i) for(int j=0;j<8;++j) Push(Qy[i][j]);
		Push(0xff),Push(0xdb),
		Push(0x00),Push(0x43),
		Push(0x01);
		ZPrint(Qc);//for(int i=0;i<8;++i) for(int j=0;j<8;++j) Push(Qc[i][j]);
		Push(0xff),Push(0xc0),
		Push(0x00),Push(0x11),
		Push(0x08),
		Push(0x01),Push(0x00),Push(0x01),Push(0x00),
		Push(0x03);
		Push(0x01),Push(0x11),Push(0x00),
		Push(0x02),Push(0x11),Push(0x01),
		Push(0x03),Push(0x11),Push(0x01);
		Deal_Huffman();
		Write_Huffman(0x00,HYDC);
		Write_Huffman(0x10,HYAC);
		Write_Huffman(0x01,HCDC);
		Write_Huffman(0x11,HCAC);
		Push(0xff),Push(0xda);
		Push(0x00),Push(0x0c),
		Push(0x03),
		Push(0x01),Push(0x00),
		Push(0x02),Push(0x11),
		Push(0x03),Push(0x11);
		Push(0x00),Push(0x3f),Push(0x00);
		while((int)ans.size()&3) ans.push_back(1);
		//puts("233");
		for(int i=0;i<(int)ans.size();i+=8){
			int t=0;
			for(int j=0;j<8;++j){
				t+=ans[i+j]<<(7-j);
			}
			//printf("%d ",t);
			Push(t);
			if(t==0xff) Push(0x00);
		}
		Push(0xff),Push(0xd9);
	}
} //带文件头、文件尾，拼接输出
int main(){
	In::ReadFile("E:/QSC2022-Autumn-Round-Two/What-is-JPEG/demo.bin",RGB,Ps);
	Task1::Work();
	Task2::DCT();
	//for(int i=0;i<Ps;++i) printf("%d ",RGB[i]);
	//freopen("test1.out","w",stdout);
	/*int i,j;
	for(int c=0;c<3;++c){
		i=j=0;
		while(i<h){
			for(int u=0;u<8;++u){
				for(int v=0;v<8;++v){
					//printf("%d ",RGB[Loc(i+u,j+v,2)]);
					//printf("s%d %d\n",i+u,j+v);
					printf("%d ",DRES[Loc(i+u,j+v,1)]);
				}
				puts("");
			}
			puts("");
			j+=8;
			if(j>=w) i+=8,j=0;
		}
	}*/
	Task3::Work();
	//for(int i=0;i<Ps;++i) printf("%d ",ZRES[i]);
	Task4::Work();
	//printf("%d\n",MCnt);
	/*for(int i=0;i<Cnt[0];++i){
		printf("%d %d %d ",i,RRES[0][i].c,RRES[0][i].x);
		for(int j=0;j<RRES[0][i].l;++j) printf("%d",RRES[0][i].s);
		puts("");
	}*/
	Task5::Huffman();
	Out::Work();
	Out::WriteFile("E:/QSC2022-Autumn-Round-Two/What-is-JPEG/a.jpeg",RES,Rlen);
	return 0;
}
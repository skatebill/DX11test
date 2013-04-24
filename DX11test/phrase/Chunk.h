#pragma once
#include"FileProcessor.h"
#include<vector>
using namespace std;
#define MAINCHUNK		0xabcd
#define MATRIALGROUPCHUNK	0x4000
#define MATRIALCHUNK		0x4001
#define MESHGROUPCHUNK		0x5000
#define MESHCHUNK			0x5001
#define BONEGROUPCHUNK		0x6000
#define BONECHUNK			0x6001
#define CHUNK_HEAD_SIZE	6
namespace MYCHUNK{
	struct point3{
		float x,y,z;
		point3(float _x = 0,float _y = 0,float _z = 0){ x=_x; y=_y; z=_z;}
	};
	struct point2{
		float u,v;
		point2(float _u = 0,float _v = 0){ u=_u; v=_v;}
	};
	struct point4{
		float x,y,z,w;
		point4(float _x = 0,float _y = 0,float _z = 0,float _w=0){ x=_x; y=_y; z=_z;w=_w;}
	};
	struct face{
		WORD a,b,c;
		WORD ta,tb,tc;
		WORD na,nb,nc;
		WORD ca,cb,cc;
		face(WORD _a = 0,WORD _b = 0,WORD _c = 0){
			a=_a; b=_b; c=_c;
			ta=0; tb=0; tc=0;
			na=0; nb=0; nc=0;
			ca=0; cb=0; cc=0;
		}
		void setTexCoordFace(WORD _a = 0,WORD _b = 0,WORD _c = 0){ ta=_a; tb=_b; tc=_c;}
		void setNormalFace(WORD _a = 0,WORD _b = 0,WORD _c = 0){ na=_a; nb=_b; nc=_c;}
		void setColorFace(WORD _a = 0,WORD _b = 0,WORD _c = 0){ ca=_a; cb=_b; cc=_c;}
	};
	struct boneTM{
		point3 translate;
		point4 rotate;
	};
	struct boneSample{
		WORD key;
		boneTM tm;
	};
	struct boneInfo{
		WORD id;
		float weight;
	};
	struct vertex{
		point3 pos;
		vector<boneInfo*> vertexBoneInfo;
		vertex(float _x = 0,float _y = 0,float _z = 0){ pos=point3(_x,_y,_z);}
	};
	struct ChunkHead{
		WORD id;
		DWORD length;
	};
	class chunk{
	public:
		WORD	id;
		DWORD	length;

		chunk():id(0),length(0){}
		virtual int computeLength()=0;
	};

	class ChunkContainer:public chunk{
	public:
		vector<chunk*>	subchunk;
		ChunkContainer():subchunk(0),chunk(){}
		virtual int computeLength(){
			length=CHUNK_HEAD_SIZE;
			vector<chunk*>::iterator t=subchunk.begin();
			while(t!=subchunk.end())
			{
				length+=(*t)->computeLength();
				t++;
			}
			return length;
		}
		virtual void addSunChunk(chunk& c){
			subchunk.push_back(&c);
		}

		int getSunCount(){return subchunk.size();}
	};
	class mainChunk:public ChunkContainer{
	public:
		mainChunk():ChunkContainer(){id=MAINCHUNK;}
	};

	class matrialGroupChunk:public ChunkContainer{
	public:
		matrialGroupChunk():ChunkContainer(){id=MATRIALGROUPCHUNK;}

	};

	class meshGroupChunk:public ChunkContainer{
	public:
		meshGroupChunk():ChunkContainer(){id=MESHGROUPCHUNK;}
	};

	class boneGroupChunk:public ChunkContainer{
	public:
		boneGroupChunk():ChunkContainer(){id=BONEGROUPCHUNK;}
	};


	class boneChunk:public chunk{
	public:
		boneChunk():chunk(){
			id=BONECHUNK;
			boneID = 0;
		}
		WORD	boneID;
		boneTM posInfo;
		vector<boneSample*> samples;
		int computeLength(){
			length=CHUNK_HEAD_SIZE;
			//bone id
			length+=2;
			//posInfo
			length+=7*4;
			//samples
			length+=4;
			length+=(2+7*4)*samples.size();
			return length;
		}
	};

	class matrialChunk:public chunk{
	public:
		matrialChunk():chunk(){
			id=MATRIALCHUNK;
			matName="";
			hasTexture=false;
			textureName=0;
		}
		char* matName;

		point4 amb;
		point4 dif;
		point4 spec;

		bool hasTexture;
		char* textureName;
		int computeLength(){
			length=CHUNK_HEAD_SIZE;
			//mat name
			length+=strlen(matName)+1;
			//amb
			length+=4*4;
			//dif
			length+=4*4;
			//spec
			length+=4*4;
			//tex
			length+=1;
			if(hasTexture&&textureName)
				length+=strlen(textureName)+1;
			return length;

		}

	};

	class meshChunk:public chunk{
	public:
		meshChunk():chunk(){
			id=MESHCHUNK;
			matName="";
		}

		char* matName;
		vector<vertex*>	vertexlist;
		vector<point2*>	texcoordlist;
		vector<point3*>	normallist;
		vector<point4*>	colorlist;
		vector<face*>	indexlist;

		int computeLength(){
			//head
			length=CHUNK_HEAD_SIZE;
			//mat id
			length+=strlen(matName)+1;
			//num part
			length+=4*5;
			//bool compute buffer
			length+=vertexlist.size()*3*4;
			int num=vertexlist.size();
			for(int i=0;i<num;i++)
			{
				length+=2;
				length+=vertexlist[i]->vertexBoneInfo.size()*6;
			}
			//tex
			length+=texcoordlist.size()*2*4;
			//normal
			length+=normallist.size()*3*4;
			//color
			length+=colorlist.size()*4*4;
			//index
			length+=indexlist.size()*3*2*4;
			return length;

		}
	};
	class chunkWriter:public FileWriter{
	private:
		void saveChunkHead(chunk& chunk){
			writeWord(chunk.id);
			writeDWord(chunk.length);
		}
		void saveChunk(chunk& chunk);
		void saveMainChunk(mainChunk& chunk);	
		void saveMeshChunk(meshChunk& chunk);
		void saveMatrialChunk(matrialChunk& chunk);
		void saveBoneChunk(boneChunk& chunk);
		void saveSubChunk(ChunkContainer& container);
	public :
		chunkWriter():FileWriter(){}
		virtual ~chunkWriter(){}
		bool saveChunkTo(mainChunk &source,char* dstFile);


	};
	class chunkReader:public FileReader{
	private:


		void skipChunk(ChunkHead&);
		ChunkHead readChunkHead();
		mainChunk* readMainChunk();	
		void readMeshGroupChunk(ChunkHead &grouphead,mainChunk &chunk);
		void readMatrialGroupChunk(ChunkHead &grouphead,mainChunk &chunk);
		void readBoneGroupChunk(ChunkHead &meshhead,mainChunk &chunk);

		void readMeshChunk(ChunkHead &meshhead,meshGroupChunk &chunk);
		void readMatrialChunk(ChunkHead &meshhead,matrialGroupChunk &chunk);
		void readBoneChunk(ChunkHead &meshhead,boneGroupChunk &chunk);
	public:
		chunkReader():FileReader(){}
		virtual ~chunkReader(){}

		mainChunk* readFile(char* dstFile);

	};


}
using namespace MYCHUNK;

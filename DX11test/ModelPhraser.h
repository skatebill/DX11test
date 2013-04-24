#pragma once
#include"ModelGroup.h"
#include"phrase\Chunk.h"
#include "BonedMeshGroup.h"
#include<fstream>
chunkReader reader;
void fillPos(XMFLOAT3& dst,point3& src){
	dst.x=src.x;
	dst.y=src.y;
	dst.z=src.z;
}
void fillNormal(XMFLOAT3& dst,point3& src){
	fillPos(dst,src);
}
void fillTex(XMFLOAT2& dst,point2& src){
	dst.x=src.u;
	dst.y=src.v;
}

ModelGroup* loadPNUModel(char* filename,ID3D11Device* device)
{
	ModelGroup* result=new ModelGroup(device);
	mainChunk* data = reader.readFile(filename);
	meshGroupChunk* meshG = (meshGroupChunk*)data->subchunk[0];
	for(vector<chunk*>::iterator ite=meshG->subchunk.begin();ite!=meshG->subchunk.end();ite++)
	{
		meshChunk* mesh=(meshChunk*)(*ite);
		int	facenum=mesh->indexlist.size();
		int vertexNum=facenum*3;
		VertexPNU* vertex=new VertexPNU[vertexNum];
		WORD*	index=new WORD[vertexNum];
		for(int i=0;i<facenum;i++)
		{
			index[i]=i;
			fillPos(vertex[3*i+0].pos,mesh->vertexlist[mesh->indexlist[i]->a]->pos);
			fillNormal(vertex[3*i+0].normal,*mesh->normallist[mesh->indexlist[i]->na]);
			fillTex(vertex[3*i+0].tex,*mesh->texcoordlist[mesh->indexlist[i]->ta]);
			
			
			fillPos(vertex[3*i+1].pos,mesh->vertexlist[mesh->indexlist[i]->b]->pos);
			fillNormal(vertex[3*i+1].normal,*mesh->normallist[mesh->indexlist[i]->nb]);
			fillTex(vertex[3*i+1].tex,*mesh->texcoordlist[mesh->indexlist[i]->tb]);
			
			fillPos(vertex[3*i+2].pos,mesh->vertexlist[mesh->indexlist[i]->c]->pos);
			fillNormal(vertex[3*i+2].normal,*mesh->normallist[mesh->indexlist[i]->nc]);
			fillTex(vertex[3*i+2].tex,*mesh->texcoordlist[mesh->indexlist[i]->tc]);
		}
		ModelMesh* m=new ModelMesh(device);
		m->setVertexSource(vertex,sizeof(VertexPNU),vertexNum);
		m->setIndexSource(index,vertexNum);
		result->addMesh(m);
	}
	return result;
}


ModelGroup* loadPUModel(char* filename,ID3D11Device* device)
{
	ModelGroup* result=new ModelGroup(device);
	mainChunk* data = reader.readFile(filename);
	meshGroupChunk* meshG = (meshGroupChunk*)data->subchunk[0];
	for(vector<chunk*>::iterator ite=meshG->subchunk.begin();ite!=meshG->subchunk.end();ite++)
	{
		meshChunk* mesh=(meshChunk*)(*ite);
		int	facenum=mesh->indexlist.size();
		int vertexNum=facenum*3;
		VertexPU* vertex=new VertexPU[vertexNum];
		WORD*	index=new WORD[vertexNum];
		for(int i=0;i<facenum;i++)
		{
			index[3*i+0]=3*i+0;
			index[3*i+1]=3*i+1;
			index[3*i+2]=3*i+2;
			fillPos(vertex[3*i+0].pos,mesh->vertexlist[mesh->indexlist[i]->a]->pos);
			fillTex(vertex[3*i+0].tex,*mesh->texcoordlist[mesh->indexlist[i]->ta]);
			
			
			fillPos(vertex[3*i+1].pos,mesh->vertexlist[mesh->indexlist[i]->b]->pos);
			fillTex(vertex[3*i+1].tex,*mesh->texcoordlist[mesh->indexlist[i]->tb]);
			
			fillPos(vertex[3*i+2].pos,mesh->vertexlist[mesh->indexlist[i]->c]->pos);
			fillTex(vertex[3*i+2].tex,*mesh->texcoordlist[mesh->indexlist[i]->tc]);
		}
		ModelMesh* m=new ModelMesh(device);
		m->setVertexSource(vertex,sizeof(VertexPU),vertexNum);
		m->setIndexSource(index,vertexNum);
		result->addMesh(m);
	}
	return result;
}




BonedMeshGroup* loadPUBModel(char* filename,ID3D11Device* device)
{
	BonedMeshGroup* result=new BonedMeshGroup(device);
	mainChunk* data = reader.readFile(filename);
	meshGroupChunk* meshG = (meshGroupChunk*)data->subchunk[0];
	matrialGroupChunk* matG = (matrialGroupChunk*)data->subchunk[1];
	boneGroupChunk* boneGC = (boneGroupChunk*)data->subchunk[2];
	BoneGroup* boneG=new BoneGroup();
	result->setBone(boneG);
	fstream log("log.txt",std::fstream::out);


	map<int,bone*> boneTest;
	for(vector<chunk*>::iterator ite=boneGC->subchunk.begin();ite!=boneGC->subchunk.end();ite++)
	{
		boneChunk* boneC=(boneChunk*)(*ite);
		bone* b=new bone();
		b->boneid=boneC->boneID;
		b->startState=boneC->posInfo;
		boneTest[b->boneid]=b;
	//	log<<b->boneid<<endl;
	//	log<<"\t"<<b->startState.translate.x<<" "<<b->startState.translate.y<<" "<<b->startState.translate.z<<" "<<endl;
	//	log<<"\t"<<b->startState.rotate.w<<" "<<b->startState.rotate.x<<" "<<b->startState.rotate.y<<" "<<b->startState.rotate.z<<" "<<endl;
		for(vector<boneSample*>::iterator bite=boneC->samples.begin();bite!=boneC->samples.end();bite++)
		{
			boneSample* samples=new boneSample();
			memcpy(samples,*bite,sizeof(boneSample));
		/*	log<<"\t"<<samples->key<<endl;
			log<<"\t\t"<<samples->tm.translate.x<<" "<<samples->tm.translate.y<<" "<<samples->tm.translate.z<<" "<<endl;
			log<<"\t\t"<<samples->tm.rotate.w<<" "<<samples->tm.rotate.x<<" "<<samples->tm.rotate.y<<" "<<samples->tm.rotate.z<<" "<<endl;*/
			b->boneSamples.push_back(samples);
		}
		boneG->addBone(b);
	}
	for(vector<chunk*>::iterator ite=meshG->subchunk.begin();ite!=meshG->subchunk.end();ite++)
	{
		meshChunk* mesh=(meshChunk*)(*ite);
		int	facenum=mesh->indexlist.size();
		int vertexNum=facenum*3;
		VertexPUB* vertexbuf=new VertexPUB[vertexNum];
		WORD*	index=new WORD[vertexNum];
		vector<int> boneIdList;
		map<int,int> boneMap;
		int bonePos=1;
		for(vector<vertex*>::iterator vite=mesh->vertexlist.begin();vite!=mesh->vertexlist.end();vite++)
		{			
			for(vector<boneInfo*>::iterator bite=(*vite)->vertexBoneInfo.begin();bite!=(*vite)->vertexBoneInfo.end();bite++)
			{
				if(!boneMap[(**bite).id])
				{
					boneMap[(**bite).id]=bonePos++;
				}
			}
		}
		int num =boneMap.size();
		int* ids=new int[num];
		for(map<int,int>::iterator mapite=boneMap.begin();mapite!=boneMap.end();mapite++)
		{
			ids[mapite->second-1]=mapite->first;
		}
		int keys=80;
		for(int i=0;i<facenum;i++)
		{
			index[3*i+0]=3*i+0;
			index[3*i+1]=3*i+1;
			index[3*i+2]=3*i+2;
			ZeroMemory(vertexbuf[3*i+0].boneWeight,sizeof(float)*BONE_MAX_NUM);
			ZeroMemory(vertexbuf[3*i+1].boneWeight,sizeof(float)*BONE_MAX_NUM);
			ZeroMemory(vertexbuf[3*i+2].boneWeight,sizeof(float)*BONE_MAX_NUM);
			vertex* a=mesh->vertexlist[mesh->indexlist[i]->a];
			vertex* b=mesh->vertexlist[mesh->indexlist[i]->b];
			vertex* c=mesh->vertexlist[mesh->indexlist[i]->c];

			XMVECTOR pa=XMVectorSet(a->pos.x,a->pos.y,a->pos.z,1);
			XMVECTOR pb=XMVectorSet(b->pos.x,b->pos.y,b->pos.z,1);
			XMVECTOR pc=XMVectorSet(c->pos.x,c->pos.y,c->pos.z,1);
			
			XMVECTOR da=pa;
			XMVECTOR db=pb;
			XMVECTOR dc=pc;

	/*		XMVECTOR da=XMVectorSet(0,0,0,0);
			XMVECTOR db=XMVectorSet(0,0,0,0);
			XMVECTOR dc=XMVectorSet(0,0,0,0);
			
			
			for(int i=0;i<a->vertexBoneInfo.size();i++)
			{
				
				XMMATRIX t=XMMatrixIdentity();
				XMVECTOR r1=XMVectorSet(boneTest[a->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.rotate.x,
										boneTest[a->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.rotate.y,
										boneTest[a->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.rotate.z,
										-boneTest[a->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.rotate.w);
				XMVECTOR r2=XMVectorSet(boneTest[a->vertexBoneInfo[i]->id]->startState.rotate.x,
										boneTest[a->vertexBoneInfo[i]->id]->startState.rotate.y,
										boneTest[a->vertexBoneInfo[i]->id]->startState.rotate.z,
										boneTest[a->vertexBoneInfo[i]->id]->startState.rotate.w);
				XMVECTOR t1=XMVectorSet(boneTest[a->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.translate.x,
										boneTest[a->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.translate.y,
										boneTest[a->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.translate.z,
										0);
				XMVECTOR t2=XMVectorSet(-boneTest[a->vertexBoneInfo[i]->id]->startState.translate.x,
										-boneTest[a->vertexBoneInfo[i]->id]->startState.translate.y,
										-boneTest[a->vertexBoneInfo[i]->id]->startState.translate.z,
										0);
				t*=XMMatrixTranslationFromVector(t2);
				t*=XMMatrixRotationQuaternion(r2);
				t*=XMMatrixRotationQuaternion(r1);
				t*=XMMatrixTranslationFromVector(t1);
				XMVECTOR ra = XMVectorSet(0,0,0,0);
				ra.m128_f32[0]=pa.m128_f32[0]*t.m[0][0]+pa.m128_f32[1]*t.m[1][0]+pa.m128_f32[2]*t.m[2][0]+pa.m128_f32[3]*t.m[3][0];
				ra.m128_f32[1]=pa.m128_f32[0]*t.m[0][1]+pa.m128_f32[1]*t.m[1][1]+pa.m128_f32[2]*t.m[2][1]+pa.m128_f32[3]*t.m[3][1];
				ra.m128_f32[2]=pa.m128_f32[0]*t.m[0][2]+pa.m128_f32[1]*t.m[1][2]+pa.m128_f32[2]*t.m[2][2]+pa.m128_f32[3]*t.m[3][2];
				ra.m128_f32[3]=pa.m128_f32[0]*t.m[0][3]+pa.m128_f32[1]*t.m[1][3]+pa.m128_f32[2]*t.m[2][3]+pa.m128_f32[3]*t.m[3][3];
				ra*=a->vertexBoneInfo[i]->weight;
				da+=ra;
			}
			
			for(int i=0;i<b->vertexBoneInfo.size();i++)
			{
				
				XMMATRIX t=XMMatrixIdentity();
				XMVECTOR r1=XMVectorSet(boneTest[b->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.rotate.x,
										boneTest[b->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.rotate.y,
										boneTest[b->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.rotate.z,
										-boneTest[b->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.rotate.w);
				XMVECTOR r2=XMVectorSet(boneTest[b->vertexBoneInfo[i]->id]->startState.rotate.x,
										boneTest[b->vertexBoneInfo[i]->id]->startState.rotate.y,
										boneTest[b->vertexBoneInfo[i]->id]->startState.rotate.z,
										boneTest[b->vertexBoneInfo[i]->id]->startState.rotate.w);
				XMVECTOR t1=XMVectorSet(boneTest[b->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.translate.x,
										boneTest[b->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.translate.y,
										boneTest[b->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.translate.z,
										0);
				XMVECTOR t2=XMVectorSet(-boneTest[b->vertexBoneInfo[i]->id]->startState.translate.x,
										-boneTest[b->vertexBoneInfo[i]->id]->startState.translate.y,
										-boneTest[b->vertexBoneInfo[i]->id]->startState.translate.z,
										0);
				t*=XMMatrixTranslationFromVector(t2);
				t*=XMMatrixRotationQuaternion(r2);
				t*=XMMatrixRotationQuaternion(r1);
				t*=XMMatrixTranslationFromVector(t1);
				XMVECTOR ra = XMVectorSet(0,0,0,0);
				ra.m128_f32[0]=pb.m128_f32[0]*t.m[0][0]+pb.m128_f32[1]*t.m[1][0]+pb.m128_f32[2]*t.m[2][0]+pb.m128_f32[3]*t.m[3][0];
				ra.m128_f32[1]=pb.m128_f32[0]*t.m[0][1]+pb.m128_f32[1]*t.m[1][1]+pb.m128_f32[2]*t.m[2][1]+pb.m128_f32[3]*t.m[3][1];
				ra.m128_f32[2]=pb.m128_f32[0]*t.m[0][2]+pb.m128_f32[1]*t.m[1][2]+pb.m128_f32[2]*t.m[2][2]+pb.m128_f32[3]*t.m[3][2];
				ra.m128_f32[3]=pb.m128_f32[0]*t.m[0][3]+pb.m128_f32[1]*t.m[1][3]+pb.m128_f32[2]*t.m[2][3]+pb.m128_f32[3]*t.m[3][3];
				ra*=b->vertexBoneInfo[i]->weight;
				db+=ra;
			}
			for(int i=0;i<c->vertexBoneInfo.size();i++)
			{
				
				XMMATRIX t=XMMatrixIdentity();
				XMVECTOR r1=XMVectorSet(boneTest[c->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.rotate.x,
										boneTest[c->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.rotate.y,
										boneTest[c->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.rotate.z,
										-boneTest[c->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.rotate.w);
				XMVECTOR r2=XMVectorSet(boneTest[c->vertexBoneInfo[i]->id]->startState.rotate.x,
										boneTest[c->vertexBoneInfo[i]->id]->startState.rotate.y,
										boneTest[c->vertexBoneInfo[i]->id]->startState.rotate.z,
										boneTest[c->vertexBoneInfo[i]->id]->startState.rotate.w);
				XMVECTOR t1=XMVectorSet(boneTest[c->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.translate.x,
										boneTest[c->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.translate.y,
										boneTest[c->vertexBoneInfo[i]->id]->boneSamples[keys]->tm.translate.z,
										0);
				XMVECTOR t2=XMVectorSet(-boneTest[c->vertexBoneInfo[i]->id]->startState.translate.x,
										-boneTest[c->vertexBoneInfo[i]->id]->startState.translate.y,
										-boneTest[c->vertexBoneInfo[i]->id]->startState.translate.z,
										0);
				t*=XMMatrixTranslationFromVector(t2);
				t*=XMMatrixRotationQuaternion(r2);
				t*=XMMatrixRotationQuaternion(r1);
				t*=XMMatrixTranslationFromVector(t1);
				XMVECTOR ra = XMVectorSet(0,0,0,0);
				ra.m128_f32[0]=pc.m128_f32[0]*t.m[0][0]+pc.m128_f32[1]*t.m[1][0]+pc.m128_f32[2]*t.m[2][0]+pc.m128_f32[3]*t.m[3][0];
				ra.m128_f32[1]=pc.m128_f32[0]*t.m[0][1]+pc.m128_f32[1]*t.m[1][1]+pc.m128_f32[2]*t.m[2][1]+pc.m128_f32[3]*t.m[3][1];
				ra.m128_f32[2]=pc.m128_f32[0]*t.m[0][2]+pc.m128_f32[1]*t.m[1][2]+pc.m128_f32[2]*t.m[2][2]+pc.m128_f32[3]*t.m[3][2];
				ra.m128_f32[3]=pc.m128_f32[0]*t.m[0][3]+pc.m128_f32[1]*t.m[1][3]+pc.m128_f32[2]*t.m[2][3]+pc.m128_f32[3]*t.m[3][3];
				ra*=c->vertexBoneInfo[i]->weight;
				dc+=ra;
			}*/
			point3 afa(da.m128_f32[0],da.m128_f32[1],da.m128_f32[2]);
			point3 afb(db.m128_f32[0],db.m128_f32[1],db.m128_f32[2]);
			point3 afc(dc.m128_f32[0],dc.m128_f32[1],dc.m128_f32[2]);
			fillPos(vertexbuf[3*i+0].pos,afa);
			fillTex(vertexbuf[3*i+0].tex,*mesh->texcoordlist[mesh->indexlist[i]->ta]);
			
			fillPos(vertexbuf[3*i+1].pos,afb);
			fillTex(vertexbuf[3*i+1].tex,*mesh->texcoordlist[mesh->indexlist[i]->tb]);
			
			fillPos(vertexbuf[3*i+2].pos,afc);
			fillTex(vertexbuf[3*i+2].tex,*mesh->texcoordlist[mesh->indexlist[i]->tc]);

			
			for(vector<boneInfo*>::iterator bite=mesh->vertexlist[mesh->indexlist[i]->a]->vertexBoneInfo.begin();bite!=mesh->vertexlist[mesh->indexlist[i]->a]->vertexBoneInfo.end();bite++)
			{
				if(boneMap[(**bite).id])
				{
					vertexbuf[3*i+0].boneWeight[boneMap[(**bite).id]-1]=(**bite).weight;
				}
			}
			for(vector<boneInfo*>::iterator bite=mesh->vertexlist[mesh->indexlist[i]->b]->vertexBoneInfo.begin();bite!=mesh->vertexlist[mesh->indexlist[i]->b]->vertexBoneInfo.end();bite++)
			{
				if(boneMap[(**bite).id])
				{
					vertexbuf[3*i+1].boneWeight[boneMap[(**bite).id]-1]=(**bite).weight;
				}
			}
			for(vector<boneInfo*>::iterator bite=mesh->vertexlist[mesh->indexlist[i]->c]->vertexBoneInfo.begin();bite!=mesh->vertexlist[mesh->indexlist[i]->c]->vertexBoneInfo.end();bite++)
			{
				if(boneMap[(**bite).id])
				{
					vertexbuf[3*i+2].boneWeight[boneMap[(**bite).id]-1]=(**bite).weight;
				}
			}
		}
		BonedMesh* m=new BonedMesh(device);
		m->setBoneList(ids,boneMap.size());
		m->setVertexSource(vertexbuf,sizeof(VertexPUB),vertexNum);
		m->setIndexSource(index,vertexNum);
		int matNum=matG->subchunk.size();
		for(int i=0;i<matNum;i++)
		{
			matrialChunk* mat=(matrialChunk*)matG->subchunk[i];
			if(strcmp(mat->matName,mesh->matName)==0)
			{
				bool r=m->loadTexture(mat->textureName);
				if(r)
				{
					int k=0;
				}
			}
		}
		result->addBonedMesh(m);
	}
	delete data;
	return result;
}


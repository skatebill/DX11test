#pragma once
#include"ModelGroup.h"
#include"phrase\Chunk.h"
#include "BonedMeshGroup.h"
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
	boneGroupChunk* boneGC = (boneGroupChunk*)data->subchunk[2];
	BoneGroup* boneG=new BoneGroup();
	result->setBone(boneG);
	for(vector<chunk*>::iterator ite=boneGC->subchunk.begin();ite!=boneGC->subchunk.end();ite++)
	{
		boneChunk* boneC=(boneChunk*)(*ite);
		bone* b=new bone();
		b->boneid=boneC->boneID;
		b->startState=boneC->posInfo;
		for(vector<boneSample*>::iterator bite=boneC->samples.begin();bite!=boneC->samples.end();bite++)
		{
			boneSample* samples=new boneSample();
			*samples=**bite;
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
		int* ids=new int[boneMap.size()];
		for(map<int,int>::iterator mapite=boneMap.begin();mapite!=boneMap.end();mapite++)
		{
			ids[mapite->second-1]=mapite->first;
		}
		for(int i=0;i<facenum;i++)
		{
			index[3*i+0]=3*i+0;
			index[3*i+1]=3*i+1;
			index[3*i+2]=3*i+2;
			ZeroMemory(vertexbuf[3*i+0].boneWeight,sizeof(float)*BONE_MAX_NUM);
			ZeroMemory(vertexbuf[3*i+1].boneWeight,sizeof(float)*BONE_MAX_NUM);
			ZeroMemory(vertexbuf[3*i+2].boneWeight,sizeof(float)*BONE_MAX_NUM);

			fillPos(vertexbuf[3*i+0].pos,mesh->vertexlist[mesh->indexlist[i]->a]->pos);
			fillTex(vertexbuf[3*i+0].tex,*mesh->texcoordlist[mesh->indexlist[i]->ta]);
			
			fillPos(vertexbuf[3*i+1].pos,mesh->vertexlist[mesh->indexlist[i]->b]->pos);
			fillTex(vertexbuf[3*i+1].tex,*mesh->texcoordlist[mesh->indexlist[i]->tb]);
			
			fillPos(vertexbuf[3*i+2].pos,mesh->vertexlist[mesh->indexlist[i]->c]->pos);
			fillTex(vertexbuf[3*i+2].tex,*mesh->texcoordlist[mesh->indexlist[i]->tc]);

			
			for(vector<boneInfo*>::iterator bite=mesh->vertexlist[mesh->indexlist[i]->a]->vertexBoneInfo.begin();bite!=mesh->vertexlist[mesh->indexlist[i]->a]->vertexBoneInfo.end();bite++)
			{
				vertexbuf[3*i+0].boneWeight[boneMap[(**bite).id]-1]=(**bite).weight;
			}
			for(vector<boneInfo*>::iterator bite=mesh->vertexlist[mesh->indexlist[i]->b]->vertexBoneInfo.begin();bite!=mesh->vertexlist[mesh->indexlist[i]->b]->vertexBoneInfo.end();bite++)
			{
				vertexbuf[3*i+1].boneWeight[boneMap[(**bite).id]-1]=(**bite).weight;
			}
			for(vector<boneInfo*>::iterator bite=mesh->vertexlist[mesh->indexlist[i]->c]->vertexBoneInfo.begin();bite!=mesh->vertexlist[mesh->indexlist[i]->c]->vertexBoneInfo.end();bite++)
			{
				vertexbuf[3*i+2].boneWeight[boneMap[(**bite).id]-1]=(**bite).weight;
			}


		}
		BonedMesh* m=new BonedMesh(device);
		m->setBoneList(ids,boneMap.size());
		m->setVertexSource(vertexbuf,sizeof(VertexPUB),vertexNum);
		m->setIndexSource(index,vertexNum);
		result->addBonedMesh(m);
	}
	delete data;
	return result;
}

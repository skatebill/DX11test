#pragma once
#include"ModelGroup.h"
#include"phrase\Chunk.h"
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
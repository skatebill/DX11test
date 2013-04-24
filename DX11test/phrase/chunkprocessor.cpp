#include"Chunk.h"

bool chunkWriter::saveChunkTo(mainChunk &source,char* dstFile){
	if(openFile(dstFile,"wb")){
		if(!source.length)
			source.computeLength();
		saveChunk(source);
		close();
		return true;
	}
	return false;
}
void chunkWriter::saveChunk(chunk& chunk){
	switch(chunk.id)
	{
	case MAINCHUNK:
		saveMainChunk(*(mainChunk*)&chunk);
		break;
	case MESHCHUNK:
		saveMeshChunk(*(meshChunk*)&chunk);
		break;
	case MATRIALCHUNK:
		saveMatrialChunk((*(matrialChunk*)&chunk));
		break;
	case BONECHUNK:
		saveBoneChunk((*(boneChunk*)&chunk));
		break;
	case MESHGROUPCHUNK:
	case MATRIALGROUPCHUNK:
	case BONEGROUPCHUNK:
		saveChunkHead(chunk);
		saveSubChunk(*(ChunkContainer*)&chunk);
		break;
	default:
		break;
	}
}

void chunkWriter::saveBoneChunk(boneChunk& chunk){
	saveChunkHead(chunk);
	//bone id
	writeWord(chunk.boneID);
	//save bone info
	writeFloat(chunk.posInfo.translate.x);
	writeFloat(chunk.posInfo.translate.y);
	writeFloat(chunk.posInfo.translate.z);

	writeFloat(chunk.posInfo.rotate.w);
	writeFloat(chunk.posInfo.rotate.x);
	writeFloat(chunk.posInfo.rotate.y);
	writeFloat(chunk.posInfo.rotate.z);

	writeDWord(chunk.samples.size());
	for(vector<boneSample*>::iterator ite=chunk.samples.begin();ite!=chunk.samples.end();ite++){
		writeWord((**ite).key);

		writeFloat((**ite).tm.translate.x);
		writeFloat((**ite).tm.translate.y);
		writeFloat((**ite).tm.translate.z);

		writeFloat((**ite).tm.rotate.w);
		writeFloat((**ite).tm.rotate.x);
		writeFloat((**ite).tm.rotate.y);
		writeFloat((**ite).tm.rotate.z);
	}
}
void chunkWriter::saveSubChunk(ChunkContainer& container){
	for(vector<chunk*>::iterator ite=container.subchunk.begin();ite!=container.subchunk.end();ite++){
		saveChunk(**ite);
	}

}
void chunkWriter::saveMainChunk(mainChunk& chunk){
	saveChunkHead(chunk);
	saveSubChunk(chunk);
}
void chunkWriter::saveMeshChunk(meshChunk& chunk){
	saveChunkHead(chunk);

	writeString(chunk.matName);
	writeDWord(chunk.vertexlist.size());
	writeDWord(chunk.texcoordlist.size());
	writeDWord(chunk.normallist.size());
	writeDWord(chunk.colorlist.size());
	writeDWord(chunk.indexlist.size());

	for(vector<vertex*>::iterator ite=chunk.vertexlist.begin();ite!=chunk.vertexlist.end();ite++){
		writeFloat((**ite).pos.x);
		writeFloat((**ite).pos.y);
		writeFloat((**ite).pos.z);
		WORD num=(**ite).vertexBoneInfo.size();
		writeWord(num);
		for(int i=0;i<num;i++)
		{
			writeWord((**ite).vertexBoneInfo[i]->id);
			writeFloat((**ite).vertexBoneInfo[i]->weight);
		}
	}
	for(vector<point2*>::iterator ite=chunk.texcoordlist.begin();ite!=chunk.texcoordlist.end();ite++){
		writeFloat((**ite).u);
		writeFloat((**ite).v);

	}
	for(vector<point3*>::iterator ite=chunk.normallist.begin();ite!=chunk.normallist.end();ite++){
		writeFloat((**ite).x);
		writeFloat((**ite).y);
		writeFloat((**ite).z);

	}
	for(vector<point4*>::iterator ite=chunk.colorlist.begin();ite!=chunk.colorlist.end();ite++){
		writeFloat((**ite).x);
		writeFloat((**ite).y);
		writeFloat((**ite).z);
		writeFloat((**ite).w);

	}
	for(vector<face*>::iterator ite=chunk.indexlist.begin();ite!=chunk.indexlist.end();ite++){
		writeWord((**ite).a);
		writeWord((**ite).b);
		writeWord((**ite).c);

		writeWord((**ite).ta);
		writeWord((**ite).tb);
		writeWord((**ite).tc);

		writeWord((**ite).na);
		writeWord((**ite).nb);
		writeWord((**ite).nc);

		writeWord((**ite).ca);
		writeWord((**ite).cb);
		writeWord((**ite).cc);


	}
}

void chunkWriter::saveMatrialChunk(matrialChunk &chunk){
	saveChunkHead(chunk);
	//mat id
	writeString(chunk.matName);
	//abm
	writeFloat(chunk.amb.x);
	writeFloat(chunk.amb.y);
	writeFloat(chunk.amb.z);
	writeFloat(chunk.amb.w);
	//dif
	writeFloat(chunk.dif.x);
	writeFloat(chunk.dif.y);
	writeFloat(chunk.dif.z);
	writeFloat(chunk.dif.w);
	//spec
	writeFloat(chunk.spec.x);
	writeFloat(chunk.spec.y);
	writeFloat(chunk.spec.z);
	writeFloat(chunk.spec.w);

	//tex
	writeByte(chunk.hasTexture);
	if(chunk.hasTexture)
	{
		writeString(chunk.textureName);
	}
	
}

void chunkReader::skipChunk(ChunkHead& head){
	fseek(m_DstFile,head.length-CHUNK_HEAD_SIZE,SEEK_CUR);
}
ChunkHead chunkReader::readChunkHead(){
	ChunkHead head;
	head.id=readWord();
	head.length=readDWord();
	return head;
}
mainChunk* chunkReader::readMainChunk(){
	ChunkHead head=readChunkHead();
	if(head.id!=MAINCHUNK)
	{
		return 0;
	}
	mainChunk* m=new mainChunk;
	m->length=head.length;

	int num=0;
	while(num<head.length-CHUNK_HEAD_SIZE)
	{
		ChunkHead head=readChunkHead();
		switch(head.id)
		{
		case MESHGROUPCHUNK:
			readMeshGroupChunk(head,*m);
			break;
		case MATRIALGROUPCHUNK:
			readMatrialGroupChunk(head,*m);
			break;
		case BONEGROUPCHUNK:
			readBoneGroupChunk(head,*m);
			break;
		default:
			skipChunk(head);
			break;
		}
		num+=head.length;
	}
	return m;
}	
void chunkReader::readBoneGroupChunk(ChunkHead &grouphead,mainChunk &chunk){
	boneGroupChunk *group=new boneGroupChunk;
	group->length=grouphead.length;
	chunk.addSunChunk(*group);
	int num=0;
	while(num<grouphead.length-CHUNK_HEAD_SIZE)
	{
		ChunkHead head=readChunkHead();
		switch(head.id)
		{
		case BONECHUNK:
			readBoneChunk(head,*group);
			break;
		default:
			skipChunk(head);
			break;
		}
		num+=head.length;
	}
}
void chunkReader::readMeshGroupChunk(ChunkHead &grouphead,mainChunk &chunk){
	meshGroupChunk *group=new meshGroupChunk;
	group->length=grouphead.length;
	chunk.addSunChunk(*group);
	int num=0;
	while(num<grouphead.length-CHUNK_HEAD_SIZE)
	{
		ChunkHead head=readChunkHead();
		switch(head.id)
		{
		case MESHCHUNK:
			readMeshChunk(head,*group);
			break;
		default:
			skipChunk(head);
			break;
		}
		num+=head.length;
	}
}
void chunkReader::readMatrialGroupChunk(ChunkHead &grouphead,mainChunk &chunk){
	matrialGroupChunk *group=new matrialGroupChunk;
	group->length=grouphead.length;
	chunk.addSunChunk(*group);
	int num=0;
	while(num<grouphead.length-CHUNK_HEAD_SIZE)
	{
		ChunkHead head=readChunkHead();
		switch(head.id)
		{
		case MATRIALCHUNK:
			readMatrialChunk(head,*group);
			break;
		default:
			skipChunk(head);
			break;
		}
		num+=head.length;
	}
}
void chunkReader::readMatrialChunk(ChunkHead &meshhead,matrialGroupChunk &chunk){
	matrialChunk *m=new matrialChunk;
	m->length=meshhead.length;
	chunk.addSunChunk(*m);
	//mat name
	char* buf=(char*)malloc(256);
	readString(buf);
	m->matName=buf;
	//amb
	m->amb.x=readFloat();
	m->amb.y=readFloat();
	m->amb.z=readFloat();
	m->amb.w=readFloat();
	//dif
	m->dif.x=readFloat();
	m->dif.y=readFloat();
	m->dif.z=readFloat();
	m->dif.w=readFloat();
	//spec
	m->spec.x=readFloat();
	m->spec.y=readFloat();
	m->spec.z=readFloat();
	m->spec.w=readFloat();
	//tex
	m->hasTexture=readByte();
	if(m->hasTexture)
	{
		m->textureName=(char*)malloc(64);
		readString(m->textureName,64);
	}

}
void chunkReader::readBoneChunk(ChunkHead &meshhead,boneGroupChunk &chunk){
	boneChunk *m=new boneChunk;
	m->length=meshhead.length;
	chunk.addSunChunk(*m);

	m->boneID=readWord();

	m->posInfo.translate.x=readFloat();
	m->posInfo.translate.y=readFloat();
	m->posInfo.translate.z=readFloat();

	
	m->posInfo.rotate.w=readFloat();
	m->posInfo.rotate.x=readFloat();
	m->posInfo.rotate.y=readFloat();
	m->posInfo.rotate.z=readFloat();

	int numSamples=readDWord();
	while(numSamples>0)
	{
		boneSample* sample=new boneSample;
		sample->key=readWord();

		sample->tm.translate.x=readFloat();
		sample->tm.translate.y=readFloat();
		sample->tm.translate.z=readFloat();

		
		sample->tm.rotate.w=readFloat();
		sample->tm.rotate.x=readFloat();
		sample->tm.rotate.y=readFloat();
		sample->tm.rotate.z=readFloat();

		m->samples.push_back(sample);

		numSamples--;
	}
}

void chunkReader::readMeshChunk(ChunkHead &meshhead,meshGroupChunk &chunk){
	meshChunk *m=new meshChunk;
	m->length=meshhead.length;
	chunk.addSunChunk(*m);
	char* buf=(char*)malloc(256);
	readString(buf);
	m->matName=buf;
	DWORD vertexnum=readDWord();
	DWORD texcoordnum=readDWord();
	DWORD normalnum=readDWord();
	DWORD colornum=readDWord();
	DWORD indexnum=readDWord();

	for(int i=0;i<vertexnum;i++){
		float x,y,z;
		x=readFloat();
		y=readFloat();
		z=readFloat();

		vertex* v=new vertex(x,y,z);
		WORD num=readWord();
		for(int i=0;i<num;i++)
		{
			boneInfo* info=new boneInfo;
			info->id=readWord();
			info->weight = readFloat();
			v->vertexBoneInfo.push_back(info);
		}


		m->vertexlist.push_back(v);

	}
	for(int i=0;i<texcoordnum;i++){
		float u,v;
		u=readFloat();
		v=readFloat();
		m->texcoordlist.push_back(new point2(u,v));

	}
	for(int i=0;i<normalnum;i++){
		float nx,ny,nz;
		nx=readFloat();
		ny=readFloat();
		nz=readFloat();
		m->normallist.push_back(new point3(nx,ny,nz));

	}
	for(int i=0;i<colornum;i++){
		float x,y,z,w;
		x=readFloat();
		y=readFloat();
		z=readFloat();
		w=readFloat();
		m->colorlist.push_back(new point4(x,y,z,w));

	}
	for(int i=0;i<indexnum;i++){
		face* f=new face();
		f->a=readWord();
		f->b=readWord();
		f->c=readWord();
		//tex
		f->ta=readWord();
		f->tb=readWord();
		f->tc=readWord();
		//normal
		f->na=readWord();
		f->nb=readWord();
		f->nc=readWord();
		//color
		f->ca=readWord();
		f->cb=readWord();
		f->cc=readWord();
		m->indexlist.push_back(f);
	}

}
mainChunk* chunkReader::readFile(char* dstFile){
	if(!openFile(dstFile,"rb"))
	{
		return 0;
	}
	mainChunk* r=readMainChunk();
	close();
	return r;
}
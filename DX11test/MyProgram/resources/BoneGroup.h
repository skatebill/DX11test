#pragma once
#include "MyResource.h"
#include "..\..\phrase\Chunk.h"
#include <vector>
#include <map>
class bone{
public:
	bone(){}
	~bone(){
		for(vector<boneSample*>::iterator ite=boneSamples.begin();ite!=boneSamples.end();ite++)
		{
			delete *ite;
		}
		boneSamples.clear();
	}
	WORD	boneid;
	boneTM	startState;
	vector<boneSample*> boneSamples;
	boneSample getBoneSample(int keyPos);
	boneTM getFirstTM(){
		return startState;
	}
};

class BoneGroup :
	public MyResource
{
private:
	map<int,bone*>	boneList;
	float framerate;
public:
	BoneGroup(void);
	~BoneGroup(void);

	void addBone(bone* b){
		boneList[b->boneid]=b;
	}
	void setFrameRate(float rate){
		framerate=rate;
	};
	boneSample getBoneSample(int boneId,float time){
		return boneList[boneId]->getBoneSample(time/framerate);
	}
	boneSample getBoneSample(int boneId,int framePos){
		return boneList[boneId]->getBoneSample(framePos);

	}
	void release(){
		for(map<int,bone*>::iterator ite=boneList.begin();ite!=boneList.end();ite++)
		{
			delete ite->second;
		}
		boneList.clear();
			
	}
	bool isReleased(){
		return boneList.size()==0;
	}
	bone* getBone(int id){
		return boneList[id];
	}
};


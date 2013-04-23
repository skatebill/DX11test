#include "BoneGroup.h"

boneSample bone::getBoneSample(int keyPos){
	/*for(vector<boneSample*>::iterator ite= boneSamples.begin();ite!=boneSamples.end();ite++)
	{

	}*/
	for(int i=0;i<boneSamples.size();i++)
	{
		if(boneSamples[i]->key>=keyPos)
		{
			return *boneSamples[i];
		}
	}
	return *boneSamples.back();
}
BoneGroup::BoneGroup(void)
{
}


BoneGroup::~BoneGroup(void)
{
}

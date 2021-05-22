#pragma once

class VectorUtils 
{
public:
	template<class T> static T randomElement(vector<T>& vec)
	{
		vec.at(rndInt(0, vec.size()));
	}

	static Vector3 getClosestLocationTo(vector<Vector3>* locations, Vector3 coords, float minDistanceTreshold = 0);
	static vector<int> numbers(int from, int to);
	static vector<int> numbers(int to);
};

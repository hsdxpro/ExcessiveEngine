//PointLight.h By Németh Richárd 2013.05.30 11:54 ported from my C# Randominezz
#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Vec3.h"

//namespace Randominezz
//{
    class PointLight
    {
	public:
        //position
        Vec3 pos;
        
        //diffuse color and intensity
        Vec3 diffuseColor;
		double diffuseIntensity;

        //specular color and intensity
        Vec3 specularColor;
        double specularIntensity;

        PointLight(Vec3 pos, Vec3 diffuseColor,Vec3 specularColor, double diffuseIntensity, double specularIntensity)
        {
            this->pos = pos;
            this->diffuseColor = diffuseColor;
            this->specularColor = specularColor;
            this->diffuseIntensity = diffuseIntensity;
            this->specularIntensity = specularIntensity;
        }

        double calcIntensity(Vec3 surfacePos, Vec3 surfaceNormal)
        {
            Vec3 lightVec = Vec3::subtract(pos, surfacePos);
            lightVec.normalize();
            surfaceNormal.normalize();

            double intensity = lightVec.dot(surfaceNormal);
            if (intensity < 0.0f)
                intensity = 0.0f;
            return intensity;
        }
    };
//}
#endif

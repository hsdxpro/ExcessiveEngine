//Shader.h By Németh Richárd 2013.05.30 12:10 ported from my C# Randominezz
#ifndef SHADER_H
#define SHADER_H

#include <list>
using namespace std;

namespace Randominezz
{
    class Shader
    {
	public:
        Vec3 calcDiffuseColor(Vec3& diffuseColor, Vec3& intersectPos, Vec3& intersectNormal, list<PointLight*>& pointLightSources, list<double>& shadowFactors)
        {
            //In DX:..........................
            //resulColor = diffuseColor + lighting;
            //lighting = diffIntensity * diffcolor * difflight

            Vec3 lighting(0,0,0);
			

			//for each light lit the surface...
			list<PointLight*>::iterator it = pointLightSources.begin();
			list<double>::iterator s_it = shadowFactors.begin();
			while(it != pointLightSources.end())
			{
				PointLight* curr = *it;
				
				double shadowfactor = *s_it;
                if (shadowfactor > 0.0)
				{
					it++;
					s_it++;
                    continue;
				}
				

                Vec3 currLighting = Vec3::product(curr->diffuseColor,(curr->diffuseIntensity * curr->calcIntensity(intersectPos, intersectNormal)));
                lighting.add(currLighting);



				it++;
				s_it++;
			}
           
            return Vec3::add(diffuseColor,lighting);
           
        }

        Vec3 calcSpecularColor(Vec3& camPos, Vec3& intersectPos, Vec3& intersectNormal, list<PointLight*>& pointLightSources, list<double>& shadowFactors)
        {

            //HLSL
            //double3 viewvec = normalize( campos - posW );
            //double3 halfvec = normalize(lightvec + viewvec);
            //speclight = pow(saturate(dot(norm, halfvec)), specexponent * 70);

            Vec3 specFinal(0,0,0);
      
            //for each light lit the surface...
			list<PointLight*>::iterator it = pointLightSources.begin();
			list<double>::iterator s_it = shadowFactors.begin();
			while(it != pointLightSources.end())
			{
			
                double shadowfactor = *s_it;
                if (shadowfactor > 0.0)
				{
					it++;
					s_it++;
                    continue;
				}
			

                PointLight* curr = *it;

                //Spec calculation
                Vec3 lightVec = Vec3::normalize(Vec3::subtract(curr->pos, intersectPos));
                Vec3 viewVec = Vec3::normalize(Vec3::subtract(camPos, intersectPos));
                Vec3 halfVec = Vec3::normalize(Vec3::add(lightVec, viewVec));
                Vec3 currSpec = Vec3::product(curr->specularColor,(powf(Vec3::dot(intersectNormal, halfVec)*curr->specularIntensity,60)));
                //@TODO pow saturate...

                specFinal.add(currSpec);

				it++;
				s_it++;
            }


            return specFinal;

        }
    };
}
#endif
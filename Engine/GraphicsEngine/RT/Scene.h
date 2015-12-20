//Scene.h By Németh Richárd 2013.05.30 12:08 ported from my C# Randominezz
#pragma once

#include "Sphere.h"
#include "PointLight.h"
#include "Camera.h"
#include "Shader.h"
#include <list>
#include "GraphicsEngineRT.h"
#include "mymath\mm_vec_func.h"
#include <algorithm>
using namespace std;

namespace Randominezz
{
	enum eBRDF
	{
		DIFFUSE__LAMBERT,
		DIFFUSE__GOTANDA,
		DIFFUSE__BURLEY,
		DIFFUSE__OREN_NAYAR,

		SPECULAR__COOK_TORRANCE
	};

	eBRDF gBRDF = DIFFUSE__LAMBERT;

    class Scene
    {
	public:

		//Sphere objects in the scene
		Sphere* spheres;
		int numSpheres;

        //Sphere objects that near rays, so ray only operates on these tmp holder
        list<Sphere*> seenSpheres;

        //Point light sources
        list<PointLight*> plights;

        //main Camera
        Camera* mainCamera;
		GraphicsEngineRT_Richard* rtEngine;

        //use Shading algorithms
        //Shader* mainShader;


		//TMP
			//intersectedSphere hold information abot intersected sphere in each [PASS]
            Sphere intersectedSphere;

            //iInfo hold information abot intersection [PASS]
            IntersectionInfo iInfo;

		Scene(GraphicsEngineRT_Richard* rtEngine, Camera* mainCam)
		:rtEngine(rtEngine), mainCamera(mainCam)
		{
			//mainCamera = new Camera(Vec3(0.0f,0.0f,0.0f), 0.01f, 1000.0f,1.5707);
			//mainShader = new Shader();

			//Sphere array
			spheres = new Sphere[400];
			numSpheres = 0;
		}

		~Scene()
		{
			delete spheres;
		}

        //add sphere to the scene
        void addSphere(Vec3 pos, double radius, Vec3 color, double reflectness)
        {
			spheres[numSpheres] = Sphere(pos, radius, color,reflectness);
			numSpheres++;
        }

        void addPointLight(Vec3 pos, Vec3 diffuseColor, Vec3 specularColor, double diffuseIntensity, double specularIntensity)
        {
            plights.push_back(new PointLight(pos,diffuseColor,specularColor,diffuseIntensity,specularIntensity));
        }

        //Get Sphere that is intersected by the ray, and the intersection information, and intersection bool
        bool getInsersectedSphere(Ray& r, Sphere& s_out, IntersectionInfo& iInfo_out)
        {
            //@TODO
            //first preprocess, gather spheres only that are so close to the ray...
            bool leastOnceIntersection = false;

            //for holding tmp Intersect Info
            IntersectionInfo nearestIntersect;

            //initial object distance
            double distance = 999999999.0f;

			for(int i = 0; i < numSpheres; i++)
			{
				Sphere& curr = spheres[i];

				//if ray intersect the sphere
                if (curr.isIntersected(r, nearestIntersect))
                {
                    //check if his distance is smaller than te current noted intersected sphere 
					double distFromRay = nearestIntersect.intersectPoint.distance(r.origin);
                    if (distFromRay < distance)
                    {
                        //save new distance
                        distance = distFromRay;

                        //save sphere properties
                        s_out.c = curr.c;
                        s_out.radius = curr.radius;
                        s_out.pos = curr.pos;
                        s_out.reflectness = curr.reflectness;

                        //least once intersection happened
                        leastOnceIntersection = true;

                        //save intersectionInfo
                        iInfo_out.intersectPoint = nearestIntersect.intersectPoint;
                    }
                }

			}
		

            if (leastOnceIntersection)
                return true;
            else
                return false;

        }

        bool isInsersectedSphere(Ray& r)
        {
			for(int i = 0; i < numSpheres; i++)
				if (spheres[i].isIntersected(r))
                    return true;

			return false;
        }

        void generateShadowFactors(list<double>& shadowFactors, Vec3& surfacePos)
        {
			list<PointLight*>::iterator it = plights.begin();
			while(it != plights.end())
			{
				//curr Light
                PointLight* curr = *it;

                //Create shadow ray, check intersection...
                //shadow ray goes from previous sphere intersection pos to light position
                //if it hits something, that pixel will be shadowed...

                Ray shadowRay = Ray(surfacePos, Vec3::subtract(curr->pos, surfacePos));

                //check shadowRay intersection..
                if (isInsersectedSphere(shadowRay))
					shadowFactors.push_back(1.0);
                else
                    shadowFactors.push_back(0.0);

				it++;
			}
			
        }

		float Pow5(float x)
		{
			float x2 = x*x;
			return x2 * x2 * x;
		}

		float cook_torrance
			(
				float NdotL,
				float NdotH,
				float NdotV,
				float VdotH
				//Vec3 normal,
				//Vec3 viewer,
				//Vec3 light
				)
		{
			float roughness_value = 0.1;

			// Compute any aliases and intermediary values
			// -------------------------------------------
			//float3 half_vector = normalize(light + viewer);
			//float NdotL = saturate(dot(normal, light));
			//float NdotH = saturate(dot(normal, half_vector));
			//float NdotV = saturate(dot(normal, viewer));
			//float VdotH = saturate(dot(viewer, half_vector));
			float r_sq = roughness_value * roughness_value;
		
		
		
			// Evaluate the geometric term
			// --------------------------------
			float geo_numerator = 2.0f * NdotH;
			float geo_denominator = VdotH;
		
			float geo_b = (geo_numerator * NdotV) / geo_denominator;
			float geo_c = (geo_numerator * NdotL) / geo_denominator;
			float geo = min(1.0f, min(geo_b, geo_c));
		
		
		
			// Now evaluate the roughness term
			// -------------------------------
			float roughness;
		
			//if (ROUGHNESS_LOOK_UP == roughness_mode)
			//{
			//	// texture coordinate is:
			//	float2 tc = { NdotH, roughness_value };
			//
			//	// Remap the NdotH value to be 0.0-1.0
			//	// instead of -1.0..+1.0
			//	tc.x += 1.0f;
			//	tc.x /= 2.0f;
			//
			//	// look up the coefficient from the texture:
			//	roughness = texRoughness.Sample(sampRoughness, tc);
			//}
			//if (ROUGHNESS_BECKMANN == roughness_mode)
			{
				float roughness_a = 1.0f / (4.0f * r_sq * pow(NdotH, 4));
				float roughness_b = NdotH * NdotH - 1.0f;
				float roughness_c = r_sq * NdotH * NdotH;
		
				roughness = roughness_a * exp(roughness_b / roughness_c);
			}
			//if (ROUGHNESS_GAUSSIAN == roughness_mode)
			//{
			//	// This variable could be exposed as a variable
			//	// for the application to control:
			//	float c = 1.0f;
			//	float alpha = acos(dot(normal, half_vector));
			//	roughness = c * exp(-(alpha / r_sq));
			//}
		
		
		
			float ref_at_norm_incidence = 0.02;
			// Next evaluate the Fresnel value
			// -------------------------------
			float fresnel = pow(1.0f - VdotH, 5.0f);
			fresnel *= (1.0f - ref_at_norm_incidence);
			fresnel += ref_at_norm_incidence;
		
		
		
			// Put all the terms together to compute
			// the specular term in the equation
			// -------------------------------------
			float Rs_numerator = fresnel * geo * roughness;
			float Rs_denominator = NdotV * NdotL;
			float Rs = Rs_numerator / Rs_denominator;
		
			return NdotL * Rs;
		
		
			// Put all the parts together to generate
			// the final colour
			// --------------------------------------
			//Vec3 final = NdotL* * Rs;// (cSpecular * Rs + cDiffuse);
			//
			//
			//
			//// Return the result
			//// -----------------
			//return float4(final, 1.0f);
		}

		Vec3 recResult;
		Vec3& traceColor(Ray r, int depth)
		{
			//recResult = Vec3(255, 0, 255);
			//return recResult;

			//recResult = cook_torrance()
			//return recResult;

			if(getInsersectedSphere(r,intersectedSphere,iInfo))
			{   
			    //sphere normal,pos
			    Vec3 intersectPos       = iInfo.intersectPoint;
			    Vec3 intersectNormal    = intersectedSphere.getNormalAtPoint(intersectPos);
			
				//Generate shadow factors
				//list<double> shadowFactors;
			    //generateShadowFactors(shadowFactors,intersectPos);
			
				//Vec3 diffuseColor = Vec3::product(intersectedSphere.c, 1.0f - intersectedSphere.reflectness);
			
			    //CALCULATE DIRECT DIFFUSE + SPECULAR
			    //diffuse component of the final color
				//recResult.add(mainShader->calcSpecularColor(mainCamera->pos, intersectPos, intersectNormal, plights, shadowFactors));
				//recResult.add(mainShader->calcDiffuseColor(diffuseColor, intersectPos, intersectNormal, plights,shadowFactors));				
			
				//if (lambert diffuse)
				//{
				//	Vec3 N = intersectNormal;
				//	Vec3 L = Vec3::subtract((*plights.begin())->pos, intersectPos);
				//	L.normalize();
				//	float intensity = N.dot(L);
				//	if (intensity < 0)
				//		intensity = 0;
				//}

				float Roughness = 0.8;

				Vec3 V = Vec3(mainCamera->GetPos().x, mainCamera->GetPos().y, mainCamera->GetPos().z);
				V.subtract(intersectPos);
				V.normalize();

				Vec3 L = Vec3(0, 0, -1);
				//Vec3 L = Vec3::subtract((*plights.begin())->pos, intersectPos);
				L.normalize();

				Vec3 N = intersectNormal;
				N.normalize();

				Vec3 H = Vec3::add(L, V);
				H.normalize();

				float VoH = V.dot(H);
				if (VoH < 0)
					VoH = 0;

				float NoV = N.dot(V);
				if (NoV < 0)
					NoV = 0;

				float NoL = N.dot(L);
				if (NoL < 0)
					NoL = 0;

				float NoH = N.dot(H);
				if (NoH < 0)
					NoH = 0;

				if (gBRDF == SPECULAR__COOK_TORRANCE)
				{
					recResult.add(intersectedSphere.c * cook_torrance(NoL, NoH, NoV, VoH));
				}
				if(gBRDF == DIFFUSE__GOTANDA)
				{
					float a = Roughness * Roughness;
					float a2 = a * a;
					float F0 = 0.04;
					float VoL = 2 * VoH * VoH - 1;		// double angle identity
					float Cosri = VoL - NoV * NoL;

					float a2_13 = a2 + 1.36053;
					float Fr = (1 - (0.542026*a2 + 0.303573*a) / a2_13) * (1 - pow(1 - NoV, 5 - 4 * a2) / a2_13) * ((-0.733996*a2*a + 1.50912*a2 - 1.16402*a) * pow(1 - NoV, 1 + 1.f / (39 * a2*a2 + 1)) + 1);
					//float Fr = ( 1 - 0.36 * a ) * ( 1 - pow( 1 - NoV, 5 - 4*a2 ) / a2_13 ) * ( -2.5 * Roughness * ( 1 - NoV ) + 1 );
					float Lm = (max(1 - 2 * a, 0.f) * (1 - Pow5(1 - NoL)) + min(2 * a, 1.f)) * (1 - 0.5*a * (NoL - 1)) * NoL;
					float Vd = (a2 / ((a2 + 0.09) * (1.31072 + 0.995584 * NoV))) * (1 - pow(1 - NoL, (1 - 0.3726732 * NoV * NoV) / (0.188566 + 0.38841 * NoV)));
					float Bp = Cosri < 0 ? 1.4 * NoV * NoL * Cosri : Cosri;
					float Lr = (21.0 / 20.0) * (1 - F0) * (Fr * Lm + Vd + Bp);
					recResult.add(intersectedSphere.c * (Lr / 3.1415));
				}
				else if (gBRDF == DIFFUSE__BURLEY)
				{
					float FD90 = 0.5 + 2 * VoH * VoH * Roughness;
					float FdV = 1 + (FD90 - 1) * Pow5(1 - NoV);
					float FdL = 1 + (FD90 - 1) * Pow5(1 - NoL);
					recResult.add(intersectedSphere.c * (FdV * FdL / 3.1415));
				}
				else if (gBRDF == DIFFUSE__OREN_NAYAR)
				{
					float a = Roughness * Roughness;
					float s = a;// / ( 1.29 + 0.5 * a );
					float s2 = s * s;
					float VoL = 2 * VoH * VoH - 1;		// double angle identity
					float Cosri = VoL - NoV * NoL;
					float C1 = 1 - 0.5 * s2 / (s2 + 0.33);
					float C2 = 0.45 * s2 / (s2 + 0.09) * Cosri * (Cosri >= 0 ? 1.f / (max(NoL, NoV)) : 1);
					recResult.add(intersectedSphere.c * ((C1 + C2) * (1 + Roughness * 0.5) / 3.1415));
				}
				else if (gBRDF == DIFFUSE__LAMBERT)
				{
					recResult.add(intersectedSphere.c * NoL);
				}

				//if(depth > 0.0f)
				//{
				//	Ray r2;
				//
				//	r2.dir = Vec3::reflect(Vec3::subtract(intersectPos,r.origin),intersectNormal);
				//	r2.origin = intersectPos;
				//	recResult.add(traceColor(r2,depth-1));
				//}
			}
			else
			{
				recResult.x = 1;
			}

			return recResult;
		}

        //[PASS]es -> Main Ray -> ShadowRay -> CALC DIRECT DIFFUSE + SPECULAR + CALC INDIRECT DIFFUSE + SPECULAR
        Vec3 getColorAtPixel(float pixelX, float pixelY, int screenWidth, int screenHeight)
        {
			recResult = Vec3(0,0,0);

			//Create ray at pixel...
            Ray r = rtEngine->getRayAtPixel(pixelX, pixelY, screenWidth, screenHeight, (double)screenWidth / (double)screenHeight);

			//trace color based on pixelRay and recursion bounce depth
			return traceColor(r,1).toColor();
			

			/*
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////MAIN RAY///////////////////////////////
        //////////////////////////////////////////////////////////////////////
            //Create ray at pixel...
            Ray r = rtEngine->getRayAtPixel(pixelX, pixelY, screenWidth, screenHeight, (double)screenWidth / (double)screenHeight);

            //Get intersected Sphere
            
            //ray intersects something
            if(getInsersectedSphere(r,intersectedSphere,iInfo))
            {   
                //sphere normal,pos
                Vec3 intersectPos       = iInfo.intersectPoint;
                Vec3 intersectNormal    = intersectedSphere.getNormalAtPoint(intersectPos);
				
          /////////////////////////////////////////////////////////////////////////
          ///////////////////////////////SHADOW RAY///////////////////////////////
          ///////////////////////////////////////////////////////////////////////
                //shadowFactors for each light source
                list<double> shadowFactors;
                generateShadowFactors(shadowFactors,intersectPos);


        ////////////////////////////////////////////////////////////////////////
        ///////////////////////DIRECT DIFFUSE + SPECULAR///////////////////////
        //////////////////////////////////////////////////////////////////////
                //@TODO nem biztos hogy az 1.0f - megoldás jó...
                //sphere diffuse color
                Vec3 diffuseColor = Vec3::product(intersectedSphere.c, 1.0f - intersectedSphere.reflectness);


                //final rendered color
                Vec3 finalcolor(0,0,0);
				
                //CALCULATE DIRECT DIFFUSE + SPECULAR
                //diffuse component of the final color
                Vec3 diffuseComponent = mainShader->calcDiffuseColor(diffuseColor, intersectPos, intersectNormal, plights,shadowFactors);
				

                //specular component of the final color
                Vec3 specularComponent = mainShader->calcSpecularColor(mainCamera->pos, intersectPos, intersectNormal, plights, shadowFactors);
				
                finalcolor.add(diffuseComponent);
                finalcolor.add(specularComponent);

				
        ////////////////////////////////////////////////////////////////////////
        //////////////////////INDIRECT DIFFUSE +  SPECULAR/////////////////////
        //////////////////////////////////////////////////////////////////////

                //send ray toward reflected direction...
                Ray reflectedRay = Ray(intersectPos, Vec3::reflect(Vec3::subtract(intersectPos,mainCamera->pos), intersectNormal));
				
				//Get intersected Sphere
				//intersectedSphere hold information abot intersected sphere in each [PASS]
				Sphere intersectedSphere2;

				//iInfo hold information abot intersection [PASS]
				IntersectionInfo iInfo2;

                //reflected photon intersects something...
				
                if (getInsersectedSphere(reflectedRay, intersectedSphere2, iInfo2))
                {
					//return diffuseColor;
                    Vec3 intersectPos1 = iInfo2.intersectPoint;
                    Vec3 intersectNormal1 = intersectedSphere2.getNormalAtPoint(intersectPos1);

                    ///SHADOW RAYS AGAIN/////////////
                    list<double> shadowFactors1;
                    generateShadowFactors(shadowFactors1,intersectPos1);

                    //Calculate color
                    Vec3 diffColor = Vec3::product(mainShader->calcDiffuseColor(intersectedSphere2.c, intersectPos1, intersectNormal1, plights, shadowFactors1), intersectedSphere2.reflectness);
                    Vec3 specColor = Vec3::product(mainShader->calcSpecularColor(mainCamera->pos, intersectPos, intersectNormal, plights, shadowFactors), intersectedSphere2.reflectness);
					
                    finalcolor.add(specColor);
					finalcolor.add(diffColor);
                }
                
				return finalcolor.toColor();
            }

            //calculate lighting...
			return Vec3(0,0,0).toColor();
			*/
        }
    };
}
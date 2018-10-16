/*
Summary: Creates spheres. Can now add spheres to scenes! Exciting stuff :)
*/

#ifndef _SPHERE_H
#define _SPHERE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Sphere : public Object{
    Vect Center;
    double Radius;
    Color color;

public:

    Sphere ();

    Sphere (Vect, double, Color);

    //method functions
    Vect getSphereCenter(){    
        return Center;
    }

    double getSphereRadius(){
        return Radius;
    }
    
    virtual Color getColor(){
        return color;
    }
    
    virtual Vect getNormalAt (Vect point){
        Vect normal_Vect = point.VectAdd(Center.Invert()).normalize();
        return normal_Vect;
    }

    virtual double FindIntersection(Ray ray){ //scalar value that returns distance from origin of ray to point of intersection. NOTE: normal always points away from the center of a sphere
        Vect ray_origin = ray.getRayOrigin();
        double ray_origin_x = ray_origin.getVectX();
        double ray_origin_y = ray_origin.getVectY();
        double ray_origin_z = ray_origin.getVectZ();

        Vect ray_direction = ray.getRayDirection();
        double ray_direction_x = ray_direction.getVectX();
        double ray_direction_y = ray_direction.getVectY();
        double ray_direction_z = ray_direction.getVectZ();

        Vect sphere_center = Center;
        double sphere_center_x = sphere_center.getVectX();
        double sphere_center_y = sphere_center.getVectY();
        double sphere_center_z = sphere_center.getVectZ();

        double a = 1; //normalized
        double b = (2*(ray_origin_x - sphere_center_x)*ray_direction_x) + (2*(ray_origin_y - sphere_center_y)*ray_direction_y) + (2*(ray_origin_z - sphere_center_z)*ray_direction_z);
        double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (Radius*Radius);

        double discriminant = b*b - 4*c;

        if (discriminant > 0){ //thus ray intersects the sphere.
            //the first root
            double root1 = ((-1*b - sqrt(discriminant))/2) - 0.001; //0.001 arbitary value to increase accuracy. noticed weird errors without it...
            double root2 = ((sqrt(discriminant) - b)/2) - 0.001;
            if (root1 > 0){ //first root is the smallest positive root
                return root1; //returned for findinmtersention function
            }else{
                //root 2 is the smallest positive root
                return root2;
            }
        }else{//the ray missed the sphere; no intersection
            return -1;
        }
    }
};

//setting default parameters (if none supplied), sphere is at origin, radius is R and color is gray
Sphere::Sphere (){
    Center = Vect(0,0,0);
    Radius = 1.0;
    color = Color(0.5,0.5,0.5,0);
}

//Parameters we assign, get passed here
Sphere::Sphere (Vect Cen, double rad, Color Col){
    Center = Cen;
    color = Col;
    Radius = rad;
}

#endif
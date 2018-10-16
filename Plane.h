/*
Summary: Creates Planes.
*/

#ifndef _PLANE_H
#define _PLANE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Plane : public Object{
   
    Vect Normal;
    double Distance;
    Color color;

public:

    Plane ();

    Plane (Vect, double, Color);

    //method functions
    Vect getPlaneNormal(){
        return Normal;
    }

    double getPlaneDistance(){
        return Distance;
    }

    virtual Color getColor(){
        return color;
    }
    
    virtual Vect getNormalAt(Vect point){
        return Normal;
    }
    

    virtual double FindIntersection(Ray ray){ //finds intersection between a ray and the plane in terms of a distance (hence why value is a double not a vect)
        Vect ray_direction = ray.getRayDirection();
        double a = ray_direction.DotProduct(Normal);

        if (a==0){ //dot product is 0, ray is thus parallel to plane
            return -1;
        }else{
            double b = Normal.DotProduct(ray.getRayOrigin().VectAdd(Normal.VectMultiply(Distance).Invert()));
            return -1*b/a; //distance from ray origin to the point of intersection
        }
    }
};

//setting default parameters (if none supplied)
Plane::Plane (){
    Normal = Vect(1,0,0);
    Distance = 0;
    color = Color(0.5,0.5,0.5,0);
}

//Parameters we assign, get passed here
Plane::Plane (Vect NormalValue, double DistanceValue, Color Col){
    Normal = NormalValue;
    color = Col;
    Distance = DistanceValue;
}

#endif
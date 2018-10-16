/*
Summary: Defines the vector and calculates a variety of different operations we will have to  do
to the vector. Also can recall certain parts of vector (such as x-coordinate).
*/

#ifndef _VECT_H
#define _VECT_H

#include "math.h"

class Vect{
    double x;
    double y;
    double z;

public:

    Vect ();

    Vect (double, double, double);//Specifing what "Vect" is given

    //method functions
    double getVectX(){ //Get x-coordinate of the vector
        return x;
    }
    double getVectY(){
        return y;
    }
    double getVectZ(){
        return z;
    }



    //Finding magnitude of vector
    double magnitude (){
        return sqrt((x*x)+(y*y)+(z*z));
    }
    //normalizing vector
    Vect normalize (){ //Normalizing is required in computer graphics and especially for ray tracing. Operations derived from online sample.
        double magnitude = sqrt((x*x)+(y*y)+(z*z));
        return Vect (x/magnitude, y/magnitude, z/magnitude); //returns a vector, makes sense since the vector returned is now normalized.
    }
    //Invert vector
    Vect Invert (){
        return Vect (-x, -y, -x);
    }
    double DotProduct(Vect v){ //takes the dot product of 2 vectors, return scalar value
        return x*v.getVectX() + y*v.getVectY() + z*v.getVectZ();
    }
    Vect CrossProduct(Vect v){ //takes the cross product of 2 vectors, returns vector value
        return Vect (y*v.getVectZ() - z*v.getVectY(), z*v.getVectX() - x*v.getVectZ(), x*v.getVectY() - y*v.getVectX());
    }
    Vect VectAdd (Vect v){ //adds 2 vectors resulting in a vector
        return Vect (x+v.getVectX(), y+v.getVectY(), z+v.getVectZ());
    }
    Vect VectMultiply (double scalar){ //multiplies a vector by a given value and returns the resulting vector.
        return Vect (x*scalar, y*scalar, z*scalar);
    }
};

//setting default parameters (Vect CTOR)
Vect::Vect (){
    x = 0;
    y = 0;
    z = 0;
}

//Parameters we assign, get passed here
Vect::Vect (double i, double j, double k){
    x = i;
    y = j;
    z = k;
}

#endif
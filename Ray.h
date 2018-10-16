/*
Summary: Very similar to vector (note we included vect.h so all properties carry over). Now, instead,
we have a ray made up of 2 vectors representing the direction and origin.
*/

#ifndef _RAY_H
#define _RAY_H

#include "vect.h"

//Similar to vect.h
//Note main difference is that ray is made of vectors and have 2 vectors associated with them, direction and origin.

class Ray{
    Vect Origin;
    Vect Direction;

public:

    Ray ();

    Ray (Vect, Vect);

    //method functions
    Vect getRayOrigin(){
        return Origin;
    }
    
    Vect getRayDirection(){
        return Direction;
    }
};

//setting default parameters (if none supplied), class specifies default origin is origin of scene and direction is in x direction
Ray::Ray (){
    Origin = Vect(0,0,0);
    Direction = Vect(1,0,0);
}

//Parameters we assign, get passed here
Ray::Ray (Vect Org, Vect Dir){
    Origin = Org;
    Direction = Dir;
}

#endif
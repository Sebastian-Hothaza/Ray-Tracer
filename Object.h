/*
Summary: Creates objects. Top of hierachy where all objects in the scene become sub classes to the object class.
*/

#ifndef _OBJECT_H
#define _OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Object{
public:
    Object ();

    virtual Color getColor(){
        return Color(0.0, 0.0, 0.0, 0); //if we want to get object color,
    }

    virtual Vect getNormalAt(Vect intersection_position){
        return Vect (0,0,0);
    }

    virtual double FindIntersection(Ray ray){
        return 0;
    }
};

//setting default parameters
Object::Object ()
{

}

#endif
#ifndef _LIGHT_H
#define _LIGHT_H

#include "Source.h"
#include "Vect.h"
#include "Color.h"

class Light : public Source{
    Vect Position;
    Color color;
public:

    Light ();

    Light (Vect, Color);

    //method functions
    virtual Vect getLightPosition(){
        return Position;
    }
    virtual Color getLightColor(){
        return color;
    }
};

//setting default parameters (if none supplied), light is at origin and the color is white
Light::Light (){
    Position = Vect(0,0,0);
    color = Color(1,1,1,0);
}

//Parameters we assign, get passed here
Light::Light (Vect Pos, Color Col){
    Position = Pos;
    color = Col;
}

#endif
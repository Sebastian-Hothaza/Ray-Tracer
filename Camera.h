#ifndef _Camera_H
#define _Camera_H

#include "vect.h"

//Deals with camera position.

class Camera{
    Vect CamPos;
    Vect CamDir;
    Vect CamHor;
    Vect CamVer;

public:
    Camera ();

    Camera (Vect, Vect, Vect, Vect);

    //method functions
    Vect getCameraPosition(){
        return CamPos;
    }
    Vect getCameraDirection(){
        return CamDir;
    }
    
    Vect getCameraHorizontal(){
        return CamHor;
    }
    
    Vect getCameraVertical(){
        return CamVer;
    }
}

//setting default parameters (if none supplied),  specifies default origin is origin of scene and direction is in z direction
Camera::Camera (){
    CamPos = Vect(0,0,0);
    CamDir = Vect(0,0,1);
    CamHor = Vect(0,0,0);
    CamVer = Vect(0,0,0);
}

//Parameters we assign, get passed here
Camera::Camera (Vect Org, Vect Dir, Vect Hor, Vect Ver){
    CamPos = Org;
    CamDir = Dir;
    CamHor = Hor;
    CamVer = Ver;
}

#endif 

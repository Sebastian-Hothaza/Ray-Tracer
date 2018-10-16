//Main.cpp
//Sebastian Hothaza
//
//Final Project, Ray Tracing
//STATE:NOT FINISHED; NOT TO BE ASSESSED


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Source.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"

using namespace std;

//Storage of colors, ranges from 0 to 1
struct RGBType{
    double r;
    double g;
    double b;
};

//Savebmp function from online resource, give a file name along with dimensions and data of each pixel
void savebmp (const char *filename, int w, int h, int dpi, RGBType *data){
    FILE *f;
    int k = w*h;
    int s = 4*k;
    int filesize = 54 + s;

    double factor = 39.375;
    int m = static_cast<int>(factor);

    int ppm = dpi*m;

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

    bmpfileheader[ 2] = (unsigned char)(filesize);
    bmpfileheader[ 3] = (unsigned char)(filesize>>8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(w);
    bmpinfoheader[ 5] = (unsigned char)(w>>8);
    bmpinfoheader[ 6] = (unsigned char)(w>>16);
    bmpinfoheader[ 7] = (unsigned char)(w>>24);

    bmpinfoheader[ 8] = (unsigned char)(h);
    bmpinfoheader[ 9] = (unsigned char)(h>>8);
    bmpinfoheader[10] = (unsigned char)(h>>16);
    bmpinfoheader[11] = (unsigned char)(h>>24);

    bmpinfoheader[21] = (unsigned char)(s);
    bmpinfoheader[22] = (unsigned char)(s>>8);
    bmpinfoheader[23] = (unsigned char)(s>>16);
    bmpinfoheader[24] = (unsigned char)(s>>24);

    bmpinfoheader[25] = (unsigned char)(ppm);
    bmpinfoheader[26] = (unsigned char)(ppm>>8);
    bmpinfoheader[27] = (unsigned char)(ppm>>16);
    bmpinfoheader[28] = (unsigned char)(ppm>>24);

    bmpinfoheader[29] = (unsigned char)(ppm);
    bmpinfoheader[30] = (unsigned char)(ppm>>8);
    bmpinfoheader[31] = (unsigned char)(ppm>>16);
    bmpinfoheader[32] = (unsigned char)(ppm>>24);

    f = fopen(filename,"wb");

    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);

    for (int i = 0; i < k; i++){
        RGBType rgb = data[i];

        double red = (data[i].r)*255;
        double green = (data[i].g)*255;
        double blue = (data[i].b)*255;

        unsigned char color[3] = {(int)floor(blue),(int)floor(green),(int)floor(red)};

        fwrite(color,1,3,f);
    }
    fclose(f);
}

int index; //value that will change as we go through the pixels. Value used to determine the x and y coordinates of an individual pixel

int winningObjectIndex(vector<double>object_intersections){ //returns the index of the winning intersection (closest to camera)
    int index_of_min_value;
    //prevent unnecesary calculations
    if (object_intersections.size()==0){//if there are no intersections
        return -1; //saves processing power
    }else if(object_intersections.size()==1){//if there is 1 intersection
        if (object_intersections.at(0) > 0){
            //if that intersection is greater than 0 then its our index of min val
            return 0; //not value of intersection, this is ONLY the index. First element in vector; element 0
        }else{
            //otherwise the only intersection value is negative, ray missed everything :(
            return -1;
        }
    }else{ //more than 1 intersection in array, need to determine which is first
        double max = 0;
        for (int i=0; i < object_intersections.size(); i++){ //looping through each intersection to find which intersection is closest to the camera
            if (max < object_intersections.at(i))
                max = object_intersections.at(i);
        }
        //then start from max value to find the min positive value
        if (max > 0){ //we only want positive intersections
            for (int i=0; i< object_intersections.size(); i++){
                if (object_intersections.at(i) > 0 && object_intersections.at(i) <= max){
                    max = object_intersections.at(i);
                    index_of_min_value = i;
                }
            }
            return index_of_min_value;
        }else{
            return -1; //all the intersections were negative, nothing usefull
        }
    }
}

Color getColorAt(Vect intersection_position,Vect intersection_ray_direction, vector<Object*>scene_objects, int ClosestObject, vector<Source*> light_sources, double accuracy, double ambientLight){
    Color winning_object_color = scene_objects.at(ClosestObject)->getColor();
    Vect winning_object_normal = scene_objects.at(ClosestObject)->getNormalAt(intersection_position);

    if (winning_object_color.getColorSpecial() == 2){ //special color 2 coresponds to the checkered pattern
        int square = (int)floor(intersection_position.getVectX()) + (int)floor(intersection_position.getVectZ());
        if((square % 2) == 0){ //black tile
            winning_object_color.setColorRed(0);
            winning_object_color.setColorGreen(0);
            winning_object_color.setColorBlue(0);
        }else{ //white tile
            winning_object_color.setColorRed(1);
            winning_object_color.setColorGreen(1);
            winning_object_color.setColorBlue(1);
        }
    }

    Color final_color = winning_object_color.colorScalar(ambientLight);//scale color of object by ambient light factor

    if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1){ //reflections, special value must be between 0 and 1
        //reflection from objects with specular intensity (shinny stuff)
        double dot1 = winning_object_normal.DotProduct(intersection_ray_direction.Invert());
        Vect scalar1 = winning_object_normal.VectMultiply(dot1);
        Vect add1 = scalar1.VectAdd(intersection_ray_direction);
        Vect scalar2 = add1.VectMultiply(2);
        Vect add2 = intersection_ray_direction.Invert().VectAdd(scalar2);
        Vect reflection_direction = add2.normalize();

        //similar to shadow ray, but now reflection ray
        Ray reflection_ray (intersection_position, reflection_direction);

        //determine what the ray intersects with first
        vector<double>reflection_intersections;

        for (int i=0; i<scene_objects.size(); i++){
            reflection_intersections.push_back(scene_objects.at(i)->FindIntersection(reflection_ray));
        }

        int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);


        if (index_of_winning_object_with_reflection != -1){ //reflection ray missed everyting else
            if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy){
                //determine the position and the direction at the point of intersection with the reflection ray
                //the ray only affects the color IF it reflected off of something first

                Vect reflection_intersection_position = intersection_position.VectAdd(reflection_direction.VectMultiply(reflection_intersections.at(index_of_winning_object_with_reflection)));
                Vect reflection_intersection_ray_direction = reflection_direction;

                //recursive process... getcolorat routine
                Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, index_of_winning_object_with_reflection, light_sources, accuracy, ambientLight);
                //modifying final color to include the reflection color influence
                final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(winning_object_color.getColorSpecial()));

            }
        }
    }

    for (int i=0; i<light_sources.size(); i++){ //looping through light sources
        Vect light_direction = light_sources.at(i)->getLightPosition().VectAdd(intersection_position.Invert()).normalize(); //problem

        float cosine_angle = winning_object_normal.DotProduct(light_direction);

        if(cosine_angle > 0){
            //test for shadows
            bool isShadow = false;

            Vect distance_to_light = light_sources.at(i)->getLightPosition().VectAdd(intersection_position.Invert()).normalize();
            float distance_to_light_magnitude = distance_to_light.magnitude(); //distance to light source from intersection

            Ray shadow_ray (intersection_position, light_sources.at(i)->getLightPosition().VectAdd(intersection_position.Invert()).normalize()); //new ray headed to light source

            vector<double>secondary_intersections;

            for (int z=0; z<scene_objects.size() && isShadow==false; z++){
                secondary_intersections.push_back(scene_objects.at(z)->FindIntersection(shadow_ray));
            }

            for (int c = 0;c<secondary_intersections.size(); c++){
                if (secondary_intersections.at(c) > accuracy){
                    if (secondary_intersections.at(c) <= distance_to_light_magnitude){
                        isShadow = true; //if loop through secondary intersection, and we find intersection less than distance to light source, than that pixel is now shadowed
                    }
                    break;
                }
            }
            if (isShadow == false){
                final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(i)->getLightColor()).colorScalar(cosine_angle)); //from online resource, difficult line

                if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <=1){ //if shiny, special is shiny when 0-1 range
                    //determing the reflection direction
                    double dot1 = winning_object_normal.DotProduct(intersection_ray_direction.Invert());
                    Vect scalar1 = winning_object_normal.VectMultiply(dot1);
                    Vect add1 = scalar1.VectAdd(intersection_ray_direction);
                    Vect scalar2 = add1.VectMultiply(2);
                    Vect add2 = intersection_ray_direction.Invert().VectAdd(scalar2);
                    Vect reflection_direction = add2.normalize();

                    double specular = reflection_direction.DotProduct(light_direction);
                    if (specular > 0){
                        specular = pow(specular, 10);
                        final_color = final_color.colorAdd(light_sources.at(i)->getLightColor().colorScalar(specular*winning_object_color.getColorSpecial()));
                    }
                }
            }
        }
    }
    return final_color.clip();
}

int main(int argc, char *argv[]){
    cout << "Rendering Image, please wait. Final will be published in root." << endl;
    //add part at end where confirms render, ask if to open rendered image

    //Declaring variables and specifing window image
    int dpi = 128;
    int width = 640;
    int height = 480;
    int n = width*height;
    RGBType *pixels = new RGBType[n]; //n is total # of pixels in image

    double AspectRatio = (double)width/(double)height;
    double ambientLight = 0.2;
    double accuracy = 0.000001; //Required for mass operation accuracy. Similar use to the value used in ray intersection

    //using vector class to create new instance, creating 3 vectors coresponding to the x y and z.
    Vect Origin (0,0,0); //origin location
    Vect Slight_Right (-4,0,-4);
    Vect Behind1 (-3, 0, -3);
    Vect Behind2 (-2, 0, -2);
    Vect X (1,0,0);
    Vect Y (0,1,0);
    Vect Z (0,0,1);


    //camera position
    Vect CamPos (5, 5, 5); //(x,y,z)
    //Initializing
    Vect Look_At (Origin);


    //3 coordinates which are the difference between cam position minus the look_at vector
    Vect Difference_Between (CamPos.getVectX() - Look_At.getVectX(), CamPos.getVectY() - Look_At.getVectY(), CamPos.getVectZ() - Look_At.getVectZ());


    //Camera Direction, we define point we want camera to look at, get difference between camera position and point we want to look at. That
    //results in a vector. Camera direction is then the normalized vector that is opposite direction of the difference between the vector
    //between the camera and in this case the origin
    Vect CamDir = Difference_Between.Invert().normalize();
    Vect CamHor = Y.CrossProduct(CamDir).normalize();
    Vect CamVer = CamHor.CrossProduct(CamDir);
    Camera scene_cam (CamPos, CamDir, CamHor, CamVer);




    //Colors
    Color white_light (1.0, 1.0, 1.0, 0);
    Color Nice_Green (0.5, 1.0, 0.5, 0.3);
    Color Crimson (0.5, 0.25, 0.25, 0.3);
    Color tile_floor (1, 1, 1, 2);
    Color gray (0.5, 0.5, 0.5, 0.3);
    Color black (0.0, 0.0, 0.0, 0);

    Vect light_position (-5, 10,8); //position of light source
    Light scene_light (light_position, white_light);
    vector<Source*> light_sources;
    light_sources.push_back(dynamic_cast<Source*>(&scene_light));

    //-----------------------------
    //  S C E N E   O B J E C T S
    //-----------------------------
    Sphere scene_sphere (Origin, 1, gray);
    Sphere scene_sphere2 (Slight_Right, 0.3, Nice_Green);
    Sphere scene_sphere3 (Behind1, 0.3, Nice_Green);
    Sphere scene_sphere4 (Behind2, 0.3, Crimson);
    Plane scene_plane (Y, -1, tile_floor);

    vector<Object*> scene_objects;

    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
    //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
    //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere3));
    //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere4));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));

    int index;
    double xAmmount;
    double yAmmount;

    //-----------------------------
    // S E N D I N G   R A Y S
    //-----------------------------
    //Starts on left side of image, moving through each pixel to the right side of image
    for (int x = 0; x<width; x++){
        //Starts on top side of image, moving through each pixel to the bottom side of image
        for (int y = 0; y<height; y++){
            index = y*width + x;
            //Start with no Anti-Aliasing
            //Offsets a position from our direction that camera is pointing in order to  create rays that go to the left/right of the camera point direction. Same with above and below
            if (width > height){ //image wider than it is tall
                xAmmount = ((x+0.5)/width)*AspectRatio - (((width-height)/(double)height)/2);
                yAmmount = ((height - y) + 0.5) / height;
            }else if (height > width){ //image taller than wide
                xAmmount = (x + 0.5)/width;
                yAmmount = (((height - y) + 0.5)/height)/AspectRatio - (((height - width)/(double)width)/2);
            }else{ //square image
                xAmmount = (x+0.5)/width;
                yAmmount = ((height - y) + 0.5) / height;
            }

            Vect cam_ray_origin = scene_cam.getCameraPosition();
            Vect cam_ray_direction = CamDir.VectAdd(CamHor.VectMultiply(xAmmount - 0.5).VectAdd(CamVer.VectMultiply(yAmmount - 0.5))).normalize();

            Ray cam_ray (cam_ray_origin, cam_ray_direction);



            //We now have rays, time to send them! Note we are at a specific pixel right now!

            vector<double> intersections;
            //loop through each of the objects in our scene and determine if the ray created intersects with
            //any of the objects in the scene
            for (int i=0; i<scene_objects.size(); i++){
                //loops through each object, asks to find intersection with camray, then pushes that value into
                //the intersection vector
                intersections.push_back(scene_objects.at(i)->FindIntersection(cam_ray));
            }
            int ClosestObject = winningObjectIndex(intersections); //sorts and returns the index of the winning object (one closest to camera). return -1, 0 or 1

            //cout<<ClosestObject;

            if (ClosestObject == -1){
                //setting background to black
                pixels[index].r = 0;
                pixels[index].g = 0;
                pixels[index].b = 0;
            }else{ //index is either 0 or 1, thus corresponds to an object in our scene
                if (intersections.at(ClosestObject) > accuracy){//if intersection is greater than accuracy, then we can go ahead and get color of that object
                    //determning the position and dir vectr at point of intersect
                    Vect intersection_position = cam_ray_origin.VectAdd(cam_ray_direction.VectMultiply(intersections.at(ClosestObject)));
                    Vect intersection_ray_direction = cam_ray_direction;

                    Color intersection_color = getColorAt(intersection_position, intersection_ray_direction, scene_objects, ClosestObject, light_sources, accuracy, ambientLight); //based on getting the color at the intersection point

                    pixels[index].r = intersection_color.getColorRed();
                    pixels[index].g = intersection_color.getColorGreen();
                    pixels[index].b = intersection_color.getColorBlue();
                }
            }
        }
    }
    savebmp("scene.bmp", width, height, dpi, pixels);
    //clearing memory
    delete pixels;
    return 0;
}
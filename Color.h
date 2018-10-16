#ifndef _COLOR_H
#define _COLOR_H

class Color{
	double Red;
    double Green;
    double Blue;
    double Special;

public:

    Color ();

    Color (double, double, double, double); //Specifing what "Color" is given

    //method functions. When we call getColorRed in main.cpp, it returns the value of red (defined above)
    double getColorRed(){
        return Red;
    }
    double getColorGreen(){
        return Green;
    }
    double getColorBlue(){
        return Blue;
    }
    double getColorSpecial(){
        return Special;
    }

    //Used to set color. When used in main.cpp, it will change the RedValue.
    double setColorRed(double redValue){
        Red = redValue;
    }
    
    double setColorGreen(double greenValue){
        Green = greenValue;
    }

    double setColorBlue(double blueValue){
        Blue = blueValue;
    }
    double setColorSpecial(double specialValue){
        Special = specialValue;
    }
    

    double brightness(){
        return (Red+Green+Blue)/3;
    }
    Color colorScalar(double scalar){//scales our colors by some scalar
        return Color(Red*scalar, Green*scalar, Blue*scalar, Special);
    }


    Color colorAdd(Color color){
        return Color (Red + color.getColorRed(), Green + color.getColorGreen(), Blue + color.getColorBlue(), Special);
    }
    Color colorMultiply(Color color){ //multiplication of each of the RGB components, similar to multiplying scalar
        return Color(Red * color.getColorRed(), Green * color.getColorGreen(), Blue * color.getColorBlue(), Special);
    }
    Color colorAverage(Color color){
        return Color((Red + color.getColorRed())/2, (Green + color.getColorGreen())/2, (Blue + color.getColorBlue())/2, Special);
    }
    

    Color clip(){ //overbleeding color values solution. Checks to see if light exceeds values and normalizes it
        double allLight = Red + Green + Blue;
        double excessLight = allLight - 3;
        if (excessLight > 0){
            Red = Red + excessLight*(Red/allLight);
            Green = Green + excessLight*(Green/allLight);
            Blue = Blue + excessLight*(Blue/allLight);
        }
        if (Red > 1)
            Red=1;
        if (Green > 1)
            Green=1;
        if (Blue > 1)
            Blue=1;
        if (Red < 0)
            Red=0;
        if (Green < 0)
            Green=0;
        if (Blue < 0)
            Blue=0;        
        return Color (Red, Green, Blue, Special);
    }
};

//setting default parameters (if none supplied) default color is gray
Color::Color (){
    Red = 0.5;
    Green = 0.5;
    Blue = 0.5;
}

//Parameters we assign, get passed here
Color::Color (double r, double g, double b, double s){
    Red = r;
    Green = g;
    Blue = b;
    Special = s;
}

#endif

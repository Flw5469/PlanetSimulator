#include <iostream>
#include <array>
#include <tuple>
#include <cmath>
using namespace std;
#define planetSize 2
#define tick 1
#define G 6.647e-11
#define loopTime 1
class Coordinate{
    public: double xCoord;
            double yCoord;
            void setCoordinate(double xInput,double yInput){
                this->xCoord=xInput;
                this->yCoord=yInput;
            }
            Coordinate(){
                this->xCoord=0;
                this->yCoord=0;
            };
            Coordinate(double xInput, double yInput){
                setCoordinate(xInput,yInput);
            }
            void add(Coordinate value){
                xCoord+=value.xCoord;
                yCoord+=value.yCoord;
            }
            void time(double value){
                xCoord*=value;
                yCoord*=value;
            }
            void display(){
                cout<<xCoord<<" "<<yCoord<<endl;
            }
};

class Planet{
    public: Coordinate coordinate;
            double mass;
            Coordinate acceleration;
            Coordinate velocity;
            int id;
            Planet(){
                mass=0;
                acceleration=Coordinate();
                velocity=Coordinate();
                coordinate=Coordinate();
            };

            Planet(int id,double xCoord, double yCoord,double mass):Planet(){
                
                this->id=id;
                this->coordinate.setCoordinate(xCoord,yCoord);
                this->mass=mass;
            }
            
            Planet(tuple<double,double,double,int> info):Planet(){
                this->coordinate.setCoordinate(get<0>(info),get<1>(info));
                this->mass=get<2>(info);
                this->id=get<3>(info);
            };
};

void render(array<Planet,planetSize> planetList){
    for (auto p:planetList){
        cout<<"coordinate of "<<p.id<<": ";
        p.coordinate.display();
    }
}

double radius(Planet a,Planet b){
    double xDist=a.coordinate.xCoord-b.coordinate.xCoord;
    double yDist=a.coordinate.yCoord-b.coordinate.yCoord;
    return sqrt(xDist*xDist+yDist*yDist);
}

Coordinate distance(Coordinate self, Coordinate target){
    double x=target.xCoord-self.xCoord;
    double y=target.yCoord-self.yCoord;
    cout<<"calculating distance"<<x<<" "<<y<<endl;
    Coordinate distance=Coordinate(target.xCoord-self.xCoord,target.yCoord-self.yCoord);
    return distance;
}

void applyAttraction(Planet& giver, Planet& receiver){
    Coordinate distanceVector=distance(giver.coordinate,receiver.coordinate);
    cout<<"distance vector:";
    distanceVector.display();
    distanceVector.time(G*giver.mass/radius(giver,receiver));
    cout<<"scaled distance vector:";
    distanceVector.display();
    receiver.acceleration.add(distanceVector);
    cout<<"calculated acceleration: "<<endl;
    receiver.acceleration.display();
};

void applyAcceleration(Planet& planet){
    cout<<"acceleration: "<<endl;
    planet.acceleration.display();
    planet.velocity.add(planet.acceleration);
};
void applyVelocity(Planet& planet){
    cout<<"velocity: "<<endl;
    planet.velocity.display();
    planet.coordinate.add(planet.velocity);
};

void update(double inputTick,array<Planet,planetSize>& planetList){
    for (auto& receiver:planetList){
        for (auto& giver:planetList){
            if (receiver.id==giver.id){
                cout<<"repeated"<<endl;
            }
            else applyAttraction(giver,receiver);
        }
    }
    for (auto& planet:planetList){
        cout<<endl;
        applyAcceleration(planet);
        applyVelocity(planet);
    }
    render(planetList);
    cout<<endl;
}
 
int main(){
    //x y mass id
   array<tuple<double, double, double, int>, 2> init = {
    make_tuple(0, 0, 0, 0),
    make_tuple(100, 100, 1000000000000, 1)
    };
    array<Planet,planetSize> planetList;
    for (int i=0;i<planetSize;i++){
        auto initInfo=init.at(i);
        planetList.at(i)=Planet(initInfo);
    }
    for (int i=0;i<loopTime;i++){
        update(tick,planetList);
    }
    return 0;
}
#include "World.h"

World::World(double dt) : dt(dt) {}

void World::tick() {

    phys::Vector phi[objects.size()];

    for (int i = 0; i < objects.size(); ++i) {

        phys::Body b = objects[i].getBody();
        phys::Vector vector = b.updateSpeed(dt) * dt;
        objects[i].move(vector);

        const phys::Vector vec = phys::Vector(objects[i].getCoord());

        for (int j = 0; j < objects.size(); ++j) {

            if (j == i)
                continue;

            Coordinate coord = objects[j].getCoord();
            coord.invert();
            phys::Vector v = vec + phys::Vector(coord);
            double l = v.length();

            //F=ma, F=GmM/R^2 => a=GM/R^2 + normalization
            v = v * (GRAV_CONST * b.getMass()/l/l/l);

            phi[j] = phi[j] + v;
        }
    }

    for (int i = 0; i < objects.size(); ++i) {
        phys::Body b = objects[i].getBody();
        b.increaseAcceleration(phi[i]);
    }
}

void World::Object::move(phys::Vector vector) {
    move(vector.getEnd().getX(),
         vector.getEnd().getY());
}

void World::Object::move(double dx, double dy) {
    coord.move(dx, dy);
}

World::Object::Object(phys::Body *b, const Coordinate &c) :
        body(b), coord(c) {}

double World::Object::getDistance(World::Object object) const {
    return this->coord.getDistance(object.coord);
}
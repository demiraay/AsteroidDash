#include "CelestialObject.h"

#include <iostream>


// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
        : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {
    // TODO: Your code here


    right_rotation = this;
    left_rotation = this ;
    next_celestial_object = nullptr;



}


// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape),  // Copy the 2D vector shape
          object_type(other->object_type),  // Copy the object type
          starting_row(other->starting_row),  // Copy the starting row
          time_of_appearance(other->time_of_appearance)  // Copy the time of appearance
{
    // TODO: Your code here
}

// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target) {
    if (!target) return;

    CelestialObject *current = target->right_rotation;
    while (current != target) {
        CelestialObject *next = current->right_rotation;
        delete current;
        current = next;
    }


    target->right_rotation = target;
    target->left_rotation = target;
}

std::vector<std::vector<bool>> CelestialObject::rotate_clockwise(const std::vector<std::vector<bool>> &shape) {
    int height = shape.size();
    int width = shape[0].size();
    std::vector<std::vector<bool>> rotated(width, std::vector<bool>(height));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            rotated[j][height - i - 1] = shape[i][j];
        }
    }






    return rotated;
}

void CelestialObject::compute_rotations() {

    // İlk şekli al
    std::vector<std::vector<bool>> current_shape = this->shape;


    CelestialObject* last_rotation = this;


    for (int r = 1; r < 4; ++r) {

        std::vector<std::vector<bool>> rotated_shape = rotate_clockwise(current_shape);

        if (is_rotation_duplicate(rotated_shape)) {
            break;
        }


        CelestialObject* new_rotation = new CelestialObject(rotated_shape, this->object_type, this->starting_row, this->time_of_appearance);






        last_rotation->right_rotation = new_rotation;
        new_rotation->left_rotation = last_rotation;
        last_rotation = new_rotation;


        current_shape = rotated_shape;
    }


    last_rotation->right_rotation = this;
    this->left_rotation = last_rotation;



}



bool CelestialObject::is_rotation_duplicate(const std::vector<std::vector<bool>> &rotated) {
    CelestialObject* current = this;
    int steps = 0;

    do {
        if (current->shape == rotated) {
            return true;
        }
        current = current->right_rotation;
        steps++;

        if (steps > 4) break;
    } while (current != this);

    return false;
}


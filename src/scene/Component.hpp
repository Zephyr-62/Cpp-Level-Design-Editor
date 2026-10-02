#pragma once

#include "editor/Inspectable.hpp"

// Wrapping to differentiate from other inspectable elements 
// These can be direct additions to a Scene Object, unlike e.g. resources
class Component : public Inspectable {

};
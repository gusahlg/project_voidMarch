#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

extern "C" {

void add_vectors(float a_x, float a_y, float b_x, float b_y, float *out_x, float *out_y);

}  // extern "C"

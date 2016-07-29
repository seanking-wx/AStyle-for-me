#include "ASResource.h"

namespace AStyle
{

/**
 * Sort comparison function.
 * Compares the length of the value of pointers in the vectors.
 * The LONGEST strings will be first in the vector.
 *
 * @param a and b, the string pointers to be compared.
 */
bool sortOnLength(const string* a, const string* b)
{
	return (*a).length() > (*b).length();
}

/**
 * Sort comparison function.
 * Compares the value of pointers in the vectors.
 *
 * @param a and b, the string pointers to be compared.
 */
bool sortOnName(const string* a, const string* b)
{
	return *a < *b;
}

}   // end of astyle namespace

/**
 * @brief Vector_t
 * int* data - pointer to array of data
 * int size - size of vector
 * int items - how many items are stored now in vector
 */
typedef struct Vector_t
{
    int* data;
    int size;
    int items;
} Vector_t;

/**
 * @brief Initialize vectori with given size
 *
 * @param size size of initialized vector
 * @return Vector_t
 */
Vector_t InitVector(int size);

/**
 * @brief Adds value to the end of the vector. If vector is full (size == items) - doubles the size of the vector
 *
 * @param vector
 * @param value
 */
void AddToVector(Vector_t* vector, int value);

/**
 * @brief Prints every item of the vector
 *
 * @param vector
 */
void PrintVector(Vector_t vector);

/**
 * @brief Releases memory allocated by the vector
 *
 */
void FreeVector();
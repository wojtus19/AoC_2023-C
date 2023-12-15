#ifndef MAX_STR
#define MAX_STR 10
#endif // !MAX_STR
/**
 * @brief Vector_t
 * int* data - pointer to array of data
 * int size - size of vector
 * int items - how many items are stored now in vector
 */
typedef struct lens_t
{
    char label[MAX_STR];
    int value;
} lens_t;

typedef struct Vector_t
{
    lens_t* data;
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
 * @brief Adds value to the end of the vector. If vector is full (size == items) - doubles the size of the vector. If there's an item with the same label, changes its value
 *
 * @param vector
 * @param label
 * @param value
 */
void AddToVector(Vector_t* vector, char* label, int value);

/**
 * @brief Deletes item of given label, if exists
 *
 * @param vector
 * @param label
 */
void DeleteFromVector(Vector_t* vector, char* label);

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
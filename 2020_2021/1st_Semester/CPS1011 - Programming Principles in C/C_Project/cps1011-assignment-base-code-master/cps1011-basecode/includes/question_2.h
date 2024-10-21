#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Types - could extract to separate header file too
typedef struct sparse_set sparse_set_t;
typedef sparse_set_t *sparse_set_ptr;

// Type of element to be stored by the sparse_set (must be an unsigned type)
typedef uint32_t sparse_set_elem_t;
typedef sparse_set_elem_t *sparse_set_elem_ptr;

// Type for index (must be an unsigned type)
typedef uint16_t sparse_set_index_t;
typedef sparse_set_index_t *sparse_set_index_ptr;

//Types - could extract to separate header file too
typedef struct sparse_set{
    sparse_set_index_t ss_capacity;
    sparse_set_index_t *sparse_array;
    sparse_set_elem_t *dense_array;
    sparse_set_elem_t max_value;
    sparse_set_elem_t current_no_elements;
}sparse_set_t;
typedef sparse_set_t *sparse_set_ptr;

/**
 * Initialise a sparse set data structure in the range of [0..max_value]
 * @param capacity The maximum number of elements stored
 * @param max_value The maximum integer value to be stored
 * @return A sparse set data structure. NULL on fail.
 */
sparse_set_ptr ss_init(sparse_set_index_t capacity, sparse_set_elem_t max_value);

/**
 * Destroys and cleans up the passed sparse set.
 * @param sparse_set The sparse set to destroy
 * @return true on success, false otherwise.
 */
bool ss_destroy(sparse_set_ptr sparse_set);

/**
 * Add @param element to @param sparse_set.
 * @param sparse_set The sparse set to add the element to
 * @param element The element to add to the sparse set
 * @return true on success, false otherwise.
 */
bool ss_add(sparse_set_ptr sparse_set, sparse_set_elem_t element);

/**
 * Remove @param element from @param sparse_set.
 * @param sparse_set The sparse set to remove the element from
 * @param element The element to remove from the sparse set
 * @return true on success, false otherwise.
 */
bool ss_remove(sparse_set_ptr sparse_set, sparse_set_elem_t element);

/**
 * Check whether @param element is contained in @param sparse_set.
 * @param sparse_set The sparse set to search in
 * @param element The element to search for
 * @return true if @param element is in @param sparse_set, false otherwise.
 */
bool ss_contained(sparse_set_ptr sparse_set, sparse_set_elem_t element);

/**
 * Sorts the sparse set @param sparse_set.
 * @param sparse_set The sparse set to be sorted
 * @return true on success, false otherwise.
 */
bool ss_sort(sparse_set_ptr sparse_set);

/**
 * Returns the number of elements contained in
 * this sparse set.
 * @param sparse_set The sparse set that will be queried
 * @return The number of elements contained in sparse_set.
 *         -1 on failure.
 */
ssize_t ss_size(sparse_set_ptr sparse_set);

/**
 * Returns the element found in position @param sparse_index
 * @param sparse_set The sparse set to query
 * @param sparse_index The i'th element to return
 * @param dest The element at the i'th position, false on failure.
 * @return
 */
bool ss_get_elem(sparse_set_ptr sparse_set, sparse_set_index_t sparse_index, sparse_set_elem_ptr dest);

/**
 * Prints out the state of the sparse_set
 * @param sparse_set The sparse set to print out details about
 * @return true on success, false otherwise.
 */
bool ss_print_status(sparse_set_ptr sparse_set, bool print_as_chars);

// For each macro definition
#define SS_FOREACH(elem, sparse_set, body) {\
    for(ssize_t i = 0; i < ss_size(sparse_set); ++i)\
    {\
        sparse_set_elem_t elem;\
        if (ss_get_elem(sparse_set, i, &elem)) {\
            body\
        }\
    }\
}

/**
 * Returns the union of sets @param a and @param b in @param result
 * @param a The first set to union
 * @param b The second set to union
 * @param result The union of set @param a with set @param b
 * @return true on success, false otherwise.
 */
bool ss_union(sparse_set_ptr a, sparse_set_ptr b, sparse_set_ptr result);

/**
 * Returns the intersection of sets @param a and @param b in @param result
 * @param a The first set to intersect
 * @param b The second set to intersect
 * @param result The intersection of set @param a with set @param b
 * @return true on success, false otherwise.
 */
bool ss_intersection(sparse_set_ptr a, sparse_set_ptr b, sparse_set_ptr result);

/**
 * Returns the difference of set @param b from set @param a in @param result
 * @param a The first set
 * @param b The second set to be subtracted from the first set
 * @param result The set difference of set @param a from set @param b
 * @return true on success, false otherwise.
 */
bool ss_difference(sparse_set_ptr a, sparse_set_ptr b, sparse_set_ptr result);

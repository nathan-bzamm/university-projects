#include <stdio.h>
#include <stdbool.h>

#include <minunit.h>

#include "question_2.h"

MU_CUSTOM_TEST_START(testSSInit)
    sparse_set_ptr ss = ss_init(4, 15);
    mu_custom_check(ss != NULL, "expected successful initialisation", 1, 1);
    ss_destroy(ss);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testSSDestroy)
    sparse_set_ptr ss = ss_init(4, 15);
    bool res = ss_destroy(ss);
    mu_custom_check(res, "expected successful destruction", 1, 1);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testSSAdd)
    bool res = true;
    sparse_set_ptr ss = ss_init(4, 15);
    res &= ss_add(ss, 13);
    mu_custom_check(res, "expected successful add of 13", 1, 1);
    ss_destroy(ss);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testSSRemove)
    bool res = true;
    sparse_set_ptr ss = ss_init(4, 15);
    res &= ss_add(ss, 13);
    res &= ss_add(ss, 11);
    res &= ss_add(ss, 15);

    res &= ss_remove(ss, 11);
    mu_custom_check(res, "expected successful remove of 11", 1, 2);

    res = ss_contained(ss, 11);
    mu_custom_check(!res, "expected element 11 not in list", 2, 2);
    ss_destroy(ss);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testSSContained)
    bool res = true;
    sparse_set_ptr ss = ss_init(4, 15);

    res &= ss_add(ss, 13);
    res &= ss_add(ss, 11);
    res &= ss_add(ss, 15);
    res &= ss_add(ss, 4);

    res = ss_contained(ss, 11);
    mu_custom_check(res, "expected element 11 in list", 1, 1);
    ss_destroy(ss);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testSSGetElem)
    bool res = true;
    sparse_set_ptr ss = ss_init(4, 15);

    res &= ss_add(ss, 13);
    res &= ss_add(ss, 11);
    res &= ss_add(ss, 15);
    res &= ss_add(ss, 4);

    sparse_set_elem_t element;
    sparse_set_index_t index = 1;
    res &= ss_get_elem(ss, index, &element);

    mu_custom_check(res, "expected TRUE", 1, 2); // To do: Improve error message.
    mu_custom_check(element == 11, "expected element 11 @ index 1", 2, 2);
    ss_destroy(ss);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testSSSize)
    bool res = true;
    sparse_set_ptr ss = ss_init(10, 100);

    res &= ss_add(ss, 13);
    res &= ss_add(ss, 11);
    res &= ss_add(ss, 11);
    res &= ss_add(ss, 15);
    res &= ss_add(ss, 13);
    res &= ss_add(ss, 15);
    res &= ss_add(ss, 15);
    res &= ss_add(ss, 90);
    res &= ss_add(ss, 20);
    res &= ss_add(ss, 4);
    mu_custom_check(res, "expected all additions to succeed", 1, 2);
    mu_custom_check(ss_size(ss) == 6,"expected size 6", 2, 2);
    ss_destroy(ss);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testSSSort)
    bool res = true;
    sparse_set_ptr ss = ss_init(4, 15);

    res &= ss_add(ss, 13);
    res &= ss_add(ss, 11);
    res &= ss_add(ss, 15);
    res &= ss_add(ss, 4);
    
    int count = ss_size(ss);

    mu_custom_check(count == 4, "expected size 4", 1, 5);

    sparse_set_elem_t current_element = 0;
    sparse_set_elem_t previous_element = 0;

    res &= ss_sort(ss);
    mu_custom_check(res, "expected successful sort return value", 2, 5);

    for(int index = 0; index < count; index++)
    {
        ss_get_elem(ss, index, &current_element);

        if(index != 0)
        {
            mu_custom_check(current_element >= previous_element, "expected previous element <= current element", index + 2, 5);
        }

        previous_element = current_element;
    }

    ss_destroy(ss);
MU_CUSTOM_TEST_END

bool init_sparse_sets(sparse_set_ptr *a, sparse_set_ptr *b, sparse_set_ptr *c)
{
    bool res = true;
    *a = ss_init(4, 15);

    res &= ss_add(*a, 13);
    res &= ss_add(*a, 11);
    res &= ss_add(*a, 15);
    res &= ss_add(*a, 4);

    *b = ss_init(4, 15);

    res &= ss_add(*b, 8);
    res &= ss_add(*b, 9);
    res &= ss_add(*b, 0);
    res &= ss_add(*b, 11);

    *c = ss_init(8, 15);

    return res;
}

MU_CUSTOM_TEST_START(testSSUnion)
    sparse_set_ptr ss_a, ss_b, ss_c;
    bool res = init_sparse_sets(&ss_a, &ss_b, &ss_c);    
    mu_custom_check(res,"expected successful initialisation", 1, 5);

    res &= ss_union(ss_a, ss_b, ss_c);
    mu_custom_check(res, "expected successful union", 2, 5);

    int count = ss_size(ss_c);
    mu_custom_check(count == 7, "expected size 7", 3, 5);

    res &= ss_sort(ss_c);

    sparse_set_elem_t element = -1;
    res  &= ss_get_elem(ss_c, 1, &element);

    mu_custom_check(res,"expected successful get element", 4, 5);
    mu_custom_check(element == 4, "expected element 4 @ index 1", 5, 5);

    ss_destroy(ss_a);
    ss_destroy(ss_b);
    ss_destroy(ss_c);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testSSIntersection)
    sparse_set_ptr ss_a, ss_b, ss_c;
    bool res = init_sparse_sets(&ss_a, &ss_b, &ss_c);    
    mu_custom_check(res, "expected successful initialisation", 1, 5);

    res &= ss_intersection(ss_a, ss_b, ss_c);
    mu_custom_check(res,"expected successful intersection", 2, 5);

    int count = ss_size(ss_c);
    mu_custom_check(count == 1, "expected size 1", 3, 5);

    res &= ss_sort(ss_c);

    sparse_set_elem_t element = -1;
    res  &= ss_get_elem(ss_c, 0, &element);

    mu_custom_check(res, "expected successful get element", 4, 5);
    mu_custom_check(element == 11, "expected element 11 @ index 0", 5, 5);

    ss_destroy(ss_a);
    ss_destroy(ss_b);
    ss_destroy(ss_c);
MU_CUSTOM_TEST_END

MU_CUSTOM_TEST_START(testSSDifference)
    sparse_set_ptr ss_a, ss_b, ss_c;
    bool res = init_sparse_sets(&ss_a, &ss_b, &ss_c);    
    mu_custom_check(res, "expected successful initialisation", 1, 4);

    res &= ss_difference(ss_a, ss_b, ss_c);
    mu_custom_check(res, "expected successful difference", 2, 4);

    int count = ss_size(ss_c);
    mu_custom_check(count == 3, "expected size 3", 3, 4);

    sparse_set_elem_t element = -1;
    res  &= ss_contained(ss_c, 11);

    mu_custom_check(!res, "element 11 should not be present", 4, 4);

    ss_destroy(ss_a);
    ss_destroy(ss_b);
    ss_destroy(ss_c);
MU_CUSTOM_TEST_END

MU_TEST_SUITE(test_suite) {
	MU_RUN_TEST(testSSInit);
	MU_RUN_TEST(testSSDestroy);
	MU_RUN_TEST(testSSAdd);
	MU_RUN_TEST(testSSRemove);

	MU_RUN_TEST(testSSContained);
	MU_RUN_TEST(testSSGetElem);
	MU_RUN_TEST(testSSSize);
	MU_RUN_TEST(testSSSort);
	MU_RUN_TEST(testSSDifference);
	MU_RUN_TEST(testSSUnion);
	MU_RUN_TEST(testSSIntersection);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}

#include <glib-object.h>
#include "gegl.h"
#include "ctest.h"
#include "csuite.h"
#include "testutils.h"
#include <string.h>

#define ADD0 .5
#define ADD1 .6
#define ADD2 .7

#define R0 .1 
#define G0 .2
#define B0 .3 

#define IMAGE_WIDTH 1 
#define IMAGE_HEIGHT 1 

static GeglOp * source;

static void
test_add_g_object_new(Test *test)
{
  {
    GeglAdd * add = g_object_new (GEGL_TYPE_ADD, NULL);  

    ct_test(test, add != NULL);
    ct_test(test, GEGL_IS_ADD(add));
    ct_test(test, g_type_parent(GEGL_TYPE_ADD) == GEGL_TYPE_UNARY);
    ct_test(test, !strcmp("GeglAdd", g_type_name(GEGL_TYPE_ADD)));

    g_object_unref(add);
  }
}

static void
test_add_g_object_properties(Test *test)
{
  {
    GeglAdd * add = g_object_new (GEGL_TYPE_ADD, 
                                  "input", 0, source,
                                  NULL);  

    ct_test(test, 2 == gegl_node_get_num_inputs(GEGL_NODE(add)));
    ct_test(test, source == (GeglOp*)gegl_node_get_source(GEGL_NODE(add), 0));

    g_object_unref(add);
  }

  {
    gfloat add0, add1, add2;
    GeglAdd * add = g_object_new (GEGL_TYPE_ADD, 
                                  "input", 0, source,
                                  "constant-rgb-float", ADD0, ADD1, ADD2,
                                  NULL);  

    g_object_get(add, "constant-rgb-float", &add0, &add1, &add2, NULL);

    ct_test(test, GEGL_FLOAT_EQUAL(ADD0, add0)); 
    ct_test(test, GEGL_FLOAT_EQUAL(ADD1, add1)); 
    ct_test(test, GEGL_FLOAT_EQUAL(ADD2, add2)); 

    {
      guint8 add0, add1, add2;
      g_object_get(add, "constant-rgb-uint8", &add0, &add1, &add2, NULL);
      ct_test(test, 128 == add0); 
    }

    g_object_unref(add);
  }
}

static void
test_add_apply(Test *test)
{
  {
    GeglOp *add = g_object_new(GEGL_TYPE_ADD,
                               "input", 0, source,
                               "constant-rgb-float", ADD0, ADD1, ADD2,
                               NULL);

    gegl_op_apply(add); 
    ct_test(test, testutils_check_pixel_rgb_float(GEGL_IMAGE(add), 
                                            R0 + ADD0, 
                                            G0 + ADD1, 
                                            B0 + ADD2));  
    g_object_unref(add);
  }

  {
    GeglOp *add1 = g_object_new(GEGL_TYPE_ADD,
                                 "input", 0, source,
                                 "constant-rgb-float", ADD0, ADD1, ADD2,
                                 NULL);

    GeglOp *add2 = g_object_new(GEGL_TYPE_ADD,
                                 "input", 0, add1,
                                 "constant-rgb-float", ADD0, ADD1, ADD2,
                                 NULL);

    gegl_op_apply(add2); 
    ct_test(test, testutils_check_pixel_rgb_float(GEGL_IMAGE(add2), 
                                            R0 + ADD0 + ADD0, 
                                            G0 + ADD1 + ADD1, 
                                            B0 + ADD2 + ADD2));  

    g_object_unref(add1);
    g_object_unref(add2);
  }
}

static void
add_test_setup(Test *test)
{
  source = g_object_new(GEGL_TYPE_COLOR, 
                        "width", IMAGE_WIDTH, 
                        "height", IMAGE_HEIGHT, 
                        "pixel-rgb-float", R0, G0, B0, 
                        NULL); 
}

static void
add_test_teardown(Test *test)
{
  g_object_unref(source);
}

Test *
create_add_test_rgb_float()
{
  Test* t = ct_create("GeglAddPixelRgbFloat");

  g_assert(ct_addSetUp(t, add_test_setup));
  g_assert(ct_addTearDown(t, add_test_teardown));

#if 1 
  g_assert(ct_addTestFun(t, test_add_g_object_new));
  g_assert(ct_addTestFun(t, test_add_g_object_properties));
  g_assert(ct_addTestFun(t, test_add_apply));
#endif

  return t; 
}

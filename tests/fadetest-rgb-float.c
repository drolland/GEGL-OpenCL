#include <glib-object.h>
#include "gegl.h"
#include "ctest.h"
#include "csuite.h"
#include "testutils.h"
#include <string.h>

#define MULTIPLIER .5

#define IMAGE_WIDTH 1 
#define IMAGE_HEIGHT 1 

static GeglOp * source;

static void
test_fade_g_object_new(Test *test)
{
  {
    GeglFade * fade = g_object_new (GEGL_TYPE_FADE, NULL);  

    ct_test(test, fade != NULL);
    ct_test(test, GEGL_IS_FADE(fade));
    ct_test(test, g_type_parent(GEGL_TYPE_FADE) == GEGL_TYPE_UNARY);
    ct_test(test, !strcmp("GeglFade", g_type_name(GEGL_TYPE_FADE)));

    g_object_unref(fade);
  }
}

static void
test_fade_g_object_properties(Test *test)
{
  {
    GeglFade * fade = g_object_new (GEGL_TYPE_FADE, 
                                    "multiplier", MULTIPLIER, 
                                    "input", 0, source,
                                     NULL);  

    ct_test(test, 2 == gegl_node_get_num_inputs(GEGL_NODE(fade)));
    ct_test(test, source == (GeglOp*)gegl_node_get_source(GEGL_NODE(fade), 0));

    g_object_unref(fade);
  }

  {
    GeglFade * fade = g_object_new (GEGL_TYPE_FADE, NULL);  

    g_object_set(fade, "multiplier", MULTIPLIER, NULL);

    g_object_unref(fade);
  }

  {
    gfloat multiplier;
    GeglFade * fade = g_object_new (GEGL_TYPE_FADE, 
                                    "multiplier", MULTIPLIER, 
                                    NULL);  

    g_object_get(fade, "multiplier", &multiplier, NULL);

    ct_test(test, MULTIPLIER == multiplier);

    g_object_unref(fade);
  }
}

static void
test_fade_apply(Test *test)
{
  {
    GeglOp *fade = g_object_new(GEGL_TYPE_FADE,
                                "input", 0, source,
                                "multiplier", MULTIPLIER,
                                NULL);

    gegl_op_apply(fade); 

    ct_test(test, testutils_check_pixel_rgb_float(GEGL_IMAGE(fade), 
                                            .1 * MULTIPLIER, 
                                            .2 * MULTIPLIER, 
                                            .3 * MULTIPLIER));  
    g_object_unref(fade);
  }

  {
    GeglOp *fade1 = g_object_new(GEGL_TYPE_FADE,
                                 "multiplier", MULTIPLIER,
                                 "input", 0, source,
                                 NULL);

    GeglOp *fade2 = g_object_new(GEGL_TYPE_FADE,
                                 "multiplier", MULTIPLIER,
                                 "input", 0, fade1,
                                 NULL);

    gegl_op_apply(fade2); 

    ct_test(test, testutils_check_pixel_rgb_float(GEGL_IMAGE(fade2), 
                                            .1 * MULTIPLIER * MULTIPLIER, 
                                            .2 * MULTIPLIER * MULTIPLIER, 
                                            .3 * MULTIPLIER * MULTIPLIER));  

    g_object_unref(fade1);
    g_object_unref(fade2);
  }
}

static void
fade_test_setup(Test *test)
{
  source = g_object_new(GEGL_TYPE_COLOR, 
                        "width", IMAGE_WIDTH, 
                        "height", IMAGE_HEIGHT, 
                        "pixel-rgb-float",.1, .2, .3, 
                        NULL); 

}

static void
fade_test_teardown(Test *test)
{
  g_object_unref(source);
}

Test *
create_fade_test_rgb_float()
{
  Test* t = ct_create("GeglFadeTestRgbFloat");

  g_assert(ct_addSetUp(t, fade_test_setup));
  g_assert(ct_addTearDown(t, fade_test_teardown));

#if 1 
  g_assert(ct_addTestFun(t, test_fade_g_object_new));
  g_assert(ct_addTestFun(t, test_fade_g_object_properties));
  g_assert(ct_addTestFun(t, test_fade_apply));
#endif

  return t; 
}

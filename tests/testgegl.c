#include <stdio.h>
#include <stdlib.h>
#include <glib-object.h>
#include "gegl.h"
#include "ctest.h"
#include "csuite.h"

extern Test * create_bfs_property_visitor_test();
extern Test * create_graph_test();
extern Test * create_dfs_property_visitor_test();
extern Test * create_dfs_node_visitor_test();
extern Test * create_mock_image_filter_test();
extern Test * create_mock_filter_0_1_test();
extern Test * create_mock_filter_1_1_test();
extern Test * create_mock_filter_1_2_test();
extern Test * create_mock_filter_2_1_test();
extern Test * create_mock_filter_2_2_test();
extern Test * create_node_connections_test();
extern Test * create_property_connections_test();
extern Test * create_update_property_test();

int
main (int argc, char *argv[])
{  

  g_log_set_always_fatal (g_log_set_always_fatal (G_LOG_FATAL_MASK) |
                                                 G_LOG_LEVEL_WARNING | 
                                                 G_LOG_LEVEL_CRITICAL);
  g_type_init_with_debug_flags (G_TYPE_DEBUG_OBJECTS);

  gegl_init(&argc, &argv);

  gegl_log_direct("Hello there");

  {
    Suite *suite = cs_create("GeglTestSuite");

    cs_addTest(suite, create_bfs_property_visitor_test());
    cs_addTest(suite, create_graph_test());
    cs_addTest(suite, create_dfs_property_visitor_test());
    cs_addTest(suite, create_dfs_node_visitor_test());
    cs_addTest(suite, create_mock_image_filter_test());
    cs_addTest(suite, create_mock_filter_0_1_test());
    cs_addTest(suite, create_mock_filter_1_1_test());
    cs_addTest(suite, create_mock_filter_1_2_test());
    cs_addTest(suite, create_mock_filter_2_1_test());
    cs_addTest(suite, create_mock_filter_2_2_test());
    cs_addTest(suite, create_node_connections_test());
    cs_addTest(suite, create_property_connections_test());
    cs_addTest(suite, create_update_property_test());

    cs_setStream(suite, stdout);
    cs_run(suite);
    cs_report(suite);
    cs_destroy(suite, TRUE);
  }

  return 0;
}

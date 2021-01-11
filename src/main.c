#include "net/net.h"
// net must come before stdio.h for windows compactability.
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "configuration_parse.h"
#include "option_execution.h"
#include "structs/dictionary.h"

int test_dict() {
  Dictionary *dict = dictionary_init(100);
  Dictionary *nested = dictionary_init(100);
  dictionary_insert(dict, "nested", (void *)nested,
                    (void (*)(void *))dictionary_free);
  dictionary_insert(nested, "hello", (void *)5, NULL);
  printf("%p\n",
         dictionary_get((Dictionary *)dictionary_get(dict, "nested"), "hello"));
  dictionary_free(dict);
}

int test_conf() {
  Dictionary *conf = parse_conf("./configuration.txt");
  dictionary_print(conf);
  dictionary_free(conf);
}
#ifdef TEST
int main(int argc, char **argv) {
  printf("[1] testing dictionaries\n");
  test_dict();
  printf("[1] test done\n");
  printf("[2] testing configuration parser\n");
  test_conf();
  printf("[2] test done\n");
}
#endif

#ifndef TEST

int main(int argc, char **argv) {
  Dictionary *conf = parse_conf("./configuration.txt");
  _debug(printf("Configuration:\n");)
      _debug(dictionary_print(conf);) char *s_port;
  if ((s_port = dictionary_get(conf, "lport")) == NULL) {
    fprintf(stderr, "Invalid configuration setup.\n");
    exit(0);
  }
  NetWorker *worker = init_net_worker(atoi(s_port));
  Options *opts = parse_options(conf, argc, argv);
  act(worker, opts);
  free(opts);
  net_worker_free(worker);
  dictionary_free(conf);
}

#endif

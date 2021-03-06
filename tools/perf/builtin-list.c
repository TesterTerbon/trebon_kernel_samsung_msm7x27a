/*
 * builtin-list.c
 *
 * Builtin list command: list all event types
 *
 * Copyright (C) 2009, Thomas Gleixner <tglx@linutronix.de>
 * Copyright (C) 2008-2009, Red Hat Inc, Ingo Molnar <mingo@redhat.com>
 * Copyright (C) 2011, Red Hat Inc, Arnaldo Carvalho de Melo <acme@redhat.com>
 */
#include "builtin.h"

#include "perf.h"

#include "util/parse-events.h"
#include "util/cache.h"
<<<<<<< HEAD

int cmd_list(int argc, const char **argv, const char *prefix __used)
{
	setup_pager();

	if (argc == 1)
		print_events(NULL);
	else {
		int i;

		for (i = 1; i < argc; ++i) {
			if (i > 1)
				putchar('\n');
			if (strncmp(argv[i], "tracepoint", 10) == 0)
				print_tracepoint_events(NULL, NULL);
			else if (strcmp(argv[i], "hw") == 0 ||
				 strcmp(argv[i], "hardware") == 0)
				print_events_type(PERF_TYPE_HARDWARE);
			else if (strcmp(argv[i], "sw") == 0 ||
				 strcmp(argv[i], "software") == 0)
				print_events_type(PERF_TYPE_SOFTWARE);
			else if (strcmp(argv[i], "cache") == 0 ||
				 strcmp(argv[i], "hwcache") == 0)
				print_hwcache_events(NULL);
			else {
				char *sep = strchr(argv[i], ':'), *s;
				int sep_idx;

				if (sep == NULL) {
					print_events(argv[i]);
					continue;
				}
				sep_idx = sep - argv[i];
				s = strdup(argv[i]);
				if (s == NULL)
					return -1;

				s[sep_idx] = '\0';
				print_tracepoint_events(s, s + sep_idx + 1);
				free(s);
			}
=======
#include "util/pmu.h"
#include "util/parse-options.h"

int cmd_list(int argc, const char **argv, const char *prefix __maybe_unused)
{
	int i;
	const struct option list_options[] = {
		OPT_END()
	};
	const char * const list_usage[] = {
		"perf list [hw|sw|cache|tracepoint|pmu|event_glob]",
		NULL
	};

	argc = parse_options(argc, argv, list_options, list_usage,
			     PARSE_OPT_STOP_AT_NON_OPTION);

	setup_pager();

	if (argc == 0) {
		print_events(NULL, false);
		return 0;
	}

	for (i = 0; i < argc; ++i) {
		if (i)
			putchar('\n');
		if (strncmp(argv[i], "tracepoint", 10) == 0)
			print_tracepoint_events(NULL, NULL, false);
		else if (strcmp(argv[i], "hw") == 0 ||
			 strcmp(argv[i], "hardware") == 0)
			print_events_type(PERF_TYPE_HARDWARE);
		else if (strcmp(argv[i], "sw") == 0 ||
			 strcmp(argv[i], "software") == 0)
			print_events_type(PERF_TYPE_SOFTWARE);
		else if (strcmp(argv[i], "cache") == 0 ||
			 strcmp(argv[i], "hwcache") == 0)
			print_hwcache_events(NULL, false);
		else if (strcmp(argv[i], "pmu") == 0)
			print_pmu_events(NULL, false);
		else if (strcmp(argv[i], "--raw-dump") == 0)
			print_events(NULL, true);
		else {
			char *sep = strchr(argv[i], ':'), *s;
			int sep_idx;

			if (sep == NULL) {
				print_events(argv[i], false);
				continue;
			}
			sep_idx = sep - argv[i];
			s = strdup(argv[i]);
			if (s == NULL)
				return -1;

			s[sep_idx] = '\0';
			print_tracepoint_events(s, s + sep_idx + 1, false);
			free(s);
>>>>>>> refs/remotes/origin/master
		}
	}
	return 0;
}

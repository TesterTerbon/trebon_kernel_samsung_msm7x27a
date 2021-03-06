/*
 * builtin-diff.c
 *
 * Builtin diff command: Analyze two perf.data input files, look up and read
 * DSOs and symbol information, sort them and produce a diff.
 */
#include "builtin.h"

#include "util/debug.h"
#include "util/event.h"
#include "util/hist.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "util/session.h"
=======
#include "util/evsel.h"
#include "util/session.h"
#include "util/tool.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include "util/sort.h"
#include "util/symbol.h"
#include "util/util.h"

#include <stdlib.h>

static char const *input_old = "perf.data.old",
		  *input_new = "perf.data";
static char	  diff__default_sort_order[] = "dso,symbol";
static bool  force;
static bool show_displacement;

<<<<<<< HEAD
=======
struct perf_diff {
	struct perf_tool tool;
	struct perf_session *session;
};

>>>>>>> refs/remotes/origin/cm-10.0
static int hists__add_entry(struct hists *self,
			    struct addr_location *al, u64 period)
{
	if (__hists__add_entry(self, al, NULL, period) != NULL)
		return 0;
	return -ENOMEM;
}

<<<<<<< HEAD
static int diff__process_sample_event(union perf_event *event,
				      struct perf_sample *sample,
				      struct perf_evsel *evsel __used,
				      struct perf_session *session)
{
	struct addr_location al;

	if (perf_event__preprocess_sample(event, session, &al, sample, NULL) < 0) {
=======
static int diff__process_sample_event(struct perf_tool *tool,
				      union perf_event *event,
				      struct perf_sample *sample,
				      struct perf_evsel *evsel __used,
				      struct machine *machine)
{
	struct perf_diff *_diff = container_of(tool, struct perf_diff, tool);
	struct perf_session *session = _diff->session;
	struct addr_location al;

	if (perf_event__preprocess_sample(event, machine, &al, sample, NULL) < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "util/evsel.h"
#include "util/evlist.h"
#include "util/session.h"
#include "util/tool.h"
#include "util/sort.h"
#include "util/symbol.h"
#include "util/util.h"
#include "util/data.h"

#include <stdlib.h>
#include <math.h>

/* Diff command specific HPP columns. */
enum {
	PERF_HPP_DIFF__BASELINE,
	PERF_HPP_DIFF__PERIOD,
	PERF_HPP_DIFF__PERIOD_BASELINE,
	PERF_HPP_DIFF__DELTA,
	PERF_HPP_DIFF__RATIO,
	PERF_HPP_DIFF__WEIGHTED_DIFF,
	PERF_HPP_DIFF__FORMULA,

	PERF_HPP_DIFF__MAX_INDEX
};

struct diff_hpp_fmt {
	struct perf_hpp_fmt	 fmt;
	int			 idx;
	char			*header;
	int			 header_width;
};

struct data__file {
	struct perf_session	*session;
	struct perf_data_file	file;
	int			 idx;
	struct hists		*hists;
	struct diff_hpp_fmt	 fmt[PERF_HPP_DIFF__MAX_INDEX];
};

static struct data__file *data__files;
static int data__files_cnt;

#define data__for_each_file_start(i, d, s)	\
	for (i = s, d = &data__files[s];	\
	     i < data__files_cnt;		\
	     i++, d = &data__files[i])

#define data__for_each_file(i, d) data__for_each_file_start(i, d, 0)
#define data__for_each_file_new(i, d) data__for_each_file_start(i, d, 1)

static char diff__default_sort_order[] = "dso,symbol";
static bool force;
static bool show_period;
static bool show_formula;
static bool show_baseline_only;
static unsigned int sort_compute;

static s64 compute_wdiff_w1;
static s64 compute_wdiff_w2;

enum {
	COMPUTE_DELTA,
	COMPUTE_RATIO,
	COMPUTE_WEIGHTED_DIFF,
	COMPUTE_MAX,
};

const char *compute_names[COMPUTE_MAX] = {
	[COMPUTE_DELTA] = "delta",
	[COMPUTE_RATIO] = "ratio",
	[COMPUTE_WEIGHTED_DIFF] = "wdiff",
};

static int compute;

static int compute_2_hpp[COMPUTE_MAX] = {
	[COMPUTE_DELTA]		= PERF_HPP_DIFF__DELTA,
	[COMPUTE_RATIO]		= PERF_HPP_DIFF__RATIO,
	[COMPUTE_WEIGHTED_DIFF]	= PERF_HPP_DIFF__WEIGHTED_DIFF,
};

#define MAX_COL_WIDTH 70

static struct header_column {
	const char *name;
	int width;
} columns[PERF_HPP_DIFF__MAX_INDEX] = {
	[PERF_HPP_DIFF__BASELINE] = {
		.name  = "Baseline",
	},
	[PERF_HPP_DIFF__PERIOD] = {
		.name  = "Period",
		.width = 14,
	},
	[PERF_HPP_DIFF__PERIOD_BASELINE] = {
		.name  = "Base period",
		.width = 14,
	},
	[PERF_HPP_DIFF__DELTA] = {
		.name  = "Delta",
		.width = 7,
	},
	[PERF_HPP_DIFF__RATIO] = {
		.name  = "Ratio",
		.width = 14,
	},
	[PERF_HPP_DIFF__WEIGHTED_DIFF] = {
		.name  = "Weighted diff",
		.width = 14,
	},
	[PERF_HPP_DIFF__FORMULA] = {
		.name  = "Formula",
		.width = MAX_COL_WIDTH,
	}
};

static int setup_compute_opt_wdiff(char *opt)
{
	char *w1_str = opt;
	char *w2_str;

	int ret = -EINVAL;

	if (!opt)
		goto out;

	w2_str = strchr(opt, ',');
	if (!w2_str)
		goto out;

	*w2_str++ = 0x0;
	if (!*w2_str)
		goto out;

	compute_wdiff_w1 = strtol(w1_str, NULL, 10);
	compute_wdiff_w2 = strtol(w2_str, NULL, 10);

	if (!compute_wdiff_w1 || !compute_wdiff_w2)
		goto out;

	pr_debug("compute wdiff w1(%" PRId64 ") w2(%" PRId64 ")\n",
		  compute_wdiff_w1, compute_wdiff_w2);

	ret = 0;

 out:
	if (ret)
		pr_err("Failed: wrong weight data, use 'wdiff:w1,w2'\n");

	return ret;
}

static int setup_compute_opt(char *opt)
{
	if (compute == COMPUTE_WEIGHTED_DIFF)
		return setup_compute_opt_wdiff(opt);

	if (opt) {
		pr_err("Failed: extra option specified '%s'", opt);
		return -EINVAL;
	}

	return 0;
}

static int setup_compute(const struct option *opt, const char *str,
			 int unset __maybe_unused)
{
	int *cp = (int *) opt->value;
	char *cstr = (char *) str;
	char buf[50];
	unsigned i;
	char *option;

	if (!str) {
		*cp = COMPUTE_DELTA;
		return 0;
	}

	option = strchr(str, ':');
	if (option) {
		unsigned len = option++ - str;

		/*
		 * The str data are not writeable, so we need
		 * to use another buffer.
		 */

		/* No option value is longer. */
		if (len >= sizeof(buf))
			return -EINVAL;

		strncpy(buf, str, len);
		buf[len] = 0x0;
		cstr = buf;
	}

	for (i = 0; i < COMPUTE_MAX; i++)
		if (!strcmp(cstr, compute_names[i])) {
			*cp = i;
			return setup_compute_opt(option);
		}

	pr_err("Failed: '%s' is not computation method "
	       "(use 'delta','ratio' or 'wdiff')\n", str);
	return -EINVAL;
}

static double period_percent(struct hist_entry *he, u64 period)
{
	u64 total = he->hists->stats.total_period;
	return (period * 100.0) / total;
}

static double compute_delta(struct hist_entry *he, struct hist_entry *pair)
{
	double old_percent = period_percent(he, he->stat.period);
	double new_percent = period_percent(pair, pair->stat.period);

	pair->diff.period_ratio_delta = new_percent - old_percent;
	pair->diff.computed = true;
	return pair->diff.period_ratio_delta;
}

static double compute_ratio(struct hist_entry *he, struct hist_entry *pair)
{
	double old_period = he->stat.period ?: 1;
	double new_period = pair->stat.period;

	pair->diff.computed = true;
	pair->diff.period_ratio = new_period / old_period;
	return pair->diff.period_ratio;
}

static s64 compute_wdiff(struct hist_entry *he, struct hist_entry *pair)
{
	u64 old_period = he->stat.period;
	u64 new_period = pair->stat.period;

	pair->diff.computed = true;
	pair->diff.wdiff = new_period * compute_wdiff_w2 -
			   old_period * compute_wdiff_w1;

	return pair->diff.wdiff;
}

static int formula_delta(struct hist_entry *he, struct hist_entry *pair,
			 char *buf, size_t size)
{
	return scnprintf(buf, size,
			 "(%" PRIu64 " * 100 / %" PRIu64 ") - "
			 "(%" PRIu64 " * 100 / %" PRIu64 ")",
			  pair->stat.period, pair->hists->stats.total_period,
			  he->stat.period, he->hists->stats.total_period);
}

static int formula_ratio(struct hist_entry *he, struct hist_entry *pair,
			 char *buf, size_t size)
{
	double old_period = he->stat.period;
	double new_period = pair->stat.period;

	return scnprintf(buf, size, "%.0F / %.0F", new_period, old_period);
}

static int formula_wdiff(struct hist_entry *he, struct hist_entry *pair,
			 char *buf, size_t size)
{
	u64 old_period = he->stat.period;
	u64 new_period = pair->stat.period;

	return scnprintf(buf, size,
		  "(%" PRIu64 " * " "%" PRId64 ") - (%" PRIu64 " * " "%" PRId64 ")",
		  new_period, compute_wdiff_w2, old_period, compute_wdiff_w1);
}

static int formula_fprintf(struct hist_entry *he, struct hist_entry *pair,
			   char *buf, size_t size)
{
	switch (compute) {
	case COMPUTE_DELTA:
		return formula_delta(he, pair, buf, size);
	case COMPUTE_RATIO:
		return formula_ratio(he, pair, buf, size);
	case COMPUTE_WEIGHTED_DIFF:
		return formula_wdiff(he, pair, buf, size);
	default:
		BUG_ON(1);
	}

	return -1;
}

static int hists__add_entry(struct hists *hists,
			    struct addr_location *al, u64 period,
			    u64 weight, u64 transaction)
{
	if (__hists__add_entry(hists, al, NULL, NULL, NULL, period, weight,
			       transaction) != NULL)
		return 0;
	return -ENOMEM;
}

static int diff__process_sample_event(struct perf_tool *tool __maybe_unused,
				      union perf_event *event,
				      struct perf_sample *sample,
				      struct perf_evsel *evsel,
				      struct machine *machine)
{
	struct addr_location al;

	if (perf_event__preprocess_sample(event, machine, &al, sample) < 0) {
>>>>>>> refs/remotes/origin/master
		pr_warning("problem processing %d event, skipping it.\n",
			   event->header.type);
		return -1;
	}

<<<<<<< HEAD
	if (al.filtered || al.sym == NULL)
		return 0;

	if (hists__add_entry(&session->hists, &al, sample->period)) {
=======
	if (al.filtered)
		return 0;

	if (hists__add_entry(&evsel->hists, &al, sample->period,
			     sample->weight, sample->transaction)) {
>>>>>>> refs/remotes/origin/master
		pr_warning("problem incrementing symbol period, skipping event\n");
		return -1;
	}

<<<<<<< HEAD
	session->hists.stats.total_period += sample->period;
	return 0;
}

<<<<<<< HEAD
static struct perf_event_ops event_ops = {
	.sample	= diff__process_sample_event,
	.mmap	= perf_event__process_mmap,
	.comm	= perf_event__process_comm,
	.exit	= perf_event__process_task,
	.fork	= perf_event__process_task,
	.lost	= perf_event__process_lost,
	.ordered_samples = true,
	.ordering_requires_timestamps = true,
=======
static struct perf_diff diff = {
	.tool = {
		.sample	= diff__process_sample_event,
		.mmap	= perf_event__process_mmap,
		.comm	= perf_event__process_comm,
		.exit	= perf_event__process_task,
		.fork	= perf_event__process_task,
		.lost	= perf_event__process_lost,
		.ordered_samples = true,
		.ordering_requires_timestamps = true,
	},
>>>>>>> refs/remotes/origin/cm-10.0
};

static void perf_session__insert_hist_entry_by_name(struct rb_root *root,
						    struct hist_entry *he)
=======
	evsel->hists.stats.total_period += sample->period;
	return 0;
}

static struct perf_tool tool = {
	.sample	= diff__process_sample_event,
	.mmap	= perf_event__process_mmap,
	.comm	= perf_event__process_comm,
	.exit	= perf_event__process_exit,
	.fork	= perf_event__process_fork,
	.lost	= perf_event__process_lost,
	.ordered_samples = true,
	.ordering_requires_timestamps = true,
};

static struct perf_evsel *evsel_match(struct perf_evsel *evsel,
				      struct perf_evlist *evlist)
{
	struct perf_evsel *e;

	evlist__for_each(evlist, e) {
		if (perf_evsel__match2(evsel, e))
			return e;
	}

	return NULL;
}

static void perf_evlist__collapse_resort(struct perf_evlist *evlist)
{
	struct perf_evsel *evsel;

	evlist__for_each(evlist, evsel) {
		struct hists *hists = &evsel->hists;

		hists__collapse_resort(hists, NULL);
	}
}

static struct hist_entry*
get_pair_data(struct hist_entry *he, struct data__file *d)
{
	if (hist_entry__has_pairs(he)) {
		struct hist_entry *pair;

		list_for_each_entry(pair, &he->pairs.head, pairs.node)
			if (pair->hists == d->hists)
				return pair;
	}

	return NULL;
}

static struct hist_entry*
get_pair_fmt(struct hist_entry *he, struct diff_hpp_fmt *dfmt)
{
	void *ptr = dfmt - dfmt->idx;
	struct data__file *d = container_of(ptr, struct data__file, fmt);

	return get_pair_data(he, d);
}

static void hists__baseline_only(struct hists *hists)
{
	struct rb_root *root;
	struct rb_node *next;

	if (sort__need_collapse)
		root = &hists->entries_collapsed;
	else
		root = hists->entries_in;

	next = rb_first(root);
	while (next != NULL) {
		struct hist_entry *he = rb_entry(next, struct hist_entry, rb_node_in);

		next = rb_next(&he->rb_node_in);
		if (!hist_entry__next_pair(he)) {
			rb_erase(&he->rb_node_in, root);
			hist_entry__free(he);
		}
	}
}

static void hists__precompute(struct hists *hists)
{
	struct rb_root *root;
	struct rb_node *next;

	if (sort__need_collapse)
		root = &hists->entries_collapsed;
	else
		root = hists->entries_in;

	next = rb_first(root);
	while (next != NULL) {
		struct hist_entry *he, *pair;

		he   = rb_entry(next, struct hist_entry, rb_node_in);
		next = rb_next(&he->rb_node_in);

		pair = get_pair_data(he, &data__files[sort_compute]);
		if (!pair)
			continue;

		switch (compute) {
		case COMPUTE_DELTA:
			compute_delta(he, pair);
			break;
		case COMPUTE_RATIO:
			compute_ratio(he, pair);
			break;
		case COMPUTE_WEIGHTED_DIFF:
			compute_wdiff(he, pair);
			break;
		default:
			BUG_ON(1);
		}
	}
}

static int64_t cmp_doubles(double l, double r)
{
	if (l > r)
		return -1;
	else if (l < r)
		return 1;
	else
		return 0;
}

static int64_t
__hist_entry__cmp_compute(struct hist_entry *left, struct hist_entry *right,
			int c)
{
	switch (c) {
	case COMPUTE_DELTA:
	{
		double l = left->diff.period_ratio_delta;
		double r = right->diff.period_ratio_delta;

		return cmp_doubles(l, r);
	}
	case COMPUTE_RATIO:
	{
		double l = left->diff.period_ratio;
		double r = right->diff.period_ratio;

		return cmp_doubles(l, r);
	}
	case COMPUTE_WEIGHTED_DIFF:
	{
		s64 l = left->diff.wdiff;
		s64 r = right->diff.wdiff;

		return r - l;
	}
	default:
		BUG_ON(1);
	}

	return 0;
}

static int64_t
hist_entry__cmp_compute(struct hist_entry *left, struct hist_entry *right,
			int c)
{
	bool pairs_left  = hist_entry__has_pairs(left);
	bool pairs_right = hist_entry__has_pairs(right);
	struct hist_entry *p_right, *p_left;

	if (!pairs_left && !pairs_right)
		return 0;

	if (!pairs_left || !pairs_right)
		return pairs_left ? -1 : 1;

	p_left  = get_pair_data(left,  &data__files[sort_compute]);
	p_right = get_pair_data(right, &data__files[sort_compute]);

	if (!p_left && !p_right)
		return 0;

	if (!p_left || !p_right)
		return p_left ? -1 : 1;

	/*
	 * We have 2 entries of same kind, let's
	 * make the data comparison.
	 */
	return __hist_entry__cmp_compute(p_left, p_right, c);
}

static void insert_hist_entry_by_compute(struct rb_root *root,
					 struct hist_entry *he,
					 int c)
>>>>>>> refs/remotes/origin/master
{
	struct rb_node **p = &root->rb_node;
	struct rb_node *parent = NULL;
	struct hist_entry *iter;

	while (*p != NULL) {
		parent = *p;
		iter = rb_entry(parent, struct hist_entry, rb_node);
<<<<<<< HEAD
		if (hist_entry__cmp(he, iter) < 0)
=======
		if (hist_entry__cmp_compute(he, iter, c) < 0)
>>>>>>> refs/remotes/origin/master
			p = &(*p)->rb_left;
		else
			p = &(*p)->rb_right;
	}

	rb_link_node(&he->rb_node, parent, p);
	rb_insert_color(&he->rb_node, root);
}

<<<<<<< HEAD
static void hists__resort_entries(struct hists *self)
{
	unsigned long position = 1;
	struct rb_root tmp = RB_ROOT;
	struct rb_node *next = rb_first(&self->entries);

	while (next != NULL) {
		struct hist_entry *n = rb_entry(next, struct hist_entry, rb_node);

		next = rb_next(&n->rb_node);
		rb_erase(&n->rb_node, &self->entries);
		n->position = position++;
		perf_session__insert_hist_entry_by_name(&tmp, n);
	}

	self->entries = tmp;
}

<<<<<<< HEAD
static void hists__set_positions(struct hists *self)
{
	hists__output_resort(self);
	hists__resort_entries(self);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct hist_entry *hists__find_entry(struct hists *self,
					    struct hist_entry *he)
{
	struct rb_node *n = self->entries.rb_node;

	while (n) {
		struct hist_entry *iter = rb_entry(n, struct hist_entry, rb_node);
		int64_t cmp = hist_entry__cmp(he, iter);

		if (cmp < 0)
			n = n->rb_left;
		else if (cmp > 0)
			n = n->rb_right;
		else 
			return iter;
	}

	return NULL;
}

static void hists__match(struct hists *older, struct hists *newer)
{
	struct rb_node *nd;

	for (nd = rb_first(&newer->entries); nd; nd = rb_next(nd)) {
		struct hist_entry *pos = rb_entry(nd, struct hist_entry, rb_node);
		pos->pair = hists__find_entry(older, pos);
=======
static void hists__compute_resort(struct hists *hists)
{
	struct rb_root *root;
	struct rb_node *next;

	if (sort__need_collapse)
		root = &hists->entries_collapsed;
	else
		root = hists->entries_in;

	hists->entries = RB_ROOT;
	next = rb_first(root);

	hists->nr_entries = 0;
	hists->stats.total_period = 0;
	hists__reset_col_len(hists);

	while (next != NULL) {
		struct hist_entry *he;

		he = rb_entry(next, struct hist_entry, rb_node_in);
		next = rb_next(&he->rb_node_in);

		insert_hist_entry_by_compute(&hists->entries, he, compute);
		hists__inc_nr_entries(hists, he);
	}
}

static void hists__process(struct hists *hists)
{
	if (show_baseline_only)
		hists__baseline_only(hists);

	if (sort_compute) {
		hists__precompute(hists);
		hists__compute_resort(hists);
	} else {
		hists__output_resort(hists);
	}

	hists__fprintf(hists, true, 0, 0, 0, stdout);
}

static void data__fprintf(void)
{
	struct data__file *d;
	int i;

	fprintf(stdout, "# Data files:\n");

	data__for_each_file(i, d)
		fprintf(stdout, "#  [%d] %s %s\n",
			d->idx, d->file.path,
			!d->idx ? "(Baseline)" : "");

	fprintf(stdout, "#\n");
}

static void data_process(void)
{
	struct perf_evlist *evlist_base = data__files[0].session->evlist;
	struct perf_evsel *evsel_base;
	bool first = true;

	evlist__for_each(evlist_base, evsel_base) {
		struct data__file *d;
		int i;

		data__for_each_file_new(i, d) {
			struct perf_evlist *evlist = d->session->evlist;
			struct perf_evsel *evsel;

			evsel = evsel_match(evsel_base, evlist);
			if (!evsel)
				continue;

			d->hists = &evsel->hists;

			hists__match(&evsel_base->hists, &evsel->hists);

			if (!show_baseline_only)
				hists__link(&evsel_base->hists,
					    &evsel->hists);
		}

		fprintf(stdout, "%s# Event '%s'\n#\n", first ? "" : "\n",
			perf_evsel__name(evsel_base));

		first = false;

		if (verbose || data__files_cnt > 2)
			data__fprintf();

		hists__process(&evsel_base->hists);
	}
}

static void data__free(struct data__file *d)
{
	int col;

	for (col = 0; col < PERF_HPP_DIFF__MAX_INDEX; col++) {
		struct diff_hpp_fmt *fmt = &d->fmt[col];

		zfree(&fmt->header);
>>>>>>> refs/remotes/origin/master
	}
}

static int __cmd_diff(void)
{
<<<<<<< HEAD
	int ret, i;
<<<<<<< HEAD
	struct perf_session *session[2];

	session[0] = perf_session__new(input_old, O_RDONLY, force, false, &event_ops);
	session[1] = perf_session__new(input_new, O_RDONLY, force, false, &event_ops);
=======
#define older (session[0])
#define newer (session[1])
	struct perf_session *session[2];

	older = perf_session__new(input_old, O_RDONLY, force, false,
				  &diff.tool);
	newer = perf_session__new(input_new, O_RDONLY, force, false,
				  &diff.tool);
>>>>>>> refs/remotes/origin/cm-10.0
	if (session[0] == NULL || session[1] == NULL)
		return -ENOMEM;

	for (i = 0; i < 2; ++i) {
<<<<<<< HEAD
		ret = perf_session__process_events(session[i], &event_ops);
		if (ret)
			goto out_delete;
	}

	hists__output_resort(&session[1]->hists);
	if (show_displacement)
		hists__set_positions(&session[0]->hists);

	hists__match(&session[0]->hists, &session[1]->hists);
	hists__fprintf(&session[1]->hists, &session[0]->hists,
		       show_displacement, stdout);
=======
		diff.session = session[i];
		ret = perf_session__process_events(session[i], &diff.tool);
		if (ret)
			goto out_delete;
		hists__output_resort(&session[i]->hists);
	}

	if (show_displacement)
		hists__resort_entries(&older->hists);

	hists__match(&older->hists, &newer->hists);
	hists__fprintf(&newer->hists, &older->hists,
		       show_displacement, true, 0, 0, stdout);
>>>>>>> refs/remotes/origin/cm-10.0
out_delete:
	for (i = 0; i < 2; ++i)
		perf_session__delete(session[i]);
	return ret;
<<<<<<< HEAD
=======
#undef older
#undef newer
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct data__file *d;
	int ret = -EINVAL, i;

	data__for_each_file(i, d) {
		d->session = perf_session__new(&d->file, false, &tool);
		if (!d->session) {
			pr_err("Failed to open %s\n", d->file.path);
			ret = -ENOMEM;
			goto out_delete;
		}

		ret = perf_session__process_events(d->session, &tool);
		if (ret) {
			pr_err("Failed to process %s\n", d->file.path);
			goto out_delete;
		}

		perf_evlist__collapse_resort(d->session->evlist);
	}

	data_process();

 out_delete:
	data__for_each_file(i, d) {
		if (d->session)
			perf_session__delete(d->session);

		data__free(d);
	}

	free(data__files);
	return ret;
>>>>>>> refs/remotes/origin/master
}

static const char * const diff_usage[] = {
	"perf diff [<options>] [old_file] [new_file]",
	NULL,
};

static const struct option options[] = {
	OPT_INCR('v', "verbose", &verbose,
		    "be more verbose (show symbol address, etc)"),
<<<<<<< HEAD
	OPT_BOOLEAN('M', "displacement", &show_displacement,
		    "Show position displacement relative to baseline"),
=======
	OPT_BOOLEAN('b', "baseline-only", &show_baseline_only,
		    "Show only items with match in baseline"),
	OPT_CALLBACK('c', "compute", &compute,
		     "delta,ratio,wdiff:w1,w2 (default delta)",
		     "Entries differential computation selection",
		     setup_compute),
	OPT_BOOLEAN('p', "period", &show_period,
		    "Show period values."),
	OPT_BOOLEAN('F', "formula", &show_formula,
		    "Show formula."),
>>>>>>> refs/remotes/origin/master
	OPT_BOOLEAN('D', "dump-raw-trace", &dump_trace,
		    "dump raw trace in ASCII"),
	OPT_BOOLEAN('f', "force", &force, "don't complain, do it"),
	OPT_BOOLEAN('m', "modules", &symbol_conf.use_modules,
		    "load module symbols - WARNING: use only with -k and LIVE kernel"),
	OPT_STRING('d', "dsos", &symbol_conf.dso_list_str, "dso[,dso...]",
		   "only consider symbols in these dsos"),
	OPT_STRING('C', "comms", &symbol_conf.comm_list_str, "comm[,comm...]",
		   "only consider symbols in these comms"),
	OPT_STRING('S', "symbols", &symbol_conf.sym_list_str, "symbol[,symbol...]",
		   "only consider these symbols"),
	OPT_STRING('s', "sort", &sort_order, "key[,key2...]",
		   "sort by key(s): pid, comm, dso, symbol, parent"),
	OPT_STRING('t', "field-separator", &symbol_conf.field_sep, "separator",
		   "separator for columns, no spaces will be added between "
		   "columns '.' is reserved."),
	OPT_STRING(0, "symfs", &symbol_conf.symfs, "directory",
		    "Look for files with symbols relative to this directory"),
<<<<<<< HEAD
	OPT_END()
};

int cmd_diff(int argc, const char **argv, const char *prefix __used)
{
	sort_order = diff__default_sort_order;
	argc = parse_options(argc, argv, options, diff_usage, 0);
	if (argc) {
		if (argc > 2)
			usage_with_options(diff_usage, options);
		if (argc == 2) {
			input_old = argv[0];
			input_new = argv[1];
		} else
			input_new = argv[0];
	} else if (symbol_conf.default_guest_vmlinux_name ||
		   symbol_conf.default_guest_kallsyms) {
		input_old = "perf.data.host";
		input_new = "perf.data.guest";
	}

	symbol_conf.exclude_other = false;
	if (symbol__init() < 0)
		return -1;

	setup_sorting(diff_usage, options);
	setup_pager();

	sort_entry__setup_elide(&sort_dso, symbol_conf.dso_list, "dso", NULL);
	sort_entry__setup_elide(&sort_comm, symbol_conf.comm_list, "comm", NULL);
	sort_entry__setup_elide(&sort_sym, symbol_conf.sym_list, "symbol", NULL);
=======
	OPT_UINTEGER('o', "order", &sort_compute, "Specify compute sorting."),
	OPT_END()
};

static double baseline_percent(struct hist_entry *he)
{
	struct hists *hists = he->hists;
	return 100.0 * he->stat.period / hists->stats.total_period;
}

static int hpp__color_baseline(struct perf_hpp_fmt *fmt,
			       struct perf_hpp *hpp, struct hist_entry *he)
{
	struct diff_hpp_fmt *dfmt =
		container_of(fmt, struct diff_hpp_fmt, fmt);
	double percent = baseline_percent(he);
	char pfmt[20] = " ";

	if (!he->dummy) {
		scnprintf(pfmt, 20, "%%%d.2f%%%%", dfmt->header_width - 1);
		return percent_color_snprintf(hpp->buf, hpp->size,
					      pfmt, percent);
	} else
		return scnprintf(hpp->buf, hpp->size, "%*s",
				 dfmt->header_width, pfmt);
}

static int hpp__entry_baseline(struct hist_entry *he, char *buf, size_t size)
{
	double percent = baseline_percent(he);
	const char *fmt = symbol_conf.field_sep ? "%.2f" : "%6.2f%%";
	int ret = 0;

	if (!he->dummy)
		ret = scnprintf(buf, size, fmt, percent);

	return ret;
}

static int __hpp__color_compare(struct perf_hpp_fmt *fmt,
				struct perf_hpp *hpp, struct hist_entry *he,
				int comparison_method)
{
	struct diff_hpp_fmt *dfmt =
		container_of(fmt, struct diff_hpp_fmt, fmt);
	struct hist_entry *pair = get_pair_fmt(he, dfmt);
	double diff;
	s64 wdiff;
	char pfmt[20] = " ";

	if (!pair)
		goto dummy_print;

	switch (comparison_method) {
	case COMPUTE_DELTA:
		if (pair->diff.computed)
			diff = pair->diff.period_ratio_delta;
		else
			diff = compute_delta(he, pair);

		if (fabs(diff) < 0.01)
			goto dummy_print;
		scnprintf(pfmt, 20, "%%%+d.2f%%%%", dfmt->header_width - 1);
		return percent_color_snprintf(hpp->buf, hpp->size,
					pfmt, diff);
	case COMPUTE_RATIO:
		if (he->dummy)
			goto dummy_print;
		if (pair->diff.computed)
			diff = pair->diff.period_ratio;
		else
			diff = compute_ratio(he, pair);

		scnprintf(pfmt, 20, "%%%d.6f", dfmt->header_width);
		return value_color_snprintf(hpp->buf, hpp->size,
					pfmt, diff);
	case COMPUTE_WEIGHTED_DIFF:
		if (he->dummy)
			goto dummy_print;
		if (pair->diff.computed)
			wdiff = pair->diff.wdiff;
		else
			wdiff = compute_wdiff(he, pair);

		scnprintf(pfmt, 20, "%%14ld", dfmt->header_width);
		return color_snprintf(hpp->buf, hpp->size,
				get_percent_color(wdiff),
				pfmt, wdiff);
	default:
		BUG_ON(1);
	}
dummy_print:
	return scnprintf(hpp->buf, hpp->size, "%*s",
			dfmt->header_width, pfmt);
}

static int hpp__color_delta(struct perf_hpp_fmt *fmt,
			struct perf_hpp *hpp, struct hist_entry *he)
{
	return __hpp__color_compare(fmt, hpp, he, COMPUTE_DELTA);
}

static int hpp__color_ratio(struct perf_hpp_fmt *fmt,
			struct perf_hpp *hpp, struct hist_entry *he)
{
	return __hpp__color_compare(fmt, hpp, he, COMPUTE_RATIO);
}

static int hpp__color_wdiff(struct perf_hpp_fmt *fmt,
			struct perf_hpp *hpp, struct hist_entry *he)
{
	return __hpp__color_compare(fmt, hpp, he, COMPUTE_WEIGHTED_DIFF);
}

static void
hpp__entry_unpair(struct hist_entry *he, int idx, char *buf, size_t size)
{
	switch (idx) {
	case PERF_HPP_DIFF__PERIOD_BASELINE:
		scnprintf(buf, size, "%" PRIu64, he->stat.period);
		break;

	default:
		break;
	}
}

static void
hpp__entry_pair(struct hist_entry *he, struct hist_entry *pair,
		int idx, char *buf, size_t size)
{
	double diff;
	double ratio;
	s64 wdiff;

	switch (idx) {
	case PERF_HPP_DIFF__DELTA:
		if (pair->diff.computed)
			diff = pair->diff.period_ratio_delta;
		else
			diff = compute_delta(he, pair);

		if (fabs(diff) >= 0.01)
			scnprintf(buf, size, "%+4.2F%%", diff);
		break;

	case PERF_HPP_DIFF__RATIO:
		/* No point for ratio number if we are dummy.. */
		if (he->dummy)
			break;

		if (pair->diff.computed)
			ratio = pair->diff.period_ratio;
		else
			ratio = compute_ratio(he, pair);

		if (ratio > 0.0)
			scnprintf(buf, size, "%14.6F", ratio);
		break;

	case PERF_HPP_DIFF__WEIGHTED_DIFF:
		/* No point for wdiff number if we are dummy.. */
		if (he->dummy)
			break;

		if (pair->diff.computed)
			wdiff = pair->diff.wdiff;
		else
			wdiff = compute_wdiff(he, pair);

		if (wdiff != 0)
			scnprintf(buf, size, "%14ld", wdiff);
		break;

	case PERF_HPP_DIFF__FORMULA:
		formula_fprintf(he, pair, buf, size);
		break;

	case PERF_HPP_DIFF__PERIOD:
		scnprintf(buf, size, "%" PRIu64, pair->stat.period);
		break;

	default:
		BUG_ON(1);
	};
}

static void
__hpp__entry_global(struct hist_entry *he, struct diff_hpp_fmt *dfmt,
		    char *buf, size_t size)
{
	struct hist_entry *pair = get_pair_fmt(he, dfmt);
	int idx = dfmt->idx;

	/* baseline is special */
	if (idx == PERF_HPP_DIFF__BASELINE)
		hpp__entry_baseline(he, buf, size);
	else {
		if (pair)
			hpp__entry_pair(he, pair, idx, buf, size);
		else
			hpp__entry_unpair(he, idx, buf, size);
	}
}

static int hpp__entry_global(struct perf_hpp_fmt *_fmt, struct perf_hpp *hpp,
			     struct hist_entry *he)
{
	struct diff_hpp_fmt *dfmt =
		container_of(_fmt, struct diff_hpp_fmt, fmt);
	char buf[MAX_COL_WIDTH] = " ";

	__hpp__entry_global(he, dfmt, buf, MAX_COL_WIDTH);

	if (symbol_conf.field_sep)
		return scnprintf(hpp->buf, hpp->size, "%s", buf);
	else
		return scnprintf(hpp->buf, hpp->size, "%*s",
				 dfmt->header_width, buf);
}

static int hpp__header(struct perf_hpp_fmt *fmt,
		       struct perf_hpp *hpp)
{
	struct diff_hpp_fmt *dfmt =
		container_of(fmt, struct diff_hpp_fmt, fmt);

	BUG_ON(!dfmt->header);
	return scnprintf(hpp->buf, hpp->size, dfmt->header);
}

static int hpp__width(struct perf_hpp_fmt *fmt,
		      struct perf_hpp *hpp __maybe_unused)
{
	struct diff_hpp_fmt *dfmt =
		container_of(fmt, struct diff_hpp_fmt, fmt);

	BUG_ON(dfmt->header_width <= 0);
	return dfmt->header_width;
}

static void init_header(struct data__file *d, struct diff_hpp_fmt *dfmt)
{
#define MAX_HEADER_NAME 100
	char buf_indent[MAX_HEADER_NAME];
	char buf[MAX_HEADER_NAME];
	const char *header = NULL;
	int width = 0;

	BUG_ON(dfmt->idx >= PERF_HPP_DIFF__MAX_INDEX);
	header = columns[dfmt->idx].name;
	width  = columns[dfmt->idx].width;

	/* Only our defined HPP fmts should appear here. */
	BUG_ON(!header);

	if (data__files_cnt > 2)
		scnprintf(buf, MAX_HEADER_NAME, "%s/%d", header, d->idx);

#define NAME (data__files_cnt > 2 ? buf : header)
	dfmt->header_width = width;
	width = (int) strlen(NAME);
	if (dfmt->header_width < width)
		dfmt->header_width = width;

	scnprintf(buf_indent, MAX_HEADER_NAME, "%*s",
		  dfmt->header_width, NAME);

	dfmt->header = strdup(buf_indent);
#undef MAX_HEADER_NAME
#undef NAME
}

static void data__hpp_register(struct data__file *d, int idx)
{
	struct diff_hpp_fmt *dfmt = &d->fmt[idx];
	struct perf_hpp_fmt *fmt = &dfmt->fmt;

	dfmt->idx = idx;

	fmt->header = hpp__header;
	fmt->width  = hpp__width;
	fmt->entry  = hpp__entry_global;

	/* TODO more colors */
	switch (idx) {
	case PERF_HPP_DIFF__BASELINE:
		fmt->color = hpp__color_baseline;
		break;
	case PERF_HPP_DIFF__DELTA:
		fmt->color = hpp__color_delta;
		break;
	case PERF_HPP_DIFF__RATIO:
		fmt->color = hpp__color_ratio;
		break;
	case PERF_HPP_DIFF__WEIGHTED_DIFF:
		fmt->color = hpp__color_wdiff;
		break;
	default:
		break;
	}

	init_header(d, dfmt);
	perf_hpp__column_register(fmt);
}

static void ui_init(void)
{
	struct data__file *d;
	int i;

	data__for_each_file(i, d) {

		/*
		 * Baseline or compute realted columns:
		 *
		 *   PERF_HPP_DIFF__BASELINE
		 *   PERF_HPP_DIFF__DELTA
		 *   PERF_HPP_DIFF__RATIO
		 *   PERF_HPP_DIFF__WEIGHTED_DIFF
		 */
		data__hpp_register(d, i ? compute_2_hpp[compute] :
					  PERF_HPP_DIFF__BASELINE);

		/*
		 * And the rest:
		 *
		 * PERF_HPP_DIFF__FORMULA
		 * PERF_HPP_DIFF__PERIOD
		 * PERF_HPP_DIFF__PERIOD_BASELINE
		 */
		if (show_formula && i)
			data__hpp_register(d, PERF_HPP_DIFF__FORMULA);

		if (show_period)
			data__hpp_register(d, i ? PERF_HPP_DIFF__PERIOD :
						  PERF_HPP_DIFF__PERIOD_BASELINE);
	}
}

static int data_init(int argc, const char **argv)
{
	struct data__file *d;
	static const char *defaults[] = {
		"perf.data.old",
		"perf.data",
	};
	bool use_default = true;
	int i;

	data__files_cnt = 2;

	if (argc) {
		if (argc == 1)
			defaults[1] = argv[0];
		else {
			data__files_cnt = argc;
			use_default = false;
		}
	} else if (perf_guest) {
		defaults[0] = "perf.data.host";
		defaults[1] = "perf.data.guest";
	}

	if (sort_compute >= (unsigned int) data__files_cnt) {
		pr_err("Order option out of limit.\n");
		return -EINVAL;
	}

	data__files = zalloc(sizeof(*data__files) * data__files_cnt);
	if (!data__files)
		return -ENOMEM;

	data__for_each_file(i, d) {
		struct perf_data_file *file = &d->file;

		file->path  = use_default ? defaults[i] : argv[i];
		file->mode  = PERF_DATA_MODE_READ,
		file->force = force,

		d->idx  = i;
	}

	return 0;
}

int cmd_diff(int argc, const char **argv, const char *prefix __maybe_unused)
{
	sort_order = diff__default_sort_order;
	argc = parse_options(argc, argv, options, diff_usage, 0);

	if (symbol__init() < 0)
		return -1;

	if (data_init(argc, argv) < 0)
		return -1;

	ui_init();

	if (setup_sorting() < 0)
		usage_with_options(diff_usage, options);

	setup_pager();

	sort__setup_elide(NULL);
>>>>>>> refs/remotes/origin/master

	return __cmd_diff();
}

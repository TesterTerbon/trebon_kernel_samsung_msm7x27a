///
<<<<<<< HEAD
/// Use PTR_RET rather than if(IS_ERR(...)) + PTR_ERR
=======
/// Use PTR_ERR_OR_ZERO rather than if(IS_ERR(...)) + PTR_ERR
>>>>>>> refs/remotes/origin/master
///
// Confidence: High
// Copyright: (C) 2012 Julia Lawall, INRIA/LIP6.  GPLv2.
// Copyright: (C) 2012 Gilles Muller, INRIA/LiP6.  GPLv2.
// URL: http://coccinelle.lip6.fr/
<<<<<<< HEAD
// Options: -no_includes -include_headers
//
// Keywords: ERR_PTR, PTR_ERR, PTR_RET
=======
// Options: --no-includes --include-headers
//
// Keywords: ERR_PTR, PTR_ERR, PTR_RET, PTR_ERR_OR_ZERO
>>>>>>> refs/remotes/origin/master
// Version min: 2.6.39
//

virtual context
virtual patch
virtual org
virtual report

@depends on patch@
expression ptr;
@@

- if (IS_ERR(ptr)) return PTR_ERR(ptr); else return 0;
<<<<<<< HEAD
+ return PTR_RET(ptr);
=======
+ return PTR_ERR_OR_ZERO(ptr);
>>>>>>> refs/remotes/origin/master

@depends on patch@
expression ptr;
@@

- if (IS_ERR(ptr)) return PTR_ERR(ptr); return 0;
<<<<<<< HEAD
+ return PTR_RET(ptr);
=======
+ return PTR_ERR_OR_ZERO(ptr);

@depends on patch@
expression ptr;
@@

- (IS_ERR(ptr) ? PTR_ERR(ptr) : 0)
+ PTR_ERR_OR_ZERO(ptr)
>>>>>>> refs/remotes/origin/master

@r1 depends on !patch@
expression ptr;
position p1;
@@

* if@p1 (IS_ERR(ptr)) return PTR_ERR(ptr); else return 0;

@r2 depends on !patch@
expression ptr;
position p2;
@@

* if@p2 (IS_ERR(ptr)) return PTR_ERR(ptr); return 0;

<<<<<<< HEAD
=======
@r3 depends on !patch@
expression ptr;
position p3;
@@

* IS_ERR@p3(ptr) ? PTR_ERR(ptr) : 0

>>>>>>> refs/remotes/origin/master
@script:python depends on org@
p << r1.p1;
@@

coccilib.org.print_todo(p[0], "WARNING: PTR_RET can be used")


@script:python depends on org@
p << r2.p2;
@@

coccilib.org.print_todo(p[0], "WARNING: PTR_RET can be used")

<<<<<<< HEAD
=======
@script:python depends on org@
p << r3.p3;
@@

coccilib.org.print_todo(p[0], "WARNING: PTR_RET can be used")

>>>>>>> refs/remotes/origin/master
@script:python depends on report@
p << r1.p1;
@@

coccilib.report.print_report(p[0], "WARNING: PTR_RET can be used")

@script:python depends on report@
p << r2.p2;
@@

coccilib.report.print_report(p[0], "WARNING: PTR_RET can be used")
<<<<<<< HEAD
=======

@script:python depends on report@
p << r3.p3;
@@

coccilib.report.print_report(p[0], "WARNING: PTR_RET can be used")
>>>>>>> refs/remotes/origin/master

type t;

/** Executes task synchronously but waits for current task to be finished */
[@bs.module "rxjs"][@bs.val]
external queue: t = "asapScheduler";

/** Schedules on the micro task queue */
[@bs.module "rxjs"][@bs.val]
external asap: t = "asapScheduler";

/** Schedules on the macro task queue */
[@bs.module "rxjs"][@bs.val]
external async: t = "asyncScheduler";

/** inside ‘requestAnimationFrame’ */
[@bs.module "rxjs"][@bs.val]
external animationFrame: t = "animationFrameScheduler";

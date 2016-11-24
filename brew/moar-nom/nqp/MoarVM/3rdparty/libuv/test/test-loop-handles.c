/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/* Tests commented out with XXX are ones that are failing on Linux */

/*
 * Purpose of this test is to check semantics of starting and stopping
 * prepare, check and idle watchers.
 *
 * - A watcher must be able to safely stop or close itself;
 * - Once a watcher is stopped or closed its callback should never be called.
 * - If a watcher is closed, it is implicitly stopped and its close_cb should
 *   be called exactly once.
 * - A watcher can safely start and stop other watchers of the same type.
 * - Prepare and check watchers are called once per event loop iterations.
 * - All active idle watchers are queued when the event loop has no more work
 *   to do. This is done repeatedly until all idle watchers are inactive.
 * - If a watcher starts another watcher of the same type its callback is not
 *   immediately queued. For check and prepare watchers, that means that if
 *   a watcher makes another of the same type active, it'll not be called until
 *   the next event loop iteration. For idle. watchers this means that the
 *   newly activated idle watcher might not be queued immediately.
 * - Prepare, check, idle watchers keep the event loop alive even when they're
 *   not active.
 *
 * This is what the test globally does:
 *
 * - prepare_1 is always active and counts event loop iterations. It also
 *   creates and starts prepare_2 every other iteration. Finally it verifies
 *   that no idle watchers are active before polling.
 * - prepare_2 is started by prepare_1 every other iteration. It immediately
 *   stops itself. It verifies that a watcher is not queued immediately
 *   if created by another watcher of the same type.
 * - There's a check watcher that stops the event loop after a certain number
 *   of iterations. It starts a varying number of idle_1 watchers.
 * - Idle_1 watchers stop themselves after being called a few times. All idle_1
 *   watchers try to start the idle_2 watcher if it is not already started or
 *   awaiting its close callback.
 * - The idle_2 watcher always exists but immediately closes itself after
 *   being started by a check_1 watcher. It verifies that a watcher is
 *   implicitly stopped when closed, and that a watcher can close itself
 *   safely.
 * - There is a repeating timer. It does not keep the event loop alive
 *   (ev_unref) but makes sure that the loop keeps polling the system for
 *   events.
 */


#include "uv.h"
#include "task.h"

#include <math.h>


#define IDLE_COUNT      7
#define ITERATIONS      21
#define TIMEOUT         100


static uv_prepare_t prepare_1_handle;
static uv_prepare_t prepare_2_handle;

static uv_check_t check_handle;

static uv_idle_t idle_1_handles[IDLE_COUNT];
static uv_idle_t idle_2_handle;

static uv_timer_t timer_handle;


static int loop_iteration = 0;

static int prepare_1_cb_called = 0;
static int prepare_1_close_cb_called = 0;

static int prepare_2_cb_called = 0;
static int prepare_2_close_cb_called = 0;

st
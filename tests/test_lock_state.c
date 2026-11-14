/* SPDX-License-Identifier: GPL-3.0-only */
#include "lock_state.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void test_round_trip_and_restart(void) {
    uint8_t slot_a[LOCK_RECORD_SIZE], slot_b[LOCK_RECORD_SIZE];
    LockState first = {.generation = 1, .locked = true};
    LockState second = {.generation = 2, .locked = true};
    LockState recovered;
    assert(lock_record_encode(&first, slot_a));
    assert(lock_record_encode(&second, slot_b));
    assert(lock_state_select(slot_a, sizeof(slot_a), slot_b, sizeof(slot_b), &recovered));
    assert(recovered.generation == 2 && recovered.locked);
}

static void test_interrupted_write_and_corruption(void) {
    uint8_t good[LOCK_RECORD_SIZE], partial[LOCK_RECORD_SIZE] = {0};
    LockState old = {.generation = 7, .locked = true};
    LockState recovered;
    assert(lock_record_encode(&old, good));
    /* A partially written next slot is rejected; prior valid state remains usable. */
    assert(lock_state_select(good, sizeof(good), partial, 9, &recovered));
    assert(recovered.generation == 7 && recovered.locked);
    good[10] ^= 0x40;
    assert(!lock_state_select(good, sizeof(good), partial, sizeof(partial), &recovered));
}

static void test_idempotency_and_generation_overflow(void) {
    LockState current = {.generation = 12, .locked = true}, next;
    assert(lock_state_next(&current, true, &next));
    assert(next.generation == 12 && next.locked);
    current.generation = UINT32_MAX;
    assert(!lock_state_next(&current, false, &next));
}

static void test_test_directory_guard(void) {
    assert(lock_test_cleanup_allowed("/ext/emergency_lock_test", true));
    assert(!lock_test_cleanup_allowed("/ext/emergency_lock_test", false));
    assert(!lock_test_cleanup_allowed("/ext/emergency_lock_test/../user", true));
    assert(!lock_test_cleanup_allowed("/ext/user", true));
    assert(!lock_test_cleanup_allowed(NULL, true));
}

int main(void) {
    test_round_trip_and_restart();
    test_interrupted_write_and_corruption();
    test_idempotency_and_generation_overflow();
    test_test_directory_guard();
    puts("lock_state_tests: all checks passed");
    return 0;
}

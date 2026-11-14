/* SPDX-License-Identifier: GPL-3.0-only */
#ifndef EMERGENCY_LOCK_STATE_H
#define EMERGENCY_LOCK_STATE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define LOCK_RECORD_SIZE 20u
#define LOCK_RECORD_VERSION 1u

typedef struct {
    uint32_t generation;
    bool locked;
} LockState;

/* Records use a fixed little-endian format and CRC-32/ISO-HDLC. */
bool lock_record_encode(const LockState* state, uint8_t out[LOCK_RECORD_SIZE]);
bool lock_record_decode(const uint8_t* data, size_t length, LockState* out);

/* Choose the newest valid record. Equal generations prefer locked state. */
bool lock_state_select(
    const uint8_t* slot_a,
    size_t slot_a_length,
    const uint8_t* slot_b,
    size_t slot_b_length,
    LockState* out);

/* Caller writes the returned record to the inactive slot, then verifies it. */
bool lock_state_next(const LockState* current, bool locked, LockState* next);

/* Permit test cleanup only for the exact dedicated directory and explicit confirmation. */
bool lock_test_cleanup_allowed(const char* path, bool confirmed);

#endif

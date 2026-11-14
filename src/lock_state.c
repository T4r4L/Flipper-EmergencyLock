/* SPDX-License-Identifier: GPL-3.0-only */
#include "lock_state.h"

#include <string.h>

#define RECORD_MAGIC 0x314b4c45u /* "ELK1" in little endian */
#define TEST_DIRECTORY "/ext/emergency_lock_test"

static void write_u32_le(uint8_t* out, uint32_t value) {
    out[0] = (uint8_t)value;
    out[1] = (uint8_t)(value >> 8);
    out[2] = (uint8_t)(value >> 16);
    out[3] = (uint8_t)(value >> 24);
}

static uint32_t read_u32_le(const uint8_t* in) {
    return (uint32_t)in[0] | ((uint32_t)in[1] << 8) | ((uint32_t)in[2] << 16) |
           ((uint32_t)in[3] << 24);
}

static uint32_t crc32(const uint8_t* data, size_t length) {
    uint32_t crc = 0xffffffffu;
    for(size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for(unsigned bit = 0; bit < 8; ++bit) {
            const uint32_t mask = (uint32_t)-(int32_t)(crc & 1u);
            crc = (crc >> 1) ^ (0xedb88320u & mask);
        }
    }
    return ~crc;
}

bool lock_record_encode(const LockState* state, uint8_t out[LOCK_RECORD_SIZE]) {
    if(!state || !out) return false;
    memset(out, 0, LOCK_RECORD_SIZE);
    write_u32_le(out, RECORD_MAGIC);
    write_u32_le(out + 4, LOCK_RECORD_VERSION);
    write_u32_le(out + 8, state->generation);
    write_u32_le(out + 12, state->locked ? 1u : 0u);
    write_u32_le(out + 16, crc32(out, 16));
    return true;
}

bool lock_record_decode(const uint8_t* data, size_t length, LockState* out) {
    if(!data || !out || length != LOCK_RECORD_SIZE) return false;
    if(read_u32_le(data) != RECORD_MAGIC || read_u32_le(data + 4) != LOCK_RECORD_VERSION) {
        return false;
    }
    const uint32_t locked = read_u32_le(data + 12);
    if(locked > 1u || read_u32_le(data + 16) != crc32(data, 16)) return false;
    out->generation = read_u32_le(data + 8);
    out->locked = locked != 0u;
    return true;
}

bool lock_state_select(
    const uint8_t* slot_a,
    size_t slot_a_length,
    const uint8_t* slot_b,
    size_t slot_b_length,
    LockState* out) {
    if(!out) return false;
    LockState a, b;
    const bool valid_a = lock_record_decode(slot_a, slot_a_length, &a);
    const bool valid_b = lock_record_decode(slot_b, slot_b_length, &b);
    if(!valid_a && !valid_b) return false;
    if(valid_a && (!valid_b || a.generation > b.generation ||
                   (a.generation == b.generation && a.locked))) {
        *out = a;
    } else {
        *out = b;
    }
    return true;
}

bool lock_state_next(const LockState* current, bool locked, LockState* next) {
    if(!current || !next) return false;
    if(current->locked == locked) {
        *next = *current;
        return true;
    }
    if(current->generation == UINT32_MAX) return false;
    next->generation = current->generation + 1u;
    next->locked = locked;
    return true;
}

bool lock_test_cleanup_allowed(const char* path, bool confirmed) {
    return confirmed && path && strcmp(path, TEST_DIRECTORY) == 0;
}

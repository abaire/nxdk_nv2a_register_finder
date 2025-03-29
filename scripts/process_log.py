#!/usr/bin/env python3

from __future__ import annotations

import logging
import re
import sys
from collections import defaultdict

logger = logging.getLogger(__name__)

BEGIN_TEST_RE = re.compile(r"BEGIN (\w+)")
END_TEST_RE = re.compile(r"END (\w+)")

HEX_CAPTURE = r"(0x[0-9a-fA-F]+)"
SUBTEST_RE = re.compile(r">\s+(.*?)\s+" + HEX_CAPTURE)
ITERATION_RE = re.compile(r"!\s+ITERATION\s+(\d+)")

# 0xFD40010C: 0x02000000 => 0x02020000
REGISTER_DIFF_RE = re.compile(HEX_CAPTURE + r":\s+" + HEX_CAPTURE + r"\s+=>\s+" + HEX_CAPTURE)


# These addresses always seem to be updated to match the value being set, regardless
# of the command.
SKIPLIST = {
    "0xFD400708",
    "0xFD40070C",
    "0xFD400748",
    "0xFD400768",
    "0xFD40076C",
    "0xFD401708",
    "0xFD40170C",
    "0xFD401748",
    "0xFD401768",
    "0xFD40176C",
}

KNOWN_REGISTERS = {
    "0xFD400FC0": "NV_PGRAPH_CSV1_A",
    "0xFD400FBC": "NV_PGRAPH_CSV1_B",
    "0xFD400FB8": "NV_PGRAPH_CSV0_C",
    "0xFD400FB4": "NV_PGRAPH_CSV0_D",
}


def register_name(register: str) -> str:
    name = KNOWN_REGISTERS.get(register)
    if name:
        return f" ({name})"
    return ""


def _process_subtest(value_applied: str, iterations: dict[int, list[str]]) -> tuple[set[str], dict[str, int]]:
    logger.debug(value_applied)

    # Values that do not change across iterations.
    unchanged_values: dict[str, tuple[int, int]] = {}
    # Values that are unstable across iterations and probably uninteresting.
    changed_values: set[str] = SKIPLIST.copy()

    for values in iterations.values():
        for value in values:
            match = REGISTER_DIFF_RE.match(value)
            if not match:
                logger.error("Unexpected iteration value: '%s'", value)
                continue

            register = match.group(1)
            original = match.group(2)
            new = match.group(3)

            if register in changed_values:
                continue

            if register not in unchanged_values:
                unchanged_values[register] = int(original, 16), int(new, 16)
            else:
                changed_values.add(register)

    exact_matches: set[str] = set()
    consistent_changes: dict[str, int] = {}

    for register in sorted(unchanged_values):
        old, new = unchanged_values[register]

        if new == int(value_applied, 16):
            exact_matches.add(register)
            logger.warning("\t** %s%s: %s => %s (%s)", register, register_name(register), old, new, bin(new))
        else:
            consistent_changes[register] = new
            logger.warning("\t%s%s: %s => %s (%s)", register, register_name(register), old, new, bin(new))

    return exact_matches, consistent_changes


def _process_test(test_name: str, subtests: dict[str, dict[int, list[str]]]) -> tuple[set[str], set[str]]:
    print(test_name)

    possible_registers: dict[str, tuple[set[str], dict[str, int]]] = {}
    for subtest, iterations in subtests.items():
        possible_registers[subtest] = _process_subtest(subtest, iterations)

    values = list(possible_registers.values())

    exact_match_regs = values[0][0]
    consistent_change_regs = set(values[0][1])

    for test_values in values[1:]:
        exact_match_regs &= test_values[0]
        consistent_change_regs &= set(test_values[1]) | test_values[0]

    exact_match_regs -= SKIPLIST

    for exact_match_reg in exact_match_regs:
        print(f"{exact_match_reg} == test value")

    for consistent_change_reg in consistent_change_regs:
        print(f"{consistent_change_reg} changed consistently with value")

    return exact_match_regs, consistent_change_regs


def main(log_file: str):
    tests: dict[str, dict[str, dict[int, list[str]]]] = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))

    with open(log_file) as logfile:
        current_test = None
        current_subtest = "UNKNOWN"
        current_iteration = 0
        for full_line in logfile:
            line = full_line.strip()
            if not line:
                continue

            match = BEGIN_TEST_RE.match(line)
            if match:
                current_test = match.group(1)
                continue

            match = END_TEST_RE.match(line)
            if match:
                current_test = None
                current_subtest = "UNKNOWN"
                current_iteration = 0
                continue

            match = SUBTEST_RE.match(line)
            if match:
                current_subtest = match.group(2)
                continue

            match = ITERATION_RE.match(line)
            if match:
                current_iteration = int(match.group(1))
                continue

            tests[current_test][current_subtest][current_iteration].append(line)

    test_results: dict[str, tuple[set[str], set[str]]] = {}

    for test, subtests in tests.items():
        test_results[test] = _process_test(test, subtests)

    # Look for exact match results that are consistent across all tests and
    # flag them for review.
    if len(tests) > 1:
        exact_matches = set()
        for exact_match_regs, _consistent_change_regs in test_results.values():
            if not exact_matches:
                exact_matches = exact_match_regs
            else:
                exact_matches &= exact_match_regs

        if exact_matches:
            print("The following registers were set to the test value across multiple tests:")
            for reg in exact_matches:
                print(reg)

    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1]))

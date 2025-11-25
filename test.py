#!/usr/bin/env python3
"""Test suite for soterg_hash module."""

import unittest
import soterg_hash
from binascii import hexlify, unhexlify


class TestSoterG(unittest.TestCase):
    """Test suite for SoterG hash module"""

    # Test vector: block header and expected hash (Block 4)
    TEST_INPUT = "00000032e9de8ebb42a4bfbe99af01e5b905e026fc9080b98a0f411c4b15b8cf3b000000abec73e42ad979a60bfcc3faf727cf4fa39713311c0fcef5f788b362cd16913d29130269c602141c024dac91"
    EXPECTED_OUTPUT = (
        b"e9e855fd28c2916d4e13f6c15eddf96625bdbf8c9943c5ec0f1e340900000000"
    )

    def test_known_hash(self):
        """Test with known hash vector"""
        test_input = unhexlify(self.TEST_INPUT)
        result = soterg_hash.getPoWHash(test_input)
        result_hex = hexlify(result)

        self.assertEqual(
            result_hex,
            self.EXPECTED_OUTPUT,
            f"Expected {self.EXPECTED_OUTPUT}, got {result_hex}",
        )

    def test_output_length(self):
        """Test that output is always 32 bytes"""
        test_input = unhexlify(self.TEST_INPUT)
        result = soterg_hash.getPoWHash(test_input)

        self.assertEqual(
            len(result), 32, f"Expected 32-byte output, got {len(result)} bytes"
        )

    def test_output_type(self):
        """Test that output is bytes"""
        test_input = unhexlify(self.TEST_INPUT)
        result = soterg_hash.getPoWHash(test_input)

        self.assertIsInstance(
            result, bytes, f"Expected bytes output, got {type(result)}"
        )

    def test_invalid_input_length(self):
        """Test that function rejects wrong input size"""
        # Input should be exactly 80 bytes
        invalid_inputs = [
            b"",  # Empty
            b"short",  # Too short
            b"x" * 79,  # One byte short
            b"x" * 81,  # One byte too long
            b"x" * 160,  # Double size
        ]

        for invalid_input in invalid_inputs:
            with self.subTest(input_length=len(invalid_input)):
                # This should either raise an exception or handle gracefully
                try:
                    result = soterg_hash.getPoWHash(invalid_input)
                    # If no exception, result should still be 32 bytes
                    self.assertEqual(len(result), 32)
                except (TypeError, ValueError) as e:
                    # It's acceptable to raise an error for invalid input
                    self.assertIn(
                        "80",
                        str(e),
                        "Error message should indicate 80-byte requirement",
                    )

    def test_deterministic(self):
        """Test that same input always produces same output"""
        test_input = unhexlify(self.TEST_INPUT)
        result1 = soterg_hash.getPoWHash(test_input)
        result2 = soterg_hash.getPoWHash(test_input)

        self.assertEqual(
            result1, result2, "Same input should always produce same output"
        )


if __name__ == "__main__":
    unittest.main(verbosity=2)

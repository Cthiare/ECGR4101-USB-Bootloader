"""
firmware_sender.py - Host-side firmware sender (Requirement 3)
Owner: Person 4  (placeholder - replace on the `python-sender` branch)

Usage:
    python firmware_sender.py --port COM5 --bin ../Lab3_Application/build/Debug/Lab3_Application.bin
"""
import argparse


def main():
    parser = argparse.ArgumentParser(description="Send firmware to the Lab 3 bootloader")
    parser.add_argument("--port", required=True, help="e.g. COM5 or /dev/ttyACM0")
    parser.add_argument("--bin", required=True, help="path to Lab3_Application.bin")
    args = parser.parse_args()

    # TODO (Person 4): "BOOT" + length, wait 'R', 256-byte chunks with 'A' acks, wait 'D'
    print(f"TODO: send {args.bin} to {args.port}")


if __name__ == "__main__":
    main()

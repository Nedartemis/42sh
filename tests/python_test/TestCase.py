from dataclasses import dataclass


@dataclass
class TestCase:
    file: str
    stdout: str
    stderr: str
    exitcode: int

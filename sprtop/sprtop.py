# /bin/python
from sprtop.sprtop_core import *
from sprtop.graph import SPRTopMap


def run():
    print(SPRCoreCHA().get_core_cha(0))

    SPRTopMap(
        [
            ["context1a", "context2a", "context3a"],
            ["context1b", "context2b", "context3b"],
        ],
        [["core1a", "core2a", "core3a"], ["core1b", "core2b", "core3b"]],
        [["map1a", "map2a", "map3a"], ["map1b", "map2b", "map3b"]],
    ).display()


if __name__ == "__main__":
    run()

# content of conftest.py
import pytest


def pytest_collection_modifyitems(items):
    # Modify this list with your specific files
    priority_files = []

    # Collect priority items
    priority_items = [item for item in items if item.location[0] in priority_files]

    # Collect the rest
    other_items = [item for item in items if item.location[0] not in priority_files]

    # Modify the collection order
    items[:] = priority_items + other_items
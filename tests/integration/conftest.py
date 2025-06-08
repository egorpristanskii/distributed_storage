import pytest
import os


@pytest.fixture(scope="session")
def service_url():
    """Fixture to provide the base URL for the service."""
    host = os.getenv("SERVICE_HOST", "localhost")
    port = os.getenv("SERVICE_PORT", "8080")
    return f"http://{host}:{port}"

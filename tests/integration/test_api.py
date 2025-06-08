import pytest
import requests


def test_allkeys(service_url):
    endpoint = f"{service_url}/allkeys"
    response = requests.get(endpoint)
    assert response.status_code == 200, f"Expected status code 200, got {response.status_code}"
    data = response.json()
    assert data is None

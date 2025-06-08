import pytest
import requests


@pytest.mark.parametrize(
        "service_request, expected_result",
        [
            (
                {
                    "path": "allkeys",
                    "method": "GET",
                },
                {
                    "status_code": 204,
                    "data": None
                }
            ),
            (
                {
                    "path": "keys/integration_test_key",
                    "method": "POST",
                    "headers": {"Content-Type": "application/x-www-form-urlencoded"},
                    "params": {"value": "testValue", "type": "string"}
                },
                {
                    "status_code": 200,
                    "data": {"key": "integration_test_key", "value": "testValue", "type": "std::string"}
                }
            ),
            (
                {
                    "path": "allkeys",
                    "method": "GET",
                },
                {
                    "status_code": 200,
                    "data": {"integration_test_key": {"value": "testValue", "type": "std::string"}}
                }
            ),
            (
                {
                    "path": "keys/integration_test_key",
                    "method": "GET",
                    "params": None,
                },
                {
                    "status_code": 200,
                    "data": {"key": "integration_test_key", "value": "testValue", "type": "std::string"}
                }
            ),
            (
                {
                    "path": "keys/integration_test_key",
                    "method": "DELETE",
                    "params": None
                },
                {
                    "status_code": 204,
                    "data": None
                }
            ),
        ]
)
def test_endpoints(service_url, service_request, expected_result):
    endpoint = f"{service_url}/{service_request["path"]}"
    print(f"Testing endpoint: {endpoint} with request: {service_request}")
    response = requests.request(
        method=service_request["method"],
        url=endpoint,
        headers=service_request.get("headers"),
        json=service_request.get("params")
    )
    print(f"Response: {response.status_code} - {response.content}")

    assert response.status_code == expected_result["status_code"]
    if response.status_code == 204:
        assert response.content == b'', "Expected no content for 204 response"
    else:
        assert response.json() == expected_result["data"], f"Expected {expected_result['data']} but got {response.json()} {response}"

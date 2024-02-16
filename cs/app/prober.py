import json
import urllib.request
from urllib.error import URLError

def check_url(url, expected_json):
    try:
        with urllib.request.urlopen(url) as response:
            content = response.read()
            content_str = content.decode('utf-8')
            actual_json = json.loads(content_str)
            assert actual_json == expected_json, "The URL did not return the expected JSON."
            print("The URL returned the expected JSON.")
    except URLError as e:
        print(f"URLError: {e}")
    except json.JSONDecodeError as e:
        print(f"JSONDecodeError: {e}")
    except AssertionError as e:
        print(f"AssertionError: {e}")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    url = "https://cs.p13i.io/json/"
    expected_json = {"key": "value", "key2": [True, False, "hello", 1.4]}
    check_url(url, expected_json)

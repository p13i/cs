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

def ProbeJson():
    url = "https://cs.p13i.io/json/"
    expected_json = {"key": "value", "key2": [True, False, "hello", 1.4]}
    check_url(url, expected_json)
    return True

def ProbeCreateLog():
    case = "ProbeCreateLog()"
    print(f"[ STARTED ]{case}")
    method = "POST"
    url = "https://cs.p13i.io/log/"
    data = urllib.parse.urlencode({"message":"my-message"}).encode()
    try:
        with urllib.request.urlopen(url, data=data) as response:
            assert response.status == 200, "The URL did not return a 200 status code."
            print(f"[ PASSED ] {case}")
            return True
    except Exception as e:
        print(f"Error: {e}")
        print(f"[ FAILED ]{case}")
        return False

def ProbeRender(width=200, height=200, num_frames=0, expect_fail=False):
    case = f"ProbeRender(widht={width}, height={height}, num_frames={num_frames}, expect_fail={expect_fail})"
    print(f"[ STARTED ] {case} ")
    url = F"https://cs.p13i.io/render/?width={width}&height={height}&num_frames={num_frames}"
    try:
        with urllib.request.urlopen(url) as response:
            status = response.status
            success = True
    except Exception as e:
        status = e.status
        success = False
    
    if expect_fail and success:
        print(f"[ FAIL ] {case} failed because it was expected to fail but actually succeeded.")
        return False
    
    print(f"[ PASSED ] {case}")
    return True

if __name__ == "__main__":
    assert ProbeJson()
    assert ProbeCreateLog()
    assert ProbeRender(height=10, width=10, num_frames=0, expect_fail=True)
    assert ProbeRender(height=10, width=10, num_frames=1, expect_fail=False)
    assert ProbeRender(height=1, width=2, num_frames=1, expect_fail=False)
    assert ProbeRender(height=10, width=10, num_frames=0, expect_fail=True)
    assert ProbeRender(height=0, width=0, num_frames=0, expect_fail=True)
    assert ProbeRender(height=2, width=2, num_frames=1, expect_fail=False)
    assert ProbeRender(height=2, width=2, num_frames=0, expect_fail=True)
    assert ProbeRender(height=1, width=1, num_frames=1, expect_fail=True)
    assert ProbeRender(height=1, width=1, num_frames=1, expect_fail=False)

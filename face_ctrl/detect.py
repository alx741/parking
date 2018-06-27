import requests
import matplotlib.pyplot as plt
from PIL import Image
from matplotlib import patches
from io import BytesIO
from os.path import expanduser

subscription_key = "572b743e7455422d876ab01415054f69"
assert subscription_key

face_api_url = 'https://westcentralus.api.cognitive.microsoft.com/face/v1.0/detect'

headers = {
        'Content-Type': 'application/octet-stream',
        'Ocp-Apim-Subscription-Key': subscription_key,
}

params = {
    'returnFaceId': 'false',
    'returnFaceLandmarks': 'false',
    'returnFaceAttributes': 'age'
}

img = open(expanduser('./face.jpg'), 'rb')
response = requests.post(face_api_url, data=img, params=params, headers=headers)
faces = response.json()
print(faces)

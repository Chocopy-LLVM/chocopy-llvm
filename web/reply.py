import requests
import json



payload = json.dumps({'input':'','passes':'rr'})
print(payload)
reply = requests.post("http://0.0.0.0:8000/compile",data=payload)
print(reply.text)


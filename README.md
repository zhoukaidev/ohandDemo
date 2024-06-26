# ohandDemo


## Service 

```js
{
  "method": "thing.service.finger.set",
  "inputData": {
    "fingers": "10,20,30,40,50,60"
  }
}
```

## ActionProtocol

```js
{
  "steps": [
    {
      "action": "setfingers",
      "params": [
        {"deviceType": "ohand", "deviceId": "001", "value": "0,0,0,0,0,0"},
        {"deviceType": "ohand", "deviceId": "002", "value": "0,0,0,0,0,0"},
      ]
    },
    {
      "action": "sleep",
      "params": 1000
    }
  ]
}
```